//-----------------------------------------------
//  ���e�F���ԕ`�揈��
//  File : goal.cpp
//  ����ҁF�؍�@��
//-----------------------------------------------

//------ �C���N���[�h�t�@�C�� ------
#include "goal.h"
#include "camera.h"
#include "player.h"
#include "sheet.h"

//------ �}�N����` ------
#define GOAL_WIDTH ( 15.0f )
#define GOAL_HEIGHT ( 15.0f )

//------ �O���[�o���ϐ� ------
GOAL g_aGoal[ GOAL_MAX ];

bool g_bMoveGoal = false;	// �I�����Ă��邩�ǂ���
bool g_bGoalSelect = false;	// �I�����Ă��邩�ǂ����i�������鎞�̔���j
int g_nMoveGoal = 0;		// �I�����Ă��鎕�Ԃ̃C���f�b�N�X
int g_nCreateGoalNum = 0;	// �������ꂽ���Ԃ̐�

//------ �����֐��錾 ------

//-----------------------------------------------
// ���ԕ`�揉����
//-----------------------------------------------
void InitGoal( void )
{
	// ���[�J���ϐ�
	//---------------------------------
	LPDIRECT3DDEVICE9 pDevice = NULL;		// �f�o�C�X�I�u�W�F�N�g
	
	// �f�o�C�X�擾
	//------------------------------------
	pDevice = GetDevice();

	for ( int i = 0; i < GOAL_MAX; i++ )
	{
		g_aGoal[ i ].bUse = false;
		g_aGoal[ i ].bRotation = false;
	}

	//{
	//	// x�t�@�C���̓ǂݍ���
	//	//------------------------------------
	//	D3DXLoadMeshFromX("data/MODEL/goal.x",		// �ǂݍ��ރt�@�C����
	//					  D3DXMESH_SYSTEMMEM,							// 
	//					  pDevice,										// 
	//					  NULL,											// 
	//					  &pBuffMatModelInit,			// 
	//					  NULL,											// 
	//					  &numMatModelInit,			// 
	//					  &pMeshModelInit );
	//}

	// �O���[�o���ϐ��̏�����
	g_bMoveGoal = false;
	g_nMoveGoal = 0;
	g_nCreateGoalNum = 0;
	g_bGoalSelect = false;

	// �ʒu�ǂݍ��ݏ����J�n	
	FILE *fp = fopen( "GoalPos.txt", "rt" );
	char strWork[ 256 ];
	D3DXVECTOR3 fWork;

	// �t�@�C���̓ǂݍ��݊J�n
	while ( strcmp( strWork, "END_SCRIPT" ) != 0 )
	{
		// ��s�ǂݍ���
		fscanf( fp, "%s = %f %f %f" , strWork, &fWork.x, &fWork.y, &fWork.z );

		if ( strcmp( strWork, "POS" ) == 0 )
		{
			// ���L���[�u����
			SetGoal( fWork );
		}
	}
}

//-----------------------------------------------
// ���ԕ`��I������
//-----------------------------------------------
void UninitGoal( void )
{
	for (int nCnt = 0; nCnt < GOAL_MAX; nCnt++)
	{
		if ( g_aGoal[ nCnt ].bUse == true )
		{
			// ���b�V�����̉���Ə�����
			//------------------------------------
			if (g_aGoal[ nCnt ].pMeshModel != NULL)
			{ 
				g_aGoal[ nCnt ].pMeshModel ->Release();
				g_aGoal[ nCnt ].pMeshModel = NULL;
			}
	
			// �}�e���A�����̉���Ə�����
			//------------------------------------
			if (g_aGoal[ nCnt ].pBuffMatModel != NULL)
			{ 
				g_aGoal[ nCnt ].pBuffMatModel ->Release();
				g_aGoal[ nCnt ].pBuffMatModel = NULL;
			}
		}
	}
}

//-----------------------------------------------
// ���ԕ`��X�V����
//-----------------------------------------------
void UpdateGoal( void )
{
	g_aGoal[ 0 ].move = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );

	{
		// ���Ԉʒu�̕ۑ�
		if ( GetKeyboardTrigger( DIK_F4 ) == true )
		{
			// �t�@�C���̃I�[�v��
			FILE *fp = fopen( "GoalPositionInfo.txt", "wt" );

			for (int nCnt = 0; nCnt < GOAL_MAX; nCnt++)
			{
				if ( g_aGoal[ nCnt ].bUse == true )
				{
					char workChar[ 256 ] = "POS = ";
					char workFloatTrans[ 256 ];

					// �ʒu�����[�N�ɓ˂�����
					sprintf( workFloatTrans, "%.2f", g_aGoal[ nCnt ].pos.x );
					strcat( workChar, workFloatTrans );		// �A��
					strcat( workChar, " " );				// �A��
					sprintf( workFloatTrans, "%.2f", g_aGoal[ nCnt ].pos.y );
					strcat( workChar, workFloatTrans );		// �A��
					strcat( workChar, " " );				// �A��
					sprintf( workFloatTrans, "%.2f", g_aGoal[ nCnt ].pos.z );
					strcat( workChar, workFloatTrans );		// �A��
					strcat( workChar, "\n" );				// �A��

					// �����o��
					fputs( workChar, fp );
				}
			}

			// �t�@�C���̃N���[�Y
			fclose( fp );
		}

		// ��]����
		for ( int i = 0; i < GOAL_MAX; i++ )
		{
			if ( g_aGoal[ i ].bUse == true )
			{
				if ( g_aGoal[ i ].bRotation == true )
				{
					g_aGoal[ i ].rot.z += 0.025f;
				}
			}
		}
	}
}

//-----------------------------------------------
// ���ԕ`��`�揈��
//-----------------------------------------------
void DrawGoal( void )
{
	// ���[�J���ϐ�
	//------------------------------------
	LPDIRECT3DDEVICE9 pDevice;						// �f�o�C�X�I�u�W�F�N�g
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;		// �v�Z�p
	D3DXMATERIAL *pMat;								// �}�e���A�������擾����ϐ�
	D3DMATERIAL9 matDef;							// ���̃}�e���A�����

	// �f�o�C�X�̎擾
	//------------------------------------
	pDevice = GetDevice();

	// �J�����̃Z�b�g
	SetCamera();

	// �V�[�g�������Ă���
	//if ( GetSheet() == true )
	{
		// ���ꂼ��̃p�[�c�̍s��v�Z�ƕ`��J�n
		for (int nCnt = 0; nCnt < GOAL_MAX; nCnt++)
		{
			if ( g_aGoal[ nCnt ].bUse == true )
			{
				D3DXMatrixIdentity( &g_aGoal[ nCnt ].mtxWorld );	// �t�H�[�}�b�g�̏�����
				D3DXMatrixIdentity( &mtxScl );						// �s��̏�����
				D3DXMatrixIdentity( &mtxRot );						// �s��̏�����
				D3DXMatrixIdentity( &mtxTranslate );				// �s��̏�����

				// �X�P�[���𔽉f
				D3DXMatrixScaling(&mtxScl, g_aGoal[ nCnt ].scl.x, g_aGoal[ nCnt ].scl.y, g_aGoal[ nCnt ].scl.z);
				D3DXMatrixMultiply(&g_aGoal[ nCnt ].mtxWorld, &g_aGoal[ nCnt ].mtxWorld, &mtxScl);

				// �����𔽉f
				D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aGoal[ nCnt ].rot.y, g_aGoal[ nCnt ].rot.x, g_aGoal[ nCnt ].rot.z);
				D3DXMatrixMultiply(&g_aGoal[ nCnt ].mtxWorld, &g_aGoal[ nCnt ].mtxWorld, &mtxRot);

				// �ʒu�𔽉f
				D3DXMatrixTranslation(&mtxTranslate, g_aGoal[ nCnt ].pos.x, g_aGoal[ nCnt ].pos.y, g_aGoal[ nCnt ].pos.z);
				D3DXMatrixMultiply(&g_aGoal[ nCnt ].mtxWorld, &g_aGoal[ nCnt ].mtxWorld, &mtxTranslate);

				// �ݒ�
				pDevice ->SetTransform(D3DTS_WORLD, &g_aGoal[ nCnt ].mtxWorld);

				// ���݂̃}�e���A������ۑ�
				pDevice ->GetMaterial( &matDef );

				// �o�b�t�@�ւ̃|�C���^���擾
				pMat = (D3DXMATERIAL*)g_aGoal[ nCnt ].pBuffMatModel ->GetBufferPointer();

				/*strSrc = strrchr(pMat ->pTextureFilename, '\\') + 1;

				strcat(strDest, strSrc);

				D3DXCreateTextureFromFile(pDevice, strDest, &g_pTextureModel[1]);*/
			
				// �}�e���A���̐��������[�v
				for (int nCntMat = 0; nCntMat < (int)g_aGoal[ nCnt ].numMatModel; nCntMat++)
				{
					pDevice ->SetMaterial( &pMat[ nCntMat ].MatD3D );			// �}�e���A���̐ݒ�
					pDevice ->SetTexture( 0, g_aGoal[ nCnt ].pTexture );							// �e�N�X�`���̃Z�b�g
					g_aGoal[ nCnt ].pMeshModel ->DrawSubset( nCntMat );		// �`��
				}
			
				// �}�e���A�������ɖ߂�
				//------------------------------------
				pDevice ->SetMaterial( &matDef );
			}
		}
	}
}

//-----------------------------------------------
// ���ԃZ�b�g
//-----------------------------------------------
int SetGoal( D3DXVECTOR3 pos )
{
	// ���[�J���ϐ�
	//---------------------------------
	LPDIRECT3DDEVICE9 pDevice = NULL;		// �f�o�C�X�I�u�W�F�N�g
	
	// �f�o�C�X�擾
	//------------------------------------
	pDevice = GetDevice();

	for ( int i = 0; i < GOAL_MAX; i++ )
	{
		if ( g_aGoal[ i ].bUse == false )
		{
			// �ʒu�̐ݒ�
			g_aGoal[ i ].pos = pos;
			g_aGoal[ i ].rot = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
			g_aGoal[ i ].scl = D3DXVECTOR3( 1.0f, 1.0f, 1.0f );

			g_aGoal[ i ].move = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );

			// �����蔻��p
			g_aGoal[ i ].rect.pos = pos;
			g_aGoal[ i ].rect.harfSize = D3DXVECTOR3( GOAL_WIDTH, GOAL_HEIGHT, GOAL_HEIGHT );

			// ���f�������R�s�[
			//g_aGoal[ i ].numMatModel = numMatModelInit;
			//g_aGoal[ i ].pBuffMatModel = pBuffMatModelInit;
			//g_aGoal[ i ].pMeshModel = pMeshModelInit;

			{
				// x�t�@�C���̓ǂݍ���
				//------------------------------------
				D3DXLoadMeshFromX("data/MODEL/gear.x",		// �ǂݍ��ރt�@�C����
								  D3DXMESH_SYSTEMMEM,			// 
								  pDevice,						// 
								  NULL,							// 
								  &g_aGoal[ i ].pBuffMatModel,	// 
								  NULL,							// 
								  &g_aGoal[ i ].numMatModel,	// 
								  &g_aGoal[ i ].pMeshModel );
			}

			// �e�N�X�`���̓ǂݍ���
			//------------------------------------
			D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Block.jpg", &g_aGoal[ i ].pTexture);

			// ��]�t���O
			g_aGoal[ i ].bRotation = false;

			g_aGoal[ i ].bUse = true;
			
			// �����������𑝂₷
			g_nCreateGoalNum++;

			// �C���f�b�N�X��Ԃ�
			return i;
		}
	}

	return GOAL_MAX;
}

//-----------------------------------------------
// �G�f�B�b�g�X�V
//-----------------------------------------------
void EditGoal( void )
{
	if ( GetKeyboardTrigger( DIK_B ) == true && g_bGoalSelect == false )
	{
		// �p�x��������
		g_aGoal[ g_nMoveGoal ].rot.y = 0.0f;

		// ���Ԃ̐���
		g_nMoveGoal = SetGoal( D3DXVECTOR3( g_aGoal[ g_nMoveGoal ].pos.x + 40.0f, g_aGoal[ g_nMoveGoal ].pos.y, g_aGoal[ g_nMoveGoal ].pos.z ) );
	}

	//if ( g_bMoveGoal == true )
	{
		// �ړ�����
		if ( GetKeyboardTrigger( DIK_A ) == true )
		{
			g_aGoal[ g_nMoveGoal ].pos.x += -40.0f;
		}
		if ( GetKeyboardTrigger( DIK_D ) == true )
		{
			g_aGoal[ g_nMoveGoal ].pos.x += 40.0f;
		}
		if ( GetKeyboardTrigger( DIK_W ) == true )
		{
			g_aGoal[ g_nMoveGoal ].pos.y += 40.0f;
		}
		if ( GetKeyboardTrigger( DIK_S ) == true )
		{
			g_aGoal[ g_nMoveGoal ].pos.y += -40.0f;
		}

		g_aGoal[ g_nMoveGoal ].rect.pos = g_aGoal[ g_nMoveGoal ].pos;
		g_aGoal[ g_nMoveGoal ].rot.y += 0.025f;
	}

	// �I�����Ă��鎕�Ԃ̕ύX
	if ( GetKeyboardTrigger( DIK_UP ) == true )
	{
		// �p�x��������
		g_aGoal[ g_nMoveGoal ].rot.y = 0.0f;
			
		g_nMoveGoal--;
		if ( g_nMoveGoal < 0 )
		{
			g_nMoveGoal = g_nCreateGoalNum - 1;
		}
	}
	if ( GetKeyboardTrigger( DIK_DOWN ) == true )
	{
		// �p�x��������
		g_aGoal[ g_nMoveGoal ].rot.y = 0.0f;

		g_nMoveGoal++;
		if ( g_nMoveGoal > g_nCreateGoalNum - 1 )
		{
			g_nMoveGoal = 0;
		}
	}

	// ���Ԃ̍폜
	if ( GetKeyboardTrigger( DIK_SPACE ) == true )
	{
		g_nCreateGoalNum--;
		if ( g_nCreateGoalNum < 0 )
		{
			g_nCreateGoalNum = 0;
		}

		for ( int i = g_nMoveGoal; i < g_nCreateGoalNum; i++ )
		{
			g_aGoal[ i ].pos = g_aGoal[ i + 1 ].pos;
			g_aGoal[ i ].rect.pos = g_aGoal[ i + 1 ].pos;
		}

		g_aGoal[ g_nCreateGoalNum ].bUse = false;
	}

	// ���Ԉʒu�̕ۑ�
	if ( GetKeyboardTrigger( DIK_F4 ) == true )
	{
		// �t�@�C���̃I�[�v��
		FILE *fp = fopen( "GoalPositionInfo.txt", "wt" );

		for (int nCnt = 0; nCnt < GOAL_MAX; nCnt++)
		{
			if ( g_aGoal[ nCnt ].bUse == true )
			{
				char workChar[ 256 ] = "POS = ";
				char workFloatTrans[ 256 ];

				// �ʒu�����[�N�ɓ˂�����
				sprintf( workFloatTrans, "%.2f", g_aGoal[ nCnt ].pos.x );
				strcat( workChar, workFloatTrans );		// �A��
				strcat( workChar, " " );				// �A��
				sprintf( workFloatTrans, "%.2f", g_aGoal[ nCnt ].pos.y );
				strcat( workChar, workFloatTrans );		// �A��
				strcat( workChar, " " );				// �A��
				sprintf( workFloatTrans, "%.2f", g_aGoal[ nCnt ].pos.z );
				strcat( workChar, workFloatTrans );		// �A��
				strcat( workChar, "\n" );				// �A��

				// �����o��
				fputs( workChar, fp );
			}
		}

		// �t�@�C���̃N���[�Y
		fclose( fp );
	}

	PrintDebugProc("[ B�L�[�Ŏ��Ԑ��� ]\n");
	PrintDebugProc("[ SPACE�L�[�őI�����Ă��鎕�Ԃ��폜 ]\n");
	PrintDebugProc("���Ԃ̌� : %d\n", g_nCreateGoalNum);
	PrintDebugProc("���A�� *** ���Ԕԍ��̑I��");
	PrintDebugProc("�I�����Ă��鎕�Ԕԍ� : %d\n", g_nMoveGoal);

	int nNum = g_nMoveGoal - 10;
	if ( nNum < 0 )
	{
		nNum = 0;
	}
	for ( int i = nNum; i < nNum+ 11; i++ )
	{
		if ( g_aGoal[ i ].bUse == true )
		{
			if ( i == g_nMoveGoal )
			{
				PrintDebugProc("->* [ %d ] * %f %f %f\n", i, g_aGoal[ i ].pos.x, g_aGoal[ i ].pos.y, g_aGoal[ i ].pos.z);
			}
			else
			{
				PrintDebugProc("* [ %d ] * %f %f %f\n", i, g_aGoal[ i ].pos.x, g_aGoal[ i ].pos.y, g_aGoal[ i ].pos.z);
			}
		}
	}
}

//-----------------------------------------------
// �����蔻��̎擾
//-----------------------------------------------
GOAL *GetGoal( void )
{
	return &g_aGoal[ 0 ];
}

//-----------------------------------------------
// ��]�����锻��
//-----------------------------------------------
void SetGoalRotation( int num, bool use )
{
	g_aGoal[ num ].bRotation = use;
}

//-----------------------------------------------
// ��]�����锻��̎擾
//-----------------------------------------------
bool GetGoalRotation( int num )
{
	return g_aGoal[ num ].bRotation;
}

//-----------------------------------------------
// �p�x����
//-----------------------------------------------
void ZeroGoalRotation( void )
{
	for (int nCnt = 0; nCnt < GOAL_MAX; nCnt++)
	{
		if ( g_aGoal[ nCnt ].bUse == true )
		{
			g_aGoal[ nCnt ].rot = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
		}
	}
}
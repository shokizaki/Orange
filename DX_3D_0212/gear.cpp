//-----------------------------------------------
//  ���e�F���ԕ`�揈��
//  File : gear.cpp
//  ����ҁF�؍�@��
//-----------------------------------------------

//------ �C���N���[�h�t�@�C�� ------
#include "gear.h"
#include "camera.h"

//------ �}�N����` ------
#define GEAR_WIDTH ( 15.0f )
#define GEAR_HEIGHT ( 15.0f )

//------ �O���[�o���ϐ� ------
GEAR g_aGear[ GEAR_MAX ];

bool g_bMoveGear = false;	// �I�����Ă��邩�ǂ���
bool g_bGearSelect = false;	// �I�����Ă��邩�ǂ����i�������鎞�̔���j
int g_nMoveGear = 0;		// �I�����Ă��鎕�Ԃ̃C���f�b�N�X
int g_nCreateGearNum = 0;	// �������ꂽ���Ԃ̐�

//------ �����֐��錾 ------

//-----------------------------------------------
// ���ԕ`�揉����
//-----------------------------------------------
void InitGear( void )
{
	// ���[�J���ϐ�
	//---------------------------------
	LPDIRECT3DDEVICE9 pDevice = NULL;		// �f�o�C�X�I�u�W�F�N�g
	
	// �f�o�C�X�擾
	//------------------------------------
	pDevice = GetDevice();

	for ( int i = 0; i < GEAR_MAX; i++ )
	{
		g_aGear[ i ].bUse = false;
		g_aGear[ i ].bRotation = false;
	}

	//{
	//	// x�t�@�C���̓ǂݍ���
	//	//------------------------------------
	//	D3DXLoadMeshFromX("data/MODEL/gear.x",		// �ǂݍ��ރt�@�C����
	//					  D3DXMESH_SYSTEMMEM,							// 
	//					  pDevice,										// 
	//					  NULL,											// 
	//					  &pBuffMatModelInit,			// 
	//					  NULL,											// 
	//					  &numMatModelInit,			// 
	//					  &pMeshModelInit );
	//}

	// �O���[�o���ϐ��̏�����
	g_bMoveGear = false;
	g_nMoveGear = 0;
	g_nCreateGearNum = 0;
	g_bGearSelect = false;
}

//-----------------------------------------------
// ���ԕ`��I������
//-----------------------------------------------
void UninitGear( void )
{
	for (int nCnt = 0; nCnt < GEAR_MAX; nCnt++)
	{
		if ( g_aGear[ nCnt ].bUse == true )
		{
			// ���b�V�����̉���Ə�����
			//------------------------------------
			if (g_aGear[ nCnt ].pMeshModel != NULL)
			{ 
				g_aGear[ nCnt ].pMeshModel ->Release();
				g_aGear[ nCnt ].pMeshModel = NULL;
			}
	
			// �}�e���A�����̉���Ə�����
			//------------------------------------
			if (g_aGear[ nCnt ].pBuffMatModel != NULL)
			{ 
				g_aGear[ nCnt ].pBuffMatModel ->Release();
				g_aGear[ nCnt ].pBuffMatModel = NULL;
			}
		}
	}
}

//-----------------------------------------------
// ���ԕ`��X�V����
//-----------------------------------------------
void UpdateGear( void )
{
	g_aGear[ 0 ].move = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );

	{
		// ���Ԉʒu�̕ۑ�
		if ( GetKeyboardTrigger( DIK_F4 ) == true )
		{
			// �t�@�C���̃I�[�v��
			FILE *fp = fopen( "GearPositionInfo.txt", "wt" );

			for (int nCnt = 0; nCnt < GEAR_MAX; nCnt++)
			{
				if ( g_aGear[ nCnt ].bUse == true )
				{
					char workChar[ 256 ] = "POS = ";
					char workFloatTrans[ 256 ];

					// �ʒu�����[�N�ɓ˂�����
					sprintf( workFloatTrans, "%.2f", g_aGear[ nCnt ].pos.x );
					strcat( workChar, workFloatTrans );		// �A��
					strcat( workChar, " " );				// �A��
					sprintf( workFloatTrans, "%.2f", g_aGear[ nCnt ].pos.y );
					strcat( workChar, workFloatTrans );		// �A��
					strcat( workChar, " " );				// �A��
					sprintf( workFloatTrans, "%.2f", g_aGear[ nCnt ].pos.z );
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
		for ( int i = 0; i < GEAR_MAX; i++ )
		{
			if ( g_aGear[ i ].bUse == true )
			{
				if ( g_aGear[ i ].bRotation == true )
				{
					g_aGear[ i ].rot.z += 0.025f;
				}
			}
		}
	}
}

//-----------------------------------------------
// ���ԕ`��`�揈��
//-----------------------------------------------
void DrawGear( void )
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

	// ���ꂼ��̃p�[�c�̍s��v�Z�ƕ`��J�n
	for (int nCnt = 0; nCnt < GEAR_MAX; nCnt++)
	{
		if ( g_aGear[ nCnt ].bUse == true )
		{
			D3DXMatrixIdentity( &g_aGear[ nCnt ].mtxWorld );	// �t�H�[�}�b�g�̏�����
			D3DXMatrixIdentity( &mtxScl );						// �s��̏�����
			D3DXMatrixIdentity( &mtxRot );						// �s��̏�����
			D3DXMatrixIdentity( &mtxTranslate );				// �s��̏�����

			// �X�P�[���𔽉f
			D3DXMatrixScaling(&mtxScl, g_aGear[ nCnt ].scl.x, g_aGear[ nCnt ].scl.y, g_aGear[ nCnt ].scl.z);
			D3DXMatrixMultiply(&g_aGear[ nCnt ].mtxWorld, &g_aGear[ nCnt ].mtxWorld, &mtxScl);

			// �����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aGear[ nCnt ].rot.y, g_aGear[ nCnt ].rot.x, g_aGear[ nCnt ].rot.z);
			D3DXMatrixMultiply(&g_aGear[ nCnt ].mtxWorld, &g_aGear[ nCnt ].mtxWorld, &mtxRot);

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTranslate, g_aGear[ nCnt ].pos.x, g_aGear[ nCnt ].pos.y, g_aGear[ nCnt ].pos.z);
			D3DXMatrixMultiply(&g_aGear[ nCnt ].mtxWorld, &g_aGear[ nCnt ].mtxWorld, &mtxTranslate);

			// �ݒ�
			pDevice ->SetTransform(D3DTS_WORLD, &g_aGear[ nCnt ].mtxWorld);

			// ���݂̃}�e���A������ۑ�
			pDevice ->GetMaterial( &matDef );

			// �o�b�t�@�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_aGear[ nCnt ].pBuffMatModel ->GetBufferPointer();

			/*strSrc = strrchr(pMat ->pTextureFilename, '\\') + 1;

			strcat(strDest, strSrc);

			D3DXCreateTextureFromFile(pDevice, strDest, &g_pTextureModel[1]);*/
			
			// �}�e���A���̐��������[�v
			for (int nCntMat = 0; nCntMat < (int)g_aGear[ nCnt ].numMatModel; nCntMat++)
			{
				pDevice ->SetMaterial( &pMat[ nCntMat ].MatD3D );			// �}�e���A���̐ݒ�
				pDevice ->SetTexture( 0, NULL );							// �e�N�X�`���̃Z�b�g
				g_aGear[ nCnt ].pMeshModel ->DrawSubset( nCntMat );		// �`��
			}
			
			// �}�e���A�������ɖ߂�
			//------------------------------------
			pDevice ->SetMaterial( &matDef );
		}
	}
}

//-----------------------------------------------
// ���ԃZ�b�g
//-----------------------------------------------
int SetGear( D3DXVECTOR3 pos )
{
	// ���[�J���ϐ�
	//---------------------------------
	LPDIRECT3DDEVICE9 pDevice = NULL;		// �f�o�C�X�I�u�W�F�N�g
	
	// �f�o�C�X�擾
	//------------------------------------
	pDevice = GetDevice();

	for ( int i = 0; i < GEAR_MAX; i++ )
	{
		if ( g_aGear[ i ].bUse == false )
		{
			// �ʒu�̐ݒ�
			g_aGear[ i ].pos = pos;
			g_aGear[ i ].rot = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
			g_aGear[ i ].scl = D3DXVECTOR3( 1.0f, 1.0f, 1.0f );

			g_aGear[ i ].move = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );

			// �����蔻��p
			g_aGear[ i ].rect.pos = pos;
			g_aGear[ i ].rect.harfSize = D3DXVECTOR3( GEAR_WIDTH, GEAR_HEIGHT, GEAR_HEIGHT );

			// ���f�������R�s�[
			//g_aGear[ i ].numMatModel = numMatModelInit;
			//g_aGear[ i ].pBuffMatModel = pBuffMatModelInit;
			//g_aGear[ i ].pMeshModel = pMeshModelInit;

			{
				// x�t�@�C���̓ǂݍ���
				//------------------------------------
				D3DXLoadMeshFromX("data/MODEL/gear.x",		// �ǂݍ��ރt�@�C����
								  D3DXMESH_SYSTEMMEM,							// 
								  pDevice,										// 
								  NULL,											// 
								  &g_aGear[ i ].pBuffMatModel,			// 
								  NULL,											// 
								  &g_aGear[ i ].numMatModel,			// 
								  &g_aGear[ i ].pMeshModel );
			}

			// ��]�t���O
			g_aGear[ i ].bRotation = false;

			g_aGear[ i ].bUse = true;
			
			// �����������𑝂₷
			g_nCreateGearNum++;

			// �C���f�b�N�X��Ԃ�
			return i;
		}
	}

	return GEAR_MAX;
}

//-----------------------------------------------
// �G�f�B�b�g�X�V
//-----------------------------------------------
void EditGear( void )
{
	if ( GetKeyboardTrigger( DIK_B ) == true && g_bGearSelect == false )
	{
		// �p�x��������
		g_aGear[ g_nMoveGear ].rot.y = 0.0f;

		// ���Ԃ̐���
		g_nMoveGear = SetGear( D3DXVECTOR3( 0.0f, 20.0f, 0.0f ) );
	}

	//if ( g_bMoveGear == true )
	{
		// �ړ�����
		if ( GetKeyboardTrigger( DIK_A ) == true )
		{
			g_aGear[ g_nMoveGear ].pos.x += -40.0f;
		}
		if ( GetKeyboardTrigger( DIK_D ) == true )
		{
			g_aGear[ g_nMoveGear ].pos.x += 40.0f;
		}
		if ( GetKeyboardTrigger( DIK_W ) == true )
		{
			g_aGear[ g_nMoveGear ].pos.y += 40.0f;
		}
		if ( GetKeyboardTrigger( DIK_S ) == true )
		{
			g_aGear[ g_nMoveGear ].pos.y += -40.0f;
		}

		g_aGear[ g_nMoveGear ].rect.pos = g_aGear[ g_nMoveGear ].pos;
		g_aGear[ g_nMoveGear ].rot.y += 0.025f;
	}

	// �I�����Ă��鎕�Ԃ̕ύX
	if ( GetKeyboardTrigger( DIK_UP ) == true )
	{
		// �p�x��������
		g_aGear[ g_nMoveGear ].rot.y = 0.0f;
			
		g_nMoveGear--;
		if ( g_nMoveGear < 0 )
		{
			g_nMoveGear = g_nCreateGearNum - 1;
		}
	}
	if ( GetKeyboardTrigger( DIK_DOWN ) == true )
	{
		// �p�x��������
		g_aGear[ g_nMoveGear ].rot.y = 0.0f;

		g_nMoveGear++;
		if ( g_nMoveGear > g_nCreateGearNum - 1 )
		{
			g_nMoveGear = 0;
		}
	}

	// ���Ԃ̍폜
	if ( GetKeyboardTrigger( DIK_SPACE ) == true )
	{
		g_nCreateGearNum--;
		if ( g_nCreateGearNum < 0 )
		{
			g_nCreateGearNum = 0;
		}

		for ( int i = g_nMoveGear; i < g_nCreateGearNum; i++ )
		{
			g_aGear[ i ].pos = g_aGear[ i + 1 ].pos;
		}

		g_aGear[ g_nCreateGearNum ].bUse = false;
	}

	// ���Ԉʒu�̕ۑ�
	if ( GetKeyboardTrigger( DIK_F4 ) == true )
	{
		// �t�@�C���̃I�[�v��
		FILE *fp = fopen( "GearPositionInfo.txt", "wt" );

		for (int nCnt = 0; nCnt < GEAR_MAX; nCnt++)
		{
			if ( g_aGear[ nCnt ].bUse == true )
			{
				char workChar[ 256 ] = "POS = ";
				char workFloatTrans[ 256 ];

				// �ʒu�����[�N�ɓ˂�����
				sprintf( workFloatTrans, "%.2f", g_aGear[ nCnt ].pos.x );
				strcat( workChar, workFloatTrans );		// �A��
				strcat( workChar, " " );				// �A��
				sprintf( workFloatTrans, "%.2f", g_aGear[ nCnt ].pos.y );
				strcat( workChar, workFloatTrans );		// �A��
				strcat( workChar, " " );				// �A��
				sprintf( workFloatTrans, "%.2f", g_aGear[ nCnt ].pos.z );
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
	PrintDebugProc("���Ԃ̌� : %d\n", g_nCreateGearNum);
	PrintDebugProc("���A�� *** ���Ԕԍ��̑I��");
	PrintDebugProc("�I�����Ă��鎕�Ԕԍ� : %d\n", g_nMoveGear);

	int nNum = g_nMoveGear - 10;
	if ( nNum < 0 )
	{
		nNum = 0;
	}
	for ( int i = nNum; i < nNum+ 11; i++ )
	{
		if ( g_aGear[ i ].bUse == true )
		{
			if ( i == g_nMoveGear )
			{
				PrintDebugProc("->* [ %d ] * %f %f %f\n", i, g_aGear[ i ].pos.x, g_aGear[ i ].pos.y, g_aGear[ i ].pos.z);
			}
			else
			{
				PrintDebugProc("* [ %d ] * %f %f %f\n", i, g_aGear[ i ].pos.x, g_aGear[ i ].pos.y, g_aGear[ i ].pos.z);
			}
		}
	}
}

//-----------------------------------------------
// �����蔻��̎擾
//-----------------------------------------------
GEAR *GetGear( void )
{
	return &g_aGear[ 0 ];
}

//-----------------------------------------------
// ��]�����锻��
//-----------------------------------------------
void SetGearRotation( int num, bool use )
{
	g_aGear[ num ].bRotation = use;
}

//-----------------------------------------------
// ��]�����锻��̎擾
//-----------------------------------------------
bool GetGearRotation( int num )
{
	return g_aGear[ num ].bRotation;
}

//-----------------------------------------------
// �p�x����
//-----------------------------------------------
void ZeroGearRotation( void )
{
	for (int nCnt = 0; nCnt < GEAR_MAX; nCnt++)
	{
		if ( g_aGear[ nCnt ].bUse == true )
		{
			g_aGear[ nCnt ].rot = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
		}
	}
}
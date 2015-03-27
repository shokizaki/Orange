//-----------------------------------------------
//  ���e�F�l�p�`�揈��
//  File : cube.cpp
//  ����ҁF�؍�@��
//-----------------------------------------------

//------ �C���N���[�h�t�@�C�� ------
#include "moveCube.h"
#include "camera.h"
#include "gear.h"
#include "player.h"
#include "sheet.h"

//------ �}�N����` ------
#define MOVECUBE_WIDTH ( 15.0f )
#define MOVECUBE_HEIGHT ( 15.0f )

//------ �O���[�o���ϐ� ------
MOVECUBE g_aMoveCube[ MOVECUBE_MAX ];

bool g_bMoveMoveCube = false;	// �I�����Ă��邩�ǂ���
bool g_bMoveCubeSelect = false;	// �I�����Ă��邩�ǂ����i�������鎞�̔���j
int g_nMoveMoveCube = 0;		// �I�����Ă���ԃL���[�u�̃C���f�b�N�X
int g_nCreateMoveCubeNum = 0;	// �������ꂽ�ԃL���[�u�̐�

//------ �����֐��錾 ------

//-----------------------------------------------
// �l�p�`�揉����
//-----------------------------------------------
void InitMoveCube( void )
{
	// ���[�J���ϐ�
	//---------------------------------
	LPDIRECT3DDEVICE9 pDevice = NULL;		// �f�o�C�X�I�u�W�F�N�g
	
	// �f�o�C�X�擾
	//------------------------------------
	pDevice = GetDevice();

	for ( int i = 0; i < MOVECUBE_MAX; i++ )
	{
		g_aMoveCube[ i ].bUse = false;
	}

	// �O���[�o���ϐ��̏�����
	g_bMoveMoveCube = false;
	g_nMoveMoveCube = 0;
	//g_bEdit = false;
	g_nCreateMoveCubeNum = 0;
	g_bMoveCubeSelect = false;

	// �ʒu�ǂݍ���
	// �t�@�C���̓ǂݍ��݊J�n
	FILE *fp = fopen( "RedCubePos.txt", "rt" );
    if( fp == NULL )
    {
        return ;
    }
	char strWork[ 256 ];
	D3DXVECTOR3 fWork;
	while ( strcmp( strWork, "END_SCRIPT" ) != 0 )
	{
		// ��s�ǂݍ���
		fscanf( fp, "%s = %f %f %f" , strWork, &fWork.x, &fWork.y, &fWork.z );

		if ( strcmp( strWork, "POS" ) == 0 )
		{
			// ���L���[�u����
			SetMoveCube( fWork );
		}
	}
    fclose(fp);
}

//-----------------------------------------------
// �l�p�`��I������
//-----------------------------------------------
void UninitMoveCube( void )
{
	for (int nCnt = 0; nCnt < MOVECUBE_MAX; nCnt++)
	{
		if ( g_aMoveCube[ nCnt ].bUse == true )
		{
			// ���b�V�����̉���Ə�����
			//------------------------------------
			if (g_aMoveCube[ nCnt ].pMeshModel != NULL)
			{ 
				g_aMoveCube[ nCnt ].pMeshModel ->Release();
				g_aMoveCube[ nCnt ].pMeshModel = NULL;
			}
	
			// �}�e���A�����̉���Ə�����
			//------------------------------------
			if (g_aMoveCube[ nCnt ].pBuffMatModel != NULL)
			{ 
				g_aMoveCube[ nCnt ].pBuffMatModel ->Release();
				g_aMoveCube[ nCnt ].pBuffMatModel = NULL;
			}
		}
	}
}

//-----------------------------------------------
// �l�p�`��X�V����
//-----------------------------------------------
void UpdateMoveCube( void )
{
	// ���[�J���ϐ�
	GEAR *pGear = GetGear();

	{
		// �ԃL���[�u�ʒu�̕ۑ�
		if ( GetKeyboardTrigger( DIK_F5 ) == true )
		{
			// �t�@�C���̃I�[�v��
			FILE *fp = fopen( "MoveCubePositionInfo.txt", "wt" );

			for (int nCnt = 0; nCnt < MOVECUBE_MAX; nCnt++)
			{
				if ( g_aMoveCube[ nCnt ].bUse == true )
				{
					char workChar[ 256 ] = "POS = ";
					char workFloatTrans[ 256 ];

					// �ʒu�����[�N�ɓ˂�����
					sprintf( workFloatTrans, "%.2f", g_aMoveCube[ nCnt ].pos.x );
					strcat( workChar, workFloatTrans );		// �A��
					strcat( workChar, " " );				// �A��
					sprintf( workFloatTrans, "%.2f", g_aMoveCube[ nCnt ].pos.y );
					strcat( workChar, workFloatTrans );		// �A��
					strcat( workChar, " " );				// �A��
					sprintf( workFloatTrans, "%.2f", g_aMoveCube[ nCnt ].pos.z );
					strcat( workChar, workFloatTrans );		// �A��
					strcat( workChar, "\n" );				// �A��

					// �����o��
					fputs( workChar, fp );
				}
			}

			// �t�@�C���̃N���[�Y
			fclose( fp );
		}

		for (int nCnt = 0; nCnt < MOVECUBE_MAX; nCnt++)
		{
			if ( g_aMoveCube[ nCnt ].bUse == true )
			{
				if ( g_aMoveCube[ nCnt ].nGearIndex != GEAR_MAX )
				{
					for ( int i = 0; i < GEAR_MAX; i++ )
					{
						if ( ( pGear + g_aMoveCube[ nCnt ].nGearIndex ) ->bRotation == true )
						{

						}
					}
				}
			}
		}		
	}
}

//-----------------------------------------------
// �l�p�`��`�揈��
//-----------------------------------------------
void DrawMoveCube( void )
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

	if ( GetSheet() == false )
	{
		// ���ꂼ��̃p�[�c�̍s��v�Z�ƕ`��J�n
		for (int nCnt = 0; nCnt < MOVECUBE_MAX; nCnt++)
		{
			if ( g_aMoveCube[ nCnt ].bUse == true )
			{
				D3DXMatrixIdentity( &g_aMoveCube[ nCnt ].mtxWorld );	// �t�H�[�}�b�g�̏�����
				D3DXMatrixIdentity( &mtxScl );						// �s��̏�����
				D3DXMatrixIdentity( &mtxRot );						// �s��̏�����
				D3DXMatrixIdentity( &mtxTranslate );				// �s��̏�����

				// �X�P�[���𔽉f
				D3DXMatrixScaling(&mtxScl, g_aMoveCube[ nCnt ].scl.x, g_aMoveCube[ nCnt ].scl.y, g_aMoveCube[ nCnt ].scl.z);
				D3DXMatrixMultiply(&g_aMoveCube[ nCnt ].mtxWorld, &g_aMoveCube[ nCnt ].mtxWorld, &mtxScl);

				// �����𔽉f
				D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aMoveCube[ nCnt ].rot.y, g_aMoveCube[ nCnt ].rot.x, g_aMoveCube[ nCnt ].rot.z);
				D3DXMatrixMultiply(&g_aMoveCube[ nCnt ].mtxWorld, &g_aMoveCube[ nCnt ].mtxWorld, &mtxRot);

				// �ʒu�𔽉f
				D3DXMatrixTranslation(&mtxTranslate, g_aMoveCube[ nCnt ].pos.x, g_aMoveCube[ nCnt ].pos.y, g_aMoveCube[ nCnt ].pos.z);
				D3DXMatrixMultiply(&g_aMoveCube[ nCnt ].mtxWorld, &g_aMoveCube[ nCnt ].mtxWorld, &mtxTranslate);

				// �ݒ�
				pDevice ->SetTransform(D3DTS_WORLD, &g_aMoveCube[ nCnt ].mtxWorld);

				// ���݂̃}�e���A������ۑ�
				pDevice ->GetMaterial( &matDef );

				// �o�b�t�@�ւ̃|�C���^���擾
				pMat = (D3DXMATERIAL*)g_aMoveCube[ nCnt ].pBuffMatModel ->GetBufferPointer();

				/*strSrc = strrchr(pMat ->pTextureFilename, '\\') + 1;

				strcat(strDest, strSrc);

				D3DXCreateTextureFromFile(pDevice, strDest, &g_pTextureModel[1]);*/
			
				// �}�e���A���̐��������[�v
				for (int nCntMat = 0; nCntMat < (int)g_aMoveCube[ nCnt ].numMatModel; nCntMat++)
				{
					pDevice ->SetMaterial( &pMat[ nCntMat ].MatD3D );			// �}�e���A���̐ݒ�
					pDevice ->SetTexture( 0, g_aMoveCube[ nCnt ].pTexture );							// �e�N�X�`���̃Z�b�g
					g_aMoveCube[ nCnt ].pMeshModel ->DrawSubset( nCntMat );		// �`��
				}
			
				// �}�e���A�������ɖ߂�
				//------------------------------------
				pDevice ->SetMaterial( &matDef );
			}
		}
	}
}

//-----------------------------------------------
// �l�p�Z�b�g
//-----------------------------------------------
int SetMoveCube( D3DXVECTOR3 pos )
{
	// ���[�J���ϐ�
	//---------------------------------
	LPDIRECT3DDEVICE9 pDevice = NULL;		// �f�o�C�X�I�u�W�F�N�g
	
	// �f�o�C�X�擾
	//------------------------------------
	pDevice = GetDevice();

	for ( int i = 0; i < MOVECUBE_MAX; i++ )
	{
		if ( g_aMoveCube[ i ].bUse == false )
		{
			// �ʒu�̐ݒ�
			g_aMoveCube[ i ].pos = pos;
			g_aMoveCube[ i ].initPos = pos;
			g_aMoveCube[ i ].targetPos = pos;
			g_aMoveCube[ i ].rot = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
			g_aMoveCube[ i ].scl = D3DXVECTOR3( 1.0f, 1.0f, 1.0f );

			g_aMoveCube[ i ].move = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );

			g_aMoveCube[ i ].nGearIndex = GEAR_MAX;

			// �����蔻��p
			g_aMoveCube[ i ].rect.pos = pos;
			g_aMoveCube[ i ].rect.harfSize = D3DXVECTOR3( MOVECUBE_WIDTH, MOVECUBE_HEIGHT, MOVECUBE_HEIGHT );

			// ���f�������R�s�[
			//g_aMoveCube[ i ].numMatModel = numMatModelInit;
			//g_aMoveCube[ i ].pBuffMatModel = pBuffMatModelInit;
			//g_aMoveCube[ i ].pMeshModel = pMeshModelInit;

			{
				// x�t�@�C���̓ǂݍ���
				//------------------------------------
				D3DXLoadMeshFromX("data/MODEL/Block_Red.x",		// �ǂݍ��ރt�@�C����
								  D3DXMESH_SYSTEMMEM,							// 
								  pDevice,										// 
								  NULL,											// 
								  &g_aMoveCube[ i ].pBuffMatModel,			// 
								  NULL,											// 
								  &g_aMoveCube[ i ].numMatModel,			// 
								  &g_aMoveCube[ i ].pMeshModel );
			}

			// �e�N�X�`���̓ǂݍ���
			//------------------------------------
			D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Block.jpg", &g_aMoveCube[ i ].pTexture);

			g_aMoveCube[ i ].bUse = true;
			
			// �����������𑝂₷
			g_nCreateMoveCubeNum++;

			// �C���f�b�N�X��Ԃ�
			return i;
		}
	}

	return MOVECUBE_MAX;
}

//-----------------------------------------------
// �G�f�B�b�g�X�V
//-----------------------------------------------
void EditMoveCube( void )
{
	if ( GetKeyboardTrigger( DIK_B ) == true && g_bMoveCubeSelect == false )
	{
		// �p�x��������
		g_aMoveCube[ g_nMoveMoveCube ].rot.y = 0.0f;

		// �ԃL���[�u�̐���
		g_nMoveMoveCube = SetMoveCube( D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );
	}

	//if ( g_bMoveMoveCube == true )
	{
		// �ړ�����
		if ( GetKeyboardTrigger( DIK_A ) == true )
		{
			g_aMoveCube[ g_nMoveMoveCube ].pos.x += -40.0f;
		}
		if ( GetKeyboardTrigger( DIK_D ) == true )
		{
			g_aMoveCube[ g_nMoveMoveCube ].pos.x += 40.0f;
		}
		if ( GetKeyboardTrigger( DIK_W ) == true )
		{
			g_aMoveCube[ g_nMoveMoveCube ].pos.y += 40.0f;
		}
		if ( GetKeyboardTrigger( DIK_S ) == true )
		{
			g_aMoveCube[ g_nMoveMoveCube ].pos.y += -40.0f;
		}

		g_aMoveCube[ g_nMoveMoveCube ].rect.pos = g_aMoveCube[ g_nMoveMoveCube ].pos;
		g_aMoveCube[ g_nMoveMoveCube ].rot.y += 0.025f;
	}

	// �I�����Ă���ԃL���[�u�̕ύX
	if ( GetKeyboardTrigger( DIK_UP ) == true )
	{
		// �p�x��������
		g_aMoveCube[ g_nMoveMoveCube ].rot.y = 0.0f;
			
		g_nMoveMoveCube--;
		if ( g_nMoveMoveCube < 0 )
		{
			g_nMoveMoveCube = g_nCreateMoveCubeNum - 1;
		}
	}
	if ( GetKeyboardTrigger( DIK_DOWN ) == true )
	{
		// �p�x��������
		g_aMoveCube[ g_nMoveMoveCube ].rot.y = 0.0f;

		g_nMoveMoveCube++;
		if ( g_nMoveMoveCube > g_nCreateMoveCubeNum - 1 )
		{
			g_nMoveMoveCube = 0;
		}
	}

	// �ԃL���[�u�̍폜
	if ( GetKeyboardTrigger( DIK_SPACE ) == true )
	{
		g_nCreateMoveCubeNum--;
		if ( g_nCreateMoveCubeNum < 0 )
		{
			g_nCreateMoveCubeNum = 0;
		}

		for ( int i = g_nMoveMoveCube; i < g_nCreateMoveCubeNum; i++ )
		{
			g_aMoveCube[ i ].pos = g_aMoveCube[ i + 1 ].pos;
			g_aMoveCube[ i ].rect.pos = g_aMoveCube[ i + 1 ].pos;
		}

		g_aMoveCube[ g_nCreateMoveCubeNum ].bUse = false;
			
	}

	// �ԃL���[�u�ʒu�̕ۑ�
	if ( GetKeyboardTrigger( DIK_F5 ) == true )
	{
		// �t�@�C���̃I�[�v��
		FILE *fp = fopen( "MoveCubePositionInfo.txt", "wt" );

		for (int nCnt = 0; nCnt < MOVECUBE_MAX; nCnt++)
		{
			if ( g_aMoveCube[ nCnt ].bUse == true )
			{
				char workChar[ 256 ] = "POS = ";
				char workFloatTrans[ 256 ];

				// �ʒu�����[�N�ɓ˂�����
				sprintf( workFloatTrans, "%.2f", g_aMoveCube[ nCnt ].pos.x );
				strcat( workChar, workFloatTrans );		// �A��
				strcat( workChar, " " );				// �A��
				sprintf( workFloatTrans, "%.2f", g_aMoveCube[ nCnt ].pos.y );
				strcat( workChar, workFloatTrans );		// �A��
				strcat( workChar, " " );				// �A��
				sprintf( workFloatTrans, "%.2f", g_aMoveCube[ nCnt ].pos.z );
				strcat( workChar, workFloatTrans );		// �A��
				strcat( workChar, "\n" );				// �A��

				// �����o��
				fputs( workChar, fp );
			}
		}

		// �t�@�C���̃N���[�Y
		fclose( fp );
	}

	PrintDebugProc("[ B�L�[�ŐԃL���[�u���� ]\n");
	PrintDebugProc("[ SPACE�L�[�őI�����Ă���ԃL���[�u���폜 ]\n");
	PrintDebugProc("�ԃL���[�u�̌� : %d\n", g_nCreateMoveCubeNum);
	PrintDebugProc("���A�� *** �ԃL���[�u�ԍ��̑I��");
	PrintDebugProc("�I�����Ă���ԃL���[�u�ԍ� : %d\n", g_nMoveMoveCube);

	int nNum = g_nMoveMoveCube - 10;
	if ( nNum < 0 )
	{
		nNum = 0;
	}
	for ( int i = nNum; i < nNum + 11; i++ )
	{
		if ( g_aMoveCube[ i ].bUse == true )
		{
			if ( i == g_nMoveMoveCube )
			{
				PrintDebugProc("->* [ %d ] * %f %f %f\n", i, g_aMoveCube[ i ].pos.x, g_aMoveCube[ i ].pos.y, g_aMoveCube[ i ].pos.z);
			}
			else
			{
				PrintDebugProc("* [ %d ] * %f %f %f\n", i, g_aMoveCube[ i ].pos.x, g_aMoveCube[ i ].pos.y, g_aMoveCube[ i ].pos.z);
			}
		}
	}
}

//-----------------------------------------------
// �����蔻��̎擾
//-----------------------------------------------
MOVECUBE *GetMoveCube( void )
{
	return &g_aMoveCube[ 0 ];
}

//-----------------------------------------------
// �p�x����
//-----------------------------------------------
void ZeroMoveCubeRotation( void )
{
	for (int nCnt = 0; nCnt < MOVECUBE_MAX; nCnt++)
	{
		if ( g_aMoveCube[ nCnt ].bUse == true )
		{
			g_aMoveCube[ nCnt ].rot = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
		}
	}
}
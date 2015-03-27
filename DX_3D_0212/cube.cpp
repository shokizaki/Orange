//-----------------------------------------------
//  ���e�F�l�p�`�揈��
//  File : cube.cpp
//  ����ҁF�؍�@��
//-----------------------------------------------

//------ �C���N���[�h�t�@�C�� ------
#include "cube.h"
#include "camera.h"
#include "player.h"
#include "sheet.h"

//------ �}�N����` ------
#define CUBE_WIDTH ( 15.0f )
#define CUBE_HEIGHT ( 15.0f )

//------ �O���[�o���ϐ� ------
CUBE g_aCube[ CUBE_MAX ];

bool g_bMoveCube = false;	// �I�����Ă��邩�ǂ���
bool g_bCubeSelect = false;	// �I�����Ă��邩�ǂ����i�������鎞�̔���j
int g_nMoveCube = 0;		// �I�����Ă���L���[�u�̃C���f�b�N�X
int g_nCreateCubeNum = 0;	// �������ꂽ�L���[�u�̐�

//------ �����֐��錾 ------

//-----------------------------------------------
// �l�p�`�揉����
//-----------------------------------------------
void InitCube( void )
{
	// ���[�J���ϐ�
	//---------------------------------
	LPDIRECT3DDEVICE9 pDevice = NULL;		// �f�o�C�X�I�u�W�F�N�g
	
	// �f�o�C�X�擾
	//------------------------------------
	pDevice = GetDevice();

	for ( int i = 0; i < CUBE_MAX; i++ )
	{
		g_aCube[ i ].bUse = false;
	}

	// �O���[�o���ϐ��̏�����
	g_bMoveCube = false;
	g_nMoveCube = 0;
	//g_bEdit = false;
	g_nCreateCubeNum = 0;
	g_bCubeSelect = false;

	// �ʒu�ǂݍ���
	// �t�@�C���̓ǂݍ��݊J�n
	FILE *fp = fopen( "CubePos.txt", "rt" );
	char strWork[ 256 ];
	D3DXVECTOR3 fWork;
	while ( strcmp( strWork, "END_SCRIPT" ) != 0 )
	{
		// ��s�ǂݍ���
		fscanf( fp, "%s = %f %f %f" , strWork, &fWork.x, &fWork.y, &fWork.z );

		if ( strcmp( strWork, "POS" ) == 0 )
		{
			// ���L���[�u����
			SetCube( fWork );
		}
	}
}

//-----------------------------------------------
// �l�p�`��I������
//-----------------------------------------------
void UninitCube( void )
{
	for (int nCnt = 0; nCnt < CUBE_MAX; nCnt++)
	{
		if ( g_aCube[ nCnt ].bUse == true )
		{
			// ���b�V�����̉���Ə�����
			//------------------------------------
			if (g_aCube[ nCnt ].pMeshModel != NULL)
			{ 
				g_aCube[ nCnt ].pMeshModel ->Release();
				g_aCube[ nCnt ].pMeshModel = NULL;
			}
	
			// �}�e���A�����̉���Ə�����
			//------------------------------------
			if (g_aCube[ nCnt ].pBuffMatModel != NULL)
			{ 
				g_aCube[ nCnt ].pBuffMatModel ->Release();
				g_aCube[ nCnt ].pBuffMatModel = NULL;
			}
		}
	}
}

//-----------------------------------------------
// �l�p�`��X�V����
//-----------------------------------------------
void UpdateCube( void )
{
	g_aCube[ 0 ].move = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );

	{
		//// �L���[�u�ʒu�̕ۑ�
		//if ( GetKeyboardTrigger( DIK_F2 ) == true )
		//{
		//	// �t�@�C���̃I�[�v��
		//	FILE *fp = fopen( "CubePositionInfo.txt", "wt" );

		//	for (int nCnt = 0; nCnt < CUBE_MAX; nCnt++)
		//	{
		//		if ( g_aCube[ nCnt ].bUse == true )
		//		{
		//			char workChar[ 256 ] = "POS = ";
		//			char workFloatTrans[ 256 ];

		//			// �ʒu�����[�N�ɓ˂�����
		//			sprintf( workFloatTrans, "%.2f", g_aCube[ nCnt ].pos.x );
		//			strcat( workChar, workFloatTrans );		// �A��
		//			strcat( workChar, " " );				// �A��
		//			sprintf( workFloatTrans, "%.2f", g_aCube[ nCnt ].pos.y );
		//			strcat( workChar, workFloatTrans );		// �A��
		//			strcat( workChar, " " );				// �A��
		//			sprintf( workFloatTrans, "%.2f", g_aCube[ nCnt ].pos.z );
		//			strcat( workChar, workFloatTrans );		// �A��
		//			strcat( workChar, "\n" );				// �A��

		//			// �����o��
		//			fputs( workChar, fp );
		//		}
		//	}

		//	// �t�@�C���̃N���[�Y
		//	fclose( fp );
		//}

		// �ړ������i�f�o�b�O�p�j
		/*if ( g_aCube[ 0 ].bUse == true )
		{
			if ( GetKeyboardPress( DIK_I ) == true )
			{
				g_aCube[ 0 ].move.y = 3.0f;
				g_aCube[ 0 ].pos.y += g_aCube[ 0 ].move.y;
				g_aCube[ 0 ].rect.pos = g_aCube[ 0 ].pos;	
			}
			if ( GetKeyboardPress( DIK_K ) == true )
			{
				g_aCube[ 0 ].move.y = -3.0f;
				g_aCube[ 0 ].pos.y += g_aCube[ 0 ].move.y;
				g_aCube[ 0 ].rect.pos = g_aCube[ 0 ].pos;	
			}
			if ( GetKeyboardPress( DIK_J ) == true )
			{
				g_aCube[ 0 ].move.x = -3.0f;
				g_aCube[ 0 ].pos.x += g_aCube[ 0 ].move.x;
				g_aCube[ 0 ].rect.pos = g_aCube[ 0 ].pos;	
			}
			if ( GetKeyboardPress( DIK_L ) == true )
			{
				g_aCube[ 0 ].move.x = 3.0f;
				g_aCube[ 0 ].pos.x += g_aCube[ 0 ].move.x;
				g_aCube[ 0 ].rect.pos = g_aCube[ 0 ].pos;	
			}
		}*/
	}
}

//-----------------------------------------------
// �l�p�`��`�揈��
//-----------------------------------------------
void DrawCube( void )
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
	if ( GetSheet() == true )
	{
		// ���ꂼ��̃p�[�c�̍s��v�Z�ƕ`��J�n
		for (int nCnt = 0; nCnt < CUBE_MAX; nCnt++)
		{
			if ( g_aCube[ nCnt ].bUse == true )
			{
				D3DXMatrixIdentity( &g_aCube[ nCnt ].mtxWorld );	// �t�H�[�}�b�g�̏�����
				D3DXMatrixIdentity( &mtxScl );						// �s��̏�����
				D3DXMatrixIdentity( &mtxRot );						// �s��̏�����
				D3DXMatrixIdentity( &mtxTranslate );				// �s��̏�����

				// �X�P�[���𔽉f
				D3DXMatrixScaling(&mtxScl, g_aCube[ nCnt ].scl.x, g_aCube[ nCnt ].scl.y, g_aCube[ nCnt ].scl.z);
				D3DXMatrixMultiply(&g_aCube[ nCnt ].mtxWorld, &g_aCube[ nCnt ].mtxWorld, &mtxScl);

				// �����𔽉f
				D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aCube[ nCnt ].rot.y, g_aCube[ nCnt ].rot.x, g_aCube[ nCnt ].rot.z);
				D3DXMatrixMultiply(&g_aCube[ nCnt ].mtxWorld, &g_aCube[ nCnt ].mtxWorld, &mtxRot);

				// �ʒu�𔽉f
				D3DXMatrixTranslation(&mtxTranslate, g_aCube[ nCnt ].pos.x, g_aCube[ nCnt ].pos.y, g_aCube[ nCnt ].pos.z);
				D3DXMatrixMultiply(&g_aCube[ nCnt ].mtxWorld, &g_aCube[ nCnt ].mtxWorld, &mtxTranslate);

				// �ݒ�
				pDevice ->SetTransform(D3DTS_WORLD, &g_aCube[ nCnt ].mtxWorld);

				// ���݂̃}�e���A������ۑ�
				pDevice ->GetMaterial( &matDef );

				// �o�b�t�@�ւ̃|�C���^���擾
				pMat = (D3DXMATERIAL*)g_aCube[ nCnt ].pBuffMatModel ->GetBufferPointer();

				/*strSrc = strrchr(pMat ->pTextureFilename, '\\') + 1;

				strcat(strDest, strSrc);

				D3DXCreateTextureFromFile(pDevice, strDest, &g_pTextureModel[1]);*/
			
				// �}�e���A���̐��������[�v
				for (int nCntMat = 0; nCntMat < (int)g_aCube[ nCnt ].numMatModel; nCntMat++)
				{
					pDevice ->SetMaterial( &pMat[ nCntMat ].MatD3D );			// �}�e���A���̐ݒ�
					pDevice ->SetTexture( 0, g_aCube[ nCnt ].pTexture );							// �e�N�X�`���̃Z�b�g
					g_aCube[ nCnt ].pMeshModel ->DrawSubset( nCntMat );		// �`��
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
int SetCube( D3DXVECTOR3 pos )
{
	// ���[�J���ϐ�
	//---------------------------------
	LPDIRECT3DDEVICE9 pDevice = NULL;		// �f�o�C�X�I�u�W�F�N�g
	
	// �f�o�C�X�擾
	//------------------------------------
	pDevice = GetDevice();

	for ( int i = 0; i < CUBE_MAX; i++ )
	{
		if ( g_aCube[ i ].bUse == false )
		{
			// �ʒu�̐ݒ�
			g_aCube[ i ].pos = pos;
			g_aCube[ i ].rot = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
			g_aCube[ i ].scl = D3DXVECTOR3( 1.0f, 1.0f, 1.0f );

			g_aCube[ i ].move = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );

			// �����蔻��p
			g_aCube[ i ].rect.pos = pos;
			g_aCube[ i ].rect.harfSize = D3DXVECTOR3( CUBE_WIDTH, CUBE_HEIGHT, CUBE_HEIGHT );

			// ���f�������R�s�[
			//g_aCube[ i ].numMatModel = numMatModelInit;
			//g_aCube[ i ].pBuffMatModel = pBuffMatModelInit;
			//g_aCube[ i ].pMeshModel = pMeshModelInit;

			{
				// x�t�@�C���̓ǂݍ���
				//------------------------------------
				D3DXLoadMeshFromX("data/MODEL/Block_Gre.x",		// �ǂݍ��ރt�@�C����
								  D3DXMESH_SYSTEMMEM,							// 
								  pDevice,										// 
								  NULL,											// 
								  &g_aCube[ i ].pBuffMatModel,			// 
								  NULL,											// 
								  &g_aCube[ i ].numMatModel,			// 
								  &g_aCube[ i ].pMeshModel );
			}

			// �e�N�X�`���̓ǂݍ���
			//------------------------------------
			D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Block.jpg", &g_aCube[ i ].pTexture);

			g_aCube[ i ].bUse = true;
			
			// �����������𑝂₷
			g_nCreateCubeNum++;

			// �C���f�b�N�X��Ԃ�
			return i;
		}
	}

	return CUBE_MAX;
}

//-----------------------------------------------
// �G�f�B�b�g�X�V
//-----------------------------------------------
void EditCube( void )
{
	if ( GetKeyboardTrigger( DIK_B ) == true && g_bCubeSelect == false )
	{
		// �p�x��������
		g_aCube[ g_nMoveCube ].rot.y = 0.0f;

		// �L���[�u�̐���
		if ( g_nCreateCubeNum == 0 )
		{
			g_nMoveCube = SetCube( D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );
		}
		else
		{
			g_nMoveCube = SetCube( D3DXVECTOR3( g_aCube[ g_nMoveCube ].pos.x + 40.0f, g_aCube[ g_nMoveCube ].pos.y, 0.0f ) );
		}
	}

	//if ( g_bMoveCube == true )
	{
		// �ړ�����
		if ( GetKeyboardTrigger( DIK_A ) == true )
		{
			g_aCube[ g_nMoveCube ].pos.x += -40.0f;
		}
		if ( GetKeyboardTrigger( DIK_D ) == true )
		{
			g_aCube[ g_nMoveCube ].pos.x += 40.0f;
		}
		if ( GetKeyboardTrigger( DIK_W ) == true )
		{
			g_aCube[ g_nMoveCube ].pos.y += 40.0f;
		}
		if ( GetKeyboardTrigger( DIK_S ) == true )
		{
			g_aCube[ g_nMoveCube ].pos.y += -40.0f;
		}

		g_aCube[ g_nMoveCube ].rect.pos = g_aCube[ g_nMoveCube ].pos;
		g_aCube[ g_nMoveCube ].rot.y += 0.025f;
	}

	// �I�����Ă���L���[�u�̕ύX
	if ( GetKeyboardTrigger( DIK_UP ) == true )
	{
		// �p�x��������
		g_aCube[ g_nMoveCube ].rot.y = 0.0f;
			
		g_nMoveCube--;
		if ( g_nMoveCube < 0 )
		{
			g_nMoveCube = g_nCreateCubeNum - 1;
		}
	}
	if ( GetKeyboardTrigger( DIK_DOWN ) == true )
	{
		// �p�x��������
		g_aCube[ g_nMoveCube ].rot.y = 0.0f;

		g_nMoveCube++;
		if ( g_nMoveCube > g_nCreateCubeNum - 1 )
		{
			g_nMoveCube = 0;
		}
	}

	// �L���[�u�̍폜
	if ( GetKeyboardTrigger( DIK_SPACE ) == true )
	{
		g_nCreateCubeNum--;
		if ( g_nCreateCubeNum < 0 )
		{
			g_nCreateCubeNum = 0;
		}

		for ( int i = g_nMoveCube; i < g_nCreateCubeNum; i++ )
		{
			g_aCube[ i ] = g_aCube[ i + 1 ];
		}

		g_aCube[ g_nCreateCubeNum ].bUse = false;
			
	}

	// �L���[�u�ʒu�̕ۑ�
	if ( GetKeyboardTrigger( DIK_F2 ) == true )
	{
		// �t�@�C���̃I�[�v��
		FILE *fp = fopen( "CubePositionInfo.txt", "wt" );

		for (int nCnt = 0; nCnt < CUBE_MAX; nCnt++)
		{
			if ( g_aCube[ nCnt ].bUse == true )
			{
				char workChar[ 256 ] = "POS = ";
				char workFloatTrans[ 256 ];

				// �ʒu�����[�N�ɓ˂�����
				sprintf( workFloatTrans, "%.2f", g_aCube[ nCnt ].pos.x );
				strcat( workChar, workFloatTrans );		// �A��
				strcat( workChar, " " );				// �A��
				sprintf( workFloatTrans, "%.2f", g_aCube[ nCnt ].pos.y );
				strcat( workChar, workFloatTrans );		// �A��
				strcat( workChar, " " );				// �A��
				sprintf( workFloatTrans, "%.2f", g_aCube[ nCnt ].pos.z );
				strcat( workChar, workFloatTrans );		// �A��
				strcat( workChar, "\n" );				// �A��

				// �����o��
				fputs( workChar, fp );
			}
		}

		// �t�@�C���̃N���[�Y
		fclose( fp );
	}

	// �f�o�b�O�\��
	PrintDebugProc("[ B�L�[�ŃL���[�u���� ]\n");
	PrintDebugProc("[ SPACE�L�[�őI�����Ă���L���[�u���폜 ]\n");
	PrintDebugProc("�L���[�u�̌� : %d\n", g_nCreateCubeNum);
	PrintDebugProc("���A�� *** �L���[�u�ԍ��̑I��");
	PrintDebugProc("�I�����Ă���L���[�u�ԍ� : %d\n", g_nMoveCube);

	int nNum = g_nMoveCube - 10;
	if ( nNum < 0 )
	{
		nNum = 0;
	}
	for ( int i = nNum; i < nNum + 11; i++ )
	{
		if ( g_aCube[ i ].bUse == true )
		{
			if ( i == g_nMoveCube )
			{
				PrintDebugProc("->* [ %d ] * %f %f %f\n", i, g_aCube[ i ].pos.x, g_aCube[ i ].pos.y, g_aCube[ i ].pos.z);
			}
			else
			{
				PrintDebugProc("* [ %d ] * %f %f %f\n", i, g_aCube[ i ].pos.x, g_aCube[ i ].pos.y, g_aCube[ i ].pos.z);
			}
		}
	}
}

//-----------------------------------------------
// �����蔻��̎擾
//-----------------------------------------------
CUBE *GetCube( void )
{
	return &g_aCube[ 0 ];
}

//-----------------------------------------------
// �p�x����
//-----------------------------------------------
void ZeroCubeRotation( void )
{
	for (int nCnt = 0; nCnt < CUBE_MAX; nCnt++)
	{
		if ( g_aCube[ nCnt ].bUse == true )
		{
			g_aCube[ nCnt ].rot = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
		}
	}
}
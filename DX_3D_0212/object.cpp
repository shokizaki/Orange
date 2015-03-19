//===============================================
//
//  ���e   �F �I�u�W�F�N�g����
//  File   �F object.cpp
//  ����� �F �؍�@��
//
//===============================================

// �C���N���[�h�t�@�C��
#include "object.h"
#include "model.h"
#include "enemy.h"
#include "billboard.h"
#include "score.h"
#include "stringDraw.h"
#include "camera.h"
#include "texture2D.h"
#include "miniMap.h"

// �}�N����`
#define OBJECT_A_FILEPASS	"data/MODEL/box000.x"
#define OBJECT_A_TEXPASS	"data/TEXTURE/exclamation000.jpg"
#define OBJECT_A_WIDTH		( 25.0f )
#define OBJECT_A_HEIGHT_Z	( 25.0f )
#define OBJECT_A_HEIGHT		( 25.0f )

#define OBJECT_B_FILEPASS	"data/MODEL/box000.x"
#define OBJECT_B_TEXPASS	"data/TEXTURE/question000.jpg"
#define OBJECT_B_WIDTH		( 25.0f )
#define OBJECT_B_HEIGHT_Z	( 25.0f )

#define OBJECT_C_FILEPASS	"data/MODEL/wood001.x"
#define OBJECT_C_WIDTH		( 15.0f )
#define OBJECT_C_HEIGHT_Z	( 15.0f )

#define OBJECT_D_FILEPASS	"data/MODEL/wood002.x"
#define OBJECT_D_WIDTH		( 50.0f )
#define OBJECT_D_HEIGHT_Z	( 50.0f )

#define OBJECT_E_FILEPASS	"data/MODEL/wood003.x"
#define OBJECT_E_WIDTH		( 40.0f )
#define OBJECT_E_HEIGHT_Z	( 40.0f )

#define DIAMETER			( 100 )

// �O���[�o���ϐ�
OBJECT g_object[ MAX_OBJECT ];
COL_RECT rectObject[ MAX_OBJECT ];
bool g_bDirection = false;
int g_nIndexObject = 0;

//===============================================
//
// �I�u�W�F�N�g�Ɋւ��鏉����
//
//===============================================
void InitObject(void)
{
	// �e�N�X�`���|�C���^�̏�����
	for ( int i = 0; i < MAX_OBJECT; i++ )
	{
		g_object[ i ].pTextureObject = NULL;
		g_object[ i ].bUse = false;
	}

	// �I�u�W�F�N�g�̃Z�b�g

	// �{�b�N�X
	SetObject( D3DXVECTOR3( 0.0f, 0.0f, -100.0f ), OBJECTTYPE_A );
	
	// ���̗�
	SetObject( D3DXVECTOR3( -1000.0f, 0.0f, 1000.0f ), OBJECTTYPE_E );
	SetObject( D3DXVECTOR3( -800.0f, 0.0f, 800.0f ),  OBJECTTYPE_E );
	SetObject( D3DXVECTOR3( -600.0f, 0.0f, 1000.0f ), OBJECTTYPE_E );
	SetObject( D3DXVECTOR3( -400.0f, 0.0f, 800.0f ),  OBJECTTYPE_E );
	SetObject( D3DXVECTOR3( -200.0f, 0.0f, 1000.0f ), OBJECTTYPE_E );
	SetObject( D3DXVECTOR3( 0.0f,    0.0f, 800.0f ),  OBJECTTYPE_E );
	SetObject( D3DXVECTOR3( 1000.0f, 0.0f, 1000.0f ), OBJECTTYPE_E );
	SetObject( D3DXVECTOR3( 800.0f,  0.0f, 800.0f ),  OBJECTTYPE_E );
	SetObject( D3DXVECTOR3( 600.0f,  0.0f, 1000.0f ), OBJECTTYPE_E );
	SetObject( D3DXVECTOR3( 400.0f,  0.0f, 800.0f ),  OBJECTTYPE_E );
	SetObject( D3DXVECTOR3( 200.0f,  0.0f, 1000.0f ), OBJECTTYPE_E );

	// ��O�̗�
	SetObject( D3DXVECTOR3( -1000.0f, 0.0f, -1000.0f ), OBJECTTYPE_E );
	SetObject( D3DXVECTOR3( -800.0f, 0.0f, -800.0f ),  OBJECTTYPE_E );
	SetObject( D3DXVECTOR3( -600.0f, 0.0f, -1000.0f ), OBJECTTYPE_E );
	SetObject( D3DXVECTOR3( -400.0f, 0.0f, -800.0f ),  OBJECTTYPE_E );
	SetObject( D3DXVECTOR3( -200.0f, 0.0f, -1000.0f ), OBJECTTYPE_E );
	SetObject( D3DXVECTOR3( 0.0f,    0.0f, -800.0f ),  OBJECTTYPE_E );
	SetObject( D3DXVECTOR3( 1000.0f, 0.0f, -1000.0f ), OBJECTTYPE_E );
	SetObject( D3DXVECTOR3( 800.0f,  0.0f, -800.0f ),  OBJECTTYPE_E );
	SetObject( D3DXVECTOR3( 600.0f,  0.0f, -1000.0f ), OBJECTTYPE_E );
	SetObject( D3DXVECTOR3( 400.0f,  0.0f, -800.0f ),  OBJECTTYPE_E );
	SetObject( D3DXVECTOR3( 200.0f,  0.0f, -1000.0f ), OBJECTTYPE_E );
}

//===============================================
//
// �I�u�W�F�N�g�Ɋւ���I������
//
//===============================================
void UninitObject(void)
{
	for ( int i = 0; i < MAX_OBJECT; i++ )
	{
		if ( g_object[ i ].pMeshObject != NULL )
		{ 
			// ���b�V���I�u�W�F�N�g�̉��
			//------------------------------------
			g_object[ i ].pMeshObject -> Release();
			g_object[ i ].pMeshObject = NULL;
		}
				// 
		//------------------------------------
		if ( g_object[ i ].pBuffMatObject != NULL )
		{ 
			// �I�u�W�F�N�g�o�b�t�@�̉��
			g_object[ i ].pBuffMatObject -> Release();
			g_object[ i ].pBuffMatObject = NULL;
		}
	}
	
	for (int i = 0; i < MAX_OBJECT; i++)
	{
		if ( g_object[ i ].pTextureObject != NULL )
		{ 
			// �g�����e�N�X�`���̉���Ə�����
			//------------------------------------
			g_object[ i ].pTextureObject ->Release();
			g_object[ i ].pTextureObject = NULL;
		}
	}
}

//===============================================
//
// �I�u�W�F�N�g�Ɋւ���X�V����
//
//===============================================
void UpdateObject(void)
{
	// ���[�J���ϐ�
	MODEL_INFO *pModelPlayer = GetModelInfo();
	MODEL_INFO *pModelEnemy = GetEnemyInfo();
	COL_RECT *pRectPlayer = GetRectPlayer();
	COL_RECT *pRectEnemy = GetRectEnemy();
	CAMERA *pCamera = GetCamera();

	for ( int i = 0; i < MAX_OBJECT; i++ )
	{
		if ( g_object[ i ].bUse == true )
		{
			if ( pModelPlayer ->direction == DIRECTION_DEPTH || pModelPlayer ->direction == DIRECTION_FRONT )
			{
				// �����蔻��
				if ( ColRect( &rectObject[ i ], pRectPlayer ) == true )
				{
					if ( ( GetKeyboardTrigger( DIK_K ) == true /*|| GetGamePadTrigger( GAMEPAD_A )*/ || GetPadElecomTrigger( PAD_4 ) ) && g_object[ i ].type == OBJECTTYPE_A )
					{
						if ( GetCatchEnemy() > 0 )
						{
							int nCatchEnemy = GetCatchEnemy();
							AddScore( nCatchEnemy * ( DIAMETER + nCatchEnemy ) );
							InitCatchEnemy();
							SetBillboard( D3DXVECTOR3( g_object[ i ].pos.x, g_object[ i ].pos.y + OBJECT_A_HEIGHT, g_object[ i ].pos.z ), TEXTUREANIM_CHARGE, 0.0f, 75.0f, 75.0f );
						
							if ( GetStringMessage() == RELEASETRY )
							{
								SetStringMessage( ATTENTIONSPEED );
							}
						}

						PlaySound( ABSORB_SE );
					}

					if ( ColRectZ_Depth( pRectPlayer, &rectObject[ i ] ) == true )
					{
						pModelPlayer ->posParts[ 0 ].z = pModelPlayer ->posOld.z;
						pModelPlayer ->move.z = -pModelPlayer ->move.z * 0.1f;

						// �����蔻��p�ϐ����X�V
						pRectPlayer ->pos.z = pModelPlayer ->posParts[ 0 ].z;
					}
					if ( ColRectZ_Front( pRectPlayer, &rectObject[ i ] ) == true )
					{
						pModelPlayer ->posParts[ 0 ].z = pModelPlayer ->posOld.z;
						pModelPlayer ->move.z = -pModelPlayer ->move.z * 0.1f;

						// �����蔻��p�ϐ����X�V
						pRectPlayer ->pos.z = pModelPlayer ->posParts[ 0 ].z;
					}
				}

				// �����蔻��
				if ( ColRect( &rectObject[ i ], pRectPlayer ) == true )
				{
					if (( GetKeyboardTrigger( DIK_K ) == true /*|| GetGamePadTrigger( GAMEPAD_A )*/ || GetPadElecomTrigger( PAD_4 ) ) && g_object[ i ].type == OBJECTTYPE_A )
					{
						if ( GetCatchEnemy() > 0 )
						{
							int nCatchEnemy = GetCatchEnemy();
							AddScore( nCatchEnemy * ( DIAMETER + nCatchEnemy ) );
							InitCatchEnemy();
							SetBillboard( D3DXVECTOR3( g_object[ i ].pos.x, g_object[ i ].pos.y + OBJECT_A_HEIGHT, g_object[ i ].pos.z ), TEXTUREANIM_CHARGE, 0.0f, 75.0f, 75.0f );
						
							if ( GetStringMessage() == RELEASETRY )
							{
								SetStringMessage( ATTENTIONSPEED );
							}
						}

						PlaySound( ABSORB_SE );
					}
				
					if ( ColRectX_Right( pRectPlayer, &rectObject[ i ] ) == true )
					{
						pModelPlayer ->posParts[ 0 ].x = pModelPlayer ->posOld.x;
						pModelPlayer ->move.x = -pModelPlayer ->move.x * 0.1f;

						// �����蔻��p�ϐ����X�V
						pRectPlayer ->pos.x = pModelPlayer ->posParts[ 0 ].x;
					}
					if ( ColRectX_Left( pRectPlayer, &rectObject[ i ] ) == true )
					{
						pModelPlayer ->posParts[ 0 ].x = pModelPlayer ->posOld.x;
						pModelPlayer ->move.x = -pModelPlayer ->move.x * 0.1f;

						// �����蔻��p�ϐ����X�V
						pRectPlayer ->pos.x = pModelPlayer ->posParts[ 0 ].x;
					}
				}

				
			}
			else
			{
				// �����蔻��
				if ( ColRect( &rectObject[ i ], pRectPlayer ) == true )
				{
					if ( ( GetKeyboardTrigger( DIK_K ) == true /*|| GetGamePadTrigger( GAMEPAD_A )*/ || GetPadElecomTrigger( PAD_4 ) ) && g_object[ i ].type == OBJECTTYPE_A )
					{
						if ( GetCatchEnemy() > 0 )
						{
							int nCatchEnemy = GetCatchEnemy();
							AddScore( nCatchEnemy * ( DIAMETER + nCatchEnemy ) );
							InitCatchEnemy();
							SetBillboard( D3DXVECTOR3( g_object[ i ].pos.x, g_object[ i ].pos.y + OBJECT_A_HEIGHT, g_object[ i ].pos.z ), TEXTUREANIM_CHARGE, 0.0f, 75.0f, 75.0f );
						
							if ( GetStringMessage() == RELEASETRY )
							{
								SetStringMessage( ATTENTIONSPEED );
							}
						}

						PlaySound( ABSORB_SE );
					}
				
					if ( ColRectX_Right( pRectPlayer, &rectObject[ i ] ) == true )
					{
						pModelPlayer ->posParts[ 0 ].x = pModelPlayer ->posOld.x;
						pModelPlayer ->move.x = -pModelPlayer ->move.x * 0.1f;

						// �����蔻��p�ϐ����X�V
						pRectPlayer ->pos.x = pModelPlayer ->posParts[ 0 ].x;
					}
					if ( ColRectX_Left( pRectPlayer, &rectObject[ i ] ) == true )
					{
						pModelPlayer ->posParts[ 0 ].x = pModelPlayer ->posOld.x;
						pModelPlayer ->move.x = -pModelPlayer ->move.x * 0.1f;

						// �����蔻��p�ϐ����X�V
						pRectPlayer ->pos.x = pModelPlayer ->posParts[ 0 ].x;
					}
				}

				// �����蔻��
				if ( ColRect( &rectObject[ i ], pRectPlayer ) == true )
				{
					if ( ( GetKeyboardTrigger( DIK_K ) == true /*|| GetGamePadTrigger( GAMEPAD_A )*/ || GetPadElecomTrigger( PAD_4 ) ) && g_object[ i ].type == OBJECTTYPE_A )
					{
						if ( GetCatchEnemy() > 0 )
						{
							int nCatchEnemy = GetCatchEnemy();
							AddScore( nCatchEnemy * ( DIAMETER + nCatchEnemy ) );
							InitCatchEnemy();
							SetBillboard( D3DXVECTOR3( g_object[ i ].pos.x, g_object[ i ].pos.y + OBJECT_A_HEIGHT, g_object[ i ].pos.z ), TEXTUREANIM_CHARGE, 0.0f, 75.0f, 75.0f );
						
							if ( GetStringMessage() == RELEASETRY )
							{
								SetStringMessage( ATTENTIONSPEED );
							}
						}

						PlaySound( ABSORB_SE );
					}

					if ( ColRectZ_Depth( pRectPlayer, &rectObject[ i ] ) == true )
					{
						pModelPlayer ->posParts[ 0 ].z = pModelPlayer ->posOld.z;
						pModelPlayer ->move.z = -pModelPlayer ->move.z * 0.1f;

						// �����蔻��p�ϐ����X�V
						pRectPlayer ->pos.z = pModelPlayer ->posParts[ 0 ].z;
					}
					if ( ColRectZ_Front( pRectPlayer, &rectObject[ i ] ) == true )
					{
						pModelPlayer ->posParts[ 0 ].z = pModelPlayer ->posOld.z;
						pModelPlayer ->move.z = -pModelPlayer ->move.z * 0.1f;

						// �����蔻��p�ϐ����X�V
						pRectPlayer ->pos.z = pModelPlayer ->posParts[ 0 ].z;
					}
				}
			}

			if ( g_object[ i ].type == OBJECTTYPE_A )
			{
				// �~�j�}�b�v�Z�b�g
				SetMiniMap( D3DXVECTOR3( g_object[ i ].pos.x, g_object[ i ].pos.y, g_object[ i ].pos.z ), MINIMAPTYPE_OBJECT, 0.0f );
			}
		}
	}

	// �{�b�N�X�̈ʒu�q���g
	//if ( g_object[ 0 ].pos.x >= pCamera ->posCameraP.x + SCREEN_WIDTH / 2 )
	//{
	//	if ( g_bDirection == false )
	//	{
	//		g_nIndexObject = SetTexture2D( D3DXVECTOR3( SCREEN_WIDTH - 40.0f, SCREEN_HEIGHT / 2 - 50.0f, 0.0f ), 40.0f, 40.0f, TEXTURE2DTYPE_BOX );
	//		g_bDirection = true;
	//	}
	//}
	//
	//else if ( g_object[ 0 ].pos.x <= pCamera ->posCameraP.x - SCREEN_WIDTH / 2 )
	//{
	//	if ( g_bDirection == false )
	//	{
	//		g_nIndexObject = SetTexture2D( D3DXVECTOR3( 40.0f, SCREEN_HEIGHT / 2 - 50.0f, 0.0f ), 40.0f, 40.0f, TEXTURE2DTYPE_BOXREVERS );
	//		g_bDirection = true;
	//	}	
	//}

	//else
	//{
	//	if ( g_bDirection == true )
	//	{
	//		g_bDirection = false;
	//		SetTexture2DUse( g_nIndexObject );
	//	}
	//}

	PrintDebugProc( "%f %f", g_object[ 0 ].pos.x, g_object[ 0 ].pos.z );
}

//===============================================
//
// �I�u�W�F�N�g�Ɋւ���`�揈��
//
//===============================================
void DrawObject(void)
{
	// ���[�J���ϐ�
	//------------------------------------
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// �f�o�C�X�I�u�W�F�N�g
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;		// �v�Z�p
	D3DXMATRIX pMtxWorld;
	D3DXMATERIAL *pMat;								// 
	D3DMATERIAL9 matDef;							// ���̃}�e���A�����

	for (int nCnt = 0; nCnt < MAX_OBJECT; nCnt++)
	{
		if ( g_object[ nCnt ].bUse == true )
		{
			// ���[���h�}�g���b�N�X�̐ݒ�
			//------------------------------------
			D3DXMatrixIdentity(&g_object[ nCnt ].mtxWorld);			// �t�H�[�}�b�g�̏�����
			D3DXMatrixIdentity(&mtxScl);
			D3DXMatrixIdentity(&mtxRot);
			D3DXMatrixIdentity(&mtxTranslate);

			//==============================================
			// 
			// ���f���鏇�Ԃ͌���I�I
			// 
			//==============================================

			// �X�P�[���𔽉f
			D3DXMatrixScaling(&mtxScl, g_object[ nCnt ].scl.x, g_object[ nCnt ].scl.y, g_object[ nCnt ].scl.z);
			D3DXMatrixMultiply(&g_object[ nCnt ].mtxWorld, &g_object[ nCnt ].mtxWorld, &mtxScl);

			// �����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_object[ nCnt ].rot.y, g_object[ nCnt ].rot.x, g_object[ nCnt ].rot.z);
			D3DXMatrixMultiply(&g_object[ nCnt ].mtxWorld, &g_object[ nCnt ].mtxWorld, &mtxRot);

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTranslate, g_object[ nCnt ].pos.x, g_object[ nCnt ].pos.y, g_object[ nCnt ].pos.z);
			D3DXMatrixMultiply(&g_object[ nCnt ].mtxWorld, &g_object[ nCnt ].mtxWorld, &mtxTranslate);
		
			// �ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_object[ nCnt ].mtxWorld);

			// ���݂̃}�e���A������ۑ�
			//------------------------------------
			pDevice->GetMaterial(&matDef);

			pMat = (D3DXMATERIAL*)g_object[ nCnt ].pBuffMatObject ->GetBufferPointer();

			/*strSrc = strrchr(pMat ->pTextureFilename, '\\') + 1;

			strcat(strDest, strSrc);

			D3DXCreateTextureFromFile(pDevice, strDest, &g_pTextureObject[1]);*/

			// �}�e���A���̐��������[�v
			for (int nCntMat = 0; nCntMat < (int)g_object[ nCnt ].numMatObject; nCntMat++)
			{
				pDevice ->SetMaterial(&pMat[ nCntMat ].MatD3D);		// �}�e���A���̐ݒ�
				pDevice ->SetTexture(0, g_object[ nCnt ].pTextureObject);						// �e�N�X�`���̃Z�b�g
				g_object[ nCnt ].pMeshObject->DrawSubset( nCntMat );	// �`��
			}

			// �}�e���A�������ɖ߂�
			//------------------------------------
			pDevice ->SetMaterial( &matDef );
		}
	}
}

//===============================================
//
// �I�u�W�F�N�g�̃Z�b�g
//
//===============================================
void SetObject( D3DXVECTOR3 pos, OBJECTTYPE type )
{
	// ���[�J���ϐ�
	LPDIRECT3DDEVICE9 pDevice = GetDevice();						// �f�o�C�X�I�u�W�F�N�g

	for ( int i = 0; i < MAX_OBJECT; i++ )
	{
		// �g���Ă��Ȃ�������
		if ( g_object[ i ].bUse == false )
		{
			// ���ꂼ��i�[
			g_object[ i ].type = type;
			g_object[ i ].pos = pos;
			g_object[ i ].rot = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
			g_object[ i ].scl = D3DXVECTOR3( 1.0f, 1.0f, 1.0f );

			// �^�C�v����
			switch ( g_object[ i ].type )
			{
			case OBJECTTYPE_A:
				g_object[ i ].harfSize = D3DXVECTOR3( OBJECT_A_WIDTH * g_object[ i ].scl.x, 0.0f, OBJECT_A_HEIGHT_Z * g_object[ i ].scl.z );
				D3DXCreateTextureFromFile(pDevice, OBJECT_A_TEXPASS, &g_object[ i ].pTextureObject);

				{
				
					// x�t�@�C���̓ǂݍ���
					//------------------------------------
					D3DXLoadMeshFromX(OBJECT_A_FILEPASS,		// �ǂݍ��ރt�@�C����
									  D3DXMESH_SYSTEMMEM,			// 
									  pDevice,						// 
									  NULL,							// 
									  &g_object[ i ].pBuffMatObject,// 
									  NULL,							// 
									  &g_object[ i ].numMatObject,	// 
									  &g_object[ i ].pMeshObject);
				}

				break;

			case OBJECTTYPE_B:
				g_object[ i ].harfSize = D3DXVECTOR3( OBJECT_B_WIDTH * g_object[ i ].scl.x, 0.0f, OBJECT_B_HEIGHT_Z * g_object[ i ].scl.z );
				D3DXCreateTextureFromFile(pDevice, OBJECT_B_TEXPASS, &g_object[ i ].pTextureObject);

				{
				
					// x�t�@�C���̓ǂݍ���
					//------------------------------------
					D3DXLoadMeshFromX(OBJECT_B_FILEPASS,		// �ǂݍ��ރt�@�C����
									  D3DXMESH_SYSTEMMEM,			// 
									  pDevice,						// 
									  NULL,							// 
									  &g_object[ i ].pBuffMatObject,// 
									  NULL,							// 
									  &g_object[ i ].numMatObject,	// 
									  &g_object[ i ].pMeshObject);
				}

				break;

			case OBJECTTYPE_C:
				g_object[ i ].harfSize = D3DXVECTOR3( OBJECT_C_WIDTH * g_object[ i ].scl.x, 0.0f, OBJECT_C_HEIGHT_Z * g_object[ i ].scl.z );
				D3DXCreateTextureFromFile(pDevice, NULL, &g_object[ i ].pTextureObject);

				{
				
					// x�t�@�C���̓ǂݍ���
					//------------------------------------
					D3DXLoadMeshFromX(OBJECT_C_FILEPASS,		// �ǂݍ��ރt�@�C����
									  D3DXMESH_SYSTEMMEM,			// 
									  pDevice,						// 
									  NULL,							// 
									  &g_object[ i ].pBuffMatObject,// 
									  NULL,							// 
									  &g_object[ i ].numMatObject,	// 
									  &g_object[ i ].pMeshObject);
				}

				break;

			case OBJECTTYPE_D:
				g_object[ i ].harfSize = D3DXVECTOR3( OBJECT_D_WIDTH * g_object[ i ].scl.x, 0.0f, OBJECT_D_HEIGHT_Z * g_object[ i ].scl.z );
				D3DXCreateTextureFromFile(pDevice, NULL, &g_object[ i ].pTextureObject);

				{
				
					// x�t�@�C���̓ǂݍ���
					//------------------------------------
					D3DXLoadMeshFromX(OBJECT_D_FILEPASS,		// �ǂݍ��ރt�@�C����
									  D3DXMESH_SYSTEMMEM,			// 
									  pDevice,						// 
									  NULL,							// 
									  &g_object[ i ].pBuffMatObject,// 
									  NULL,							// 
									  &g_object[ i ].numMatObject,	// 
									  &g_object[ i ].pMeshObject);
				}

				break;

			case OBJECTTYPE_E:
				g_object[ i ].harfSize = D3DXVECTOR3( OBJECT_E_WIDTH * g_object[ i ].scl.x, 0.0f, OBJECT_E_HEIGHT_Z * g_object[ i ].scl.z );
				D3DXCreateTextureFromFile(pDevice, NULL, &g_object[ i ].pTextureObject);

				{
				
					// x�t�@�C���̓ǂݍ���
					//------------------------------------
					D3DXLoadMeshFromX(OBJECT_E_FILEPASS,		// �ǂݍ��ރt�@�C����
									  D3DXMESH_SYSTEMMEM,			// 
									  pDevice,						// 
									  NULL,							// 
									  &g_object[ i ].pBuffMatObject,// 
									  NULL,							// 
									  &g_object[ i ].numMatObject,	// 
									  &g_object[ i ].pMeshObject);
				}

				break;
			}

			// �����蔻��p�ϐ��Ɋi�[
			rectObject[ i ].pos = g_object[ i ].pos;
			rectObject[ i ].harfSize = g_object[ i ].harfSize;

			g_object[ i ].bUse = true;		// �g�pON
			break;
		}
	}
}

//===============================================
//
// �I�u�W�F�N�g�\���̕ϐ��擾
//
//===============================================
OBJECT *GetObject( void )
{
	return &g_object[ 0 ];
}

//===============================================
//
// �����蔻��̏����擾
//
//===============================================
COL_RECT *GetRectObject( void )
{
	return &rectObject[ 0 ];
}

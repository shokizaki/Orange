//===============================================
//
//  ���e   �F ���f���Ɋւ��鏈��
//  File   �F model.cpp
//  ����� �F �؍�@��
//
//===============================================

//===============================================
// 
//  �C���N���[�h�t�@�C��
//
//===============================================
#include "model.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "meshField.h"
#include "billboard.h"
#include "enemy.h"
#include "object.h"
#include "bullet.h"
#include "texture2D.h"
#include "stringDraw.h"
#include "motion.h"
#include "miniMap.h"

//===============================================
//
//�@�}�N����`
//
//===============================================
#define MODEL_TEXTURE_MAX 	( 2 )			// �g�p����e�N�X�`���̐�
#define MODEL_MAX			( 7 )			// ���f���̍ő吔
#define MOVE_VAL			( 7.0f )		// 
#define GRAVITY_VAL			( 0.50f )		// 
#define BRAKE_VAL			( 2 )		// �����̌W��
#define ROT_BRAKE_VAL		( 0.1f )		// 
#define BULLET_SPEED		( 1.5f )		// 
#define SLOW_VAL			( ( 1.0f - ( float )g_nNumEnemyCatch / 30 ) )	// �x�������

#define PLAYER_WIDTH		( 50.0f )		// 
#define PLAYER_HEIGHT_Z		( 50.0f )		// 
#define PLAYER_HEIGHT_Y		( 100.0f )		// 

#define TEX_PASS		"data/TEXTURE/tairu03.jpg"

//-----------------------------------------------
//  �O���[�o���ϐ�
//-----------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureModel[MODEL_TEXTURE_MAX];		// �e�N�X�`���ւ̃|�C���^

// ���f���Ɋւ���ϐ�
MODEL_INFO g_playerInfo;				// ���f���ϐ�
COL_RECT rectPlayer;					// �����蔻��̏��
BULLETTYPE bulletType;					// �o���b�g�̃^�C�v
PLAYER_STATUS playerStatus;				// �v���C���[�̏��
int g_nNumEnemyCatch = 0;					// �G��߂܂�����

static D3DXMATRIX g_mtxProjection;		// �v���W�F�N�V�����}�g���b�N�X
static D3DXMATRIX g_mtxView;			// �r���[�}�g���b�N�X

D3DXVECTOR3 g_posModel;			// �ʒu
D3DXVECTOR3 g_rotModel;			// ����
D3DXVECTOR3 g_sclModel;			// �傫��

D3DXMATRIX g_mtxWorld;			// ���[���h�}�g���b�N�X

//------ �����֐��錾 ---------------------------

//-----------------------------------------------
// �v���C���[�ړ������i�p�b�h�j
//-----------------------------------------------
void UpdatePlayerMovePad( void );

//-----------------------------------------------
// �v���C���[�ړ������i�L�[�{�[�h�j
//-----------------------------------------------
void UpdatePlayerMoveKeyboard( void );

//-----------------------------------------------

//===============================================
//
// ���f���Ɋւ��鏉����
//
//===============================================
void InitModel(void)
{
	// ���[�J���ϐ�
	//------------------------------------
	LPDIRECT3DDEVICE9 pDevice;						// �f�o�C�X�I�u�W�F�N�g
	char strDest[256] = "data/TEXTURE/";

	// �O���[�o���ϐ��̏�����
	g_nNumEnemyCatch = 0;
	g_posModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_rotModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_sclModel = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	g_playerInfo.targetRotParts = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	playerStatus.bHitEnemy = false;
	playerStatus.nFrameCount = 0;

	// �f�o�C�X�̎擾
	//------------------------------------
	pDevice = GetDevice();

	g_playerInfo.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �o���b�g�^�C�v�̏�����
	bulletType = BULLETTYPE_ONP_RED;

	// ���[�V�����̏�����
	InitModelMotionEx( &g_playerInfo, "data/SCRIPT/motion.txt" );

	for (int nCnt = 0; nCnt < MODEL_MAX; nCnt++)
	{
		// ���f���Ɋւ��鏉����
		//------------------------------------
		g_playerInfo.scl[ nCnt ] = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_playerInfo.rotParts[ nCnt ] = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	}

	for (int nCnt = 0; nCnt < g_playerInfo.nNumParts; nCnt++)
	{
		// x�t�@�C���̓ǂݍ���
		//------------------------------------
		D3DXLoadMeshFromX(&g_playerInfo.modelFileName[ nCnt ][ 0 ],		// �ǂݍ��ރt�@�C����
						  D3DXMESH_SYSTEMMEM,							// 
						  pDevice,										// 
						  NULL,											// 
						  &g_playerInfo.pBuffMatModel[ nCnt ],			// 
						  NULL,											// 
						  &g_playerInfo.numMatModel[ nCnt ],			// 
						  &g_playerInfo.pMeshModel[ nCnt ] );
	}

	if ( GetMode() == MODE_TITLE )
	{
		g_playerInfo.posParts[ 0 ] = D3DXVECTOR3( -250.0f, -50.0f, 0.0f );
	}

	// �e�N�X�`���ւ̃|�C���^��������
	//------------------------------------
	for (int nCnt = 0; nCnt < MODEL_TEXTURE_MAX; nCnt++)				
	{
		g_pTextureModel[nCnt] = NULL;
	}

	// �����蔻����̊i�[
	rectPlayer.pos = g_playerInfo.posParts[ 0 ];
	rectPlayer.harfSize = D3DXVECTOR3( PLAYER_WIDTH / 2, 0.0f, PLAYER_HEIGHT_Z / 2 );

	// �e�̃Z�b�g(�C���f�b�N�X���󂯎��)
	g_playerInfo.nShadowNum = SetShadowEx(g_playerInfo.posParts[ 0 ], D3DXVECTOR3(0.0f, 0.0f, 0.0f), g_playerInfo.scl[ 0 ], 30.0f, 30.0f, 0.7f);
}

//===============================================
//
// ���f���Ɋւ���I������
//
//===============================================
void UninitModel(void)
{
	// ���[�J���ϐ�
	//------------------------------------
	int nCnt;

	// �I������
	UninitModelMotion( &g_playerInfo );

	// �g�����e�N�X�`���̉���Ə�����
	//------------------------------------
	for (nCnt = 0; nCnt < MODEL_TEXTURE_MAX; nCnt++)
	{
		if (g_pTextureModel[ nCnt ] != NULL)
		{ 
			g_pTextureModel[ nCnt ] ->Release();
			g_pTextureModel[ nCnt ] = NULL;
		}
	}
	
}

//===============================================
//
// ���f���Ɋւ���X�V����
//
//===============================================
void UpdateModel(void)
{
	// ���[�J���ϐ�
	//------------------------------------
	CAMERA *pCamera = GetCamera();				// �J�����̏��擾
	MESHFIELD *pMeshField = GetMeshField();		// ���b�V���t�B�[���h�̏����擾
	COL_RECT *pRectObject = GetRectObject();	// �I�u�W�F�N�g�̓����蔻����
	OBJECT *pObject = GetObject();				// �I�u�W�F�N�g���擾

	// �O��̈ʒu��ۑ�
	//------------------------------------
	g_playerInfo.posOld = g_playerInfo.posParts[ 0 ];
	g_playerInfo.statusOld = g_playerInfo.status;

	// �W�����v����
	//------------------------------------
	if ( GetKeyboardTrigger( DIK_J ) == true )
	{
		g_playerInfo.fGravity = g_playerInfo.fJumpVal;
		g_playerInfo.status = JUMP;
	}

	// �d�͂�������
	//------------------------------------
	g_playerInfo.posParts[ 0 ].y += g_playerInfo.fGravity;
	g_playerInfo.fGravity -= GRAVITY_VAL;

	// �߂荞�܂Ȃ�����
	//------------------------------------
	if ( g_playerInfo.posParts[ 0 ].y - 20.0f <= 0.0f )
	{
		g_playerInfo.posParts[ 0 ].y = g_playerInfo.posOld.y;		// �O���Y���W�ɖ߂�
		rectPlayer.pos = g_playerInfo.posParts[ 0 ];					// �����蔻��p�ϐ��̍X�V
		g_playerInfo.fGravity = 0.0f;							// �W�����v�ʂ�������
	}

	// �ړ�����
	//----------------------------------------------
	if ( ( ( GetKeyboardPress(DIK_W) || GetKeyboardPress(DIK_S) || GetKeyboardPress(DIK_D) || GetKeyboardPress(DIK_A) ) || GetGamePadCrossKeyX() != 0 || GetGamePadCrossKeyY() != 0 )
	&&	 ( ( GetMode() == MODE_TUTORIAL && GetEnterFlag() == true ) || GetMode() == MODE_GAME ) )
	{
		// ��ړ�
		if ( GetKeyboardPress(DIK_W) || GetGamePadCrossKeyY() == -1 )
		{
			// ���΂ߏ�ړ�
			if (GetKeyboardPress(DIK_A) || GetGamePadCrossKeyX() == -1 )
			{
				// ���f���̖ڕW�p�x��ݒ�
				g_playerInfo.targetRotParts.y = (D3DX_PI - (D3DX_PI / 4)) - pCamera ->rotCamera.y;

				// �ړ��ʂ�ݒ�
				g_playerInfo.move.x -= sinf(pCamera->rotCamera.y + D3DX_PI / 4) * MOVE_VAL * SLOW_VAL;
				g_playerInfo.move.z += cosf(pCamera->rotCamera.y + D3DX_PI / 4) * MOVE_VAL * SLOW_VAL;

				g_playerInfo.directionSecond = DIRECTION_LEFT;
			}

			// �E�΂ߏ�ړ�
			else if (GetKeyboardPress(DIK_D) || GetGamePadCrossKeyX() == 1 )
			{
				// ���f���̖ڕW�p�x��ݒ�
				g_playerInfo.targetRotParts.y = (D3DX_PI + (D3DX_PI / 4)) - pCamera ->rotCamera.y;

				// �ړ��ʂ�ݒ�
				g_playerInfo.move.x -= sinf(pCamera->rotCamera.y - D3DX_PI / 4) * MOVE_VAL * SLOW_VAL;
				g_playerInfo.move.z += cosf(pCamera->rotCamera.y - D3DX_PI / 4) * MOVE_VAL * SLOW_VAL;

				g_playerInfo.directionSecond = DIRECTION_RIGHT;
			}

			else
			{
				// ���f���̖ڕW�p�x��ݒ�
				g_playerInfo.targetRotParts.y = D3DX_PI - pCamera ->rotCamera.y;

				// �ړ��ʂ�ݒ�
				g_playerInfo.move.x -= sinf( pCamera->rotCamera.y ) * MOVE_VAL * SLOW_VAL;
				g_playerInfo.move.z += cosf( pCamera->rotCamera.y ) * MOVE_VAL * SLOW_VAL;
			}

			if ( g_playerInfo.bMove == false )
			{
				g_playerInfo.direction = DIRECTION_DEPTH;
			}

			g_playerInfo.bMove = true;	// �ړ����肠��
		}

		// ���ړ�
		else if (GetKeyboardPress(DIK_S) || GetGamePadCrossKeyY() == 1 )
		{
			// ���΂߉��ړ�
			if (GetKeyboardPress(DIK_A) || GetGamePadCrossKeyX() == -1 )
			{
				// ���f���̖ڕW�p�x��ݒ�
				g_playerInfo.targetRotParts.y = (D3DX_PI / 4) - pCamera ->rotCamera.y;

				// �ړ��ʂ�ݒ�
				g_playerInfo.move.x += sinf(pCamera->rotCamera.y - D3DX_PI / 4) * MOVE_VAL * SLOW_VAL;
				g_playerInfo.move.z -= cosf(pCamera->rotCamera.y - D3DX_PI / 4) * MOVE_VAL * SLOW_VAL;
			
				g_playerInfo.directionSecond = DIRECTION_LEFT;
			}

			// �E�΂߉��ړ�
			else if (GetKeyboardPress(DIK_D) || GetGamePadCrossKeyX() == 1 )
			{
				// ���f���̖ڕW�p�x��ݒ�
				g_playerInfo.targetRotParts.y = -(D3DX_PI / 4) - pCamera ->rotCamera.y;

				// �ړ��ʂ�ݒ�
				g_playerInfo.move.x += sinf(pCamera->rotCamera.y + D3DX_PI / 4) * MOVE_VAL * SLOW_VAL;
				g_playerInfo.move.z -= cosf(pCamera->rotCamera.y + D3DX_PI / 4) * MOVE_VAL * SLOW_VAL;
			
				g_playerInfo.directionSecond = DIRECTION_RIGHT;
			}

			else
			{
				// ���f���̖ڕW�p�x��ݒ�
				g_playerInfo.targetRotParts.y = 0.0f - pCamera ->rotCamera.y;

				// �ړ��ʂ�ݒ�
				g_playerInfo.move.x += sinf(pCamera->rotCamera.y) * MOVE_VAL * SLOW_VAL;
				g_playerInfo.move.z -= cosf(pCamera->rotCamera.y) * MOVE_VAL * SLOW_VAL;
			}

			if ( g_playerInfo.bMove == false )
			{
				g_playerInfo.direction = DIRECTION_FRONT;
			}
			
			g_playerInfo.bMove = true;	// �ړ����肠��
		}

		// ���ړ�
		else if (GetKeyboardPress(DIK_A) || GetGamePadCrossKeyX() == -1 )
		{
			// ���f���̖ڕW�p�x��ݒ�
			g_playerInfo.targetRotParts.y = D3DX_PI / 2 - pCamera ->rotCamera.y;

			// �ړ��ʂ�ݒ�
			g_playerInfo.move.x -= cosf(pCamera->rotCamera.y) * MOVE_VAL * SLOW_VAL;
			g_playerInfo.move.z -= sinf(pCamera->rotCamera.y) * MOVE_VAL * SLOW_VAL;

			if ( g_playerInfo.bMove == false )
			{
				g_playerInfo.direction = DIRECTION_LEFT;
			}

			g_playerInfo.bMove = true;	// �ړ����肠��
		}

		// �E�ړ�
		else if (GetKeyboardPress(DIK_D) || GetGamePadCrossKeyX() == 1 )
		{
			// ���f���̖ڕW�p�x��ݒ�
			g_playerInfo.targetRotParts.y = -(D3DX_PI / 2) - pCamera ->rotCamera.y;

			// �ړ��ʂ�ݒ�
			g_playerInfo.move.x += cosf(pCamera ->rotCamera.y) * MOVE_VAL * SLOW_VAL;
			g_playerInfo.move.z += sinf(pCamera ->rotCamera.y) * MOVE_VAL * SLOW_VAL;

			if ( g_playerInfo.bMove == false )
			{
				g_playerInfo.direction = DIRECTION_RIGHT;
			}

			g_playerInfo.bMove = true;	// �ړ����肠��
		}

		g_playerInfo.status = WALK;
		
		// �����蔻��p�̏����X�V
		rectPlayer.pos = g_playerInfo.posParts[ 0 ];
	}
	else
	{
		if ( g_playerInfo.status == WALK )
		{
			g_playerInfo.status = NEUTRAL;
			g_playerInfo.nFrameCount = 0;
			g_playerInfo.nKey = 0;
		}
		g_playerInfo.bMove = false;
	}

	/*if ( g_playerInfo.bMotion == false )
	{
		if ( g_playerInfo.status != g_playerInfo.statusOld )
		{
			g_playerInfo.nFrameCount = 0;
			g_playerInfo.nKey = 0;
		}
	}*/

	// �o���b�g�̃^�C�v��ύX
	//------------------------------------
	if ( ( GetKeyboardTrigger( DIK_UP ) == true /*|| GetGamePadTrigger( GAMEPAD_X )*/ || GetPadElecomTrigger( PAD_2 ) )
	&& ( ( GetMode() == MODE_TUTORIAL && GetEnterFlag() == true ) || GetMode() == MODE_GAME ))
	{
		bulletType = ( BULLETTYPE )( ( int )bulletType - 1 );
		if ( bulletType < BULLETTYPE_ONP_RED )
		{
			bulletType = BULLETTYPE_ONP_BLUE;
		}
	}
	if ( ( GetKeyboardTrigger( DIK_DOWN ) == true /*|| GetGamePadTrigger( GAMEPAD_B )*/ || GetPadElecomTrigger( PAD_3 ) ) 
	&& ( ( GetMode() == MODE_TUTORIAL && GetEnterFlag() == true ) || GetMode() == MODE_GAME ) )
	{
		bulletType = ( BULLETTYPE )( ( int )bulletType + 1 );
		if ( bulletType == BULLETTYPE_MAX )
		{
			bulletType = BULLETTYPE_ONP_RED;
		}
	}

	// �o���b�g��������
	//------------------------------------
	if ( ( GetKeyboardTrigger( DIK_SPACE ) == true /*|| GetGamePadTrigger( GAMEPAD_Y )*/ || GetPadElecomTrigger( PAD_1 ) )
	&& ( ( GetMode() == MODE_TUTORIAL && GetEnterFlag() == true ) || GetMode() == MODE_GAME ) )
	{
		// �R�����ɔ���
		SetBullet( g_playerInfo.posParts[ 0 ], D3DXVECTOR3( 0.0f, 0.0f, 0.f ), D3DXVECTOR3( 1.0f, 1.0f, 1.0f ),
			D3DXVECTOR3( cosf( g_playerInfo.rotParts[ 0 ].y - D3DX_PI / 4 ) * BULLET_SPEED, 0.0f, sinf( g_playerInfo.rotParts[ 0 ].y - D3DX_PI / 4 ) * BULLET_SPEED ),
			bulletType );

		SetBullet( g_playerInfo.posParts[ 0 ], D3DXVECTOR3( 0.0f, 0.0f, 0.f ), D3DXVECTOR3( 1.0f, 1.0f, 1.0f ),
			D3DXVECTOR3( cosf( g_playerInfo.rotParts[ 0 ].y - D3DX_PI / 2 ) * BULLET_SPEED, 0.0f, sinf( g_playerInfo.rotParts[ 0 ].y - D3DX_PI / 2 ) * BULLET_SPEED ),
			bulletType );

		SetBullet( g_playerInfo.posParts[ 0 ], D3DXVECTOR3( 0.0f, 0.0f, 0.f ), D3DXVECTOR3( 1.0f, 1.0f, 1.0f ),
			D3DXVECTOR3( cosf( g_playerInfo.rotParts[ 0 ].y - D3DX_PI / 1.5f ) * BULLET_SPEED, 0.0f, sinf( g_playerInfo.rotParts[ 0 ].y - D3DX_PI / 1.5f ) * BULLET_SPEED ),
			bulletType );

		PlaySound( SUZU000_SE );
	}

	// �G�Ƃ̓����蔻�肪��������
	//------------------------------------
	if ( playerStatus.bHitEnemy == true )
	{
		playerStatus.nFrameCount++;
		if ( playerStatus.nFrameCount == 60 )
		{
			playerStatus.bHitEnemy = false;
			playerStatus.nFrameCount = 0;
		}
	}

	// �ڕW�̌������K��͈͂𒴂��Ă�����ۂߍ���
	//--------------------------------------------
	if (g_playerInfo.targetRot[ 0 ].y > D3DX_PI)
	{
		g_playerInfo.targetRot[ 0 ].y = ( ( g_playerInfo.targetRot[ 0 ].y ) - ( D3DX_PI * 2 ) );
	}
	if (g_playerInfo.targetRot[ 0 ].y < -D3DX_PI)
	{
		g_playerInfo.targetRot[ 0 ].y = ( ( g_playerInfo.targetRot[ 0 ].y ) + ( D3DX_PI * 2 ) );
	}

	// ���������X�Ɏ�߂�
	g_playerInfo.move.x += (0.0f - g_playerInfo.move.x) / BRAKE_VAL;
	g_playerInfo.move.z += (0.0f - g_playerInfo.move.z) / BRAKE_VAL;

	// ��������l�����Z
	g_playerInfo.posParts[ 0 ].x += g_playerInfo.move.x;
	g_playerInfo.posParts[ 0 ].z += g_playerInfo.move.z;

	// �ǂɓ���������
	//------------------------------------
	if (g_playerInfo.posParts[ 0 ].x - 30.0f < -(pMeshField ->nNumX * pMeshField ->fWidth) / 2 
	||	g_playerInfo.posParts[ 0 ].x + 30.0f > (pMeshField ->nNumX * pMeshField ->fWidth) / 2)
	{
		// X���W�݂̂��Ƃɖ߂�
		g_playerInfo.posParts[ 0 ].x = g_playerInfo.posOld.x;
	}
	if (g_playerInfo.posParts[ 0 ].z - 30.0f < -(pMeshField ->nNumZ * pMeshField ->fHeight) / 2
	||	g_playerInfo.posParts[ 0 ].z + 30.0f > (pMeshField ->nNumZ * pMeshField ->fHeight) / 2)
	{
		// Z���W�݂̂��Ƃɖ߂�
		g_playerInfo.posParts[ 0 ].z = g_playerInfo.posOld.z;
	}

	// �����蔻��p�̏����X�V
	rectPlayer.pos = g_playerInfo.posParts[ 0 ];

	// �A�N�V����
	//-----------------------------------
	/*if ( GetKeyboardTrigger( DIK_K ) )
	{
		g_playerInfo.status = ACTION;
	}*/

	if ( playerStatus.bHitEnemy == false)
	{
		playerStatus.nFrameCount = 0;
	}

	// ���f���̌����̊�������
	//-----------------------------------
	RotateModel();

	// ���[�V�����̍X�V����
	//-----------------------------------
	UpdateModelMotion( &g_playerInfo );

// �f�o�b�O�\��
PrintDebugProc("/-------------------------\n");
PrintDebugProc("MOTION : NEUTRAL\n");
PrintDebugProc("LOOP   : ON\n");
PrintDebugProc("KEY    : %d / %d\n", g_playerInfo.nKey, g_playerInfo.aMotion[ g_playerInfo.status ].nNumKey);
PrintDebugProc("FRAME  : %d / %d\n", g_playerInfo.nFrameCount, g_playerInfo.aMotion[ g_playerInfo.status ].nFrame[ g_playerInfo.nKey ]);
PrintDebugProc("-------------------------/\n\n");


	// �e�̍X�V�i�C���f�b�N�X�w��j
	//-----------------------------------
	//UpdateShadowEx( g_playerInfo.nShadowNum, g_playerInfo.posParts[ 0 ] );
	SetShadow( D3DXVECTOR3( g_playerInfo.posParts[ 0 ].x, 0.1f, g_playerInfo.posParts[ 0 ].z ), rectPlayer.harfSize.x, rectPlayer.harfSize.z, 0.7f );

	// �~�j�}�b�v�Z�b�g
	SetMiniMap(D3DXVECTOR3( g_playerInfo.posParts[ 0 ].x, 0.0f, g_playerInfo.posParts[ 0 ].z ), MINIMAPTYPE_PLAYER, g_playerInfo.rotParts[ 0 ].y );

	// �����蔻��p�̏����X�V
	//-----------------------------------
	rectPlayer.pos = g_playerInfo.posParts[ 0 ];
}

//===============================================
//
// �v���C���[�̈ړ���������
//
//===============================================
void UpdatePlayerMovePad( void )
{
	// ���[�J���ϐ�
	//------------------------------------
	CAMERA *pCamera = GetCamera();				// �J�����̏��擾
	MODEL_INFO *pEnemyInfo = GetEnemyInfo();
	COL_RECT *pRectEnemy = GetRectEnemy();
	bool *pEnemyUse = GetEnemyUse();

	// �����œ���������ON�i�U���{�^���j
	if ( GetPadElecomTrigger( PAD_4 ) == true )
	{
		g_playerInfo.bAutoMove = true;
	}

	if ( g_playerInfo.bAutoMove == false )
	{
		// �Q�[���p�b�h�ł̈ړ�����
		if ( ( GetGamePadCrossKeyX() > 0 || GetGamePadCrossKeyY() > 0 || GetGamePadCrossKeyX() < 0 || GetGamePadCrossKeyY() < 0 ) )
		{
			// �����Ă�����̐ݒ�i�����蔻��p�j
			
			if ( g_playerInfo.bMove == false )
			{
				if ( GetGamePadCrossKeyY() == 1 )
				{
					g_playerInfo.direction = DIRECTION_FRONT;
				}
				else if ( GetGamePadCrossKeyY() == -1 )
				{
					g_playerInfo.direction = DIRECTION_DEPTH;
				}
				else if ( GetGamePadCrossKeyX() == 1 )
				{
					g_playerInfo.direction = DIRECTION_RIGHT;
				}
				else if ( GetGamePadCrossKeyX() == -1 )
				{
					g_playerInfo.direction = DIRECTION_LEFT;
				}
			}

			// ���[�V�����ύX
			g_playerInfo.status = WALK;
			g_playerInfo.bMove = true;

			// �ړ�����
			g_playerInfo.posParts[ 0 ].x += cosf( atan2f( ( float )GetGamePadCrossKeyX(), ( float )GetGamePadCrossKeyY() ) + pCamera ->rotCamera.y ) * MOVE_VAL;
			g_playerInfo.posParts[ 0 ].z += sinf( atan2f( ( float )GetGamePadCrossKeyX(), ( float )GetGamePadCrossKeyY() ) + pCamera ->rotCamera.y ) * MOVE_VAL;	

			// �����̖ڕW�l��ݒ�
			g_playerInfo.targetRotParts.y = -atan2f( ( float )GetGamePadCrossKeyX(), ( float )GetGamePadCrossKeyY() ) - D3DX_PI / 2 - pCamera ->rotCamera.y;
			if (g_playerInfo.targetRotParts.y > D3DX_PI)
			{
				g_playerInfo.targetRotParts.y = ( ( g_playerInfo.targetRotParts.y ) - ( D3DX_PI * 2 ) );
			}
			if (g_playerInfo.targetRotParts.y < -D3DX_PI)
			{
				g_playerInfo.targetRotParts.y = ( ( g_playerInfo.targetRotParts.y ) + ( D3DX_PI * 2 ) );
			}

			g_playerInfo.bAutoMove = false;
		}
		else
		{
			g_playerInfo.bMove = false;
			g_playerInfo.status = NEUTRAL;
			g_playerInfo.direction = DIRECTION_MAX;
			PrintDebugProc("DIRECTION_MAX\n");
		}
		
		PrintDebugProc("[ �v���C���[���� ] : %f\n", g_playerInfo.targetRotParts.y);
	}

	// �����œ���������OFF�i�K�[�h�⍶�X�e�B�b�N�j
	if ( ( GetPadElecomTrigger( PAD_1 ) == true )
	||	 ( GetGamePadCrossKeyX() > 0 || GetGamePadCrossKeyY() > 0 || GetGamePadCrossKeyX() < 0 || GetGamePadCrossKeyY() < 0 ) )
	{
		g_playerInfo.bAutoMove = false;
	}

	// �w�肵���ʒu�Ɏ����œ�������
	//if ( g_playerInfo.bAutoMove == true )
	//{
	//	g_playerInfo.status = WALK;

	//	float x = ( pEnemyInfo ->posParts[ 0 ].x - g_playerInfo.posParts[ 0 ].x );
	//	float z = ( pEnemyInfo ->posParts[ 0 ].z - g_playerInfo.posParts[ 0 ].z );

	//	g_playerInfo.targetRotParts.y = -atan2f( z, x ) - D3DX_PI / 2;
	//	if (g_playerInfo.targetRotParts.y > D3DX_PI)
	//	{
	//		g_playerInfo.targetRotParts.y = ( ( g_playerInfo.targetRotParts.y ) - ( D3DX_PI * 2 ) );
	//	}
	//	if (g_playerInfo.targetRotParts.y < -D3DX_PI)
	//	{
	//		g_playerInfo.targetRotParts.y = ( ( g_playerInfo.targetRotParts.y ) + ( D3DX_PI * 2 ) );
	//	}

	//	g_playerInfo.posParts[ 0 ].x += cosf( atan2f( z, x ) ) * MOVE_VAL;
	//	g_playerInfo.posParts[ 0 ].z += sinf( atan2f( z, x ) ) * MOVE_VAL;

	//	for ( int i = 0; i < ENEMY_MAX; i++ )
	//	{
	//		if ( *( pEnemyUse + i ) == true )
	//		{
	//			if ( ColCircle( &rectPlayer, ( pRectEnemy + i ) ) == true )
	//			{
	//				g_playerInfo.bAutoMove = false;
	//				//g_playerInfo.status = ACTION;

	//				break;
	//			}
	//		}
	//	}
	//}
}

//-----------------------------------------------
// �v���C���[�ړ������i�L�[�{�[�h�j
//-----------------------------------------------
void UpdatePlayerMoveKeyboard( void )
{
	// ���[�J���ϐ�
	//------------------------------------
	CAMERA *pCamera = GetCamera();				// �J�����̏��擾
	MODEL_INFO *pEnemyInfo = GetEnemyInfo();

	// �����œ���������ON�i�U���{�^���j
	if ( GetKeyboardTrigger( DIK_L ) == true )
	{
		g_playerInfo.bAutoMove = true;
	}

	if ( g_playerInfo.bAutoMove == false )
	{
		// �ړ�����
		//----------------------------------------------
		if ( ( GetKeyboardPress(DIK_W) || GetKeyboardPress(DIK_S) || GetKeyboardPress(DIK_D) || GetKeyboardPress(DIK_A) ) )
		{
			// ��ړ�
			if ( GetKeyboardPress(DIK_W) )
			{
				// ���΂ߏ�ړ�
				if ( GetKeyboardPress(DIK_A) )
				{
					// ���f���̖ڕW�p�x��ݒ�
					g_playerInfo.targetRotParts.y = -atan2f( -1.0f, -1.0f ) - D3DX_PI / 2 - pCamera ->rotCamera.y;

					// �ړ��ʂ�ݒ�
					g_playerInfo.move.x += cosf( atan2f( -1.0f, -1.0f ) + pCamera ->rotCamera.y ) * MOVE_VAL * SLOW_VAL;
					g_playerInfo.move.z += sinf( atan2f( -1.0f, -1.0f ) + pCamera ->rotCamera.y ) * MOVE_VAL * SLOW_VAL;

					g_playerInfo.directionSecond = DIRECTION_LEFT;
				}

				// �E�΂ߏ�ړ�
				else if ( GetKeyboardPress(DIK_D) )
				{
					// ���f���̖ڕW�p�x��ݒ�
					g_playerInfo.targetRotParts.y = -atan2f( 1.0f, -1.0f ) - D3DX_PI / 2 - pCamera ->rotCamera.y;

					// �ړ��ʂ�ݒ�
					g_playerInfo.move.x += cosf( atan2f( 1.0f, -1.0f ) + pCamera ->rotCamera.y ) * MOVE_VAL * SLOW_VAL;
					g_playerInfo.move.z += sinf( atan2f( 1.0f, -1.0f ) + pCamera ->rotCamera.y ) * MOVE_VAL * SLOW_VAL;

					g_playerInfo.directionSecond = DIRECTION_RIGHT;
				}

				else
				{
					// ���f���̖ڕW�p�x��ݒ�
					g_playerInfo.targetRotParts.y = -atan2f( 0.0f, -1.0f ) - D3DX_PI / 2 - pCamera ->rotCamera.y;

					// �ړ��ʂ�ݒ�
					g_playerInfo.move.x += cosf( atan2f( 0.0f, -1.0f ) + pCamera ->rotCamera.y ) * MOVE_VAL * SLOW_VAL;
					g_playerInfo.move.z += sinf( atan2f( 0.0f, -1.0f ) + pCamera ->rotCamera.y ) * MOVE_VAL * SLOW_VAL;
				}

				if ( g_playerInfo.bMove == false )
				{
					g_playerInfo.direction = DIRECTION_DEPTH;
				}

				g_playerInfo.bMove = true;	// �ړ����肠��
			}

			// ���ړ�
			else if ( GetKeyboardPress(DIK_S) )
			{
				// ���΂߉��ړ�
				if ( GetKeyboardPress(DIK_A) )
				{
					// ���f���̖ڕW�p�x��ݒ�
					g_playerInfo.targetRotParts.y = -atan2f( -1.0f, 1.0f ) - D3DX_PI / 2 - pCamera ->rotCamera.y;

					// �ړ��ʂ�ݒ�
					g_playerInfo.move.x += cosf( atan2f( -1.0f, 1.0f ) + pCamera ->rotCamera.y ) * MOVE_VAL * SLOW_VAL;
					g_playerInfo.move.z += sinf( atan2f( -1.0f, 1.0f ) + pCamera ->rotCamera.y ) * MOVE_VAL * SLOW_VAL;
			
					g_playerInfo.directionSecond = DIRECTION_LEFT;
				}

				// �E�΂߉��ړ�
				else if ( GetKeyboardPress(DIK_D) )
				{
					// ���f���̖ڕW�p�x��ݒ�
					g_playerInfo.targetRotParts.y = -atan2f( 1.0f, 1.0f ) - D3DX_PI / 2 - pCamera ->rotCamera.y;

					// �ړ��ʂ�ݒ�
					g_playerInfo.move.x += cosf( atan2f( 1.0f, 1.0f ) + pCamera ->rotCamera.y ) * MOVE_VAL * SLOW_VAL;
					g_playerInfo.move.z += sinf( atan2f( 1.0f, 1.0f ) + pCamera ->rotCamera.y ) * MOVE_VAL * SLOW_VAL;
			
					g_playerInfo.directionSecond = DIRECTION_RIGHT;
				}

				else
				{
					// ���f���̖ڕW�p�x��ݒ�
					g_playerInfo.targetRotParts.y = -atan2f( 0.0f, 1.0f ) - D3DX_PI / 2 - pCamera ->rotCamera.y;

					// �ړ��ʂ�ݒ�
					g_playerInfo.move.x += cosf( atan2f( 0.0f, 1.0f ) + pCamera ->rotCamera.y ) * MOVE_VAL * SLOW_VAL;
					g_playerInfo.move.z += sinf( atan2f( 0.0f, 1.0f ) + pCamera ->rotCamera.y ) * MOVE_VAL * SLOW_VAL;
				}

				if ( g_playerInfo.bMove == false )
				{
					g_playerInfo.direction = DIRECTION_FRONT;
				}
			
				g_playerInfo.bMove = true;	// �ړ����肠��
			}

			// ���ړ�
			else if ( GetKeyboardPress(DIK_A) )
			{
				// ���f���̖ڕW�p�x��ݒ�
				g_playerInfo.targetRotParts.y = -atan2f( -1.0f, 0.0f ) - D3DX_PI / 2 - pCamera ->rotCamera.y;

				// �ړ��ʂ�ݒ�
				g_playerInfo.move.x += cosf( atan2f( -1.0f, 0.0f ) + pCamera ->rotCamera.y ) * MOVE_VAL * SLOW_VAL;
				g_playerInfo.move.z += sinf( atan2f( -1.0f, 0.0f ) + pCamera ->rotCamera.y ) * MOVE_VAL * SLOW_VAL;

				if ( g_playerInfo.bMove == false )
				{
					g_playerInfo.direction = DIRECTION_LEFT;
				}

				g_playerInfo.bMove = true;	// �ړ����肠��
			}

			// �E�ړ�
			else if ( GetKeyboardPress(DIK_D) )
			{
				// ���f���̖ڕW�p�x��ݒ�
				g_playerInfo.targetRotParts.y = -atan2f( 1.0f, 0.0f ) - D3DX_PI / 2 - pCamera ->rotCamera.y;
			
				// �ړ��ʂ�ݒ�
				g_playerInfo.move.x += cosf( atan2f( 1.0f, 0.0f ) + pCamera ->rotCamera.y ) * MOVE_VAL * SLOW_VAL;
				g_playerInfo.move.z += sinf( atan2f( 1.0f, 0.0f ) + pCamera ->rotCamera.y ) * MOVE_VAL * SLOW_VAL;

				if ( g_playerInfo.bMove == false )
				{
					g_playerInfo.direction = DIRECTION_RIGHT;
				}

				g_playerInfo.bMove = true;	// �ړ����肠��
			}

			g_playerInfo.status = WALK;
		
			// �����蔻��p�̏����X�V
			rectPlayer.pos = g_playerInfo.posParts[ 0 ];
		}
		else
		{
			g_playerInfo.status = NEUTRAL;
			g_playerInfo.bMove = false;
			g_playerInfo.direction = DIRECTION_MAX;
		}
	}

	// �����œ���������OFF�i�K�[�h��ړ��L�[���́j
	if ( ( GetKeyboardTrigger( DIK_J ) == true )
	||	 ( GetKeyboardPress(DIK_W) || GetKeyboardPress(DIK_S) || GetKeyboardPress(DIK_D) || GetKeyboardPress(DIK_A) ) )
	{
		g_playerInfo.bAutoMove = false;
	}

	// �w�肵���ʒu�Ɏ����œ�������
	if ( g_playerInfo.bAutoMove == true )
	{
		g_playerInfo.status = WALK;

		float x = ( pEnemyInfo ->posParts[ 0 ].x - g_playerInfo.posParts[ 0 ].x );
		float z = ( pEnemyInfo ->posParts[ 0 ].z - g_playerInfo.posParts[ 0 ].z );

		g_playerInfo.targetRotParts.y = -atan2f( z, x ) - D3DX_PI / 2;
		if (g_playerInfo.targetRotParts.y > D3DX_PI)
		{
			g_playerInfo.targetRotParts.y = ( ( g_playerInfo.targetRotParts.y ) - ( D3DX_PI * 2 ) );
		}
		if (g_playerInfo.targetRotParts.y < -D3DX_PI)
		{
			g_playerInfo.targetRotParts.y = ( ( g_playerInfo.targetRotParts.y ) + ( D3DX_PI * 2 ) );
		}

		g_playerInfo.posParts[ 0 ].x += cosf( atan2f( z, x ) ) * MOVE_VAL;
		g_playerInfo.posParts[ 0 ].z += sinf( atan2f( z, x ) ) * MOVE_VAL;

		g_playerInfo.bMove = true;
	}

	g_playerInfo.move.x += ( 0.0f - g_playerInfo.move.x ) / 2;
	g_playerInfo.move.z += ( 0.0f - g_playerInfo.move.z ) / 2;

	g_playerInfo.posParts[ 0 ].x += g_playerInfo.move.x;
	g_playerInfo.posParts[ 0 ].z += g_playerInfo.move.z;
}

//===============================================
//
// ���f���Ɋւ���`�揈��
//
//===============================================
void DrawModel(void)
{
	// ���[�J���ϐ�
	//------------------------------------
	LPDIRECT3DDEVICE9 pDevice;						// �f�o�C�X�I�u�W�F�N�g

	// �f�o�C�X�̎擾
	//------------------------------------
	pDevice = GetDevice();

	if ( playerStatus.bHitEnemy == true)
	{
		if ( ( playerStatus.nFrameCount % 20 ) <= 20 / 3 )
		{
			// �`��
			//------------------------------------
			DrawModelMotion( &g_playerInfo );
		}
	}
	else
	{
		// �`��
		//------------------------------------
		DrawModelMotion( &g_playerInfo );
	}

	//// �f�o�b�O�\��
	//PrintDebugProc("/----- ���f�����[ 0 ] -------------/\n");
	//PrintDebugProc("[ ���[���h�ʒu ]\nX : %f  Y : %f  Z : %f\n", g_playerInfo.mtxWorld[ nCnt ]._41, g_playerInfo.mtxWorld[ nCnt ]._42, g_playerInfo.mtxWorld[ nCnt ]._43);
	//PrintDebugProc("[ ���[�J���ʒu ]\nX : %f  Y : %f  Z : %f\n", g_playerInfo.posParts[ 0 ].x, g_playerInfo.posParts[ 0 ].y, g_playerInfo.posParts[ 0 ].z);
	//PrintDebugProc("[ ���݂̌��� ]\nY : %f\n", g_playerInfo.rotModel.y);
	//PrintDebugProc("/-----------------------------------/\n\n");

	//PrintDebugProc("�߂܂����� : %d\n", g_nNumEnemyCatch);

	/*PrintDebugProc("/----- ���f�����[ 1 ] -------------/\n");
	PrintDebugProc("[ ���[�J���ʒu ]\nX : %f  Y : %f  Z : %f\n", g_model[1].posParts[ 0 ].x, g_model[1].posParts[ 0 ].y, g_model[1].posParts[ 0 ].z);
	PrintDebugProc("[ ���݂̌��� ]\nY : %f\n", g_model[1].rotModel.y);
	PrintDebugProc("/-----------------------------------/\n\n");*/
}

//===============================================
//
// ���f���̌����̊�������
//
//===============================================
void RotateModel(void)
{
	// D3DX_PI���傫���l�ɂȂ�����ۂߍ���
	//----------------------------------------------
	if (g_playerInfo.targetRotParts.y - g_playerInfo.rotParts[ 0 ].y > D3DX_PI)
	{
		// �����̏����Ɗۂߍ��݌v�Z
		g_playerInfo.rotParts[ 0 ].y += ((g_playerInfo.targetRotParts.y - g_playerInfo.rotParts[ 0 ].y) - (D3DX_PI * 2)) * ROT_BRAKE_VAL;

		// �K��͈͊O��������ۂߍ���
		//---------------------------------------------
		if (g_playerInfo.rotParts[ 0 ].y > D3DX_PI)
		{
			// �ۂߍ��݌v�Z
			g_playerInfo.rotParts[ 0 ].y = ((g_playerInfo.rotParts[ 0 ].y) - (D3DX_PI * 2));
		}
		else if (g_playerInfo.rotParts[ 0 ].y < -D3DX_PI)
		{
			// �ۂߍ��݌v�Z
			g_playerInfo.rotParts[ 0 ].y = ((g_playerInfo.rotParts[ 0 ].y) + (D3DX_PI * 2));
		}
	}

	// -D3DX_PI��菬���Ȓl�ɂȂ�����
	else if (g_playerInfo.targetRotParts.y - g_playerInfo.rotParts[ 0 ].y < -D3DX_PI)
	{
		// �����̏����Ɗۂߍ��݌v�Z
		g_playerInfo.rotParts[ 0 ].y += ((g_playerInfo.targetRotParts.y - g_playerInfo.rotParts[ 0 ].y) + (D3DX_PI * 2)) * ROT_BRAKE_VAL;

		// �K��͈͊O��������ۂߍ���
		//---------------------------------------------
		if (g_playerInfo.rotParts[ 0 ].y > D3DX_PI)
		{
			// �ۂߍ��݌v�Z
			g_playerInfo.rotParts[ 0 ].y = ((g_playerInfo.rotParts[ 0 ].y) - (D3DX_PI * 2));
		}
		else if (g_playerInfo.rotParts[ 0 ].y < -D3DX_PI)
		{
			// �ۂߍ��݌v�Z
			g_playerInfo.rotParts[ 0 ].y = ((g_playerInfo.rotParts[ 0 ].y) + (D3DX_PI * 2));
		}
	}
	else
	{
		// �ʏ�̊����̌v�Z
		g_playerInfo.rotParts[ 0 ].y += (g_playerInfo.targetRotParts.y - g_playerInfo.rotParts[ 0 ].y) * ROT_BRAKE_VAL;
	}
}

//===============================================
//
// ���f���\���̕ϐ��擾
//
//===============================================
MODEL_INFO *GetModelInfo( void )
{
	return &g_playerInfo;
}

//===============================================
//
// �����蔻��̏����擾
//
//===============================================
COL_RECT *GetRectPlayer()
{
	return &rectPlayer;
}

//===============================================
//
// �G�̕߂܂�������������ϐ�
//
//===============================================
void AddCatchEnemy( int num )
{
	g_nNumEnemyCatch += num;
	if ( g_nNumEnemyCatch < 0 )
	{
		g_nNumEnemyCatch = 0;
	}
}

//===============================================
//
// �G�̕߂܂��������擾
//
//===============================================
int GetCatchEnemy( void )
{
	return g_nNumEnemyCatch;
}

//===============================================
//
// �G�̕߂܂�����������������
//
//===============================================
void InitCatchEnemy( void )
{
	g_nNumEnemyCatch = 0;
}

//===============================================
//
// �o���b�g�^�C�v�̎擾
//
//===============================================
BULLETTYPE GetBulletType( void )
{
	return bulletType;
}

//===============================================
//
// �v���C���[��Ԃ��擾
//
//===============================================
PLAYER_STATUS *GetPlayerStatus( void )
{
	return &playerStatus;
}

//===============================================
//
// �^�C�g���ł̍X�V����
//
//===============================================
void UpdateModelTitle( void )
{
	CAMERA *pCamera = GetCamera();				// �J�����̏��擾

	g_playerInfo.statusOld = g_playerInfo.status;

	// ���f���̖ڕW�p�x��ݒ�
	g_playerInfo.targetRotParts.y = -(D3DX_PI / 2) - pCamera ->rotCamera.y;

	g_playerInfo.status = WALK;

	g_playerInfo.posParts[ 0 ].x += 3.0f;

	if ( g_playerInfo.posParts[ 0 ].x >= SCREEN_WIDTH + 100.0f )
	{
		g_playerInfo.posParts[ 0 ].x = -400.0f;
	}

	// ���f���̌����̊�������
	//-----------------------------------
	RotateModel();

	// ���[�V�����̍X�V����
	//-----------------------------------
	UpdateModelMotion( &g_playerInfo );
}
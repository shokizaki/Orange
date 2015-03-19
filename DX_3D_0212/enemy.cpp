//===============================================
//
//  ���e   �F ���f���Ɋւ��鏈��
//  File   �F enemy.cpp
//  ����� �F �؍�@��
//
//===============================================

//===============================================
// 
//  �C���N���[�h�t�@�C��
//
//===============================================
#include "enemy.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "meshField.h"
#include "model.h"
#include "billboard.h"
#include "bullet.h"
#include "object.h"
#include "stringDraw.h"
#include "score.h"
#include "timer.h"
#include "miniMap.h"

//===============================================
//
//�@�}�N����`
//
//===============================================
#define MODEL_TEXTURE_MAX 	( 2 )			// �g�p����e�N�X�`���̐�
#define MODEL_MAX			( 3 )			// ���f���̍ő吔
#define MOVE_VAL			( 0.3f )		// �ړ���
#define MOVE_VAL_SLOW		( 0.05f )		// �X���E��Ԃ̈ړ���
#define BRAKE_VAL			( 0.07f )		// �����̌W��
#define ROT_BRAKE_VAL		( 0.1f )		// ���f���̉�]�u���[�L
#define GRAVITY_VAL			( 0.5f )		// �d�͗�
#define CURE_SLOW_FRAME		( 180 )			// �X���E��Ԃ���̕��A�t���[��

#define SMALLENEMY_WIDTH	( 30.0f )
#define SMALLENEMY_HEIGHT	( 70.0f )
#define SMALLENEMY_DEPS		( 30.0f )
#define SMALLENEMY_SCORE	( 300 )

#define ENEMY_WIDTH			( 50.0f )
#define ENEMY_HEIGHT		( 80.0f )
#define ENEMY_DEPS			( 50.0f )
#define ENEMY_SCORE			( 200 )

#define BIGENEMY_WIDTH		( 90.0f )
#define BIGENEMY_HEIGHT		( 120.0f )
#define BIGENEMY_DEPS		( 90.0f )
#define BIGENEMY_SCORE		( 100 )

#define APPEAR_FIRST		( 35 )
#define APPEAR_SECOND		( 25 )
#define APPEAR_THIRD		( 15 )
#define APPEAR_FORTH		( 5 )

#define TEX_PASS		"data/TEXTURE/tairu03.jpg"

//-----------------------------------------------
//  �O���[�o���ϐ�
//-----------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureEnemy[MODEL_TEXTURE_MAX];		// �e�N�X�`���ւ̃|�C���^

// �G�Ɋւ���ϐ�
MODEL_INFO g_enemyInit;
MODEL_INFO g_aEnemyInfo[ ENEMY_MAX ];	// �G�̏��
COL_RECT rectEnemy[ ENEMY_MAX ];		// �����蔻��
ENEMY_STATUS enemyStatus[ ENEMY_MAX ];	// �G�̃X�e�[�^�X
bool g_bCol = false;
static bool g_bEnemyAppear = false;
static int g_nEnemyCount = 0;

static D3DXMATRIX g_mtxProjection;		// �v���W�F�N�V�����}�g���b�N�X
static D3DXMATRIX g_mtxView;			// �r���[�}�g���b�N�X
static D3DXMATRIX g_mtxWorld;			// ���[���h�}�g���b�N�X

D3DXVECTOR3 g_posInit;			// �ʒu
D3DXVECTOR3 g_rotInit;			// ����
D3DXVECTOR3 g_sclInit;			// �傫��

//===============================================
//
// ���f���Ɋւ��鏉����
//
//===============================================
void InitEnemy(void)
{
	// ���[�J���ϐ�
	//------------------------------------
	LPDIRECT3DDEVICE9 pDevice = GetDevice();						// �f�o�C�X�I�u�W�F�N�g

	// �O���[�o���ϐ��̏�����
	//------------------------------------
	g_posInit = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	g_rotInit = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	g_sclInit = D3DXVECTOR3( 1.0f, 1.0f, 1.0f );
	g_nEnemyCount = 0;
	g_bEnemyAppear = false;
	g_bCol = false;

	for ( int i = 0; i < ENEMY_MAX; i++ )
	{
		// �g�p����
		enemyStatus[ i ].bUse = false;
	}

	// ���[�V�����̏�����
	InitModelMotionEx( &g_enemyInit, "data/SCRIPT/motionEnemy.txt" );

	// �G�̐���
	if ( GetMode() == MODE_GAME )
	{
		SetEnemy( ENEMYTYPE_WHITE, D3DXVECTOR3( 100.0f, 100.0f, 100.0f ) );
		SetEnemy( ENEMYTYPE_WHITE, D3DXVECTOR3( -200.0f, 100.0f, 300.0f ) );
		SetEnemy( ENEMYTYPE_BLUE, D3DXVECTOR3( -100.0f, 100.0f, 100.0f ) );
		SetEnemy( ENEMYTYPE_BLUE, D3DXVECTOR3( 500.0f, 100.0f, -300.0f ) );
		SetEnemy( ENEMYTYPE_RED, D3DXVECTOR3( 500.0f, 100.0f, 100.0f ) );
		SetEnemy( ENEMYTYPE_RED, D3DXVECTOR3( 300.0f, 100.0f, -100.0f ) );
		SetEnemy( ENEMYTYPE_YELLOW, D3DXVECTOR3( -600.0f, 100.0f, 100.0f ) );
		SetEnemy( ENEMYTYPE_YELLOW, D3DXVECTOR3( -450.0f, 100.0f, -100.0f ) );
	}

	if ( GetMode() == MODE_TITLE )
	{
		SetEnemy( ENEMYTYPE_WHITE, D3DXVECTOR3( -100.0f, -50.0f, 0.0f ) );
	}
}

//===============================================
//
// ���f���Ɋւ���I������
//
//===============================================
void UninitEnemy(void)
{
	// ���[�J���ϐ�
	//------------------------------------

	for ( int i = 0; i < ENEMY_MAX; i++ )
	{
		// �I��
		UninitModelMotion( &g_aEnemyInfo[ i ] );
	}

	for (int nCnt = 0; nCnt < MODEL_TEXTURE_MAX; nCnt++)
	{
		if (g_pTextureEnemy[nCnt] != NULL)
		{ 
			// �g�����e�N�X�`���̉���Ə�����
			//------------------------------------
			g_pTextureEnemy[nCnt] -> Release();
			g_pTextureEnemy[nCnt] = NULL;
		}
	}
	
	// �\���̂̏�����
	for ( int i = 0; i < ENEMY_MAX; i++ )
	{
		memset( &g_aEnemyInfo[ i ], 0, sizeof( MODEL_INFO ) );
	}
}	

//===============================================
//
// ���f���Ɋւ���X�V����
//
//===============================================
void UpdateEnemy(void)
{
	// ���[�J���ϐ�
	//------------------------------------
	CAMERA *pCamera = GetCamera();				// �J�����̏��擾
	COL_RECT *pRectPlayer = GetRectPlayer();			// 
	MESHFIELD *pMeshField = GetMeshField();		// ���b�V���t�B�[���h�̏����擾
	SHADOW *pShadow = GetShadow();
	COL_RECT *pRectBullet = GetRectBullet();
	BULLET *pBullet = GetBullet();
	COL_RECT *pRectObject = GetRectObject();
	MODEL_INFO *pPlayerInfo = GetModelInfo();
	PLAYER_STATUS *pPlayerStatus = GetPlayerStatus();

	static bool bTutorial = false;
	static bool bTutorialTry = false;

	if ( GetMode() == MODE_TUTORIAL )
	{
		if ( GetStringMessage() == NEXTEXE && bTutorial == false )
		{
			if ( pCamera ->posCameraP.x + 500.0f > (pMeshField ->nNumX * pMeshField ->fWidth) / 2 )
			{
				SetEnemy( ENEMYTYPE_WHITE, D3DXVECTOR3( pCamera ->posCameraP.x - 500.0f, 100.0f, pRectPlayer ->pos.z ) );
			}
			else if( pCamera ->posCameraP.x - 500.0f < -(pMeshField ->nNumX * pMeshField ->fWidth) / 2 )
			{
				SetEnemy( ENEMYTYPE_WHITE, D3DXVECTOR3( pCamera ->posCameraP.x + 500.0f, 100.0f, pRectPlayer ->pos.z ) );
			}
			else
			{
				SetEnemy( ENEMYTYPE_WHITE, D3DXVECTOR3( pCamera ->posCameraP.x + 500.0f, 100.0f, pRectPlayer ->pos.z ) );
			}

			bTutorial = true;
		}

		if (  GetStringMessage() == TRY && g_nEnemyCount == 0 )
		{
			SetEnemy( ENEMYTYPE_WHITE,  D3DXVECTOR3( 500.0f, 100.0f, -100.0f ) );
			SetEnemy( ENEMYTYPE_RED,    D3DXVECTOR3( 300.0f, 100.0f, 200.0f ) );
			SetEnemy( ENEMYTYPE_BLUE,   D3DXVECTOR3( 600.0f, 100.0f, 100.0f ) );
			SetEnemy( ENEMYTYPE_YELLOW, D3DXVECTOR3( 200.0f, 100.0f, -200.0f ) );
		}
	}

	// �G�̍ďo������
	if ( ( GetTimer() == APPEAR_FIRST ||  GetTimer() == APPEAR_SECOND || GetTimer() == APPEAR_THIRD || GetTimer() == APPEAR_FORTH ) && g_bEnemyAppear == false )
	{
		for ( int i = g_nEnemyCount, j = g_nEnemyCount; i < ( j + 5 ); i++ )
		{
			switch ( i % 4 )
			{
			case 0:
				SetEnemy( ENEMYTYPE_WHITE,  D3DXVECTOR3( (float)i * -50.0f, 100.0f, ( i - 1 ) * 50.0f ) );	
				break;

			case 1:
				SetEnemy( ENEMYTYPE_RED,  D3DXVECTOR3( (float)i * -70.0f, 100.0f, i * 80.0f ) );	
				break;

			case 2:
				SetEnemy( ENEMYTYPE_YELLOW,  D3DXVECTOR3( (float)i * -100.0f, 100.0f, i * -50.0f ) );	
				break;

			case 3:
				SetEnemy( ENEMYTYPE_BLUE,  D3DXVECTOR3( (float)i * 50.0f, 100.0f, i * -70.0f ) );	
				break;
			}		
		}

		g_bEnemyAppear = true;
	}
	if ( ( GetTimer() == APPEAR_FIRST - 1 ||  GetTimer() == APPEAR_SECOND - 1 || GetTimer() == APPEAR_THIRD - 1 || GetTimer() == APPEAR_FORTH - 1 ) && g_bEnemyAppear == true )
	{
		g_bEnemyAppear = false;
	}

	for ( int i = 0; i < ENEMY_MAX; i++ )
	{
		if ( enemyStatus[ i ].bUse == true )
		{
			// �O��̈ʒu��ۑ�
			g_aEnemyInfo[ i ].posOld = g_aEnemyInfo[ i ].posParts[ 0 ];
			g_aEnemyInfo[ i ].statusOld = g_aEnemyInfo[ i ].status;

			// �d�͂�������
			//------------------------------------
			g_aEnemyInfo[ i ].posParts[ 0 ].y += g_aEnemyInfo[ i ].fGravity;
			g_aEnemyInfo[ i ].fGravity -= GRAVITY_VAL;

			// �߂荞�܂Ȃ�����
			//------------------------------------
			if ( g_aEnemyInfo[ i ].posParts[ 0 ].y - 5.0f <= 0.0f )
			{
				g_aEnemyInfo[ i ].posParts[ 0 ].y = g_aEnemyInfo[ i ].posOld.y;		// �O���Y���W�ɖ߂�
				rectEnemy[ i ].pos = g_aEnemyInfo[ i ].posParts[ 0 ];					// �����蔻��p�ϐ��̍X�V
				g_aEnemyInfo[ i ].fGravity = 0.0f;									// �W�����v�ʂ�������
			}

			// �o���b�g������������Ԃ��ǂ���
			if ( enemyStatus[ i ].bHitBullet == false )
			{
				enemyStatus[ i ].move = D3DXVECTOR3( MOVE_VAL, 0.0f, MOVE_VAL );
			}
			else
			{
				enemyStatus[ i ].move = D3DXVECTOR3( MOVE_VAL_SLOW, 0.0f, MOVE_VAL_SLOW );
			}

			if ( GetStringMessage() >= ABSORB )
			{
				switch ( enemyStatus[ i ].movePattern )
				{
				case MOVEPATTERN_A:
					g_aEnemyInfo[ i ].nMoveCnt++;

					// �E�ړ�
					if ( g_aEnemyInfo[ i ].nMoveCnt >= 180 && g_aEnemyInfo[ i ].nMoveCnt < 360 )
					{
						// ���f���̖ڕW�p�x��ݒ�
						g_aEnemyInfo[ i ].targetRotParts.y = -( D3DX_PI / 2 ) - pCamera ->rotCamera.y;

						// �ړ��ʂ�ݒ�
						g_aEnemyInfo[ i ].move.x += cosf( pCamera ->rotCamera.y ) * enemyStatus[ i ].move.x;
						g_aEnemyInfo[ i ].move.z += sinf( pCamera ->rotCamera.y ) * enemyStatus[ i ].move.z;

						g_aEnemyInfo[ i ].status = WALK;
					}
					// ���ړ�
					else if ( g_aEnemyInfo[ i ].nMoveCnt >= 360 && g_aEnemyInfo[ i ].nMoveCnt < 800 )
					{
						// ���f���̖ڕW�p�x��ݒ�
						g_aEnemyInfo[ i ].targetRotParts.y = D3DX_PI / 2 - pCamera ->rotCamera.y;

						// �ړ��ʂ�ݒ�
						g_aEnemyInfo[ i ].move.x -= cosf( pCamera ->rotCamera.y ) * enemyStatus[ i ].move.x;
						g_aEnemyInfo[ i ].move.z -= sinf( pCamera ->rotCamera.y ) * enemyStatus[ i ].move.z;

						g_aEnemyInfo[ i ].status = WALK;
					}
					else if ( g_aEnemyInfo[ i ].nMoveCnt == 800 )
					{
						g_aEnemyInfo[ i ].nMoveCnt = 0;

						g_aEnemyInfo[ i ].status = NEUTRAL;
					}

					break;

				case MOVEPATTERN_B:
					g_aEnemyInfo[ i ].nMoveCnt++;

					// ��ړ�
					if ( g_aEnemyInfo[ i ].nMoveCnt >= 180 && g_aEnemyInfo[ i ].nMoveCnt < 420 )
					{
						// ���f���̖ڕW�p�x��ݒ�
						g_aEnemyInfo[ i ].targetRotParts.y = D3DX_PI - pCamera ->rotCamera.y;

						// �ړ��ʂ�ݒ�
						g_aEnemyInfo[ i ].move.x -= sinf( pCamera->rotCamera.y ) * enemyStatus[ i ].move.x;
						g_aEnemyInfo[ i ].move.z += cosf( pCamera->rotCamera.y ) * enemyStatus[ i ].move.z;

						g_aEnemyInfo[ i ].status = WALK;
					}
					// ���ړ�
					else if ( g_aEnemyInfo[ i ].nMoveCnt >= 420 && g_aEnemyInfo[ i ].nMoveCnt < 650 )
					{
						// ���f���̖ڕW�p�x��ݒ�
						g_aEnemyInfo[ i ].targetRotParts.y = 0.0f - pCamera ->rotCamera.y;

						// �ړ��ʂ�ݒ�
						g_aEnemyInfo[ i ].move.x += sinf( pCamera->rotCamera.y ) * enemyStatus[ i ].move.x;
						g_aEnemyInfo[ i ].move.z -= cosf( pCamera->rotCamera.y ) * enemyStatus[ i ].move.z;

						g_aEnemyInfo[ i ].status = WALK;
					}
					else if ( g_aEnemyInfo[ i ].nMoveCnt == 650 )
					{
						g_aEnemyInfo[ i ].nMoveCnt = 0;

						g_aEnemyInfo[ i ].status = NEUTRAL;
					}

					break;

				case MOVEPATTERN_C:
					g_aEnemyInfo[ i ].nMoveCnt++;

					// �E�ړ�
					if ( g_aEnemyInfo[ i ].nMoveCnt >= 100 && g_aEnemyInfo[ i ].nMoveCnt < 360 )
					{
						// ���f���̖ڕW�p�x��ݒ�
						g_aEnemyInfo[ i ].targetRotParts.y = -( D3DX_PI / 2 ) - pCamera ->rotCamera.y;

						// �ړ��ʂ�ݒ�
						g_aEnemyInfo[ i ].move.x += cosf( pCamera->rotCamera.y ) * enemyStatus[ i ].move.x;
						g_aEnemyInfo[ i ].move.z += sinf( pCamera->rotCamera.y ) * enemyStatus[ i ].move.z;

						g_aEnemyInfo[ i ].status = WALK;
					}
					// ���ړ�
					else if ( g_aEnemyInfo[ i ].nMoveCnt >= 360 && g_aEnemyInfo[ i ].nMoveCnt < 600 )
					{
						// ���f���̖ڕW�p�x��ݒ�
						g_aEnemyInfo[ i ].targetRotParts.y = D3DX_PI / 2 - pCamera ->rotCamera.y;

						// �ړ��ʂ�ݒ�
						g_aEnemyInfo[ i ].move.x -= cosf( pCamera->rotCamera.y ) * enemyStatus[ i ].move.x;
						g_aEnemyInfo[ i ].move.z -= sinf( pCamera->rotCamera.y ) * enemyStatus[ i ].move.z;

						g_aEnemyInfo[ i ].status = WALK;
					}
					else if ( g_aEnemyInfo[ i ].nMoveCnt == 600 )
					{
						g_aEnemyInfo[ i ].nMoveCnt = 0;

						g_aEnemyInfo[ i ].status = NEUTRAL;
					}

					break;

				case MOVEPATTERN_D:
					g_aEnemyInfo[ i ].nMoveCnt++;

					// ��ړ�
					if ( g_aEnemyInfo[ i ].nMoveCnt >= 40 && g_aEnemyInfo[ i ].nMoveCnt < 360 )
					{
						// ���f���̖ڕW�p�x��ݒ�
						g_aEnemyInfo[ i ].targetRotParts.y = D3DX_PI - pCamera ->rotCamera.y;

						// �ړ��ʂ�ݒ�
						g_aEnemyInfo[ i ].move.x -= sinf( pCamera->rotCamera.y ) * enemyStatus[ i ].move.x;
						g_aEnemyInfo[ i ].move.z += cosf( pCamera->rotCamera.y ) * enemyStatus[ i ].move.z;

						g_aEnemyInfo[ i ].status = WALK;
					}
					// ���ړ�
					else if ( g_aEnemyInfo[ i ].nMoveCnt >= 360 && g_aEnemyInfo[ i ].nMoveCnt < 500 )
					{
						// ���f���̖ڕW�p�x��ݒ�
						g_aEnemyInfo[ i ].targetRotParts.y = 0.0f - pCamera ->rotCamera.y;

						// �ړ��ʂ�ݒ�
						g_aEnemyInfo[ i ].move.x += sinf( pCamera->rotCamera.y ) * enemyStatus[ i ].move.x;
						g_aEnemyInfo[ i ].move.z -= cosf( pCamera->rotCamera.y ) * enemyStatus[ i ].move.z;

						g_aEnemyInfo[ i ].status = WALK;
					}
					else if ( g_aEnemyInfo[ i ].nMoveCnt == 500 )
					{
						g_aEnemyInfo[ i ].nMoveCnt = 0;

						g_aEnemyInfo[ i ].status = NEUTRAL;
					}

					break;
				}
			}

			// �O��̃��[�V�����ƈ������
			if ( g_aEnemyInfo[ i ].statusOld != g_aEnemyInfo[ i ].status )
			{
				g_aEnemyInfo[ i ].nKey = 0;
				g_aEnemyInfo[ i ].nFrameCount = 0;
				if ( g_aEnemyInfo[ i ].statusOld == WALK )
				{
					g_aEnemyInfo[ i ].status = NEUTRAL;
				}
			}

			// �ڕW�̌������K��͈͂𒴂��Ă�����ۂߍ���
			if (g_aEnemyInfo[ i ].targetRotParts.y > D3DX_PI)
			{
				g_aEnemyInfo[ i ].targetRotParts.y = ((g_aEnemyInfo[ i ].targetRotParts.y) - (D3DX_PI * 2));
			}
			if (g_aEnemyInfo[ i ].targetRotParts.y < -D3DX_PI)
			{
				g_aEnemyInfo[ i ].targetRotParts.y = ((g_aEnemyInfo[ i ].targetRotParts.y) + (D3DX_PI * 2));
			}

			// ���������X�Ɏ�߂�
			g_aEnemyInfo[ i ].move.x += (0.0f - g_aEnemyInfo[ i ].move.x) * 0.07f;
			g_aEnemyInfo[ i ].move.z += (0.0f - g_aEnemyInfo[ i ].move.z) * 0.07f;

			// ��������l�����Z
			g_aEnemyInfo[ i ].posParts[ 0 ].x += g_aEnemyInfo[ i ].move.x;
			g_aEnemyInfo[ i ].posParts[ 0 ].z += g_aEnemyInfo[ i ].move.z;

			// �ǂɓ���������
			//------------------------------------
			if (g_aEnemyInfo[ i ].posParts[ 0 ].x - 30.0f < -(pMeshField ->nNumX * pMeshField ->fWidth) / 2 
			||	g_aEnemyInfo[ i ].posParts[ 0 ].x + 30.0f > (pMeshField ->nNumX * pMeshField ->fWidth) / 2)
			{
				// X���W�݂̂��Ƃɖ߂�
				g_aEnemyInfo[ i ].posParts[ 0 ].x = g_aEnemyInfo[ i ].posOld.x;
				g_aEnemyInfo[ i ].move.x = -g_aEnemyInfo[ i ].move.x;
			}
			if (g_aEnemyInfo[ i ].posParts[ 0 ].z - 30.0f < -(pMeshField ->nNumZ * pMeshField ->fHeight) / 2
			||	g_aEnemyInfo[ i ].posParts[ 0 ].z + 30.0f > (pMeshField ->nNumZ * pMeshField ->fHeight) / 2)
			{
				// Z���W�݂̂��Ƃɖ߂�
				g_aEnemyInfo[ i ].posParts[ 0 ].z = g_aEnemyInfo[ i ].posOld.z;
				g_aEnemyInfo[ i ].move.z = -g_aEnemyInfo[ i ].move.z;
			}

			// �����蔻��p�̏����X�V
			rectEnemy[ i ].pos = g_aEnemyInfo[ i ].posParts[ 0 ];

			// �v���C���[�Ƃ̓����蔻��
			if ( pPlayerStatus ->bHitEnemy == false )
			{
				if ( ColRect( pRectPlayer, &rectEnemy[ i ] ) == true )
				{
					if ( enemyStatus[ i ].enemyType != ENEMYTYPE_WHITE )
					{
						if ( enemyStatus[ i ].bHitBullet == true )
						{
							PlaySound( ABSORB_SE );

							enemyStatus[ i ].bUse = false;
							( pShadow + i + 1 ) ->bUse = false;
							AddCatchEnemy( 1 );		// �߂܂�����

							// �G�̑��������炷
							g_nEnemyCount--;

							// �X�R�A���Z
							AddScore( enemyStatus[ i ].nScore );

							// �`���[�g���A���̎�
							if ( GetStringMessage() == GO )
							{
								SetEnterFlag( false );
								SetStringMessage( ABSORB );
							}

							// �����������肪�Ȃ�������
							if ( g_bCol == false )
							{
								pPlayerInfo ->nBillboardNum = SetBillboard( pRectPlayer ->pos, TEXTUREANIM_CHARGE, 10.0f, 80.0f, 80.0f );
							}

							g_bCol= true;		// �����������肠��

							enemyStatus[ i ].bHitBullet = false;
							SetUseBillboard( g_aEnemyInfo[ i ].nBillboardNum );

							// �\���̂̒��g��������
							memset( &g_aEnemyInfo[ i ], 0, sizeof( MODEL_INFO ) );
						}
						else
						{
							AddCatchEnemy( -3 );

							pPlayerStatus ->bHitEnemy = true;
						}
					}
					else
					{
						// ��
						PlaySound( ABSORB_SE );

						enemyStatus[ i ].bUse = false;
						( pShadow + i + 1 ) ->bUse = false;
						AddCatchEnemy( 1 );		// �߂܂�����

						// �G�̑��������炷
						g_nEnemyCount--;

						// �X�R�A���Z
						AddScore( enemyStatus[ i ].nScore );

						// �`���[�g���A���̎�
						if ( GetStringMessage() == GO )
						{
							SetEnterFlag( false );
							SetStringMessage( ABSORB );
						}

						// �����������肪�Ȃ�������
						if ( g_bCol == false )
						{
							pPlayerInfo ->nBillboardNum = SetBillboard( pRectPlayer ->pos, TEXTUREANIM_CHARGE, 10.0f, 80.0f, 80.0f );
						}

						g_bCol= true;		// �����������肠��

						// �\���̂̒��g��������
						memset( &g_aEnemyInfo[ i ], 0, sizeof( MODEL_INFO ) );
					}
				}
			}

			// �����Ƃ̓����蔻��
			for ( int j = 0; j < BULLET_MAX; j++ )
			{
				if ( ( pBullet + j ) ->bUse == true )
				{
					if ( ColRect( pRectBullet + j, &rectEnemy[ i ] ) == true )
					{
						// �����̎�ނƍ��v���Ă�����
						if ( ( ( int )( ( pBullet + j ) ->type ) == ( int )enemyStatus[ i ].enemyType ) )
						{
							// ��
							PlaySound(ONPCOL_SE);

							// �����������肪�Ȃ�������
							if ( enemyStatus[ i ].bHitBullet == false )
							{
								// �r���{�[�h�̃Z�b�g
								g_aEnemyInfo[ i ].nBillboardNum = SetBillboard( g_aEnemyInfo[ i ].posParts[ 0 ], TEXTUREANIM_HASTY, rectEnemy[ i ].harfSize.y * 2, 15.0f, 15.0f );
							}

							( pBullet + j ) ->bUse = false;				// �o���b�g�̎g�p��OFF
							UseShadow( ( pBullet + j ) ->nShadowNum );	// �o���b�g�̉e��OFF
							
							// �����������肠��
							enemyStatus[ i ].bHitBullet = true;
						}
					}
				}
			}

			// �I�u�W�F�N�g�Ƃ̓����蔻��
			for ( int j = 0; j < MAX_OBJECT; j++ )
			{
				if ( ColRect( &pRectObject[ j ], &rectEnemy[ i ] ) == true )
				{
					g_aEnemyInfo[ i ].posParts[ 0 ] = g_aEnemyInfo[ i ].posOld;
					rectEnemy[ i ].pos = g_aEnemyInfo[ i ].posParts[ 0 ];
				}
			}

			// �G�ƃv���C���[�������������肪��������
			if ( g_bCol == true )
			{
				SetPosBillboard( pPlayerInfo ->nBillboardNum, D3DXVECTOR3( pPlayerInfo ->posParts[ 0 ].x, pPlayerInfo ->posParts[ 0 ].y - 10, pPlayerInfo ->posParts[ 0 ].z ) );
			}

			// ����������Ԃ�������
			if ( enemyStatus[ i ].bHitBullet == true )
			{
				// ���A����܂ł̃t���[�������Z
				enemyStatus[ i ].nHitFrame++;

				// �r���{�[�h�̈ʒu��ݒ�
				SetPosBillboard( g_aEnemyInfo[ i ].nBillboardNum, g_aEnemyInfo[ i ].posParts[ 0 ] );

				// ���A�t���[���ɂȂ�����
				if ( enemyStatus[ i ].nHitFrame == CURE_SLOW_FRAME )
				{
					enemyStatus[ i ].bHitBullet = false;
					enemyStatus[ i ].nHitFrame = 0;
					SetUseBillboard( g_aEnemyInfo[ i ].nBillboardNum );
				}
			}

			// �e�̍X�V�i�C���f�b�N�X�w��j
			//UpdateShadowEx( g_aEnemyInfo[ i ].nShadowNum, g_aEnemyInfo[ i ].posParts[ 0 ] );

			SetShadow( g_aEnemyInfo[ i ].posParts[ 0 ], rectEnemy[ i ].harfSize.x, rectEnemy[ i ].harfSize.y, 0.7f );

			// ���[�V�����̍X�V����
			UpdateModelMotion( &g_aEnemyInfo[ i ] );

			// �~�j�}�b�v�Z�b�g
			SetMiniMap(D3DXVECTOR3( g_aEnemyInfo[ i ].posParts[ 0 ].x, 0.0f, g_aEnemyInfo[ i ].posParts[ 0 ].z ), MINIMAPTYPE_ENEMY, g_aEnemyInfo[ i ].targetRotParts.y);

		}
	}

	// ���f���̌����̊�������
	//-----------------------------------
	RotateEnemy();
}

//===============================================
//
// ���f���Ɋւ���`�揈��
//
//===============================================
void DrawEnemy(void)
{
	// ���[�J���ϐ�
	//------------------------------------
	LPDIRECT3DDEVICE9 pDevice;						// �f�o�C�X�I�u�W�F�N�g

	// �f�o�C�X�̎擾
	//------------------------------------
	pDevice = GetDevice();

	// �`��
	//------------------------------------
	for ( int i = 0; i < ENEMY_MAX; i++ )
	{
		DrawModelMotion( &g_aEnemyInfo[ i ] );
	}

	// �f�o�b�O�\��
	/*PrintDebugProc("/----- ���f�����[ 0 ] -------------/\n");
	PrintDebugProc("[ ���[���h�ʒu ]\nX : %f  Y : %f  Z : %f\n", g_aEnemyInfo[ i ][ 0 ].mtxWorld._41, g_aEnemyInfo[ i ][ 0 ].mtxWorld._42, g_aEnemyInfo[ i ][ 0 ].mtxWorld._43);
	PrintDebugProc("[ ���[�J���ʒu ]\nX : %f  Y : %f  Z : %f\n", g_aEnemyInfo[ i ].posParts[ 0 ].x, g_aEnemyInfo[ i ].posParts[ 0 ].y, g_aEnemyInfo[ i ].posParts[ 0 ].z);
	PrintDebugProc("[ ���݂̌��� ]\nY : %f\n", g_aEnemyInfo[ i ].rotParts[ 0 ].y);
	PrintDebugProc("/-----------------------------------/\n\n");

	PrintDebugProc("/----- ���f�����[ 1 ] -------------/\n");
	PrintDebugProc("[ ���[�J���ʒu ]\nX : %f  Y : %f  Z : %f\n", g_aEnemyInfo[1].posModel.x, g_aEnemyInfo[1].posModel.y, g_aEnemyInfo[1].posModel.z);
	PrintDebugProc("[ ���݂̌��� ]\nY : %f\n", g_aEnemyInfo[1].rotModel.y);
	PrintDebugProc("/-----------------------------------/\n\n");*/
}

//===============================================
//
// ���f���̌����̊�������
//
//===============================================
void RotateEnemy(void)
{
	for ( int i = 0; i < ENEMY_MAX; i++ )
	{
		// D3DX_PI���傫���l�ɂȂ�����ۂߍ���
		//----------------------------------------------
		if (g_aEnemyInfo[ i ].targetRotParts.y - g_aEnemyInfo[ i ].rotParts[ 0 ].y > D3DX_PI)
		{
			// �����̏����Ɗۂߍ��݌v�Z
			g_aEnemyInfo[ i ].rotParts[ 0 ].y += ((g_aEnemyInfo[ i ].targetRotParts.y - g_aEnemyInfo[ i ].rotParts[ 0 ].y) - (D3DX_PI * 2)) * ROT_BRAKE_VAL;

			// �K��͈͊O��������ۂߍ���
			//---------------------------------------------
			if (g_aEnemyInfo[ i ].rotParts[ 0 ].y > D3DX_PI)
			{
				// �ۂߍ��݌v�Z
				g_aEnemyInfo[ i ].rotParts[ 0 ].y = ((g_aEnemyInfo[ i ].rotParts[ 0 ].y) - (D3DX_PI * 2));
			}
			else if (g_aEnemyInfo[ i ].rotParts[ 0 ].y < -D3DX_PI)
			{
				// �ۂߍ��݌v�Z
				g_aEnemyInfo[ i ].rotParts[ 0 ].y = ((g_aEnemyInfo[ i ].rotParts[ 0 ].y) + (D3DX_PI * 2));
			}
		}

		// -D3DX_PI��菬���Ȓl�ɂȂ�����
		else if (g_aEnemyInfo[ i ].targetRotParts.y - g_aEnemyInfo[ i ].rotParts[ 0 ].y < -D3DX_PI)
		{
			// �����̏����Ɗۂߍ��݌v�Z
			g_aEnemyInfo[ i ].rotParts[ 0 ].y += ((g_aEnemyInfo[ i ].targetRotParts.y - g_aEnemyInfo[ i ].rotParts[ 0 ].y) + (D3DX_PI * 2)) * ROT_BRAKE_VAL;

			// �K��͈͊O��������ۂߍ���
			//---------------------------------------------
			if (g_aEnemyInfo[ i ].rotParts[ 0 ].y > D3DX_PI)
			{
				// �ۂߍ��݌v�Z
				g_aEnemyInfo[ i ].rotParts[ 0 ].y = ((g_aEnemyInfo[ i ].rotParts[ 0 ].y) - (D3DX_PI * 2));
			}
			else if (g_aEnemyInfo[ i ].rotParts[ 0 ].y < -D3DX_PI)
			{
				// �ۂߍ��݌v�Z
				g_aEnemyInfo[ i ].rotParts[ 0 ].y = ((g_aEnemyInfo[ i ].rotParts[ 0 ].y) + (D3DX_PI * 2));
			}
		}
		else
		{
			// �ʏ�̊����̌v�Z
			g_aEnemyInfo[ i ].rotParts[ 0 ].y += (g_aEnemyInfo[ i ].targetRotParts.y - g_aEnemyInfo[ i ].rotParts[ 0 ].y) * ROT_BRAKE_VAL;
		}
	}
}

//===============================================
//
// ���f���\���̕ϐ��擾
//
//===============================================
void SetEnemy( ENEMYTYPE type, D3DXVECTOR3 pos )
{
	// ���[�J���ϐ�
	//------------------------------------
	LPDIRECT3DDEVICE9 pDevice = GetDevice();						// �f�o�C�X�I�u�W�F�N�g

	for ( int i = 0; i < ENEMY_MAX; i++ )
	{
		if ( enemyStatus[ i ].bUse == false )
		{
			enemyStatus[ i ].enemyType = type;
			switch ( type )
			{
			case ENEMYTYPE_WHITE:
				// x�t�@�C���̓ǂݍ���
				//------------------------------------
				D3DXLoadMeshFromX("data/MODEL/eBody.x",		// �ǂݍ��ރt�@�C����
								  D3DXMESH_SYSTEMMEM,		// 
								  pDevice,					// 
								  NULL,						// 
								  &g_aEnemyInfo[ i ].pBuffMatModel[ 0 ],			// 
								  NULL,						// 
								  &g_aEnemyInfo[ i ].numMatModel[ 0 ],			// 
								  &g_aEnemyInfo[ i ].pMeshModel[ 0 ]);

				// x�t�@�C���̓ǂݍ���
				//------------------------------------
				D3DXLoadMeshFromX("data/MODEL/eArmL.x",		// �ǂݍ��ރt�@�C����
								  D3DXMESH_SYSTEMMEM,		// 
								  pDevice,					// 
								  NULL,						// 
								  &g_aEnemyInfo[ i ].pBuffMatModel[ 1 ],			// 
								  NULL,						// 
								  &g_aEnemyInfo[ i ].numMatModel[ 1 ],			// 
								  &g_aEnemyInfo[ i ].pMeshModel[ 1 ]);

				// x�t�@�C���̓ǂݍ���
				//------------------------------------
				D3DXLoadMeshFromX("data/MODEL/eArmR.x",		// �ǂݍ��ރt�@�C����
								  D3DXMESH_SYSTEMMEM,		// 
								  pDevice,					// 
								  NULL,						// 
								  &g_aEnemyInfo[ i ].pBuffMatModel[ 2 ],			// 
								  NULL,						// 
								  &g_aEnemyInfo[ i ].numMatModel[ 2 ],			// 
								  &g_aEnemyInfo[ i ].pMeshModel[ 2 ]);

				// �傫���̐ݒ�
				rectEnemy[ i ].harfSize = D3DXVECTOR3( ENEMY_WIDTH / 2, ENEMY_HEIGHT / 2, ENEMY_DEPS / 2 );
				enemyStatus[ i ].nScore = ENEMY_SCORE;
				break;

			case ENEMYTYPE_RED:
				// x�t�@�C���̓ǂݍ���
				//------------------------------------
				D3DXLoadMeshFromX("data/MODEL/eBody_red.x",		// �ǂݍ��ރt�@�C����
								  D3DXMESH_SYSTEMMEM,		// 
								  pDevice,					// 
								  NULL,						// 
								  &g_aEnemyInfo[ i ].pBuffMatModel[ 0 ],			// 
								  NULL,						// 
								  &g_aEnemyInfo[ i ].numMatModel[ 0 ],			// 
								  &g_aEnemyInfo[ i ].pMeshModel[ 0 ]);

				// x�t�@�C���̓ǂݍ���
				//------------------------------------
				D3DXLoadMeshFromX("data/MODEL/eArmL_red.x",		// �ǂݍ��ރt�@�C����
								  D3DXMESH_SYSTEMMEM,		// 
								  pDevice,					// 
								  NULL,						// 
								  &g_aEnemyInfo[ i ].pBuffMatModel[ 1 ],			// 
								  NULL,						// 
								  &g_aEnemyInfo[ i ].numMatModel[ 1 ],			// 
								  &g_aEnemyInfo[ i ].pMeshModel[ 1 ]);

				// x�t�@�C���̓ǂݍ���
				//------------------------------------
				D3DXLoadMeshFromX("data/MODEL/eArmR_red.x",		// �ǂݍ��ރt�@�C����
								  D3DXMESH_SYSTEMMEM,		// 
								  pDevice,					// 
								  NULL,						// 
								  &g_aEnemyInfo[ i ].pBuffMatModel[ 2 ],			// 
								  NULL,						// 
								  &g_aEnemyInfo[ i ].numMatModel[ 2 ],			// 
								  &g_aEnemyInfo[ i ].pMeshModel[ 2 ]);

				// �傫���̐ݒ�
				rectEnemy[ i ].harfSize = D3DXVECTOR3( ENEMY_WIDTH / 2, ENEMY_HEIGHT / 2, ENEMY_DEPS / 2 );
				enemyStatus[ i ].nScore = ENEMY_SCORE;				
				break;

			case ENEMYTYPE_BLUE:
				// x�t�@�C���̓ǂݍ���
				//------------------------------------
				D3DXLoadMeshFromX("data/MODEL/eBody_blue.x",		// �ǂݍ��ރt�@�C����
								  D3DXMESH_SYSTEMMEM,		// 
								  pDevice,					// 
								  NULL,						// 
								  &g_aEnemyInfo[ i ].pBuffMatModel[ 0 ],			// 
								  NULL,						// 
								  &g_aEnemyInfo[ i ].numMatModel[ 0 ],			// 
								  &g_aEnemyInfo[ i ].pMeshModel[ 0 ]);

				// x�t�@�C���̓ǂݍ���
				//------------------------------------
				D3DXLoadMeshFromX("data/MODEL/eArmL_blue.x",		// �ǂݍ��ރt�@�C����
								  D3DXMESH_SYSTEMMEM,		// 
								  pDevice,					// 
								  NULL,						// 
								  &g_aEnemyInfo[ i ].pBuffMatModel[ 1 ],			// 
								  NULL,						// 
								  &g_aEnemyInfo[ i ].numMatModel[ 1 ],			// 
								  &g_aEnemyInfo[ i ].pMeshModel[ 1 ]);

				// x�t�@�C���̓ǂݍ���
				//------------------------------------
				D3DXLoadMeshFromX("data/MODEL/eArmR_blue.x",		// �ǂݍ��ރt�@�C����
								  D3DXMESH_SYSTEMMEM,		// 
								  pDevice,					// 
								  NULL,						// 
								  &g_aEnemyInfo[ i ].pBuffMatModel[ 2 ],			// 
								  NULL,						// 
								  &g_aEnemyInfo[ i ].numMatModel[ 2 ],			// 
								  &g_aEnemyInfo[ i ].pMeshModel[ 2 ]);

				// �傫���̐ݒ�
				rectEnemy[ i ].harfSize = D3DXVECTOR3( BIGENEMY_WIDTH / 2, BIGENEMY_HEIGHT / 2, BIGENEMY_DEPS / 2 );
				enemyStatus[ i ].nScore = BIGENEMY_SCORE;				
				break;

			case ENEMYTYPE_YELLOW:
				// x�t�@�C���̓ǂݍ���
				//------------------------------------
				D3DXLoadMeshFromX("data/MODEL/eBody_yellow.x",		// �ǂݍ��ރt�@�C����
								  D3DXMESH_SYSTEMMEM,		// 
								  pDevice,					// 
								  NULL,						// 
								  &g_aEnemyInfo[ i ].pBuffMatModel[ 0 ],			// 
								  NULL,						// 
								  &g_aEnemyInfo[ i ].numMatModel[ 0 ],			// 
								  &g_aEnemyInfo[ i ].pMeshModel[ 0 ]);

				// x�t�@�C���̓ǂݍ���
				//------------------------------------
				D3DXLoadMeshFromX("data/MODEL/eArmL_yellow.x",		// �ǂݍ��ރt�@�C����
								  D3DXMESH_SYSTEMMEM,		// 
								  pDevice,					// 
								  NULL,						// 
								  &g_aEnemyInfo[ i ].pBuffMatModel[ 1 ],			// 
								  NULL,						// 
								  &g_aEnemyInfo[ i ].numMatModel[ 1 ],			// 
								  &g_aEnemyInfo[ i ].pMeshModel[ 1 ]);

				// x�t�@�C���̓ǂݍ���
				//------------------------------------
				D3DXLoadMeshFromX("data/MODEL/eArmR_yellow.x",		// �ǂݍ��ރt�@�C����
								  D3DXMESH_SYSTEMMEM,		// 
								  pDevice,					// 
								  NULL,						// 
								  &g_aEnemyInfo[ i ].pBuffMatModel[ 2 ],			// 
								  NULL,						// 
								  &g_aEnemyInfo[ i ].numMatModel[ 2 ],			// 
								  &g_aEnemyInfo[ i ].pMeshModel[ 2 ]);

				// �傫���̐ݒ�
				rectEnemy[ i ].harfSize = D3DXVECTOR3( SMALLENEMY_WIDTH / 2, SMALLENEMY_HEIGHT / 2, SMALLENEMY_DEPS / 2 );
				enemyStatus[ i ].nScore = SMALLENEMY_SCORE;
				break;
			}

			// �����ʒu
			g_aEnemyInfo[ i ].posParts[ 0 ] = pos;
			g_aEnemyInfo[ i ].nNumParts = g_enemyInit.nNumParts;
			g_aEnemyInfo[ i ].targetRotParts = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
			g_aEnemyInfo[ i ].nKey = 0;
			g_aEnemyInfo[ i ].nFrameCount = 0;
			g_aEnemyInfo[ i ].nMoveCnt = 0;

			// �����R�s�[
			for ( int j = 0; j < g_aEnemyInfo[ i ].nNumParts; j++ )
			{
				g_aEnemyInfo[ i ].rotParts[ j ] = g_aEnemyInfo[ i ].rotParts[ 0 ];
				g_aEnemyInfo[ i ].scl[ j ] = D3DXVECTOR3( 1.0f, 1.0f, 1.0f );
				g_aEnemyInfo[ i ].nIndex[ j ] = g_enemyInit.nIndex[ j ];
				g_aEnemyInfo[ i ].nParent[ j ] = g_enemyInit.nParent[ j ];
				g_aEnemyInfo[ i ].rotDevide[ j ] = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
				g_aEnemyInfo[ i ].posDevide[ j ] = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );

				// ���[�V�������̃R�s�[
				for ( int k = 0; k < 5; k++ )
				{
					g_aEnemyInfo[ i ].aMotion[ k ] = g_enemyInit.aMotion[ k ];
					g_aEnemyInfo[ i ].aKeyInfo[ k ] = g_enemyInit.aKeyInfo[ k ];
				}
			}

			// �G�̎�ނƈړ��p�^�[���̐ݒ�
			switch ( i % 4 )
			{
			case 0:
				enemyStatus[ i ].movePattern = MOVEPATTERN_A;
				break;

			case 1:
				enemyStatus[ i ].movePattern = MOVEPATTERN_B;
				break;

			case 2:
				enemyStatus[ i ].movePattern = MOVEPATTERN_C;
				break;

			case 3:
				enemyStatus[ i ].movePattern = MOVEPATTERN_D;
				break;
			}

			// �����蔻����i�[
			rectEnemy[ i ].pos = g_aEnemyInfo[ i ].posParts[ 0 ];
		
			// �e�̐ݒ�i�C���f�b�N�X�w��j
			g_aEnemyInfo[ i ].nShadowNum = SetShadowEx( g_aEnemyInfo[ i ].posParts[ 0 ], D3DXVECTOR3(0.0f, 0.0f, 0.0f), g_aEnemyInfo[ i ].scl[ 0 ], 30.0f, 30.0f, 1.0f );

			// ���[�V����
			g_aEnemyInfo[ i ].status = NEUTRAL;

			// �g�pON
			enemyStatus[ i ].bUse = true;

			// �G�̐��𑝂₷
			g_nEnemyCount++;

			break;
		}
	}
}

//===============================================
//
// ���f���\���̕ϐ��擾
//
//===============================================
MODEL_INFO *GetEnemyInfo(void)
{
	return &g_aEnemyInfo[ 0 ];
}

//===============================================
//
// �����蔻��̏����擾
//
//===============================================
COL_RECT *GetRectEnemy()
{
	return &rectEnemy[ 0 ];
}

//===============================================
//
// �G�̎g�p�󋵂��擾
//
//===============================================
bool *GetEnemyUse( void )
{
	return &enemyStatus[ 0 ].bUse;
}

//===============================================
//
// ��������������擾
//
//===============================================
bool GetCollision( void )
{
	return g_bCol;
}

//===============================================
//
// ��������������Z�b�g
//
//===============================================
void SetCollision( bool set )
{
	g_bCol = set;
}

//===============================================
//
// �^�C�g���ł̍X�V����
//
//===============================================
void UpdateEnemyTitle( void )
{
	CAMERA *pCamera = GetCamera();				// �J�����̏��擾

	for ( int i = 0; i < ENEMY_MAX; i++ )
	{
		if ( enemyStatus[ i ].bUse == true )
		{
			g_aEnemyInfo[ i ].statusOld = g_aEnemyInfo[ i ].status;

			// ���f���̖ڕW�p�x��ݒ�
			g_aEnemyInfo[ i ].targetRotParts.y = -(D3DX_PI / 2) - pCamera ->rotCamera.y;

			g_aEnemyInfo[ i ].status = WALK;

			g_aEnemyInfo[ i ].posParts[ 0 ].x += 3.0f;
			if ( g_aEnemyInfo[ i ].posParts[ 0 ].x >= SCREEN_WIDTH + 100.0f )
			{
				g_aEnemyInfo[ i ].posParts[ 0 ].x = -400.0f;
			}

			// ���[�V�����̍X�V����
			UpdateModelMotion( &g_aEnemyInfo[ i ] );
		}
	}

	RotateEnemy();
}
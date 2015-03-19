//===============================================
//
//  内容   ： モデルに関する処理
//  File   ： enemy.cpp
//  制作者 ： 木崎　翔
//
//===============================================

//===============================================
// 
//  インクルードファイル
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
//　マクロ定義
//
//===============================================
#define MODEL_TEXTURE_MAX 	( 2 )			// 使用するテクスチャの数
#define MODEL_MAX			( 3 )			// モデルの最大数
#define MOVE_VAL			( 0.3f )		// 移動量
#define MOVE_VAL_SLOW		( 0.05f )		// スロウ状態の移動量
#define BRAKE_VAL			( 0.07f )		// 慣性の係数
#define ROT_BRAKE_VAL		( 0.1f )		// モデルの回転ブレーキ
#define GRAVITY_VAL			( 0.5f )		// 重力量
#define CURE_SLOW_FRAME		( 180 )			// スロウ状態からの復帰フレーム

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
//  グローバル変数
//-----------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureEnemy[MODEL_TEXTURE_MAX];		// テクスチャへのポインタ

// 敵に関する変数
MODEL_INFO g_enemyInit;
MODEL_INFO g_aEnemyInfo[ ENEMY_MAX ];	// 敵の情報
COL_RECT rectEnemy[ ENEMY_MAX ];		// 当たり判定
ENEMY_STATUS enemyStatus[ ENEMY_MAX ];	// 敵のステータス
bool g_bCol = false;
static bool g_bEnemyAppear = false;
static int g_nEnemyCount = 0;

static D3DXMATRIX g_mtxProjection;		// プロジェクションマトリックス
static D3DXMATRIX g_mtxView;			// ビューマトリックス
static D3DXMATRIX g_mtxWorld;			// ワールドマトリックス

D3DXVECTOR3 g_posInit;			// 位置
D3DXVECTOR3 g_rotInit;			// 向き
D3DXVECTOR3 g_sclInit;			// 大きさ

//===============================================
//
// モデルに関する初期化
//
//===============================================
void InitEnemy(void)
{
	// ローカル変数
	//------------------------------------
	LPDIRECT3DDEVICE9 pDevice = GetDevice();						// デバイスオブジェクト

	// グローバル変数の初期化
	//------------------------------------
	g_posInit = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	g_rotInit = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	g_sclInit = D3DXVECTOR3( 1.0f, 1.0f, 1.0f );
	g_nEnemyCount = 0;
	g_bEnemyAppear = false;
	g_bCol = false;

	for ( int i = 0; i < ENEMY_MAX; i++ )
	{
		// 使用判定
		enemyStatus[ i ].bUse = false;
	}

	// モーションの初期化
	InitModelMotionEx( &g_enemyInit, "data/SCRIPT/motionEnemy.txt" );

	// 敵の生成
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
// モデルに関する終了処理
//
//===============================================
void UninitEnemy(void)
{
	// ローカル変数
	//------------------------------------

	for ( int i = 0; i < ENEMY_MAX; i++ )
	{
		// 終了
		UninitModelMotion( &g_aEnemyInfo[ i ] );
	}

	for (int nCnt = 0; nCnt < MODEL_TEXTURE_MAX; nCnt++)
	{
		if (g_pTextureEnemy[nCnt] != NULL)
		{ 
			// 使ったテクスチャの解放と初期化
			//------------------------------------
			g_pTextureEnemy[nCnt] -> Release();
			g_pTextureEnemy[nCnt] = NULL;
		}
	}
	
	// 構造体の初期化
	for ( int i = 0; i < ENEMY_MAX; i++ )
	{
		memset( &g_aEnemyInfo[ i ], 0, sizeof( MODEL_INFO ) );
	}
}	

//===============================================
//
// モデルに関する更新処理
//
//===============================================
void UpdateEnemy(void)
{
	// ローカル変数
	//------------------------------------
	CAMERA *pCamera = GetCamera();				// カメラの情報取得
	COL_RECT *pRectPlayer = GetRectPlayer();			// 
	MESHFIELD *pMeshField = GetMeshField();		// メッシュフィールドの情報を取得
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

	// 敵の再出現処理
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
			// 前回の位置を保存
			g_aEnemyInfo[ i ].posOld = g_aEnemyInfo[ i ].posParts[ 0 ];
			g_aEnemyInfo[ i ].statusOld = g_aEnemyInfo[ i ].status;

			// 重力をかける
			//------------------------------------
			g_aEnemyInfo[ i ].posParts[ 0 ].y += g_aEnemyInfo[ i ].fGravity;
			g_aEnemyInfo[ i ].fGravity -= GRAVITY_VAL;

			// めり込まない処理
			//------------------------------------
			if ( g_aEnemyInfo[ i ].posParts[ 0 ].y - 5.0f <= 0.0f )
			{
				g_aEnemyInfo[ i ].posParts[ 0 ].y = g_aEnemyInfo[ i ].posOld.y;		// 前回のY座標に戻す
				rectEnemy[ i ].pos = g_aEnemyInfo[ i ].posParts[ 0 ];					// 当たり判定用変数の更新
				g_aEnemyInfo[ i ].fGravity = 0.0f;									// ジャンプ量を初期化
			}

			// バレットが当たった状態かどうか
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

					// 右移動
					if ( g_aEnemyInfo[ i ].nMoveCnt >= 180 && g_aEnemyInfo[ i ].nMoveCnt < 360 )
					{
						// モデルの目標角度を設定
						g_aEnemyInfo[ i ].targetRotParts.y = -( D3DX_PI / 2 ) - pCamera ->rotCamera.y;

						// 移動量を設定
						g_aEnemyInfo[ i ].move.x += cosf( pCamera ->rotCamera.y ) * enemyStatus[ i ].move.x;
						g_aEnemyInfo[ i ].move.z += sinf( pCamera ->rotCamera.y ) * enemyStatus[ i ].move.z;

						g_aEnemyInfo[ i ].status = WALK;
					}
					// 左移動
					else if ( g_aEnemyInfo[ i ].nMoveCnt >= 360 && g_aEnemyInfo[ i ].nMoveCnt < 800 )
					{
						// モデルの目標角度を設定
						g_aEnemyInfo[ i ].targetRotParts.y = D3DX_PI / 2 - pCamera ->rotCamera.y;

						// 移動量を設定
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

					// 上移動
					if ( g_aEnemyInfo[ i ].nMoveCnt >= 180 && g_aEnemyInfo[ i ].nMoveCnt < 420 )
					{
						// モデルの目標角度を設定
						g_aEnemyInfo[ i ].targetRotParts.y = D3DX_PI - pCamera ->rotCamera.y;

						// 移動量を設定
						g_aEnemyInfo[ i ].move.x -= sinf( pCamera->rotCamera.y ) * enemyStatus[ i ].move.x;
						g_aEnemyInfo[ i ].move.z += cosf( pCamera->rotCamera.y ) * enemyStatus[ i ].move.z;

						g_aEnemyInfo[ i ].status = WALK;
					}
					// 下移動
					else if ( g_aEnemyInfo[ i ].nMoveCnt >= 420 && g_aEnemyInfo[ i ].nMoveCnt < 650 )
					{
						// モデルの目標角度を設定
						g_aEnemyInfo[ i ].targetRotParts.y = 0.0f - pCamera ->rotCamera.y;

						// 移動量を設定
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

					// 右移動
					if ( g_aEnemyInfo[ i ].nMoveCnt >= 100 && g_aEnemyInfo[ i ].nMoveCnt < 360 )
					{
						// モデルの目標角度を設定
						g_aEnemyInfo[ i ].targetRotParts.y = -( D3DX_PI / 2 ) - pCamera ->rotCamera.y;

						// 移動量を設定
						g_aEnemyInfo[ i ].move.x += cosf( pCamera->rotCamera.y ) * enemyStatus[ i ].move.x;
						g_aEnemyInfo[ i ].move.z += sinf( pCamera->rotCamera.y ) * enemyStatus[ i ].move.z;

						g_aEnemyInfo[ i ].status = WALK;
					}
					// 左移動
					else if ( g_aEnemyInfo[ i ].nMoveCnt >= 360 && g_aEnemyInfo[ i ].nMoveCnt < 600 )
					{
						// モデルの目標角度を設定
						g_aEnemyInfo[ i ].targetRotParts.y = D3DX_PI / 2 - pCamera ->rotCamera.y;

						// 移動量を設定
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

					// 上移動
					if ( g_aEnemyInfo[ i ].nMoveCnt >= 40 && g_aEnemyInfo[ i ].nMoveCnt < 360 )
					{
						// モデルの目標角度を設定
						g_aEnemyInfo[ i ].targetRotParts.y = D3DX_PI - pCamera ->rotCamera.y;

						// 移動量を設定
						g_aEnemyInfo[ i ].move.x -= sinf( pCamera->rotCamera.y ) * enemyStatus[ i ].move.x;
						g_aEnemyInfo[ i ].move.z += cosf( pCamera->rotCamera.y ) * enemyStatus[ i ].move.z;

						g_aEnemyInfo[ i ].status = WALK;
					}
					// 下移動
					else if ( g_aEnemyInfo[ i ].nMoveCnt >= 360 && g_aEnemyInfo[ i ].nMoveCnt < 500 )
					{
						// モデルの目標角度を設定
						g_aEnemyInfo[ i ].targetRotParts.y = 0.0f - pCamera ->rotCamera.y;

						// 移動量を設定
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

			// 前回のモーションと違ったら
			if ( g_aEnemyInfo[ i ].statusOld != g_aEnemyInfo[ i ].status )
			{
				g_aEnemyInfo[ i ].nKey = 0;
				g_aEnemyInfo[ i ].nFrameCount = 0;
				if ( g_aEnemyInfo[ i ].statusOld == WALK )
				{
					g_aEnemyInfo[ i ].status = NEUTRAL;
				}
			}

			// 目標の向きが規定範囲を超えていたら丸め込む
			if (g_aEnemyInfo[ i ].targetRotParts.y > D3DX_PI)
			{
				g_aEnemyInfo[ i ].targetRotParts.y = ((g_aEnemyInfo[ i ].targetRotParts.y) - (D3DX_PI * 2));
			}
			if (g_aEnemyInfo[ i ].targetRotParts.y < -D3DX_PI)
			{
				g_aEnemyInfo[ i ].targetRotParts.y = ((g_aEnemyInfo[ i ].targetRotParts.y) + (D3DX_PI * 2));
			}

			// 慣性を徐々に弱める
			g_aEnemyInfo[ i ].move.x += (0.0f - g_aEnemyInfo[ i ].move.x) * 0.07f;
			g_aEnemyInfo[ i ].move.z += (0.0f - g_aEnemyInfo[ i ].move.z) * 0.07f;

			// 加速する値を加算
			g_aEnemyInfo[ i ].posParts[ 0 ].x += g_aEnemyInfo[ i ].move.x;
			g_aEnemyInfo[ i ].posParts[ 0 ].z += g_aEnemyInfo[ i ].move.z;

			// 壁に当たったら
			//------------------------------------
			if (g_aEnemyInfo[ i ].posParts[ 0 ].x - 30.0f < -(pMeshField ->nNumX * pMeshField ->fWidth) / 2 
			||	g_aEnemyInfo[ i ].posParts[ 0 ].x + 30.0f > (pMeshField ->nNumX * pMeshField ->fWidth) / 2)
			{
				// X座標のみもとに戻す
				g_aEnemyInfo[ i ].posParts[ 0 ].x = g_aEnemyInfo[ i ].posOld.x;
				g_aEnemyInfo[ i ].move.x = -g_aEnemyInfo[ i ].move.x;
			}
			if (g_aEnemyInfo[ i ].posParts[ 0 ].z - 30.0f < -(pMeshField ->nNumZ * pMeshField ->fHeight) / 2
			||	g_aEnemyInfo[ i ].posParts[ 0 ].z + 30.0f > (pMeshField ->nNumZ * pMeshField ->fHeight) / 2)
			{
				// Z座標のみもとに戻す
				g_aEnemyInfo[ i ].posParts[ 0 ].z = g_aEnemyInfo[ i ].posOld.z;
				g_aEnemyInfo[ i ].move.z = -g_aEnemyInfo[ i ].move.z;
			}

			// 当たり判定用の情報を更新
			rectEnemy[ i ].pos = g_aEnemyInfo[ i ].posParts[ 0 ];

			// プレイヤーとの当たり判定
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
							AddCatchEnemy( 1 );		// 捕まえた数

							// 敵の総数を減らす
							g_nEnemyCount--;

							// スコア加算
							AddScore( enemyStatus[ i ].nScore );

							// チュートリアルの時
							if ( GetStringMessage() == GO )
							{
								SetEnterFlag( false );
								SetStringMessage( ABSORB );
							}

							// 当たった判定がなかったら
							if ( g_bCol == false )
							{
								pPlayerInfo ->nBillboardNum = SetBillboard( pRectPlayer ->pos, TEXTUREANIM_CHARGE, 10.0f, 80.0f, 80.0f );
							}

							g_bCol= true;		// 当たった判定あり

							enemyStatus[ i ].bHitBullet = false;
							SetUseBillboard( g_aEnemyInfo[ i ].nBillboardNum );

							// 構造体の中身を初期化
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
						// 音
						PlaySound( ABSORB_SE );

						enemyStatus[ i ].bUse = false;
						( pShadow + i + 1 ) ->bUse = false;
						AddCatchEnemy( 1 );		// 捕まえた数

						// 敵の総数を減らす
						g_nEnemyCount--;

						// スコア加算
						AddScore( enemyStatus[ i ].nScore );

						// チュートリアルの時
						if ( GetStringMessage() == GO )
						{
							SetEnterFlag( false );
							SetStringMessage( ABSORB );
						}

						// 当たった判定がなかったら
						if ( g_bCol == false )
						{
							pPlayerInfo ->nBillboardNum = SetBillboard( pRectPlayer ->pos, TEXTUREANIM_CHARGE, 10.0f, 80.0f, 80.0f );
						}

						g_bCol= true;		// 当たった判定あり

						// 構造体の中身を初期化
						memset( &g_aEnemyInfo[ i ], 0, sizeof( MODEL_INFO ) );
					}
				}
			}

			// 音符との当たり判定
			for ( int j = 0; j < BULLET_MAX; j++ )
			{
				if ( ( pBullet + j ) ->bUse == true )
				{
					if ( ColRect( pRectBullet + j, &rectEnemy[ i ] ) == true )
					{
						// 音符の種類と合致していたら
						if ( ( ( int )( ( pBullet + j ) ->type ) == ( int )enemyStatus[ i ].enemyType ) )
						{
							// 音
							PlaySound(ONPCOL_SE);

							// 当たった判定がなかったら
							if ( enemyStatus[ i ].bHitBullet == false )
							{
								// ビルボードのセット
								g_aEnemyInfo[ i ].nBillboardNum = SetBillboard( g_aEnemyInfo[ i ].posParts[ 0 ], TEXTUREANIM_HASTY, rectEnemy[ i ].harfSize.y * 2, 15.0f, 15.0f );
							}

							( pBullet + j ) ->bUse = false;				// バレットの使用をOFF
							UseShadow( ( pBullet + j ) ->nShadowNum );	// バレットの影をOFF
							
							// 当たった判定あり
							enemyStatus[ i ].bHitBullet = true;
						}
					}
				}
			}

			// オブジェクトとの当たり判定
			for ( int j = 0; j < MAX_OBJECT; j++ )
			{
				if ( ColRect( &pRectObject[ j ], &rectEnemy[ i ] ) == true )
				{
					g_aEnemyInfo[ i ].posParts[ 0 ] = g_aEnemyInfo[ i ].posOld;
					rectEnemy[ i ].pos = g_aEnemyInfo[ i ].posParts[ 0 ];
				}
			}

			// 敵とプレイヤーが当たった判定があったら
			if ( g_bCol == true )
			{
				SetPosBillboard( pPlayerInfo ->nBillboardNum, D3DXVECTOR3( pPlayerInfo ->posParts[ 0 ].x, pPlayerInfo ->posParts[ 0 ].y - 10, pPlayerInfo ->posParts[ 0 ].z ) );
			}

			// 当たった状態だったら
			if ( enemyStatus[ i ].bHitBullet == true )
			{
				// 復帰するまでのフレームを加算
				enemyStatus[ i ].nHitFrame++;

				// ビルボードの位置を設定
				SetPosBillboard( g_aEnemyInfo[ i ].nBillboardNum, g_aEnemyInfo[ i ].posParts[ 0 ] );

				// 復帰フレームになったら
				if ( enemyStatus[ i ].nHitFrame == CURE_SLOW_FRAME )
				{
					enemyStatus[ i ].bHitBullet = false;
					enemyStatus[ i ].nHitFrame = 0;
					SetUseBillboard( g_aEnemyInfo[ i ].nBillboardNum );
				}
			}

			// 影の更新（インデックス指定）
			//UpdateShadowEx( g_aEnemyInfo[ i ].nShadowNum, g_aEnemyInfo[ i ].posParts[ 0 ] );

			SetShadow( g_aEnemyInfo[ i ].posParts[ 0 ], rectEnemy[ i ].harfSize.x, rectEnemy[ i ].harfSize.y, 0.7f );

			// モーションの更新処理
			UpdateModelMotion( &g_aEnemyInfo[ i ] );

			// ミニマップセット
			SetMiniMap(D3DXVECTOR3( g_aEnemyInfo[ i ].posParts[ 0 ].x, 0.0f, g_aEnemyInfo[ i ].posParts[ 0 ].z ), MINIMAPTYPE_ENEMY, g_aEnemyInfo[ i ].targetRotParts.y);

		}
	}

	// モデルの向きの慣性処理
	//-----------------------------------
	RotateEnemy();
}

//===============================================
//
// モデルに関する描画処理
//
//===============================================
void DrawEnemy(void)
{
	// ローカル変数
	//------------------------------------
	LPDIRECT3DDEVICE9 pDevice;						// デバイスオブジェクト

	// デバイスの取得
	//------------------------------------
	pDevice = GetDevice();

	// 描画
	//------------------------------------
	for ( int i = 0; i < ENEMY_MAX; i++ )
	{
		DrawModelMotion( &g_aEnemyInfo[ i ] );
	}

	// デバッグ表示
	/*PrintDebugProc("/----- モデル情報[ 0 ] -------------/\n");
	PrintDebugProc("[ ワールド位置 ]\nX : %f  Y : %f  Z : %f\n", g_aEnemyInfo[ i ][ 0 ].mtxWorld._41, g_aEnemyInfo[ i ][ 0 ].mtxWorld._42, g_aEnemyInfo[ i ][ 0 ].mtxWorld._43);
	PrintDebugProc("[ ローカル位置 ]\nX : %f  Y : %f  Z : %f\n", g_aEnemyInfo[ i ].posParts[ 0 ].x, g_aEnemyInfo[ i ].posParts[ 0 ].y, g_aEnemyInfo[ i ].posParts[ 0 ].z);
	PrintDebugProc("[ 現在の向き ]\nY : %f\n", g_aEnemyInfo[ i ].rotParts[ 0 ].y);
	PrintDebugProc("/-----------------------------------/\n\n");

	PrintDebugProc("/----- モデル情報[ 1 ] -------------/\n");
	PrintDebugProc("[ ローカル位置 ]\nX : %f  Y : %f  Z : %f\n", g_aEnemyInfo[1].posModel.x, g_aEnemyInfo[1].posModel.y, g_aEnemyInfo[1].posModel.z);
	PrintDebugProc("[ 現在の向き ]\nY : %f\n", g_aEnemyInfo[1].rotModel.y);
	PrintDebugProc("/-----------------------------------/\n\n");*/
}

//===============================================
//
// モデルの向きの慣性処理
//
//===============================================
void RotateEnemy(void)
{
	for ( int i = 0; i < ENEMY_MAX; i++ )
	{
		// D3DX_PIより大きい値になったら丸め込む
		//----------------------------------------------
		if (g_aEnemyInfo[ i ].targetRotParts.y - g_aEnemyInfo[ i ].rotParts[ 0 ].y > D3DX_PI)
		{
			// 慣性の処理と丸め込み計算
			g_aEnemyInfo[ i ].rotParts[ 0 ].y += ((g_aEnemyInfo[ i ].targetRotParts.y - g_aEnemyInfo[ i ].rotParts[ 0 ].y) - (D3DX_PI * 2)) * ROT_BRAKE_VAL;

			// 規定範囲外だったら丸め込む
			//---------------------------------------------
			if (g_aEnemyInfo[ i ].rotParts[ 0 ].y > D3DX_PI)
			{
				// 丸め込み計算
				g_aEnemyInfo[ i ].rotParts[ 0 ].y = ((g_aEnemyInfo[ i ].rotParts[ 0 ].y) - (D3DX_PI * 2));
			}
			else if (g_aEnemyInfo[ i ].rotParts[ 0 ].y < -D3DX_PI)
			{
				// 丸め込み計算
				g_aEnemyInfo[ i ].rotParts[ 0 ].y = ((g_aEnemyInfo[ i ].rotParts[ 0 ].y) + (D3DX_PI * 2));
			}
		}

		// -D3DX_PIより小さな値になったら
		else if (g_aEnemyInfo[ i ].targetRotParts.y - g_aEnemyInfo[ i ].rotParts[ 0 ].y < -D3DX_PI)
		{
			// 慣性の処理と丸め込み計算
			g_aEnemyInfo[ i ].rotParts[ 0 ].y += ((g_aEnemyInfo[ i ].targetRotParts.y - g_aEnemyInfo[ i ].rotParts[ 0 ].y) + (D3DX_PI * 2)) * ROT_BRAKE_VAL;

			// 規定範囲外だったら丸め込む
			//---------------------------------------------
			if (g_aEnemyInfo[ i ].rotParts[ 0 ].y > D3DX_PI)
			{
				// 丸め込み計算
				g_aEnemyInfo[ i ].rotParts[ 0 ].y = ((g_aEnemyInfo[ i ].rotParts[ 0 ].y) - (D3DX_PI * 2));
			}
			else if (g_aEnemyInfo[ i ].rotParts[ 0 ].y < -D3DX_PI)
			{
				// 丸め込み計算
				g_aEnemyInfo[ i ].rotParts[ 0 ].y = ((g_aEnemyInfo[ i ].rotParts[ 0 ].y) + (D3DX_PI * 2));
			}
		}
		else
		{
			// 通常の慣性の計算
			g_aEnemyInfo[ i ].rotParts[ 0 ].y += (g_aEnemyInfo[ i ].targetRotParts.y - g_aEnemyInfo[ i ].rotParts[ 0 ].y) * ROT_BRAKE_VAL;
		}
	}
}

//===============================================
//
// モデル構造体変数取得
//
//===============================================
void SetEnemy( ENEMYTYPE type, D3DXVECTOR3 pos )
{
	// ローカル変数
	//------------------------------------
	LPDIRECT3DDEVICE9 pDevice = GetDevice();						// デバイスオブジェクト

	for ( int i = 0; i < ENEMY_MAX; i++ )
	{
		if ( enemyStatus[ i ].bUse == false )
		{
			enemyStatus[ i ].enemyType = type;
			switch ( type )
			{
			case ENEMYTYPE_WHITE:
				// xファイルの読み込み
				//------------------------------------
				D3DXLoadMeshFromX("data/MODEL/eBody.x",		// 読み込むファイル名
								  D3DXMESH_SYSTEMMEM,		// 
								  pDevice,					// 
								  NULL,						// 
								  &g_aEnemyInfo[ i ].pBuffMatModel[ 0 ],			// 
								  NULL,						// 
								  &g_aEnemyInfo[ i ].numMatModel[ 0 ],			// 
								  &g_aEnemyInfo[ i ].pMeshModel[ 0 ]);

				// xファイルの読み込み
				//------------------------------------
				D3DXLoadMeshFromX("data/MODEL/eArmL.x",		// 読み込むファイル名
								  D3DXMESH_SYSTEMMEM,		// 
								  pDevice,					// 
								  NULL,						// 
								  &g_aEnemyInfo[ i ].pBuffMatModel[ 1 ],			// 
								  NULL,						// 
								  &g_aEnemyInfo[ i ].numMatModel[ 1 ],			// 
								  &g_aEnemyInfo[ i ].pMeshModel[ 1 ]);

				// xファイルの読み込み
				//------------------------------------
				D3DXLoadMeshFromX("data/MODEL/eArmR.x",		// 読み込むファイル名
								  D3DXMESH_SYSTEMMEM,		// 
								  pDevice,					// 
								  NULL,						// 
								  &g_aEnemyInfo[ i ].pBuffMatModel[ 2 ],			// 
								  NULL,						// 
								  &g_aEnemyInfo[ i ].numMatModel[ 2 ],			// 
								  &g_aEnemyInfo[ i ].pMeshModel[ 2 ]);

				// 大きさの設定
				rectEnemy[ i ].harfSize = D3DXVECTOR3( ENEMY_WIDTH / 2, ENEMY_HEIGHT / 2, ENEMY_DEPS / 2 );
				enemyStatus[ i ].nScore = ENEMY_SCORE;
				break;

			case ENEMYTYPE_RED:
				// xファイルの読み込み
				//------------------------------------
				D3DXLoadMeshFromX("data/MODEL/eBody_red.x",		// 読み込むファイル名
								  D3DXMESH_SYSTEMMEM,		// 
								  pDevice,					// 
								  NULL,						// 
								  &g_aEnemyInfo[ i ].pBuffMatModel[ 0 ],			// 
								  NULL,						// 
								  &g_aEnemyInfo[ i ].numMatModel[ 0 ],			// 
								  &g_aEnemyInfo[ i ].pMeshModel[ 0 ]);

				// xファイルの読み込み
				//------------------------------------
				D3DXLoadMeshFromX("data/MODEL/eArmL_red.x",		// 読み込むファイル名
								  D3DXMESH_SYSTEMMEM,		// 
								  pDevice,					// 
								  NULL,						// 
								  &g_aEnemyInfo[ i ].pBuffMatModel[ 1 ],			// 
								  NULL,						// 
								  &g_aEnemyInfo[ i ].numMatModel[ 1 ],			// 
								  &g_aEnemyInfo[ i ].pMeshModel[ 1 ]);

				// xファイルの読み込み
				//------------------------------------
				D3DXLoadMeshFromX("data/MODEL/eArmR_red.x",		// 読み込むファイル名
								  D3DXMESH_SYSTEMMEM,		// 
								  pDevice,					// 
								  NULL,						// 
								  &g_aEnemyInfo[ i ].pBuffMatModel[ 2 ],			// 
								  NULL,						// 
								  &g_aEnemyInfo[ i ].numMatModel[ 2 ],			// 
								  &g_aEnemyInfo[ i ].pMeshModel[ 2 ]);

				// 大きさの設定
				rectEnemy[ i ].harfSize = D3DXVECTOR3( ENEMY_WIDTH / 2, ENEMY_HEIGHT / 2, ENEMY_DEPS / 2 );
				enemyStatus[ i ].nScore = ENEMY_SCORE;				
				break;

			case ENEMYTYPE_BLUE:
				// xファイルの読み込み
				//------------------------------------
				D3DXLoadMeshFromX("data/MODEL/eBody_blue.x",		// 読み込むファイル名
								  D3DXMESH_SYSTEMMEM,		// 
								  pDevice,					// 
								  NULL,						// 
								  &g_aEnemyInfo[ i ].pBuffMatModel[ 0 ],			// 
								  NULL,						// 
								  &g_aEnemyInfo[ i ].numMatModel[ 0 ],			// 
								  &g_aEnemyInfo[ i ].pMeshModel[ 0 ]);

				// xファイルの読み込み
				//------------------------------------
				D3DXLoadMeshFromX("data/MODEL/eArmL_blue.x",		// 読み込むファイル名
								  D3DXMESH_SYSTEMMEM,		// 
								  pDevice,					// 
								  NULL,						// 
								  &g_aEnemyInfo[ i ].pBuffMatModel[ 1 ],			// 
								  NULL,						// 
								  &g_aEnemyInfo[ i ].numMatModel[ 1 ],			// 
								  &g_aEnemyInfo[ i ].pMeshModel[ 1 ]);

				// xファイルの読み込み
				//------------------------------------
				D3DXLoadMeshFromX("data/MODEL/eArmR_blue.x",		// 読み込むファイル名
								  D3DXMESH_SYSTEMMEM,		// 
								  pDevice,					// 
								  NULL,						// 
								  &g_aEnemyInfo[ i ].pBuffMatModel[ 2 ],			// 
								  NULL,						// 
								  &g_aEnemyInfo[ i ].numMatModel[ 2 ],			// 
								  &g_aEnemyInfo[ i ].pMeshModel[ 2 ]);

				// 大きさの設定
				rectEnemy[ i ].harfSize = D3DXVECTOR3( BIGENEMY_WIDTH / 2, BIGENEMY_HEIGHT / 2, BIGENEMY_DEPS / 2 );
				enemyStatus[ i ].nScore = BIGENEMY_SCORE;				
				break;

			case ENEMYTYPE_YELLOW:
				// xファイルの読み込み
				//------------------------------------
				D3DXLoadMeshFromX("data/MODEL/eBody_yellow.x",		// 読み込むファイル名
								  D3DXMESH_SYSTEMMEM,		// 
								  pDevice,					// 
								  NULL,						// 
								  &g_aEnemyInfo[ i ].pBuffMatModel[ 0 ],			// 
								  NULL,						// 
								  &g_aEnemyInfo[ i ].numMatModel[ 0 ],			// 
								  &g_aEnemyInfo[ i ].pMeshModel[ 0 ]);

				// xファイルの読み込み
				//------------------------------------
				D3DXLoadMeshFromX("data/MODEL/eArmL_yellow.x",		// 読み込むファイル名
								  D3DXMESH_SYSTEMMEM,		// 
								  pDevice,					// 
								  NULL,						// 
								  &g_aEnemyInfo[ i ].pBuffMatModel[ 1 ],			// 
								  NULL,						// 
								  &g_aEnemyInfo[ i ].numMatModel[ 1 ],			// 
								  &g_aEnemyInfo[ i ].pMeshModel[ 1 ]);

				// xファイルの読み込み
				//------------------------------------
				D3DXLoadMeshFromX("data/MODEL/eArmR_yellow.x",		// 読み込むファイル名
								  D3DXMESH_SYSTEMMEM,		// 
								  pDevice,					// 
								  NULL,						// 
								  &g_aEnemyInfo[ i ].pBuffMatModel[ 2 ],			// 
								  NULL,						// 
								  &g_aEnemyInfo[ i ].numMatModel[ 2 ],			// 
								  &g_aEnemyInfo[ i ].pMeshModel[ 2 ]);

				// 大きさの設定
				rectEnemy[ i ].harfSize = D3DXVECTOR3( SMALLENEMY_WIDTH / 2, SMALLENEMY_HEIGHT / 2, SMALLENEMY_DEPS / 2 );
				enemyStatus[ i ].nScore = SMALLENEMY_SCORE;
				break;
			}

			// 初期位置
			g_aEnemyInfo[ i ].posParts[ 0 ] = pos;
			g_aEnemyInfo[ i ].nNumParts = g_enemyInit.nNumParts;
			g_aEnemyInfo[ i ].targetRotParts = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
			g_aEnemyInfo[ i ].nKey = 0;
			g_aEnemyInfo[ i ].nFrameCount = 0;
			g_aEnemyInfo[ i ].nMoveCnt = 0;

			// 情報をコピー
			for ( int j = 0; j < g_aEnemyInfo[ i ].nNumParts; j++ )
			{
				g_aEnemyInfo[ i ].rotParts[ j ] = g_aEnemyInfo[ i ].rotParts[ 0 ];
				g_aEnemyInfo[ i ].scl[ j ] = D3DXVECTOR3( 1.0f, 1.0f, 1.0f );
				g_aEnemyInfo[ i ].nIndex[ j ] = g_enemyInit.nIndex[ j ];
				g_aEnemyInfo[ i ].nParent[ j ] = g_enemyInit.nParent[ j ];
				g_aEnemyInfo[ i ].rotDevide[ j ] = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
				g_aEnemyInfo[ i ].posDevide[ j ] = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );

				// モーション情報のコピー
				for ( int k = 0; k < 5; k++ )
				{
					g_aEnemyInfo[ i ].aMotion[ k ] = g_enemyInit.aMotion[ k ];
					g_aEnemyInfo[ i ].aKeyInfo[ k ] = g_enemyInit.aKeyInfo[ k ];
				}
			}

			// 敵の種類と移動パターンの設定
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

			// 当たり判定を格納
			rectEnemy[ i ].pos = g_aEnemyInfo[ i ].posParts[ 0 ];
		
			// 影の設定（インデックス指定）
			g_aEnemyInfo[ i ].nShadowNum = SetShadowEx( g_aEnemyInfo[ i ].posParts[ 0 ], D3DXVECTOR3(0.0f, 0.0f, 0.0f), g_aEnemyInfo[ i ].scl[ 0 ], 30.0f, 30.0f, 1.0f );

			// モーション
			g_aEnemyInfo[ i ].status = NEUTRAL;

			// 使用ON
			enemyStatus[ i ].bUse = true;

			// 敵の数を増やす
			g_nEnemyCount++;

			break;
		}
	}
}

//===============================================
//
// モデル構造体変数取得
//
//===============================================
MODEL_INFO *GetEnemyInfo(void)
{
	return &g_aEnemyInfo[ 0 ];
}

//===============================================
//
// 当たり判定の情報を取得
//
//===============================================
COL_RECT *GetRectEnemy()
{
	return &rectEnemy[ 0 ];
}

//===============================================
//
// 敵の使用状況を取得
//
//===============================================
bool *GetEnemyUse( void )
{
	return &enemyStatus[ 0 ].bUse;
}

//===============================================
//
// 当たった判定を取得
//
//===============================================
bool GetCollision( void )
{
	return g_bCol;
}

//===============================================
//
// 当たった判定をセット
//
//===============================================
void SetCollision( bool set )
{
	g_bCol = set;
}

//===============================================
//
// タイトルでの更新処理
//
//===============================================
void UpdateEnemyTitle( void )
{
	CAMERA *pCamera = GetCamera();				// カメラの情報取得

	for ( int i = 0; i < ENEMY_MAX; i++ )
	{
		if ( enemyStatus[ i ].bUse == true )
		{
			g_aEnemyInfo[ i ].statusOld = g_aEnemyInfo[ i ].status;

			// モデルの目標角度を設定
			g_aEnemyInfo[ i ].targetRotParts.y = -(D3DX_PI / 2) - pCamera ->rotCamera.y;

			g_aEnemyInfo[ i ].status = WALK;

			g_aEnemyInfo[ i ].posParts[ 0 ].x += 3.0f;
			if ( g_aEnemyInfo[ i ].posParts[ 0 ].x >= SCREEN_WIDTH + 100.0f )
			{
				g_aEnemyInfo[ i ].posParts[ 0 ].x = -400.0f;
			}

			// モーションの更新処理
			UpdateModelMotion( &g_aEnemyInfo[ i ] );
		}
	}

	RotateEnemy();
}
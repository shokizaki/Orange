//===============================================
//
//  内容   ： モデルに関する処理
//  File   ： model.cpp
//  制作者 ： 木崎　翔
//
//===============================================

//===============================================
// 
//  インクルードファイル
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
//　マクロ定義
//
//===============================================
#define MODEL_TEXTURE_MAX 	( 2 )			// 使用するテクスチャの数
#define MODEL_MAX			( 7 )			// モデルの最大数
#define MOVE_VAL			( 7.0f )		// 
#define GRAVITY_VAL			( 0.50f )		// 
#define BRAKE_VAL			( 2 )		// 慣性の係数
#define ROT_BRAKE_VAL		( 0.1f )		// 
#define BULLET_SPEED		( 1.5f )		// 
#define SLOW_VAL			( ( 1.0f - ( float )g_nNumEnemyCatch / 30 ) )	// 遅くする量

#define PLAYER_WIDTH		( 50.0f )		// 
#define PLAYER_HEIGHT_Z		( 50.0f )		// 
#define PLAYER_HEIGHT_Y		( 100.0f )		// 

#define TEX_PASS		"data/TEXTURE/tairu03.jpg"

//-----------------------------------------------
//  グローバル変数
//-----------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureModel[MODEL_TEXTURE_MAX];		// テクスチャへのポインタ

// モデルに関する変数
MODEL_INFO g_playerInfo;				// モデル変数
COL_RECT rectPlayer;					// 当たり判定の情報
BULLETTYPE bulletType;					// バレットのタイプ
PLAYER_STATUS playerStatus;				// プレイヤーの状態
int g_nNumEnemyCatch = 0;					// 敵を捕まえた数

static D3DXMATRIX g_mtxProjection;		// プロジェクションマトリックス
static D3DXMATRIX g_mtxView;			// ビューマトリックス

D3DXVECTOR3 g_posModel;			// 位置
D3DXVECTOR3 g_rotModel;			// 向き
D3DXVECTOR3 g_sclModel;			// 大きさ

D3DXMATRIX g_mtxWorld;			// ワールドマトリックス

//------ 内部関数宣言 ---------------------------

//-----------------------------------------------
// プレイヤー移動処理（パッド）
//-----------------------------------------------
void UpdatePlayerMovePad( void );

//-----------------------------------------------
// プレイヤー移動処理（キーボード）
//-----------------------------------------------
void UpdatePlayerMoveKeyboard( void );

//-----------------------------------------------

//===============================================
//
// モデルに関する初期化
//
//===============================================
void InitModel(void)
{
	// ローカル変数
	//------------------------------------
	LPDIRECT3DDEVICE9 pDevice;						// デバイスオブジェクト
	char strDest[256] = "data/TEXTURE/";

	// グローバル変数の初期化
	g_nNumEnemyCatch = 0;
	g_posModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_rotModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_sclModel = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	g_playerInfo.targetRotParts = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	playerStatus.bHitEnemy = false;
	playerStatus.nFrameCount = 0;

	// デバイスの取得
	//------------------------------------
	pDevice = GetDevice();

	g_playerInfo.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// バレットタイプの初期化
	bulletType = BULLETTYPE_ONP_RED;

	// モーションの初期化
	InitModelMotionEx( &g_playerInfo, "data/SCRIPT/motion.txt" );

	for (int nCnt = 0; nCnt < MODEL_MAX; nCnt++)
	{
		// モデルに関する初期化
		//------------------------------------
		g_playerInfo.scl[ nCnt ] = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_playerInfo.rotParts[ nCnt ] = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	}

	for (int nCnt = 0; nCnt < g_playerInfo.nNumParts; nCnt++)
	{
		// xファイルの読み込み
		//------------------------------------
		D3DXLoadMeshFromX(&g_playerInfo.modelFileName[ nCnt ][ 0 ],		// 読み込むファイル名
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

	// テクスチャへのポインタを初期化
	//------------------------------------
	for (int nCnt = 0; nCnt < MODEL_TEXTURE_MAX; nCnt++)				
	{
		g_pTextureModel[nCnt] = NULL;
	}

	// 当たり判定情報の格納
	rectPlayer.pos = g_playerInfo.posParts[ 0 ];
	rectPlayer.harfSize = D3DXVECTOR3( PLAYER_WIDTH / 2, 0.0f, PLAYER_HEIGHT_Z / 2 );

	// 影のセット(インデックスを受け取る)
	g_playerInfo.nShadowNum = SetShadowEx(g_playerInfo.posParts[ 0 ], D3DXVECTOR3(0.0f, 0.0f, 0.0f), g_playerInfo.scl[ 0 ], 30.0f, 30.0f, 0.7f);
}

//===============================================
//
// モデルに関する終了処理
//
//===============================================
void UninitModel(void)
{
	// ローカル変数
	//------------------------------------
	int nCnt;

	// 終了処理
	UninitModelMotion( &g_playerInfo );

	// 使ったテクスチャの解放と初期化
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
// モデルに関する更新処理
//
//===============================================
void UpdateModel(void)
{
	// ローカル変数
	//------------------------------------
	CAMERA *pCamera = GetCamera();				// カメラの情報取得
	MESHFIELD *pMeshField = GetMeshField();		// メッシュフィールドの情報を取得
	COL_RECT *pRectObject = GetRectObject();	// オブジェクトの当たり判定情報
	OBJECT *pObject = GetObject();				// オブジェクト情報取得

	// 前回の位置を保存
	//------------------------------------
	g_playerInfo.posOld = g_playerInfo.posParts[ 0 ];
	g_playerInfo.statusOld = g_playerInfo.status;

	// ジャンプ処理
	//------------------------------------
	if ( GetKeyboardTrigger( DIK_J ) == true )
	{
		g_playerInfo.fGravity = g_playerInfo.fJumpVal;
		g_playerInfo.status = JUMP;
	}

	// 重力をかける
	//------------------------------------
	g_playerInfo.posParts[ 0 ].y += g_playerInfo.fGravity;
	g_playerInfo.fGravity -= GRAVITY_VAL;

	// めり込まない処理
	//------------------------------------
	if ( g_playerInfo.posParts[ 0 ].y - 20.0f <= 0.0f )
	{
		g_playerInfo.posParts[ 0 ].y = g_playerInfo.posOld.y;		// 前回のY座標に戻す
		rectPlayer.pos = g_playerInfo.posParts[ 0 ];					// 当たり判定用変数の更新
		g_playerInfo.fGravity = 0.0f;							// ジャンプ量を初期化
	}

	// 移動処理
	//----------------------------------------------
	if ( ( ( GetKeyboardPress(DIK_W) || GetKeyboardPress(DIK_S) || GetKeyboardPress(DIK_D) || GetKeyboardPress(DIK_A) ) || GetGamePadCrossKeyX() != 0 || GetGamePadCrossKeyY() != 0 )
	&&	 ( ( GetMode() == MODE_TUTORIAL && GetEnterFlag() == true ) || GetMode() == MODE_GAME ) )
	{
		// 上移動
		if ( GetKeyboardPress(DIK_W) || GetGamePadCrossKeyY() == -1 )
		{
			// 左斜め上移動
			if (GetKeyboardPress(DIK_A) || GetGamePadCrossKeyX() == -1 )
			{
				// モデルの目標角度を設定
				g_playerInfo.targetRotParts.y = (D3DX_PI - (D3DX_PI / 4)) - pCamera ->rotCamera.y;

				// 移動量を設定
				g_playerInfo.move.x -= sinf(pCamera->rotCamera.y + D3DX_PI / 4) * MOVE_VAL * SLOW_VAL;
				g_playerInfo.move.z += cosf(pCamera->rotCamera.y + D3DX_PI / 4) * MOVE_VAL * SLOW_VAL;

				g_playerInfo.directionSecond = DIRECTION_LEFT;
			}

			// 右斜め上移動
			else if (GetKeyboardPress(DIK_D) || GetGamePadCrossKeyX() == 1 )
			{
				// モデルの目標角度を設定
				g_playerInfo.targetRotParts.y = (D3DX_PI + (D3DX_PI / 4)) - pCamera ->rotCamera.y;

				// 移動量を設定
				g_playerInfo.move.x -= sinf(pCamera->rotCamera.y - D3DX_PI / 4) * MOVE_VAL * SLOW_VAL;
				g_playerInfo.move.z += cosf(pCamera->rotCamera.y - D3DX_PI / 4) * MOVE_VAL * SLOW_VAL;

				g_playerInfo.directionSecond = DIRECTION_RIGHT;
			}

			else
			{
				// モデルの目標角度を設定
				g_playerInfo.targetRotParts.y = D3DX_PI - pCamera ->rotCamera.y;

				// 移動量を設定
				g_playerInfo.move.x -= sinf( pCamera->rotCamera.y ) * MOVE_VAL * SLOW_VAL;
				g_playerInfo.move.z += cosf( pCamera->rotCamera.y ) * MOVE_VAL * SLOW_VAL;
			}

			if ( g_playerInfo.bMove == false )
			{
				g_playerInfo.direction = DIRECTION_DEPTH;
			}

			g_playerInfo.bMove = true;	// 移動判定あり
		}

		// 下移動
		else if (GetKeyboardPress(DIK_S) || GetGamePadCrossKeyY() == 1 )
		{
			// 左斜め下移動
			if (GetKeyboardPress(DIK_A) || GetGamePadCrossKeyX() == -1 )
			{
				// モデルの目標角度を設定
				g_playerInfo.targetRotParts.y = (D3DX_PI / 4) - pCamera ->rotCamera.y;

				// 移動量を設定
				g_playerInfo.move.x += sinf(pCamera->rotCamera.y - D3DX_PI / 4) * MOVE_VAL * SLOW_VAL;
				g_playerInfo.move.z -= cosf(pCamera->rotCamera.y - D3DX_PI / 4) * MOVE_VAL * SLOW_VAL;
			
				g_playerInfo.directionSecond = DIRECTION_LEFT;
			}

			// 右斜め下移動
			else if (GetKeyboardPress(DIK_D) || GetGamePadCrossKeyX() == 1 )
			{
				// モデルの目標角度を設定
				g_playerInfo.targetRotParts.y = -(D3DX_PI / 4) - pCamera ->rotCamera.y;

				// 移動量を設定
				g_playerInfo.move.x += sinf(pCamera->rotCamera.y + D3DX_PI / 4) * MOVE_VAL * SLOW_VAL;
				g_playerInfo.move.z -= cosf(pCamera->rotCamera.y + D3DX_PI / 4) * MOVE_VAL * SLOW_VAL;
			
				g_playerInfo.directionSecond = DIRECTION_RIGHT;
			}

			else
			{
				// モデルの目標角度を設定
				g_playerInfo.targetRotParts.y = 0.0f - pCamera ->rotCamera.y;

				// 移動量を設定
				g_playerInfo.move.x += sinf(pCamera->rotCamera.y) * MOVE_VAL * SLOW_VAL;
				g_playerInfo.move.z -= cosf(pCamera->rotCamera.y) * MOVE_VAL * SLOW_VAL;
			}

			if ( g_playerInfo.bMove == false )
			{
				g_playerInfo.direction = DIRECTION_FRONT;
			}
			
			g_playerInfo.bMove = true;	// 移動判定あり
		}

		// 左移動
		else if (GetKeyboardPress(DIK_A) || GetGamePadCrossKeyX() == -1 )
		{
			// モデルの目標角度を設定
			g_playerInfo.targetRotParts.y = D3DX_PI / 2 - pCamera ->rotCamera.y;

			// 移動量を設定
			g_playerInfo.move.x -= cosf(pCamera->rotCamera.y) * MOVE_VAL * SLOW_VAL;
			g_playerInfo.move.z -= sinf(pCamera->rotCamera.y) * MOVE_VAL * SLOW_VAL;

			if ( g_playerInfo.bMove == false )
			{
				g_playerInfo.direction = DIRECTION_LEFT;
			}

			g_playerInfo.bMove = true;	// 移動判定あり
		}

		// 右移動
		else if (GetKeyboardPress(DIK_D) || GetGamePadCrossKeyX() == 1 )
		{
			// モデルの目標角度を設定
			g_playerInfo.targetRotParts.y = -(D3DX_PI / 2) - pCamera ->rotCamera.y;

			// 移動量を設定
			g_playerInfo.move.x += cosf(pCamera ->rotCamera.y) * MOVE_VAL * SLOW_VAL;
			g_playerInfo.move.z += sinf(pCamera ->rotCamera.y) * MOVE_VAL * SLOW_VAL;

			if ( g_playerInfo.bMove == false )
			{
				g_playerInfo.direction = DIRECTION_RIGHT;
			}

			g_playerInfo.bMove = true;	// 移動判定あり
		}

		g_playerInfo.status = WALK;
		
		// 当たり判定用の情報を更新
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

	// バレットのタイプを変更
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

	// バレットを撃つ処理
	//------------------------------------
	if ( ( GetKeyboardTrigger( DIK_SPACE ) == true /*|| GetGamePadTrigger( GAMEPAD_Y )*/ || GetPadElecomTrigger( PAD_1 ) )
	&& ( ( GetMode() == MODE_TUTORIAL && GetEnterFlag() == true ) || GetMode() == MODE_GAME ) )
	{
		// ３方向に発射
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

	// 敵との当たり判定があったら
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

	// 目標の向きが規定範囲を超えていたら丸め込む
	//--------------------------------------------
	if (g_playerInfo.targetRot[ 0 ].y > D3DX_PI)
	{
		g_playerInfo.targetRot[ 0 ].y = ( ( g_playerInfo.targetRot[ 0 ].y ) - ( D3DX_PI * 2 ) );
	}
	if (g_playerInfo.targetRot[ 0 ].y < -D3DX_PI)
	{
		g_playerInfo.targetRot[ 0 ].y = ( ( g_playerInfo.targetRot[ 0 ].y ) + ( D3DX_PI * 2 ) );
	}

	// 慣性を徐々に弱める
	g_playerInfo.move.x += (0.0f - g_playerInfo.move.x) / BRAKE_VAL;
	g_playerInfo.move.z += (0.0f - g_playerInfo.move.z) / BRAKE_VAL;

	// 加速する値を加算
	g_playerInfo.posParts[ 0 ].x += g_playerInfo.move.x;
	g_playerInfo.posParts[ 0 ].z += g_playerInfo.move.z;

	// 壁に当たったら
	//------------------------------------
	if (g_playerInfo.posParts[ 0 ].x - 30.0f < -(pMeshField ->nNumX * pMeshField ->fWidth) / 2 
	||	g_playerInfo.posParts[ 0 ].x + 30.0f > (pMeshField ->nNumX * pMeshField ->fWidth) / 2)
	{
		// X座標のみもとに戻す
		g_playerInfo.posParts[ 0 ].x = g_playerInfo.posOld.x;
	}
	if (g_playerInfo.posParts[ 0 ].z - 30.0f < -(pMeshField ->nNumZ * pMeshField ->fHeight) / 2
	||	g_playerInfo.posParts[ 0 ].z + 30.0f > (pMeshField ->nNumZ * pMeshField ->fHeight) / 2)
	{
		// Z座標のみもとに戻す
		g_playerInfo.posParts[ 0 ].z = g_playerInfo.posOld.z;
	}

	// 当たり判定用の情報を更新
	rectPlayer.pos = g_playerInfo.posParts[ 0 ];

	// アクション
	//-----------------------------------
	/*if ( GetKeyboardTrigger( DIK_K ) )
	{
		g_playerInfo.status = ACTION;
	}*/

	if ( playerStatus.bHitEnemy == false)
	{
		playerStatus.nFrameCount = 0;
	}

	// モデルの向きの慣性処理
	//-----------------------------------
	RotateModel();

	// モーションの更新処理
	//-----------------------------------
	UpdateModelMotion( &g_playerInfo );

// デバッグ表示
PrintDebugProc("/-------------------------\n");
PrintDebugProc("MOTION : NEUTRAL\n");
PrintDebugProc("LOOP   : ON\n");
PrintDebugProc("KEY    : %d / %d\n", g_playerInfo.nKey, g_playerInfo.aMotion[ g_playerInfo.status ].nNumKey);
PrintDebugProc("FRAME  : %d / %d\n", g_playerInfo.nFrameCount, g_playerInfo.aMotion[ g_playerInfo.status ].nFrame[ g_playerInfo.nKey ]);
PrintDebugProc("-------------------------/\n\n");


	// 影の更新（インデックス指定）
	//-----------------------------------
	//UpdateShadowEx( g_playerInfo.nShadowNum, g_playerInfo.posParts[ 0 ] );
	SetShadow( D3DXVECTOR3( g_playerInfo.posParts[ 0 ].x, 0.1f, g_playerInfo.posParts[ 0 ].z ), rectPlayer.harfSize.x, rectPlayer.harfSize.z, 0.7f );

	// ミニマップセット
	SetMiniMap(D3DXVECTOR3( g_playerInfo.posParts[ 0 ].x, 0.0f, g_playerInfo.posParts[ 0 ].z ), MINIMAPTYPE_PLAYER, g_playerInfo.rotParts[ 0 ].y );

	// 当たり判定用の情報を更新
	//-----------------------------------
	rectPlayer.pos = g_playerInfo.posParts[ 0 ];
}

//===============================================
//
// プレイヤーの移動処理処理
//
//===============================================
void UpdatePlayerMovePad( void )
{
	// ローカル変数
	//------------------------------------
	CAMERA *pCamera = GetCamera();				// カメラの情報取得
	MODEL_INFO *pEnemyInfo = GetEnemyInfo();
	COL_RECT *pRectEnemy = GetRectEnemy();
	bool *pEnemyUse = GetEnemyUse();

	// 自動で動く処理をON（攻撃ボタン）
	if ( GetPadElecomTrigger( PAD_4 ) == true )
	{
		g_playerInfo.bAutoMove = true;
	}

	if ( g_playerInfo.bAutoMove == false )
	{
		// ゲームパッドでの移動処理
		if ( ( GetGamePadCrossKeyX() > 0 || GetGamePadCrossKeyY() > 0 || GetGamePadCrossKeyX() < 0 || GetGamePadCrossKeyY() < 0 ) )
		{
			// 向いてる向きの設定（当たり判定用）
			
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

			// モーション変更
			g_playerInfo.status = WALK;
			g_playerInfo.bMove = true;

			// 移動処理
			g_playerInfo.posParts[ 0 ].x += cosf( atan2f( ( float )GetGamePadCrossKeyX(), ( float )GetGamePadCrossKeyY() ) + pCamera ->rotCamera.y ) * MOVE_VAL;
			g_playerInfo.posParts[ 0 ].z += sinf( atan2f( ( float )GetGamePadCrossKeyX(), ( float )GetGamePadCrossKeyY() ) + pCamera ->rotCamera.y ) * MOVE_VAL;	

			// 向きの目標値を設定
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
		
		PrintDebugProc("[ プレイヤー向き ] : %f\n", g_playerInfo.targetRotParts.y);
	}

	// 自動で動く処理をOFF（ガードや左スティック）
	if ( ( GetPadElecomTrigger( PAD_1 ) == true )
	||	 ( GetGamePadCrossKeyX() > 0 || GetGamePadCrossKeyY() > 0 || GetGamePadCrossKeyX() < 0 || GetGamePadCrossKeyY() < 0 ) )
	{
		g_playerInfo.bAutoMove = false;
	}

	// 指定した位置に自動で動く処理
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
// プレイヤー移動処理（キーボード）
//-----------------------------------------------
void UpdatePlayerMoveKeyboard( void )
{
	// ローカル変数
	//------------------------------------
	CAMERA *pCamera = GetCamera();				// カメラの情報取得
	MODEL_INFO *pEnemyInfo = GetEnemyInfo();

	// 自動で動く処理をON（攻撃ボタン）
	if ( GetKeyboardTrigger( DIK_L ) == true )
	{
		g_playerInfo.bAutoMove = true;
	}

	if ( g_playerInfo.bAutoMove == false )
	{
		// 移動処理
		//----------------------------------------------
		if ( ( GetKeyboardPress(DIK_W) || GetKeyboardPress(DIK_S) || GetKeyboardPress(DIK_D) || GetKeyboardPress(DIK_A) ) )
		{
			// 上移動
			if ( GetKeyboardPress(DIK_W) )
			{
				// 左斜め上移動
				if ( GetKeyboardPress(DIK_A) )
				{
					// モデルの目標角度を設定
					g_playerInfo.targetRotParts.y = -atan2f( -1.0f, -1.0f ) - D3DX_PI / 2 - pCamera ->rotCamera.y;

					// 移動量を設定
					g_playerInfo.move.x += cosf( atan2f( -1.0f, -1.0f ) + pCamera ->rotCamera.y ) * MOVE_VAL * SLOW_VAL;
					g_playerInfo.move.z += sinf( atan2f( -1.0f, -1.0f ) + pCamera ->rotCamera.y ) * MOVE_VAL * SLOW_VAL;

					g_playerInfo.directionSecond = DIRECTION_LEFT;
				}

				// 右斜め上移動
				else if ( GetKeyboardPress(DIK_D) )
				{
					// モデルの目標角度を設定
					g_playerInfo.targetRotParts.y = -atan2f( 1.0f, -1.0f ) - D3DX_PI / 2 - pCamera ->rotCamera.y;

					// 移動量を設定
					g_playerInfo.move.x += cosf( atan2f( 1.0f, -1.0f ) + pCamera ->rotCamera.y ) * MOVE_VAL * SLOW_VAL;
					g_playerInfo.move.z += sinf( atan2f( 1.0f, -1.0f ) + pCamera ->rotCamera.y ) * MOVE_VAL * SLOW_VAL;

					g_playerInfo.directionSecond = DIRECTION_RIGHT;
				}

				else
				{
					// モデルの目標角度を設定
					g_playerInfo.targetRotParts.y = -atan2f( 0.0f, -1.0f ) - D3DX_PI / 2 - pCamera ->rotCamera.y;

					// 移動量を設定
					g_playerInfo.move.x += cosf( atan2f( 0.0f, -1.0f ) + pCamera ->rotCamera.y ) * MOVE_VAL * SLOW_VAL;
					g_playerInfo.move.z += sinf( atan2f( 0.0f, -1.0f ) + pCamera ->rotCamera.y ) * MOVE_VAL * SLOW_VAL;
				}

				if ( g_playerInfo.bMove == false )
				{
					g_playerInfo.direction = DIRECTION_DEPTH;
				}

				g_playerInfo.bMove = true;	// 移動判定あり
			}

			// 下移動
			else if ( GetKeyboardPress(DIK_S) )
			{
				// 左斜め下移動
				if ( GetKeyboardPress(DIK_A) )
				{
					// モデルの目標角度を設定
					g_playerInfo.targetRotParts.y = -atan2f( -1.0f, 1.0f ) - D3DX_PI / 2 - pCamera ->rotCamera.y;

					// 移動量を設定
					g_playerInfo.move.x += cosf( atan2f( -1.0f, 1.0f ) + pCamera ->rotCamera.y ) * MOVE_VAL * SLOW_VAL;
					g_playerInfo.move.z += sinf( atan2f( -1.0f, 1.0f ) + pCamera ->rotCamera.y ) * MOVE_VAL * SLOW_VAL;
			
					g_playerInfo.directionSecond = DIRECTION_LEFT;
				}

				// 右斜め下移動
				else if ( GetKeyboardPress(DIK_D) )
				{
					// モデルの目標角度を設定
					g_playerInfo.targetRotParts.y = -atan2f( 1.0f, 1.0f ) - D3DX_PI / 2 - pCamera ->rotCamera.y;

					// 移動量を設定
					g_playerInfo.move.x += cosf( atan2f( 1.0f, 1.0f ) + pCamera ->rotCamera.y ) * MOVE_VAL * SLOW_VAL;
					g_playerInfo.move.z += sinf( atan2f( 1.0f, 1.0f ) + pCamera ->rotCamera.y ) * MOVE_VAL * SLOW_VAL;
			
					g_playerInfo.directionSecond = DIRECTION_RIGHT;
				}

				else
				{
					// モデルの目標角度を設定
					g_playerInfo.targetRotParts.y = -atan2f( 0.0f, 1.0f ) - D3DX_PI / 2 - pCamera ->rotCamera.y;

					// 移動量を設定
					g_playerInfo.move.x += cosf( atan2f( 0.0f, 1.0f ) + pCamera ->rotCamera.y ) * MOVE_VAL * SLOW_VAL;
					g_playerInfo.move.z += sinf( atan2f( 0.0f, 1.0f ) + pCamera ->rotCamera.y ) * MOVE_VAL * SLOW_VAL;
				}

				if ( g_playerInfo.bMove == false )
				{
					g_playerInfo.direction = DIRECTION_FRONT;
				}
			
				g_playerInfo.bMove = true;	// 移動判定あり
			}

			// 左移動
			else if ( GetKeyboardPress(DIK_A) )
			{
				// モデルの目標角度を設定
				g_playerInfo.targetRotParts.y = -atan2f( -1.0f, 0.0f ) - D3DX_PI / 2 - pCamera ->rotCamera.y;

				// 移動量を設定
				g_playerInfo.move.x += cosf( atan2f( -1.0f, 0.0f ) + pCamera ->rotCamera.y ) * MOVE_VAL * SLOW_VAL;
				g_playerInfo.move.z += sinf( atan2f( -1.0f, 0.0f ) + pCamera ->rotCamera.y ) * MOVE_VAL * SLOW_VAL;

				if ( g_playerInfo.bMove == false )
				{
					g_playerInfo.direction = DIRECTION_LEFT;
				}

				g_playerInfo.bMove = true;	// 移動判定あり
			}

			// 右移動
			else if ( GetKeyboardPress(DIK_D) )
			{
				// モデルの目標角度を設定
				g_playerInfo.targetRotParts.y = -atan2f( 1.0f, 0.0f ) - D3DX_PI / 2 - pCamera ->rotCamera.y;
			
				// 移動量を設定
				g_playerInfo.move.x += cosf( atan2f( 1.0f, 0.0f ) + pCamera ->rotCamera.y ) * MOVE_VAL * SLOW_VAL;
				g_playerInfo.move.z += sinf( atan2f( 1.0f, 0.0f ) + pCamera ->rotCamera.y ) * MOVE_VAL * SLOW_VAL;

				if ( g_playerInfo.bMove == false )
				{
					g_playerInfo.direction = DIRECTION_RIGHT;
				}

				g_playerInfo.bMove = true;	// 移動判定あり
			}

			g_playerInfo.status = WALK;
		
			// 当たり判定用の情報を更新
			rectPlayer.pos = g_playerInfo.posParts[ 0 ];
		}
		else
		{
			g_playerInfo.status = NEUTRAL;
			g_playerInfo.bMove = false;
			g_playerInfo.direction = DIRECTION_MAX;
		}
	}

	// 自動で動く処理をOFF（ガードや移動キー入力）
	if ( ( GetKeyboardTrigger( DIK_J ) == true )
	||	 ( GetKeyboardPress(DIK_W) || GetKeyboardPress(DIK_S) || GetKeyboardPress(DIK_D) || GetKeyboardPress(DIK_A) ) )
	{
		g_playerInfo.bAutoMove = false;
	}

	// 指定した位置に自動で動く処理
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
// モデルに関する描画処理
//
//===============================================
void DrawModel(void)
{
	// ローカル変数
	//------------------------------------
	LPDIRECT3DDEVICE9 pDevice;						// デバイスオブジェクト

	// デバイスの取得
	//------------------------------------
	pDevice = GetDevice();

	if ( playerStatus.bHitEnemy == true)
	{
		if ( ( playerStatus.nFrameCount % 20 ) <= 20 / 3 )
		{
			// 描画
			//------------------------------------
			DrawModelMotion( &g_playerInfo );
		}
	}
	else
	{
		// 描画
		//------------------------------------
		DrawModelMotion( &g_playerInfo );
	}

	//// デバッグ表示
	//PrintDebugProc("/----- モデル情報[ 0 ] -------------/\n");
	//PrintDebugProc("[ ワールド位置 ]\nX : %f  Y : %f  Z : %f\n", g_playerInfo.mtxWorld[ nCnt ]._41, g_playerInfo.mtxWorld[ nCnt ]._42, g_playerInfo.mtxWorld[ nCnt ]._43);
	//PrintDebugProc("[ ローカル位置 ]\nX : %f  Y : %f  Z : %f\n", g_playerInfo.posParts[ 0 ].x, g_playerInfo.posParts[ 0 ].y, g_playerInfo.posParts[ 0 ].z);
	//PrintDebugProc("[ 現在の向き ]\nY : %f\n", g_playerInfo.rotModel.y);
	//PrintDebugProc("/-----------------------------------/\n\n");

	//PrintDebugProc("捕まえた数 : %d\n", g_nNumEnemyCatch);

	/*PrintDebugProc("/----- モデル情報[ 1 ] -------------/\n");
	PrintDebugProc("[ ローカル位置 ]\nX : %f  Y : %f  Z : %f\n", g_model[1].posParts[ 0 ].x, g_model[1].posParts[ 0 ].y, g_model[1].posParts[ 0 ].z);
	PrintDebugProc("[ 現在の向き ]\nY : %f\n", g_model[1].rotModel.y);
	PrintDebugProc("/-----------------------------------/\n\n");*/
}

//===============================================
//
// モデルの向きの慣性処理
//
//===============================================
void RotateModel(void)
{
	// D3DX_PIより大きい値になったら丸め込む
	//----------------------------------------------
	if (g_playerInfo.targetRotParts.y - g_playerInfo.rotParts[ 0 ].y > D3DX_PI)
	{
		// 慣性の処理と丸め込み計算
		g_playerInfo.rotParts[ 0 ].y += ((g_playerInfo.targetRotParts.y - g_playerInfo.rotParts[ 0 ].y) - (D3DX_PI * 2)) * ROT_BRAKE_VAL;

		// 規定範囲外だったら丸め込む
		//---------------------------------------------
		if (g_playerInfo.rotParts[ 0 ].y > D3DX_PI)
		{
			// 丸め込み計算
			g_playerInfo.rotParts[ 0 ].y = ((g_playerInfo.rotParts[ 0 ].y) - (D3DX_PI * 2));
		}
		else if (g_playerInfo.rotParts[ 0 ].y < -D3DX_PI)
		{
			// 丸め込み計算
			g_playerInfo.rotParts[ 0 ].y = ((g_playerInfo.rotParts[ 0 ].y) + (D3DX_PI * 2));
		}
	}

	// -D3DX_PIより小さな値になったら
	else if (g_playerInfo.targetRotParts.y - g_playerInfo.rotParts[ 0 ].y < -D3DX_PI)
	{
		// 慣性の処理と丸め込み計算
		g_playerInfo.rotParts[ 0 ].y += ((g_playerInfo.targetRotParts.y - g_playerInfo.rotParts[ 0 ].y) + (D3DX_PI * 2)) * ROT_BRAKE_VAL;

		// 規定範囲外だったら丸め込む
		//---------------------------------------------
		if (g_playerInfo.rotParts[ 0 ].y > D3DX_PI)
		{
			// 丸め込み計算
			g_playerInfo.rotParts[ 0 ].y = ((g_playerInfo.rotParts[ 0 ].y) - (D3DX_PI * 2));
		}
		else if (g_playerInfo.rotParts[ 0 ].y < -D3DX_PI)
		{
			// 丸め込み計算
			g_playerInfo.rotParts[ 0 ].y = ((g_playerInfo.rotParts[ 0 ].y) + (D3DX_PI * 2));
		}
	}
	else
	{
		// 通常の慣性の計算
		g_playerInfo.rotParts[ 0 ].y += (g_playerInfo.targetRotParts.y - g_playerInfo.rotParts[ 0 ].y) * ROT_BRAKE_VAL;
	}
}

//===============================================
//
// モデル構造体変数取得
//
//===============================================
MODEL_INFO *GetModelInfo( void )
{
	return &g_playerInfo;
}

//===============================================
//
// 当たり判定の情報を取得
//
//===============================================
COL_RECT *GetRectPlayer()
{
	return &rectPlayer;
}

//===============================================
//
// 敵の捕まえた数を加える変数
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
// 敵の捕まえた数を取得
//
//===============================================
int GetCatchEnemy( void )
{
	return g_nNumEnemyCatch;
}

//===============================================
//
// 敵の捕まえた数を初期化する
//
//===============================================
void InitCatchEnemy( void )
{
	g_nNumEnemyCatch = 0;
}

//===============================================
//
// バレットタイプの取得
//
//===============================================
BULLETTYPE GetBulletType( void )
{
	return bulletType;
}

//===============================================
//
// プレイヤー状態を取得
//
//===============================================
PLAYER_STATUS *GetPlayerStatus( void )
{
	return &playerStatus;
}

//===============================================
//
// タイトルでの更新処理
//
//===============================================
void UpdateModelTitle( void )
{
	CAMERA *pCamera = GetCamera();				// カメラの情報取得

	g_playerInfo.statusOld = g_playerInfo.status;

	// モデルの目標角度を設定
	g_playerInfo.targetRotParts.y = -(D3DX_PI / 2) - pCamera ->rotCamera.y;

	g_playerInfo.status = WALK;

	g_playerInfo.posParts[ 0 ].x += 3.0f;

	if ( g_playerInfo.posParts[ 0 ].x >= SCREEN_WIDTH + 100.0f )
	{
		g_playerInfo.posParts[ 0 ].x = -400.0f;
	}

	// モデルの向きの慣性処理
	//-----------------------------------
	RotateModel();

	// モーションの更新処理
	//-----------------------------------
	UpdateModelMotion( &g_playerInfo );
}
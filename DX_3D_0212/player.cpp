//-----------------------------------------------
//  内容：プレイヤー処理
//  File : player.cpp
//  制作者：木崎　翔
//-----------------------------------------------

//------ インクルードファイル ------
#include "player.h"
#include "camera.h"
#include "collision.h"
#include "cube.h"
#include "Edit.h"
#include "gear.h"
#include "moveCube.h"
#include "billboard.h"
#include "sheet.h"

//------ マクロ定義 ------
#define PLAYER_WIDTH ( 20.0f )
#define PLAYER_HEIGHT ( 20.0f )
#define PLAYER_POS_X ( -200.0f )
#define PLAYER_POS_Y ( 40.0f )
#define PLAYER_POS_Z ( 0.0f )

#define MOVE_VAL ( 3.0f )
#define GRAVITY_VAL ( 0.5f )

#define PLAYER_TEX_MAX ( 1 )

//------ グローバル変数 ------
D3DXMATRIX mtxWorldPlayer;				// ワールドマトリックス
LPD3DXMESH pMeshModelPlayer;			// メッシュ情報へのポインタ
LPD3DXBUFFER pBuffMatModelPlayer;		// マテリアル情報へのポインタ
DWORD numMatModelPlayer;				// マテリアルの数
LPDIRECT3DTEXTURE9	g_pTexturePlayer[ PLAYER_TEX_MAX ]; 		// テクスチャへのポインタ

D3DXVECTOR3 posPlayer;		// プレイヤーの位置
D3DXVECTOR3 posPlayerOld;	// プレイヤーの前回位置	
D3DXVECTOR3 rotPlayer;		// 角度
D3DXVECTOR3 sclPlayer;		// スケール
COL_RECT rectPlayer;		// 当たり判定
float fJumpVal = 0.0f;		// ジャンプ量
int g_nCubeIndex = 0;		// キューブのインデックス
int g_nRedCubeIndex = 0;	// 赤キューブのインデックス
bool g_bJump = false;		// ジャンプ中？

bool g_bDirect = true;		// 向き
	
bool g_bColorChange = false;

//------ 内部関数宣言 ------

//-----------------------------------------------
// プレイヤー初期化
//-----------------------------------------------
void InitPlayer( void )
{
	// ローカル変数
	//---------------------------------
	LPDIRECT3DDEVICE9 pDevice = NULL;		// デバイスオブジェクト
	
	// デバイス取得
	//------------------------------------
	pDevice = GetDevice();

	{
		// xファイルの読み込み
		//------------------------------------
		D3DXLoadMeshFromX("data/MODEL/player.x",		// 読み込むファイル名
						  D3DXMESH_SYSTEMMEM,							// 
						  pDevice,										// 
						  NULL,											// 
						  &pBuffMatModelPlayer,			// 
						  NULL,											// 
						  &numMatModelPlayer,			// 
						  &pMeshModelPlayer );
	}

	// テクスチャポインタの初期化
	g_pTexturePlayer[ 0 ] = NULL;

	// テクスチャの読み込み
	//------------------------------------
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Player.jpg", &g_pTexturePlayer[ 0 ]);

	// グローバル変数の初期化
	sclPlayer = D3DXVECTOR3( 1.0f, 1.0f, 1.0f ); 
	rotPlayer = D3DXVECTOR3( 0.0f, 0.0f, 0.0f ); 
	posPlayer = D3DXVECTOR3( PLAYER_POS_X, PLAYER_POS_Y, PLAYER_POS_Z ); 
	posPlayerOld = posPlayer;

	rectPlayer.pos = posPlayer;
	rectPlayer.harfSize = D3DXVECTOR3( PLAYER_WIDTH, PLAYER_HEIGHT, PLAYER_HEIGHT );

	fJumpVal = 0.0f;
	g_nCubeIndex = CUBE_MAX;
	g_nRedCubeIndex = 0;
	g_bDirect = true;
	g_bJump = false;
	g_bColorChange = false;
}

//-----------------------------------------------
// プレイヤー終了処理
//-----------------------------------------------
void UninitPlayer( void )
{
	// メッシュ情報の解放と初期化
	//------------------------------------
	if (pMeshModelPlayer != NULL)
	{ 
		pMeshModelPlayer ->Release();
		pMeshModelPlayer = NULL;
	}
	
	// マテリアル情報の解放と初期化
	//------------------------------------
	if (pBuffMatModelPlayer != NULL)
	{ 
		pBuffMatModelPlayer ->Release();
		pBuffMatModelPlayer = NULL;
	}

	// 使ったテクスチャの解放と初期化
	//------------------------------------
	//for (int nCnt = 0; nCnt < TEXTURE_MAX; nCnt++)
	{
		if (g_pTexturePlayer[ 0 ] != NULL)
		{ 
			g_pTexturePlayer[ 0 ] ->Release();
			g_pTexturePlayer[ 0 ] = NULL;
		}
	}
}

//-----------------------------------------------
// プレイヤー更新処理
//-----------------------------------------------
void UpdatePlayer( void )
{
	// ローカル変数
	CUBE *pCube = GetCube();
	GEAR *pGear = GetGear();
	MOVECUBE *pMoveCube = GetMoveCube();

	if ( GetEdit() == false )
	{
		// 前回位置を保存
		posPlayerOld = posPlayer;
		g_nCubeIndex = CUBE_MAX;
		g_bColorChange = false;

		// デバッグ（赤シート）
		/*if ( ( GetPadElecomTrigger( PAD_6 ) == true || GetKeyboardTrigger( DIK_DOWN ) == true ) && GetSheet() == false )
		{
			GetSheet() = true;
		}
		else if ( ( GetPadElecomTrigger( PAD_6 ) == true || GetKeyboardTrigger( DIK_DOWN ) == true ) && GetSheet() == true )
		{
			GetSheet() = false;
		}*/

		// 重力をかける
		fJumpVal += GRAVITY_VAL;
		posPlayer.y -= fJumpVal;
		rectPlayer.pos.y = posPlayer.y;

		// 歯車との当たり判定
		for ( int i = 0; i < GEAR_MAX; i++ )
		{
			if ( ( pGear + i ) ->bUse == true )
			{
				if ( ColRectXY( &rectPlayer, &( pGear + i ) ->rect ) == true )
				{
					//if ( GetKeyboardTrigger( DIK_RETURN ) == true )
					{
						posPlayer.y = posPlayerOld.y;
						rectPlayer.pos.y = posPlayer.y;
						fJumpVal = 0.0f;
						g_bJump = false;
					}
				}
			}
		}

		if ( GetSheet() == false )
		{
			// 赤キューブとの当たり判定
			for ( int i = 0; i < MOVECUBE_MAX; i++ )
			{
				if ( ( pMoveCube + i ) ->bUse == true )
				{
					if ( ColRectXY( &rectPlayer, &( pMoveCube + i ) ->rect ) == true )
					{
						posPlayer.y = posPlayerOld.y;
						rectPlayer.pos.y = posPlayer.y;
						fJumpVal = 0.0f;
						g_bJump = false;

						g_nCubeIndex = i;
					}
				}
			}
		}
		else
		{
			// キューブとの当たり判定
			for ( int i = 0; i < CUBE_MAX; i++ )
			{
				if ( ( pCube + i ) ->bUse == true )
				{
					if ( ColRectXY( &rectPlayer, &( pCube + i ) ->rect ) == true )
					{
						posPlayer.y = posPlayerOld.y;
						rectPlayer.pos.y = posPlayer.y;
						fJumpVal = 0.0f;
						g_bJump = false;

						g_nCubeIndex = i;
					}
				}
			}
		}

		// 地面判定
		if ( posPlayer.y + PLAYER_HEIGHT <= PLAYER_HEIGHT )
		{
			posPlayer.y = posPlayerOld.y;
			rectPlayer.pos.y = posPlayer.y;
			fJumpVal = 0.0f;			
			g_bJump = false;
		}

		// 移動処理（キーボード）
		if ( GetKeyboardPress( DIK_A ) == true )
		{
			posPlayer.x -= 3.0f;
			rectPlayer.pos.x = posPlayer.x;
			g_bDirect = false;
		}
		if ( GetKeyboardPress( DIK_D ) == true )
		{
			posPlayer.x += 3.0f;
			rectPlayer.pos.x = posPlayer.x;
			g_bDirect = true;
		}

		// ゲームパッドでの移動処理
		if ( ( GetGamePadCrossKeyX() > 0 || GetGamePadCrossKeyY() > 0 || GetGamePadCrossKeyX() < 0 || GetGamePadCrossKeyY() < 0 ) )
		{
			// 向いてる向きの設定（当たり判定用）
			if ( GetGamePadCrossKeyX() > 0 )
			{
				g_bDirect = true;
			}
			else if ( GetGamePadCrossKeyX() < 0 )
			{
				g_bDirect = false;
			}

			// モーション変更

			// 移動処理
			posPlayer.x += GetGamePadCrossKeyX() * MOVE_VAL;
			rectPlayer.pos.x = posPlayer.x;

			// 向きの目標値を設定

		}
		/*else
		{
			if ( g_playerInfo.bMotion == false )
			{
				g_playerInfo.bMove = false;
				g_playerInfo.status = NEUTRAL;
				g_playerInfo.direction = DIRECTION_MAX;
				PrintDebugProc("DIRECTION_MAX\n");
			}
		}*/

		// 歯車との当たり判定
		for ( int i = 0; i < GEAR_MAX; i++ )
		{
			if ( ( pGear + i ) ->bUse == true )
			{
				if ( ColRectXY( &rectPlayer, &( pGear + i ) ->rect ) == true )
				{
					//if ( GetKeyboardTrigger( DIK_RETURN ) == true )
					{
						posPlayer.x = posPlayerOld.x;
						rectPlayer.pos.x = posPlayer.x;
					}
				}
			}
		}

		if ( GetSheet() == false )
		{
			// 赤キューブとの当たり判定
			for ( int i = 0; i < MOVECUBE_MAX; i++ )
			{
				if ( ( pMoveCube + i ) ->bUse == true )
				{
					if ( ColRectXY( &rectPlayer, &( pMoveCube + i ) ->rect ) == true )
					{
						posPlayer.x = posPlayerOld.x;
						rectPlayer.pos.x = posPlayer.x;
					}
				}
			}
		}
		else
		{
			// キューブとの当たり判定
			for ( int i = 0; i < CUBE_MAX; i++ )
			{
				if ( ( pCube + i ) ->bUse == true )
				{
					if ( ColRectXY( &rectPlayer, &( pCube + i ) ->rect ) == true )
					{
						posPlayer.x = posPlayerOld.x;
						rectPlayer.pos.x = posPlayer.x;
					}
				}
			}
		}

		// ジャンプ処理
		if ( ( GetKeyboardTrigger( DIK_SPACE ) == true || GetPadElecomTrigger( PAD_3 ) ) && g_bJump == false )
		{
			fJumpVal -= 8.0f;
			g_bJump = true;
		}

		// 移動床を判定するやつ
		if ( g_nCubeIndex < CUBE_MAX )
		{
			posPlayer += ( pCube + g_nCubeIndex ) ->move;
			rectPlayer.pos = posPlayer;
		}

		// 色を変える処理
		if ( GetPadElecomTrigger( PAD_4 ) == true || GetKeyboardTrigger( DIK_RETURN ) == true )
		{
			if ( GetSheet() == false )
			{
				// 赤キューブとの当たり判定
				for ( int i = 0; i < MOVECUBE_MAX; i++ )
				{
					if ( ( pMoveCube + i ) ->bUse == true )
					{
						if ( g_bDirect == true )
						{
							// 作業用変数
							COL_RECT work;

							// 現在の状態を保存
							work = rectPlayer;

							if ( GetKeyboardTrigger( DIK_S ) == true )
							{
								work.pos.y += 10.0f;
							}
							else
							{
								work.pos.x += 10.0f;
							}

							if ( ColRectXY( &work, &( pMoveCube + i ) ->rect ) == true )
							{
								// 白キューブの生成
								SetCube( ( pMoveCube + i ) ->pos );

								SetBillboard( ( pMoveCube + i ) ->pos, 20.0f, 20.0f, TEXTURETYPE_GREEN );
							
								// 赤キューブをなくす
								( pMoveCube + i ) ->bUse = false;

								g_bColorChange = true;

								break;	// ループを抜ける
							}
						}
						else
						{
							// 作業用変数
							COL_RECT work;

							// 現在の状態を保存
							work = rectPlayer;

							if ( GetKeyboardTrigger( DIK_S ) == true )
							{
								work.pos.y += 10.0f;
							}
							else
							{
								work.pos.x += 10.0f;
							}
							
							if ( ColRectXY( &work, &( pMoveCube + i ) ->rect ) == true )
							{
								// 白キューブの生成
								SetCube( ( pMoveCube + i ) ->pos );
								
								SetBillboard( ( pMoveCube + i ) ->pos, 20.0f, 20.0f, TEXTURETYPE_GREEN );

								// 赤キューブをなくす
								( pMoveCube + i ) ->bUse = false;

								g_bColorChange = true;

								break;	// ループを抜ける
							}						
						}
					}
				}
			}

			else //if ( GetSheet() == true )
			{
				// 白キューブとの当たり判定
				for ( int i = 0; i < CUBE_MAX; i++ )
				{
					if ( ( pCube + i ) ->bUse == true )
					{
						if ( g_bDirect == true )
						{
							// 作業用変数
							COL_RECT work;

							// 現在の状態を保存
							work = rectPlayer;

							if ( GetKeyboardTrigger( DIK_S ) == true )
							{
								work.pos.y += 10.0f;
							}
							else
							{
								work.pos.x += 10.0f;
							}
							
							if ( ColRectXY( &work, &( pCube + i ) ->rect ) == true )
							{
								// 赤キューブの生成
								SetMoveCube( ( pCube + i ) ->pos );

								SetBillboard( ( pCube + i ) ->pos, 20.0f, 20.0f, TEXTURETYPE_RED );
							
								// 白キューブをなくす
								( pCube + i ) ->bUse = false;

								break;	// ループを抜ける
							}
						}
						else
						{
							// 作業用変数
							COL_RECT work;

							// 現在の状態を保存
							work = rectPlayer;

							if ( GetKeyboardTrigger( DIK_S ) == true )
							{
								work.pos.y += 10.0f;
							}
							else
							{
								work.pos.x -= 10.0f;
							}
							
							if ( ColRectXY( &work, &( pCube + i ) ->rect ) == true )
							{
								// 赤キューブの生成
								SetMoveCube( ( pCube + i ) ->pos );

								SetBillboard( ( pCube + i ) ->pos, 20.0f, 20.0f, TEXTURETYPE_RED );
							
								// 白キューブをなくす
								( pCube + i ) ->bUse = false;

								break;	// ループを抜ける
							}
						}
					}
				}
			}
		}

		//
		if ( GetKeyboardPress( DIK_RIGHT ) )
		{
			rotPlayer.y += 0.01f;
		}

		PrintDebugProc("[ プレイヤー位置 ] %f %f %f\n", posPlayer.x, posPlayer.y, posPlayer.z);
	}
}

//-----------------------------------------------
// プレイヤー描画処理
//-----------------------------------------------
void DrawPlayer( void )
{
	// ローカル変数
	//------------------------------------
	LPDIRECT3DDEVICE9 pDevice;						// デバイスオブジェクト
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;		// 計算用
	D3DXMATERIAL *pMat;								// マテリアル情報を取得する変数
	D3DMATERIAL9 matDef;							// 元のマテリアル情報

	// デバイスの取得
	//------------------------------------
	pDevice = GetDevice();

	// カメラのセット
	SetCamera();

	D3DXMatrixIdentity( &mtxWorldPlayer );	// フォーマットの初期化
	D3DXMatrixIdentity( &mtxScl );						// 行列の初期化
	D3DXMatrixIdentity( &mtxRot );						// 行列の初期化
	D3DXMatrixIdentity( &mtxTranslate );				// 行列の初期化

	// スケールを反映
	D3DXMatrixScaling(&mtxScl, sclPlayer.x, sclPlayer.y, sclPlayer.z);
	D3DXMatrixMultiply(&mtxWorldPlayer, &mtxWorldPlayer, &mtxScl);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rotPlayer.y, rotPlayer.x, rotPlayer.z);
	D3DXMatrixMultiply(&mtxWorldPlayer, &mtxWorldPlayer, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTranslate, posPlayer.x, posPlayer.y, posPlayer.z);
	D3DXMatrixMultiply(&mtxWorldPlayer, &mtxWorldPlayer, &mtxTranslate);

	// 設定
	pDevice ->SetTransform(D3DTS_WORLD, &mtxWorldPlayer);

	// 現在のマテリアル情報を保存
	pDevice ->GetMaterial( &matDef );

	// バッファへのポインタを取得
	pMat = (D3DXMATERIAL*)pBuffMatModelPlayer ->GetBufferPointer();

	/*strSrc = strrchr(pMat ->pTextureFilename, '\\') + 1;

	strcat(strDest, strSrc);

	D3DXCreateTextureFromFile(pDevice, strDest, &g_pTextureModel[1]);*/
			
	// マテリアルの数だけループ
	for (int nCntMat = 0; nCntMat < (int)numMatModelPlayer; nCntMat++)
	{
		pDevice ->SetMaterial( &pMat[ nCntMat ].MatD3D );			// マテリアルの設定
		pDevice ->SetTexture( 0, g_pTexturePlayer[ 0 ] );							// テクスチャのセット
		pMeshModelPlayer ->DrawSubset( nCntMat );		// 描画
	}
			
	// マテリアルを元に戻す
	//------------------------------------
	pDevice ->SetMaterial( &matDef );
}

//-----------------------------------------------
// プレイヤー位置取得処理
//-----------------------------------------------
D3DXVECTOR3 GetPlayerPos( void )
{
	return posPlayer;
}

// デバッグ用
bool GetRed( void )
{
	return GetSheet();
}
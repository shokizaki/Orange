//=======================================================================================
//	クラス：AT11C272 30623 13
//	制作者：木崎　翔
//	内容：ポーズ処理
//
//=======================================================================================

//-----------------------------------------------
//
//  インクルードファイル
//
//-----------------------------------------------
#include "pause.h"
#include "camera.h"

// マクロ定義
#define PAUSE_OFFSET_Y ( 120.0f )

// グローバル変数
PAUSE g_pause[ 5 ];
PAUSE_SELECT g_select;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause = NULL;		// 頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTexturePause[ 5 ];				// テクスチャへのポインタ
bool g_bPauseDraw = true;								// ポーズ画面の描画判定

//-----------------------------------------------
//
//　ポーズ初期化
//　
//-----------------------------------------------
void InitPause(void)
{
	// ローカル変数
	//---------------------------------
	LPDIRECT3DDEVICE9 pDevice;		// デバイスオブジェクト

	// デバイス取得
	//------------------------------------
	pDevice = GetDevice();

	pDevice ->CreateVertexBuffer( (sizeof( VERTEX_2D ) * 4 * 5 ),	// 頂点データ用に確保するバッファ
								 D3DUSAGE_WRITEONLY,				// 頂点バッファの使用方法
								 FVF_VERTEX_2D,						// 使用する頂点フォーマット
								 D3DPOOL_MANAGED,					// バッファを保持するメモリクラスの指定
								 &g_pVtxBuffPause,					// 頂点バッファへのポインタ
								 NULL);								// NULL固定

	// それぞれの位置の初期化
	g_pause[ 0 ].posPause = D3DXVECTOR3( SCREEN_WIDTH / 2, PAUSE_OFFSET_Y, 0.0f );	
	g_pause[ 0 ].rotPause = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	g_pause[ 0 ].sclPause = D3DXVECTOR3( 1.0f, 1.0f, 1.0f );
	g_pause[ 0 ].fWidth = 100.0f;
	g_pause[ 0 ].fheight = 20.0f;
	g_pause[ 0 ].a = 1.0f;

	g_pause[ 1 ].posPause = D3DXVECTOR3( SCREEN_WIDTH / 2, PAUSE_OFFSET_Y * 2, 0.0f );	
	g_pause[ 1 ].rotPause = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	g_pause[ 1 ].sclPause = D3DXVECTOR3( 1.0f, 1.0f, 1.0f );
	g_pause[ 1 ].fWidth = 100.0f;
	g_pause[ 1 ].fheight = 20.0f;
	g_pause[ 1 ].a = 1.0f;

	g_pause[ 2 ].posPause = D3DXVECTOR3( SCREEN_WIDTH / 2, PAUSE_OFFSET_Y * 3, 0.0f );	
	g_pause[ 2 ].rotPause = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	g_pause[ 2 ].sclPause = D3DXVECTOR3( 1.0f, 1.0f, 1.0f );
	g_pause[ 2 ].fWidth = 100.0f;
	g_pause[ 2 ].fheight = 20.0f;
	g_pause[ 2 ].a = 1.0f;

	g_pause[ 3 ].posPause = D3DXVECTOR3( SCREEN_WIDTH / 2, PAUSE_OFFSET_Y * 4, 0.0f );	
	g_pause[ 3 ].rotPause = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	g_pause[ 3 ].sclPause = D3DXVECTOR3( 1.0f, 1.0f, 1.0f );
	g_pause[ 3 ].fWidth = 100.0f;
	g_pause[ 3 ].fheight = 20.0f;
	g_pause[ 3 ].a = 1.0f;

	g_pause[ 4 ].posPause = D3DXVECTOR3( SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f );	
	g_pause[ 4 ].rotPause = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	g_pause[ 4 ].sclPause = D3DXVECTOR3( 1.0f, 1.0f, 1.0f );
	g_pause[ 4 ].fWidth = 200.0f;
	g_pause[ 4 ].fheight = 220.0f;
	g_pause[ 4 ].a = 0.5f;

	// 選択しているモードの初期化
	g_select = CONTINUE;
	
	g_bPauseDraw = true;

	// テクスチャの読み込み
	//------------------------------------
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/pause.png",			&g_pTexturePause[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/return_game.png",		&g_pTexturePause[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/retry.png",			&g_pTexturePause[2]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/return_title.png",		&g_pTexturePause[3]);
}


//-----------------------------------------------
//
//　ポーズセットアップ
//　
//-----------------------------------------------
void SetupPause(void)
{
	// ローカル変数
	//------------------------------------
	int nCnt;						// カウンタ変数
	VERTEX_2D *pVtx;				// 頂点の構造体ポインタ
	LPDIRECT3DDEVICE9 pDevice;		// デバイスオブジェクト

	// デバイス取得
	//------------------------------------
	pDevice = GetDevice();

	// データの位置を確保する（ロック）
	//------------------------------------
	g_pVtxBuffPause ->Lock(0,
						0,
						(void**)&pVtx,
						0);

	for ( nCnt = 0; nCnt < 4; nCnt++ )
	{
		// フェードポリゴンの大きさ
		//------------------------------------
		pVtx[ 0 + nCnt * 4 ].pos.x = g_pause[ nCnt ].posPause.x - g_pause[ nCnt ].fWidth;
		pVtx[ 0 + nCnt * 4 ].pos.y = g_pause[ nCnt ].posPause.y - g_pause[ nCnt ].fheight;	// 頂点の位置
		pVtx[ 0 + nCnt * 4 ].pos.z = 0.0f;												// 頂点の位置

		pVtx[ 1 + nCnt * 4 ].pos.x = g_pause[ nCnt ].posPause.x + g_pause[ nCnt ].fWidth;
		pVtx[ 1 + nCnt * 4 ].pos.y = g_pause[ nCnt ].posPause.y - g_pause[ nCnt ].fheight;	// 頂点の位置
		pVtx[ 1 + nCnt * 4 ].pos.z = 0.0f;												// 頂点の位置

		pVtx[ 2 + nCnt * 4 ].pos.x = g_pause[ nCnt ].posPause.x - g_pause[ nCnt ].fWidth;
		pVtx[ 2 + nCnt * 4 ].pos.y = g_pause[ nCnt ].posPause.y + g_pause[ nCnt ].fheight;	// 頂点の位置
		pVtx[ 2 + nCnt * 4 ].pos.z = 0.0f;												// 頂点の位置

		pVtx[ 3 + nCnt * 4 ].pos.x = g_pause[ nCnt ].posPause.x + g_pause[ nCnt ].fWidth;
		pVtx[ 3 + nCnt * 4 ].pos.y = g_pause[ nCnt ].posPause.y + g_pause[ nCnt ].fheight;	// 頂点の位置
		pVtx[ 3 + nCnt * 4 ].pos.z = 0.0f;												// 頂点の位置

		// 法線
		pVtx[ 0 + nCnt * 4 ].rhw = 1.0f;
		pVtx[ 1 + nCnt * 4 ].rhw = 1.0f;
		pVtx[ 2 + nCnt * 4 ].rhw = 1.0f;
		pVtx[ 3 + nCnt * 4 ].rhw = 1.0f;			

		// フェードの色
		//------------------------------------
		pVtx[ 0 + nCnt * 4 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, g_pause[ nCnt ].a );			// 頂点ごとの色の設定
		pVtx[ 1 + nCnt * 4 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, g_pause[ nCnt ].a );			//			↓
		pVtx[ 2 + nCnt * 4 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, g_pause[ nCnt ].a );			//			↓
		pVtx[ 3 + nCnt * 4 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, g_pause[ nCnt ].a );			//			↓

		pVtx[ 0 + nCnt * 4 ].tex = D3DXVECTOR2( 0.0f, 0.0f );
		pVtx[ 1 + nCnt * 4 ].tex = D3DXVECTOR2( 1.0f, 0.0f );
		pVtx[ 2 + nCnt * 4 ].tex = D3DXVECTOR2( 0.0f, 1.0f );
		pVtx[ 3 + nCnt * 4 ].tex = D3DXVECTOR2( 1.0f, 1.0f );
	}

	// フェードポリゴンの大きさ
	//------------------------------------
	pVtx[ 0 + 4 * 4 ].pos.x = g_pause[ 4 ].posPause.x - g_pause[ 4 ].fWidth;
	pVtx[ 0 + 4 * 4 ].pos.y = g_pause[ 4 ].posPause.y - g_pause[ 4 ].fheight;	// 頂点の位置
	pVtx[ 0 + 4 * 4 ].pos.z = 0.0f;												// 頂点の位置

	pVtx[ 1 + 4 * 4 ].pos.x = g_pause[ 4 ].posPause.x + g_pause[ 4 ].fWidth;
	pVtx[ 1 + 4 * 4 ].pos.y = g_pause[ 4 ].posPause.y - g_pause[ 4 ].fheight;	// 頂点の位置
	pVtx[ 1 + 4 * 4 ].pos.z = 0.0f;												// 頂点の位置

	pVtx[ 2 + 4 * 4 ].pos.x = g_pause[ 4 ].posPause.x - g_pause[ 4 ].fWidth;
	pVtx[ 2 + 4 * 4 ].pos.y = g_pause[ 4 ].posPause.y + g_pause[ 4 ].fheight;	// 頂点の位置
	pVtx[ 2 + 4 * 4 ].pos.z = 0.0f;												// 頂点の位置

	pVtx[ 3 + 4 * 4 ].pos.x = g_pause[ 4 ].posPause.x + g_pause[ 4 ].fWidth;
	pVtx[ 3 + 4 * 4 ].pos.y = g_pause[ 4 ].posPause.y + g_pause[ 4 ].fheight;	// 頂点の位置
	pVtx[ 3 + 4 * 4 ].pos.z = 0.0f;												// 頂点の位置

	// パースペクティブコレクト
	pVtx[ 0 + 4 * 4 ].rhw = 1.0f;
	pVtx[ 1 + 4 * 4 ].rhw = 1.0f;
	pVtx[ 2 + 4 * 4 ].rhw = 1.0f;
	pVtx[ 3 + 4 * 4 ].rhw = 1.0f;			

	// フェードの色
	//------------------------------------
	pVtx[ 0 + 4 * 4 ].col = D3DXCOLOR( 0.0f, 0.0f, 0.0f, g_pause[ 4 ].a );			// 頂点ごとの色の設定
	pVtx[ 1 + 4 * 4 ].col = D3DXCOLOR( 0.0f, 0.0f, 0.0f, g_pause[ 4 ].a );			//			↓
	pVtx[ 2 + 4 * 4 ].col = D3DXCOLOR( 0.0f, 0.0f, 0.0f, g_pause[ 4 ].a );			//			↓
	pVtx[ 3 + 4 * 4 ].col = D3DXCOLOR( 0.0f, 0.0f, 0.0f, g_pause[ 4 ].a );			//			↓

	pVtx[ 0 + 4 * 4 ].tex = D3DXVECTOR2( 0.0f, 0.0f );
	pVtx[ 1 + 4 * 4 ].tex = D3DXVECTOR2( 1.0f, 0.0f );
	pVtx[ 2 + 4 * 4 ].tex = D3DXVECTOR2( 0.0f, 1.0f );
	pVtx[ 3 + 4 * 4 ].tex = D3DXVECTOR2( 1.0f, 1.0f );

	// 解放
	//------------------------------------
	g_pVtxBuffPause -> Unlock();
}

//-----------------------------------------------
//
//　ポーズ終了処理
//　
//-----------------------------------------------
void UninitPause(void)
{
	for ( int nCnt = 0; nCnt < 4; nCnt++ )
	{
		if (g_pTexturePause[ nCnt ] != NULL)
		{ // 使ったテクスチャの解放と初期化
		  //------------------------------------
			g_pTexturePause[ nCnt ] -> Release();
			g_pTexturePause[ nCnt ] = NULL;
		}
	}

	if (g_pVtxBuffPause != NULL)
	{ // 頂点バッファの解放と初期化
	  //------------------------------------
		g_pVtxBuffPause->Release();
		g_pVtxBuffPause = NULL;
	}
}

//-----------------------------------------------
//
//　ポーズ更新処理
//　
//-----------------------------------------------
void UpdatePause(void)
{
	/*if ( GetFade() != FADE_NONE )
	{
		return;
	}*/

	// ポーズの描画のON / OFF
	if ( GetKeyboardTrigger( DIK_F4 ) == true && g_bPauseDraw == true )
	{
		g_bPauseDraw = false;
	}
	else if ( GetKeyboardTrigger( DIK_F4 ) == true && g_bPauseDraw == false )
	{
		g_bPauseDraw = true;
	}

	// メニュー選択処理
	if ( GetKeyboardTrigger( DIK_S ) == true /*|| GetGamePadCrossKeyY() == -1000*/ || GetPadElecomCrossKeyDownTrigger() )
	{
		PlaySound( CURSOR_SE );
		g_select = (PAUSE_SELECT)( (int)g_select + 1);
		if ( g_select >= SELECT_MAX )
		{
			g_select = CONTINUE;
		}
	}
	else if ( GetKeyboardTrigger( DIK_W ) == true /*|| GetGamePadCrossKeyY() == -1000*/ || GetPadElecomCrossKeyUpTrigger() )
	{
		PlaySound( CURSOR_SE );
		g_select = (PAUSE_SELECT)( (int)g_select - 1);
		if ( (int)g_select < (int)CONTINUE )
		{
			g_select = RETURN_TITLE;
		}
	}

	// メニュー選択状態処理（ 透明 / 半透明 ）
	switch ( g_select )
	{
	case CONTINUE:
		g_pause[ 1 ].a = 1.0f;
		g_pause[ 2 ].a = 0.5f;
		g_pause[ 3 ].a = 0.5f;

		if ( GetKeyboardTrigger( DIK_RETURN ) == true /*|| GetGamePadTrigger( GAMEPAD_A )*/ || GetPadElecomTrigger( PAD_4 ) )
		{
			PlaySound( CANCEL_SE );
			SetMenu();
			g_select = CONTINUE;
			g_bPauseDraw = true;
		}
		break;

	case RETRY:
		g_pause[ 1 ].a = 0.5f;
		g_pause[ 2 ].a = 1.0f;
		g_pause[ 3 ].a = 0.5f;

		if ( GetKeyboardTrigger( DIK_RETURN ) == true /*|| GetGamePadTrigger( GAMEPAD_A )*/ || GetPadElecomTrigger( PAD_4 ) )
		{
			PlaySound( DESIDE4_SE );
			SetFade( FADE_OUT, 60 );
			SetMode( MODE_GAME );
		}
		break;

	case RETURN_TITLE:
		g_pause[ 1 ].a = 0.5f;
		g_pause[ 2 ].a = 0.5f;
		g_pause[ 3 ].a = 1.0f;

		if ( GetKeyboardTrigger( DIK_RETURN ) == true /*|| GetGamePadTrigger( GAMEPAD_A )*/ || GetPadElecomTrigger( PAD_4 ) )
		{
			PlaySound( DESIDE4_SE );
			SetFade( FADE_OUT, 60 );
			SetMode( MODE_TITLE );
		}
		break;
	}

	// 頂点情報セット
	//------------------------------------
	SetupPause();
}

//-----------------------------------------------
//
//　ポーズ描画処理
//　
//-----------------------------------------------
void DrawPause(void)
{
	// ローカル変数
	//------------------------------------
	LPDIRECT3DDEVICE9 pDevice;						// デバイスオブジェクト
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;		// 計算用
	
	// デバイスの取得
	//------------------------------------
	pDevice = GetDevice();

	// 頂点バッファをデータストリームにバインド
	//-----------------------------------------
	pDevice ->SetStreamSource(0,
								g_pVtxBuffPause,
								0,
								sizeof(VERTEX_2D));	// 頂点データの間隔

	// 頂点フォーマットの設定
	//------------------------------------
	pDevice -> SetFVF(FVF_VERTEX_2D);

	if ( g_bPauseDraw == true )
	{
		// テクスチャの設定
		//------------------------------------
		pDevice -> SetTexture(0, NULL);

		// ポリゴンの描画
		//------------------------------------
		pDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,	// プリミティブの種類
										16,				// 読み込む最初の番号
										2);				// 描画するプリミティブ数

	
		for ( int i = 0; i < 4; i++ )
		{
			// テクスチャの設定
			//------------------------------------
			pDevice -> SetTexture(0, g_pTexturePause[ i ]);

			// ポリゴンの描画
			//------------------------------------
			pDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,	// プリミティブの種類
											i * 4,				// 読み込む最初の番号
											2);				// 描画するプリミティブ数
		}
	}
}

//-----------------------------------------------
//
//　ポーズ描画処理
//　
//-----------------------------------------------
bool GetPauseDraw( void )
{
	return g_bPauseDraw;
}

//-----------------------------------------------
//
//　ポーズ描画状態セット
//　
//-----------------------------------------------
void SetPauseDraw( bool pauseDraw )
{
	g_bPauseDraw = pauseDraw;
}

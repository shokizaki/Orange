//===============================================
//
//  内容   ： ユーザーインターフェース処理
//  File   ： StringDraw.h
//  制作者 ： 木崎　翔
//
//===============================================

//------ インクルードファイル ------
#include "stringDraw.h"
#include "bullet.h"
#include "model.h"

//------ マクロ定義 ------
#define STRINGDRAW_TEX_MAX	( 25 )

#define TEX000			"data/TEXTURE/0_frame.png"
#define TEX001			"data/TEXTURE/1_helloRingle.png"
#define TEX002			"data/TEXTURE/2_moveExe.png"
#define TEX003			"data/TEXTURE/3_moveButton.png"
#define TEX004			"data/TEXTURE/4_move.png"
#define TEX005			"data/TEXTURE/5_nextExe.png"
#define TEX006			"data/TEXTURE/6_goTouch.png"
#define TEX007			"data/TEXTURE/7_go.png"
#define TEX008			"data/TEXTURE/8_absorbExe.png"
#define TEX009			"data/TEXTURE/9_attention.png"
#define TEX010			"data/TEXTURE/10_steps.png"
#define TEX011			"data/TEXTURE/11_onpExe.png"
#define TEX012			"data/TEXTURE/12_try.png"
#define TEX013			"data/TEXTURE/13_box.png"
#define TEX014			"data/TEXTURE/14_releaseTry.png"
#define TEX015			"data/TEXTURE/15_attentionSpeed.png"
#define TEX016			"data/TEXTURE/16_last.png"
#define TEX099			"data/TEXTURE/99_6button.png"
#define TEX100			"data/TEXTURE/10ButtonToSkipTutorial.png"

#define TEX003KEY		"data/TEXTURE/3_moveButtonKey.png"
#define TEX099KEY		"data/TEXTURE/99_enterToNext.png"
#define TEX011KEY		"data/TEXTURE/11_onpExeKey.png"
#define TEX013KEY		"data/TEXTURE/13_boxKey.png"
#define TEX100KEY		"data/TEXTURE/p_ToSkip.png"

#define TEX101SIX		"data/TEXTURE/6button.png"
#define TEX101ENTER		"data/TEXTURE/enterKey.png"

#define TEX102TUTO		"data/TEXTURE/tutorial.png"

#define STRING_WIDTH	( 300.0f )
#define STRING_HEIGHT	( 100.0f )
#define STRING_POS_X	( SCREEN_WIDTH / 2 )
#define STRING_POS_Y	( SCREEN_HEIGHT - 120.0f )

#define PRESS_P_WIDTH	( 160.0f )
#define PRESS_P_HEIGHT	( 10.0f )
#define PRESS_P_POS_X	( SCREEN_WIDTH - 240.0f )
#define PRESS_P_POS_Y	( SCREEN_HEIGHT - 230.0f )

#define PRESS_6_WIDTH	( 30.0f )
#define PRESS_6_HEIGHT	( 20.0f )
#define PRESS_6_POS_X	( SCREEN_WIDTH - 170.0f )
#define PRESS_6_POS_Y	( SCREEN_HEIGHT - 60.0f )

#define TUTORIAL_WIDTH	( 120.0f )
#define TUTORIAL_HEIGHT	( 30.0f )
#define TUTORIAL_POS_X	( SCREEN_WIDTH / 2 )
#define TUTORIAL_POS_Y	( 60.0f )


//------ グローバル変数 ------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffStringDraw = NULL;					// 頂点バッファへのポインタ
LPDIRECT3DTEXTURE9		g_pTextureStringDraw[ STRINGDRAW_TEX_MAX ];		// テクスチャへのポインタ

STRINGDRAW g_aStringDraw[ STRINGDRAW_MAX ];

D3DXMATRIX g_mtxProjectionStringDraw;		// プロジェクションマトリックス

float g_fAlpha = 0.8f;
bool g_bMessageSkip = false;
bool g_bFlagEnter = false;
MESSAGE g_message = HELLO;

//===============================================
//
// StringDrawに関する初期化
//
//===============================================
void InitStringDraw( void )
{
	// ローカル変数
	//---------------------------------
	LPDIRECT3DDEVICE9 pDevice = NULL;		// デバイスオブジェクト

	// グローバル変数の初期化
	//---------------------------------
	g_fAlpha = 0.8f;
	g_bMessageSkip = false;
	g_bFlagEnter = false;
	g_message = HELLO;

	// デバイス取得
	//------------------------------------
	pDevice = GetDevice();

	// 頂点バッファの確保
	//------------------------------------
	pDevice ->CreateVertexBuffer((sizeof(VERTEX_2D) * 4 * STRINGDRAW_MAX),		// 頂点データ用に確保するバッファ
								 D3DUSAGE_WRITEONLY,					// 頂点バッファの使用方法
								 FVF_VERTEX_2D,							// 使用する頂点フォーマット
								 D3DPOOL_MANAGED,						// バッファを保持するメモリクラスの指定
								 &g_pVtxBuffStringDraw,							// 頂点バッファへのポインタ
								 NULL);									// NULL固定	

	// テクスチャへのポインタを初期化
	//------------------------------------
	for (int nCnt = 0; nCnt < STRINGDRAW_TEX_MAX; nCnt++)				
	{
		g_pTextureStringDraw[ nCnt ] = NULL;
	}

	// テクスチャの読み込み
	//------------------------------------
	if ( GetGamePadUse() == true )
	{
		D3DXCreateTextureFromFile(pDevice, TEX000, &g_pTextureStringDraw[ 0 ]);
		D3DXCreateTextureFromFile(pDevice, TEX001, &g_pTextureStringDraw[ 1 ]);
		D3DXCreateTextureFromFile(pDevice, TEX002, &g_pTextureStringDraw[ 2 ]);
		D3DXCreateTextureFromFile(pDevice, TEX003, &g_pTextureStringDraw[ 3 ]);
		D3DXCreateTextureFromFile(pDevice, TEX004, &g_pTextureStringDraw[ 4 ]);
		D3DXCreateTextureFromFile(pDevice, TEX005, &g_pTextureStringDraw[ 5 ]);
		D3DXCreateTextureFromFile(pDevice, TEX006, &g_pTextureStringDraw[ 6 ]);
		D3DXCreateTextureFromFile(pDevice, TEX007, &g_pTextureStringDraw[ 7 ]);
		D3DXCreateTextureFromFile(pDevice, TEX008, &g_pTextureStringDraw[ 8 ]);
		D3DXCreateTextureFromFile(pDevice, TEX009, &g_pTextureStringDraw[ 9 ]);
		D3DXCreateTextureFromFile(pDevice, TEX010, &g_pTextureStringDraw[ 10 ]);
		D3DXCreateTextureFromFile(pDevice, TEX011, &g_pTextureStringDraw[ 11 ]);
		D3DXCreateTextureFromFile(pDevice, TEX012, &g_pTextureStringDraw[ 12 ]);
		D3DXCreateTextureFromFile(pDevice, TEX013, &g_pTextureStringDraw[ 13 ]);
		D3DXCreateTextureFromFile(pDevice, TEX014, &g_pTextureStringDraw[ 14 ]);
		D3DXCreateTextureFromFile(pDevice, TEX015, &g_pTextureStringDraw[ 15 ]);
		D3DXCreateTextureFromFile(pDevice, TEX016, &g_pTextureStringDraw[ 16 ]);
		D3DXCreateTextureFromFile(pDevice, TEX100, &g_pTextureStringDraw[ 18 ]);
		D3DXCreateTextureFromFile(pDevice, TEX099, &g_pTextureStringDraw[ 19 ]);
		D3DXCreateTextureFromFile(pDevice, TEX101SIX, &g_pTextureStringDraw[ 20 ]);
		D3DXCreateTextureFromFile(pDevice, TEX102TUTO, &g_pTextureStringDraw[ 21 ]);
	}
	else
	{
		D3DXCreateTextureFromFile(pDevice, TEX000, &g_pTextureStringDraw[ 0 ]);
		D3DXCreateTextureFromFile(pDevice, TEX001, &g_pTextureStringDraw[ 1 ]);
		D3DXCreateTextureFromFile(pDevice, TEX002, &g_pTextureStringDraw[ 2 ]);
		D3DXCreateTextureFromFile(pDevice, TEX003KEY, &g_pTextureStringDraw[ 3 ]);
		D3DXCreateTextureFromFile(pDevice, TEX004, &g_pTextureStringDraw[ 4 ]);
		D3DXCreateTextureFromFile(pDevice, TEX005, &g_pTextureStringDraw[ 5 ]);
		D3DXCreateTextureFromFile(pDevice, TEX006, &g_pTextureStringDraw[ 6 ]);
		D3DXCreateTextureFromFile(pDevice, TEX007, &g_pTextureStringDraw[ 7 ]);
		D3DXCreateTextureFromFile(pDevice, TEX008, &g_pTextureStringDraw[ 8 ]);
		D3DXCreateTextureFromFile(pDevice, TEX009, &g_pTextureStringDraw[ 9 ]);
		D3DXCreateTextureFromFile(pDevice, TEX010, &g_pTextureStringDraw[ 10 ]);
		D3DXCreateTextureFromFile(pDevice, TEX011KEY, &g_pTextureStringDraw[ 11 ]);
		D3DXCreateTextureFromFile(pDevice, TEX012, &g_pTextureStringDraw[ 12 ]);
		D3DXCreateTextureFromFile(pDevice, TEX013KEY, &g_pTextureStringDraw[ 13 ]);
		D3DXCreateTextureFromFile(pDevice, TEX014, &g_pTextureStringDraw[ 14 ]);
		D3DXCreateTextureFromFile(pDevice, TEX015, &g_pTextureStringDraw[ 15 ]);
		D3DXCreateTextureFromFile(pDevice, TEX016, &g_pTextureStringDraw[ 16 ]);
		D3DXCreateTextureFromFile(pDevice, TEX100KEY, &g_pTextureStringDraw[ 18 ]);
		D3DXCreateTextureFromFile(pDevice, TEX099KEY, &g_pTextureStringDraw[ 19 ]);
		D3DXCreateTextureFromFile(pDevice, TEX101ENTER, &g_pTextureStringDraw[ 20 ]);
		D3DXCreateTextureFromFile(pDevice, TEX102TUTO, &g_pTextureStringDraw[ 21 ]);
	}

	// 頂点情報のセット
	//------------------------------------
	SetupStringDraw();
}

//===============================================
//
// StringDrawの頂点情報セット
//
//===============================================
void SetupStringDraw( void )
{
	// ローカル変数
	//---------------------------------
	LPDIRECT3DDEVICE9 pDevice = NULL;		// デバイスオブジェクト
	VERTEX_2D *pVtx2D;

	// デバイス取得
	//------------------------------------
	pDevice = GetDevice();

	g_pVtxBuffStringDraw ->Lock( 0, 0, (void**)&pVtx2D, 0 );

	for ( int i = 0; i < STRINGDRAW_MAX - 1; i++ )
	{
		// 位置
		pVtx2D[ 0 + i * 4 ].pos = D3DXVECTOR3( STRING_POS_X - STRING_WIDTH, STRING_POS_Y - STRING_HEIGHT, 0.0f );
		pVtx2D[ 1 + i * 4 ].pos = D3DXVECTOR3( STRING_POS_X + STRING_WIDTH, STRING_POS_Y - STRING_HEIGHT, 0.0f );
		pVtx2D[ 2 + i * 4 ].pos = D3DXVECTOR3( STRING_POS_X - STRING_WIDTH, STRING_POS_Y + STRING_HEIGHT, 0.0f );
		pVtx2D[ 3 + i * 4 ].pos = D3DXVECTOR3( STRING_POS_X + STRING_WIDTH, STRING_POS_Y + STRING_HEIGHT, 0.0f );

		// パースペクティブコレクト＆色
		for ( int j = 0; j < 4; j++ )
		{
			pVtx2D[ j + i * 4 ].rhw = 1.0f;
			pVtx2D[ j + i * 4 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, g_fAlpha );
		}

		// テクスチャ座標
		pVtx2D[ 0 + i * 4 ].tex = D3DXVECTOR2( 0.0f, 0.0f );
		pVtx2D[ 1 + i * 4 ].tex = D3DXVECTOR2( 1.0f, 0.0f );
		pVtx2D[ 2 + i * 4 ].tex = D3DXVECTOR2( 0.0f, 1.0f );
		pVtx2D[ 3 + i * 4 ].tex = D3DXVECTOR2( 1.0f, 1.0f );
	}

	// 位置
	pVtx2D[ 12 ].pos = D3DXVECTOR3( PRESS_P_POS_X - PRESS_P_WIDTH, PRESS_P_POS_Y - PRESS_P_HEIGHT, 0.0f );
	pVtx2D[ 13 ].pos = D3DXVECTOR3( PRESS_P_POS_X + PRESS_P_WIDTH, PRESS_P_POS_Y - PRESS_P_HEIGHT, 0.0f );
	pVtx2D[ 14 ].pos = D3DXVECTOR3( PRESS_P_POS_X - PRESS_P_WIDTH, PRESS_P_POS_Y + PRESS_P_HEIGHT, 0.0f );
	pVtx2D[ 15 ].pos = D3DXVECTOR3( PRESS_P_POS_X + PRESS_P_WIDTH, PRESS_P_POS_Y + PRESS_P_HEIGHT, 0.0f );

	// パースペクティブコレクト＆色
	for ( int j = 0; j < 4; j++ )
	{
		pVtx2D[ j + 3 * 4 ].rhw = 1.0f;
		pVtx2D[ j + 3 * 4 ].col = D3DXCOLOR( 1.0f, 1.0f, 0.0f, g_fAlpha );
	}

	// テクスチャ座標
	pVtx2D[ 12 ].tex = D3DXVECTOR2( 0.0f, 0.0f );
	pVtx2D[ 13 ].tex = D3DXVECTOR2( 1.0f, 0.0f );
	pVtx2D[ 14 ].tex = D3DXVECTOR2( 0.0f, 1.0f );
	pVtx2D[ 15 ].tex = D3DXVECTOR2( 1.0f, 1.0f );

	// 位置
	pVtx2D[ 16 ].pos = D3DXVECTOR3( PRESS_6_POS_X - PRESS_6_WIDTH, PRESS_6_POS_Y - PRESS_6_HEIGHT, 0.0f );
	pVtx2D[ 17 ].pos = D3DXVECTOR3( PRESS_6_POS_X + PRESS_6_WIDTH, PRESS_6_POS_Y - PRESS_6_HEIGHT, 0.0f );
	pVtx2D[ 18 ].pos = D3DXVECTOR3( PRESS_6_POS_X - PRESS_6_WIDTH, PRESS_6_POS_Y + PRESS_6_HEIGHT, 0.0f );
	pVtx2D[ 19 ].pos = D3DXVECTOR3( PRESS_6_POS_X + PRESS_6_WIDTH, PRESS_6_POS_Y + PRESS_6_HEIGHT, 0.0f );

	// パースペクティブコレクト＆色
	for ( int j = 0; j < 4; j++ )
	{
		pVtx2D[ j + 4 * 4 ].rhw = 1.0f;
		pVtx2D[ j + 4 * 4 ].col = D3DXCOLOR( 0.0f, 0.0f, 0.0f, g_fAlpha );
	}

	// テクスチャ座標
	pVtx2D[ 20 ].tex = D3DXVECTOR2( 0.0f, 0.0f );
	pVtx2D[ 21 ].tex = D3DXVECTOR2( 1.0f, 0.0f );
	pVtx2D[ 22 ].tex = D3DXVECTOR2( 0.0f, 1.0f );
	pVtx2D[ 23 ].tex = D3DXVECTOR2( 1.0f, 1.0f );

	// 位置
	pVtx2D[ 20 ].pos = D3DXVECTOR3( TUTORIAL_POS_X - TUTORIAL_WIDTH, TUTORIAL_POS_Y - TUTORIAL_HEIGHT, 0.0f );
	pVtx2D[ 21 ].pos = D3DXVECTOR3( TUTORIAL_POS_X + TUTORIAL_WIDTH, TUTORIAL_POS_Y - TUTORIAL_HEIGHT, 0.0f );
	pVtx2D[ 22 ].pos = D3DXVECTOR3( TUTORIAL_POS_X - TUTORIAL_WIDTH, TUTORIAL_POS_Y + TUTORIAL_HEIGHT, 0.0f );
	pVtx2D[ 23 ].pos = D3DXVECTOR3( TUTORIAL_POS_X + TUTORIAL_WIDTH, TUTORIAL_POS_Y + TUTORIAL_HEIGHT, 0.0f );

	// パースペクティブコレクト＆色
	for ( int j = 0; j < 4; j++ )
	{
		pVtx2D[ j + 5 * 4 ].rhw = 1.0f;
		pVtx2D[ j + 5 * 4 ].col = D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f );
	}

	// テクスチャ座標
	pVtx2D[ 20 ].tex = D3DXVECTOR2( 0.0f, 0.0f );
	pVtx2D[ 21 ].tex = D3DXVECTOR2( 1.0f, 0.0f );
	pVtx2D[ 22 ].tex = D3DXVECTOR2( 0.0f, 1.0f );
	pVtx2D[ 23 ].tex = D3DXVECTOR2( 1.0f, 1.0f );

	g_pVtxBuffStringDraw ->Unlock();
}

//===============================================
//
// StringDrawに関する終了処理
//
//===============================================
void UninitStringDraw( void )
{
	// 使ったテクスチャの解放と初期化
	//------------------------------------
	for (int nCnt = 0; nCnt < STRINGDRAW_TEX_MAX; nCnt++)
	{
		if (g_pTextureStringDraw[ nCnt ] != NULL)
		{ 
			g_pTextureStringDraw[ nCnt ] ->Release();
			g_pTextureStringDraw[ nCnt ] = NULL;
		}
	}

	// 頂点バッファの解放と初期化
	//------------------------------------
	if (g_pVtxBuffStringDraw != NULL)
	{ 
		g_pVtxBuffStringDraw ->Release();
		g_pVtxBuffStringDraw = NULL;
	}

	g_message = MESSAGE_MAX;
}

//===============================================
//
// StringDrawに関する更新処理
//
//===============================================
void UpdateStringDraw( void )
{
	// 読み進める処理
	if ( ( GetKeyboardTrigger( DIK_RETURN )/* || GetGamePadTrigger( GAMEPAD_R )*/ || GetPadElecomTrigger( PAD_6 ) ) && GetFade() == FADE_NONE )
	{
		PlaySound( DESIDE_SE );
		g_message = ( MESSAGE )( ( int )g_message + 1 );
		if ( g_message == MESSAGE_MAX )
		{
			g_message = ( MESSAGE )( ( int )g_message - 1 );
			SetFade( FADE_OUT );
			SetMode( MODE_GAME );
		}
		if ( g_message == MOVE || g_message == GO || g_message == TRY || g_message == RELEASETRY )
		{
			g_bFlagEnter = true;
		}
		else
		{
			g_bFlagEnter = false;
		}
	}

	// スキップ処理
	if ( ( GetKeyboardTrigger( DIK_P ) /*|| GetGamePadTrigger( GAMEPAD_START )*/ || GetPadElecomTrigger( PAD_10 ) ) && GetFade() == FADE_NONE )
	{
		PlaySound( CANCEL_SE );
		SetFade( FADE_OUT );
		SetMode( MODE_GAME );
	}

	// 頂点情報のセット
	//------------------------------------
	SetupStringDraw();
}

//===============================================
//
// StringDrawに関する描画処理
//
//===============================================
void DrawStringDraw( void )
{
	// ローカル変数
	//------------------------------------
	LPDIRECT3DDEVICE9 pDevice;						// デバイスオブジェクト
	
	// デバイスの取得
	//------------------------------------
	pDevice = GetDevice();

	// 頂点バッファをデータストリームにバインド
	//-----------------------------------------
	pDevice ->SetStreamSource(0,
								g_pVtxBuffStringDraw,
								0,
								sizeof(VERTEX_2D));	// 頂点データの間隔

	
	// 頂点フォーマットの設定
	//------------------------------------
	pDevice ->SetFVF( FVF_VERTEX_2D );

	// テクスチャの設定[ フレーム ]
	//------------------------------------
	pDevice -> SetTexture(0, g_pTextureStringDraw[ 0 ]);

	// ポリゴンの描画
	//------------------------------------
	pDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,	// プリミティブの種類
									0,				// 読み込む最初の番号
									2);				// 描画するプリミティブ数

	// テクスチャの設定[ チュートリアル中 ]
	//------------------------------------
	pDevice -> SetTexture(0, g_pTextureStringDraw[ 21 ]);

	// ポリゴンの描画
	//------------------------------------
	pDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,	// プリミティブの種類
									20,				// 読み込む最初の番号
									2);				// 描画するプリミティブ数

	// テクスチャの設定[ 文章 ]
	//------------------------------------
	pDevice -> SetTexture( 0, g_pTextureStringDraw[ ( int )g_message ] );

	// ポリゴンの描画
	//------------------------------------
	pDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,	// プリミティブの種類
									4,				// 読み込む最初の番号
									2);				// 描画するプリミティブ数

	// テクスチャの設定[ Skip ]
	//------------------------------------
	pDevice -> SetTexture(0, g_pTextureStringDraw[ 18 ]);

	// ポリゴンの描画
	//------------------------------------
	pDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,	// プリミティブの種類
									12,				// 読み込む最初の番号
									2);				// 描画するプリミティブ数

	if ( g_bFlagEnter == true )
	{
		// テクスチャの設定[ Enterで次へ ]
		//------------------------------------
		pDevice -> SetTexture(0, g_pTextureStringDraw[ 19 ]);

		// ポリゴンの描画
		//------------------------------------
		pDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,	// プリミティブの種類
										8,				// 読み込む最初の番号
										2);				// 描画するプリミティブ数
	}
	else
	{
		// テクスチャの設定[ ６ ]
		//------------------------------------
		pDevice -> SetTexture(0, g_pTextureStringDraw[ 20 ]);

		// ポリゴンの描画
		//------------------------------------
		pDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,	// プリミティブの種類
										16,				// 読み込む最初の番号
										2);				// 描画するプリミティブ数
	}
}

//===============================================
//
// [ Enterで次へ ]がでているかどうか
//
//===============================================
bool GetEnterFlag( void )
{
	return g_bFlagEnter;
}

//===============================================
//
// [ Enterで次へ ]の設定
//
//===============================================
void SetEnterFlag( bool bFlag )
{
	g_bFlagEnter = bFlag;
}

//===============================================
//
// 現在のメッセージの番号を取得
//
//===============================================
MESSAGE GetStringMessage( void )
{
	return g_message;
}

//===============================================
//
// メッセージの番号を設定
//
//===============================================
void SetStringMessage( MESSAGE type )
{
	g_message = type;
}
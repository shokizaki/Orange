//===============================================
//
//  内容   ： タイトルロゴ処理
//  File   ： titleLogo.cpp
//  制作者 ： 木崎　翔
//
//===============================================

//------ インクルードファイル ------
#include "titleLogo.h"
#include "title.h"

//------ マクロ定義 ------
#define TITLELOGO_TEX_MAX	( 5 )

#define TITLELOGO_TEX			"data/TEXTURE/titleLogo.png"
#define TITLELOGO_WIDTH			( 200.0f )
#define TITLELOGO_HEIGHT		( 70.0f )
#define TITLELOGO_TARGETPOS_X	( SCREEN_WIDTH / 2 )
#define TITLELOGO_TARGETPOS_Y	( 150.0f )

#define PRESS10_TEX			"data/TEXTURE/press10button.png"
#define PRESSENTER_TEX			"data/TEXTURE/pressEnter.png"
#define PRESSENTER_WIDTH		( 150.0f )
#define PRESSENTER_HEIGHT		( 35.0f )
#define PRESSENTER_POS_X		( SCREEN_WIDTH / 2 )
#define PRESSENTER_POS_Y		( SCREEN_HEIGHT - 150.0f )

//------ グローバル変数 ------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitleLogo = NULL;						// 頂点バッファへのポインタ
LPDIRECT3DTEXTURE9		g_pTextureTitleLogo[ TITLELOGO_TEX_MAX ];		// テクスチャへのポインタ

TITLELOGO g_aTitleLogo[ TITLELOGO_MAX ];
int g_nFrameCountPressEnter;
float g_TitleLogoMove = 0;
int g_nCntBound = 0;

D3DXMATRIX g_mtxProjectionTitleLogo;		// プロジェクションマトリックス

//------ 関数プロトタイプ宣言 ------

//===============================================
//
// タイトルロゴに関する初期化
//
//===============================================
void InitTitleLogo( void )
{
	// ローカル変数
	//---------------------------------
	LPDIRECT3DDEVICE9 pDevice = NULL;		// デバイスオブジェクト

	// グローバル変数の初期化
	//---------------------------------
	g_nFrameCountPressEnter = 60;
	g_TitleLogoMove = 1.0f;
	g_nCntBound = 0;

	// [ ロゴ ]
	g_aTitleLogo[ 0 ].pos = D3DXVECTOR3( SCREEN_WIDTH / 2, -300.0f, 0.0f );
	g_aTitleLogo[ 0 ].fWidth =TITLELOGO_WIDTH;
	g_aTitleLogo[ 0 ].fHeight = TITLELOGO_HEIGHT;
	g_aTitleLogo[ 0 ].color = D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f );

	g_aTitleLogo[ 1 ].pos = D3DXVECTOR3( PRESSENTER_POS_X, PRESSENTER_POS_Y, 0.0f );
	g_aTitleLogo[ 1 ].fWidth =PRESSENTER_WIDTH;
	g_aTitleLogo[ 1 ].fHeight = PRESSENTER_HEIGHT;
	g_aTitleLogo[ 1 ].color = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );

	// デバイス取得
	//------------------------------------
	pDevice = GetDevice();

	// 頂点バッファの確保
	//------------------------------------
	pDevice ->CreateVertexBuffer((sizeof(VERTEX_2D) * 4 * TITLELOGO_MAX),		// 頂点データ用に確保するバッファ
								 D3DUSAGE_WRITEONLY,					// 頂点バッファの使用方法
								 FVF_VERTEX_2D,							// 使用する頂点フォーマット
								 D3DPOOL_MANAGED,						// バッファを保持するメモリクラスの指定
								 &g_pVtxBuffTitleLogo,							// 頂点バッファへのポインタ
								 NULL);									// NULL固定	

	// テクスチャへのポインタを初期化
	//------------------------------------
	for (int nCnt = 0; nCnt < TITLELOGO_TEX_MAX; nCnt++)				
	{
		g_pTextureTitleLogo[ nCnt ] = NULL;
	}

	// テクスチャの読み込み
	//------------------------------------
	D3DXCreateTextureFromFile(pDevice, TITLELOGO_TEX, &g_pTextureTitleLogo[ 0 ]);
	if ( GetGamePadUse() == true )
	{
		D3DXCreateTextureFromFile(pDevice, PRESS10_TEX, &g_pTextureTitleLogo[ 1 ]);
	}
	else
	{
		D3DXCreateTextureFromFile(pDevice, PRESSENTER_TEX, &g_pTextureTitleLogo[ 1 ]);
	}

	// 頂点情報のセット
	//------------------------------------
	SetupTitleLogo();
}

//===============================================
//
// タイトルロゴに関する終了処理
//
//===============================================
void UninitTitleLogo( void )
{
	// 使ったテクスチャの解放と初期化
	//------------------------------------
	for (int nCnt = 0; nCnt < TITLELOGO_TEX_MAX; nCnt++)
	{
		if (g_pTextureTitleLogo[ nCnt ] != NULL)
		{ 
			g_pTextureTitleLogo[ nCnt ] ->Release();
			g_pTextureTitleLogo[ nCnt ] = NULL;
		}
	}

	// 頂点バッファの解放と初期化
	//------------------------------------
	if (g_pVtxBuffTitleLogo != NULL)
	{ 
		g_pVtxBuffTitleLogo ->Release();
		g_pVtxBuffTitleLogo = NULL;
	}

}

//===============================================
//
// タイトルロゴに関する更新処理
//
//===============================================
void UpdateTitleLogo( void )
{
	// ローカル変数
	//------------------------------------
	
	// エンターキーが押されたら
	if ( GetTitleFade() == true )
	{
		// 点滅速度を速める
		g_nFrameCountPressEnter = 10;
		g_nCntBound = 3;
	}

	if ( GetTitleFade() == false && g_nCntBound < 3 )
	{
		// タイトルロゴ移動処理
		g_TitleLogoMove += 1.f;
		g_aTitleLogo[ 0 ].pos.y += g_TitleLogoMove;

		if ( g_aTitleLogo[ 0 ].pos.y >= TITLELOGO_TARGETPOS_Y )
		{
			g_aTitleLogo[ 0 ].pos.y = TITLELOGO_TARGETPOS_Y;
			g_TitleLogoMove = -g_TitleLogoMove + 10.0f;

			g_nCntBound++;
		}
	}
	else if ( g_nCntBound == 3 )
	{
		g_aTitleLogo[ 0 ].pos.y = TITLELOGO_TARGETPOS_Y;
	}

	// 頂点情報のセット
	//------------------------------------
	SetupTitleLogo();
}

//===============================================
//
// タイトルロゴに関する描画処理
//
//===============================================
void DrawTitleLogo( void )
{
	// ローカル変数
	//------------------------------------
	LPDIRECT3DDEVICE9 pDevice;						// デバイスオブジェクト
	static int nFrameCount = 0;
	
	// デバイスの取得
	//------------------------------------
	pDevice = GetDevice();

	// 頂点バッファをデータストリームにバインド
	//-----------------------------------------
	pDevice ->SetStreamSource(0,
								g_pVtxBuffTitleLogo,
								0,
								sizeof(VERTEX_2D));	// 頂点データの間隔

	
	// 頂点フォーマットの設定
	//------------------------------------
	pDevice ->SetFVF( FVF_VERTEX_2D );

	// [ titleLogo ]描画設定

	// テクスチャの設定
	//----------------------- -------------
	pDevice -> SetTexture(0, g_pTextureTitleLogo[ 0 ]);

	// ポリゴンの描画
	//------------------------------------
	pDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,	// プリミティブの種類
									0,				// 読み込む最初の番号
									2);				// 描画するプリミティブ数


	// [ Press Enter ]点滅描画
	if ( ( nFrameCount = ++nFrameCount % g_nFrameCountPressEnter ) <= g_nFrameCountPressEnter / 2 )
	{
		// テクスチャの設定
		//----------------------- -------------
		pDevice -> SetTexture(0, g_pTextureTitleLogo[ 1 ]);

		// ポリゴンの描画
		//------------------------------------
		pDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,	// プリミティブの種類
										4,				// 読み込む最初の番号
										2);				// 描画するプリミティブ数
	}
}

//===============================================
//
// タイトルロゴの頂点情報セット
//
//===============================================
void SetupTitleLogo( void )
{
		// ローカル変数
	//---------------------------------
	LPDIRECT3DDEVICE9 pDevice = NULL;		// デバイスオブジェクト
	VERTEX_2D *pVtx2D;

	// デバイス取得
	//------------------------------------
	pDevice = GetDevice();

	g_pVtxBuffTitleLogo ->Lock( 0, 0, (void**)&pVtx2D, 0 );

	for ( int i = 0; i < TITLELOGO_MAX; i++ )
	{
		// 位置
		pVtx2D[ 0 + i * 4 ].pos = D3DXVECTOR3( g_aTitleLogo[ i ].pos.x - g_aTitleLogo[ i ].fWidth, g_aTitleLogo[ i ].pos.y - g_aTitleLogo[ i ].fHeight, 0.0f );
		pVtx2D[ 1 + i * 4 ].pos = D3DXVECTOR3( g_aTitleLogo[ i ].pos.x + g_aTitleLogo[ i ].fWidth, g_aTitleLogo[ i ].pos.y - g_aTitleLogo[ i ].fHeight, 0.0f );
		pVtx2D[ 2 + i * 4 ].pos = D3DXVECTOR3( g_aTitleLogo[ i ].pos.x - g_aTitleLogo[ i ].fWidth, g_aTitleLogo[ i ].pos.y + g_aTitleLogo[ i ].fHeight, 0.0f );
		pVtx2D[ 3 + i * 4 ].pos = D3DXVECTOR3( g_aTitleLogo[ i ].pos.x + g_aTitleLogo[ i ].fWidth, g_aTitleLogo[ i ].pos.y + g_aTitleLogo[ i ].fHeight, 0.0f );

		// パースペクティブコレクト＆色
		for ( int j = 0; j < 4; j++ )
		{
			pVtx2D[ j + i * 4 ].rhw = 1.0f;
			pVtx2D[ j + i * 4 ].col = g_aTitleLogo[ i ].color;
		}

		// テクスチャ座標
		pVtx2D[ 0 + i * 4  ].tex = D3DXVECTOR2( 0.0f, 0.0f );
		pVtx2D[ 1 + i * 4  ].tex = D3DXVECTOR2( 1.0f, 0.0f );
		pVtx2D[ 2 + i * 4  ].tex = D3DXVECTOR2( 0.0f, 1.0f );
		pVtx2D[ 3 + i * 4  ].tex = D3DXVECTOR2( 1.0f, 1.0f );
	}

	g_pVtxBuffTitleLogo ->Unlock();
}


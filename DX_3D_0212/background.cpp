//===============================================
//
//  内容   ： タイトルロゴ処理
//  File   ： titleLogo.cpp
//  制作者 ： 木崎　翔
//
//===============================================

//------ インクルードファイル ------
#include "background.h"
#include "title.h"
#include "billboard.h"
#include "camera.h"

//------ マクロ定義 ------
#define TITLELOGO_TEX_MAX	( 5 )

//------ グローバル変数 ------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBackground = NULL;						// 頂点バッファへのポインタ
LPDIRECT3DTEXTURE9		g_pTextureBackground[ TITLELOGO_TEX_MAX ];		// テクスチャへのポインタ

//------ 関数プロトタイプ宣言 ------

//===============================================
//
// タイトルロゴに関する初期化
//
//===============================================
void InitBackground( void )
{
	// ローカル変数
	//---------------------------------
	LPDIRECT3DDEVICE9 pDevice = NULL;		// デバイスオブジェクト
	CAMERA *pCamera = GetCamera();

	// デバイス取得
	//------------------------------------
	pDevice = GetDevice();

	// 頂点バッファの確保
	//------------------------------------
	pDevice ->CreateVertexBuffer((sizeof(VERTEX_2D) * 4),		// 頂点データ用に確保するバッファ
								 D3DUSAGE_WRITEONLY,					// 頂点バッファの使用方法
								 FVF_VERTEX_2D,							// 使用する頂点フォーマット
								 D3DPOOL_MANAGED,						// バッファを保持するメモリクラスの指定
								 &g_pVtxBuffBackground,							// 頂点バッファへのポインタ
								 NULL);									// NULL固定	

	// テクスチャへのポインタを初期化
	//------------------------------------
	for (int nCnt = 0; nCnt < TITLELOGO_TEX_MAX; nCnt++)				
	{
		g_pTextureBackground[ nCnt ] = NULL;
	}

	// テクスチャの読み込み
	//------------------------------------
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ringleDash_backTitle.jpg", &g_pTextureBackground[ 0 ]);

	SetBillboard( D3DXVECTOR3( pCamera ->posCameraP.x, pCamera ->posCameraP.y - 100.0f, 40.0f ), TEXTUREANIM_TITLE, 0.0f, SCREEN_HEIGHT / 2.5f, SCREEN_WIDTH / 2.5f );

	// 頂点情報のセット
	//------------------------------------
	SetupBackground();
}

//===============================================
//
// タイトルロゴに関する終了処理
//
//===============================================
void UninitBackground( void )
{
	// 使ったテクスチャの解放と初期化
	//------------------------------------
	for (int nCnt = 0; nCnt < TITLELOGO_TEX_MAX; nCnt++)
	{
		if (g_pTextureBackground[ nCnt ] != NULL)
		{ 
			g_pTextureBackground[ nCnt ] ->Release();
			g_pTextureBackground[ nCnt ] = NULL;
		}
	}

	// 頂点バッファの解放と初期化
	//------------------------------------
	if (g_pVtxBuffBackground != NULL)
	{ 
		g_pVtxBuffBackground ->Release();
		g_pVtxBuffBackground = NULL;
	}

}

//===============================================
//
// タイトルロゴに関する更新処理
//
//===============================================
void UpdateBackground( void )
{

}

//===============================================
//
// タイトルロゴに関する描画処理
//
//===============================================
void DrawBackground( void )
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
								g_pVtxBuffBackground,
								0,
								sizeof(VERTEX_2D));	// 頂点データの間隔

	
	// 頂点フォーマットの設定
	//------------------------------------
	pDevice ->SetFVF( FVF_VERTEX_2D );

	// [ titleLogo ]描画設定

	// テクスチャの設定
	//----------------------- -------------
	pDevice -> SetTexture(0, g_pTextureBackground[ 0 ]);

	// ポリゴンの描画
	//------------------------------------
	pDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,	// プリミティブの種類
									0,				// 読み込む最初の番号
									2);				// 描画するプリミティブ数
}

//===============================================
//
// タイトルロゴの頂点情報セット
//
//===============================================
void SetupBackground( void )
{
		// ローカル変数
	//---------------------------------
	LPDIRECT3DDEVICE9 pDevice = NULL;		// デバイスオブジェクト
	VERTEX_2D *pVtx2D;

	// デバイス取得
	//------------------------------------
	pDevice = GetDevice();

	g_pVtxBuffBackground ->Lock( 0, 0, (void**)&pVtx2D, 0 );

	// 位置
	pVtx2D[ 0 ].pos = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	pVtx2D[ 1 ].pos = D3DXVECTOR3( SCREEN_WIDTH, 0.0f, 0.0f );
	pVtx2D[ 2 ].pos = D3DXVECTOR3( 0.0f, SCREEN_HEIGHT, 0.0f );
	pVtx2D[ 3 ].pos = D3DXVECTOR3( SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f );

	// パースペクティブコレクト＆色
	for ( int j = 0; j < 4; j++ )
	{
		pVtx2D[ j ].rhw = 1.0f;
		pVtx2D[ j ].col = D3DXCOLOR( 1.0, 1.0f, 1.0f, 1.0f );
	}

	// テクスチャ座標
	pVtx2D[ 0 ].tex = D3DXVECTOR2( 0.0f, 0.0f );
	pVtx2D[ 1 ].tex = D3DXVECTOR2( 1.0f, 0.0f );
	pVtx2D[ 2 ].tex = D3DXVECTOR2( 0.0f, 1.0f );
	pVtx2D[ 3 ].tex = D3DXVECTOR2( 1.0f, 1.0f );

	g_pVtxBuffBackground ->Unlock();
}


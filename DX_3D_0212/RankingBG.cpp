//-----------------------------------------------
//  内容：ランキング背景
//  Fail:RankingBG.cpp
//  制作者：安井　あゆみ
//-----------------------------------------------

//-----------------------------------------------
//  インクルードファイル
//-----------------------------------------------
#include "RankingBG.h"

//-----------------------------------------------
//　マクロ定義
//-----------------------------------------------

//-----------------------------------------------
//　構造体定義
//-----------------------------------------------

typedef struct tagRESULT
{
    LPDIRECT3DVERTEXBUFFER9 vtx;    // 頂点バッファ
    LPDIRECT3DTEXTURE9      tex;    // テクスチャバッファ
}RESULT;

//-----------------------------------------------
//  グローバル変数
//-----------------------------------------------
RESULT g_RankingBG;

//-----------------------------------------------
// 背景初期化処理
//-----------------------------------------------
void InitRankingBG()
{
	LPDIRECT3DDEVICE9 pDevice;		// デバイスオブジェクト

	// デバイス取得
	//------------------------------------
	pDevice = GetDevice();

	pDevice ->CreateVertexBuffer(( sizeof( VERTEX_2D ) * 4 ),	// 頂点データ用に確保するバッファ
								 D3DUSAGE_WRITEONLY,				// 頂点バッファの使用方法
								 FVF_VERTEX_2D,						// 使用する頂点フォーマット
								 D3DPOOL_MANAGED,					// バッファを保持するメモリクラスの指定
								 &g_RankingBG.vtx,					// 頂点バッファへのポインタ
								 NULL);								// NULL固定
		// データの位置を確保する（ロック）
	//------------------------------------
	VERTEX_2D *pVtx;
	g_RankingBG.vtx ->Lock( 0, 0, (void**)&pVtx, 0 );

    pVtx[ 0 ].pos.x = 0.0f;
	pVtx[ 0 ].pos.y = 0.0f;	// 頂点の位置
	pVtx[ 0 ].pos.z = 1.0f;												// 頂点の位置

	pVtx[ 1 ].pos.x = SCREEN_WIDTH;
	pVtx[ 1 ].pos.y = 0.0f;	// 頂点の位置
	pVtx[ 1 ].pos.z = 1.0f;												// 頂点の位置

	pVtx[ 2 ].pos.x = 0.0f;
	pVtx[ 2 ].pos.y = SCREEN_HEIGHT;	// 頂点の位置
	pVtx[ 2 ].pos.z = 1.0f;												// 頂点の位置

	pVtx[ 3 ].pos.x = SCREEN_WIDTH;
	pVtx[ 3 ].pos.y = SCREEN_HEIGHT;	// 頂点の位置
	pVtx[ 3 ].pos.z = 1.0f;												// 頂点の位置

	// 法線
	pVtx[ 0 ].rhw = 1.0f;
	pVtx[ 1 ].rhw = 1.0f;
	pVtx[ 2 ].rhw = 1.0f;
	pVtx[ 3 ].rhw = 1.0f;			

	// フェードの色
	//------------------------------------
	pVtx[ 0 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );			// 頂点ごとの色の設定
	pVtx[ 1 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );			//			↓
	pVtx[ 2 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );			//			↓
	pVtx[ 3 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );			//			↓

	pVtx[ 0 ].tex = D3DXVECTOR2( 0.0f, 0.0f );
	pVtx[ 1 ].tex = D3DXVECTOR2( 1.0f, 0.0f );
	pVtx[ 2 ].tex = D3DXVECTOR2( 0.0f, 1.0f );
	pVtx[ 3 ].tex = D3DXVECTOR2( 1.0f, 1.0f );


	// 解放
	//------------------------------------
	g_RankingBG.vtx -> Unlock();

	// テクスチャの読み込み
	//------------------------------------
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Ranking.jpg", &g_RankingBG.tex);
}

//-----------------------------------------------
// 背景終了処理
//-----------------------------------------------
void UninitRankingBG()
{
	if (g_RankingBG.tex != NULL)
	{ // 使ったテクスチャの解放と初期化
		//------------------------------------
		g_RankingBG.tex -> Release();
		g_RankingBG.tex = NULL;
	}

	if (g_RankingBG.vtx != NULL)
	{ // 頂点バッファの解放と初期化
	  //------------------------------------
		g_RankingBG.vtx->Release();
		g_RankingBG.vtx = NULL;
	}

    // BGM
	StopSound();
}

//-----------------------------------------------
// リザルト更新処理
//-----------------------------------------------
void UpdateRankingBG()
{
}

//-----------------------------------------------
// 背景描画処理
//-----------------------------------------------
void DrawRankingBG()
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
								g_RankingBG.vtx,
								0,
								sizeof(VERTEX_2D));	// 頂点データの間隔

	// 頂点フォーマットの設定
	//------------------------------------
	pDevice -> SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	//------------------------------------
	pDevice -> SetTexture( 0, g_RankingBG.tex );

	// ポリゴンの描画
	//------------------------------------
	pDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,	// プリミティブの種類
									0,				// 読み込む最初の番号
									2);				// 描画するプリミティブ数
}


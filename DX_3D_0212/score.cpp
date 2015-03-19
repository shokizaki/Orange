//-----------------------------------------------
//  内容：スコア処理
//  File : score.cpp
//  制作者：木崎　翔
//-----------------------------------------------

//-----------------------------------------------
//  インクルードファイル
//-----------------------------------------------
#include "score.h"

//-----------------------------------------------
//　マクロ定義
//-----------------------------------------------

//-----------------------------------------------
//  関数プロトタイプ宣言
//-----------------------------------------------

//-----------------------------------------------
//　マクロ定義
//-----------------------------------------------
#define SCORE_WIDTH		( 20.0f )
#define SCORE_HEIGHT	( 20.0f )
#define SCORE_DIGIT		( 6 )
#define SCORE_POS_X		( SCREEN_WIDTH - ( ( SCORE_DIGIT * 2 - 1 ) * SCORE_WIDTH ) )
#define SCORE_POS_Y		( 80.0f )

#define SCOREPOLI_WIDTH		( 80.0f )
#define SCOREPOLI_HEIGHT	( 20.0f )
#define SCOREPOLI_POS_X		( SCREEN_WIDTH - 120.0f )
#define SCOREPOLI_POS_Y		( 40.0f )

#define SCORE_PASS			"data/TEXTURE/score.png"

//-----------------------------------------------
//  グローバル変数
//-----------------------------------------------
NUMBER g_score;
int g_nScore = 0;

LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL;			// 頂点バッファへのポインタ
LPDIRECT3DTEXTURE9		g_pTextureScore = NULL;			// テクスチャへのポインタ

//-----------------------------------------------
// 背景初期化処理
//-----------------------------------------------
void InitScore()
{
	// ローカル変数
	//---------------------------------
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// デバイスオブジェクト
	VERTEX_2D *pVtx2D;

	// グローバル変数初期化
	g_nScore = 0;
	g_pTextureScore = NULL;

	// 頂点バッファの確保
	//------------------------------------
	pDevice ->CreateVertexBuffer(( sizeof(VERTEX_2D) * 4 ),		// 頂点データ用に確保するバッファ
								 D3DUSAGE_WRITEONLY,					// 頂点バッファの使用方法
								 FVF_VERTEX_2D,							// 使用する頂点フォーマット
								 D3DPOOL_MANAGED,						// バッファを保持するメモリクラスの指定
								 &g_pVtxBuffScore,							// 頂点バッファへのポインタ
								 NULL);									// NULL固定	

	g_pVtxBuffScore ->Lock( 0, 0, ( void** )&pVtx2D, 0 );

	pVtx2D[ 0 ].pos = D3DXVECTOR3( SCOREPOLI_POS_X - SCOREPOLI_WIDTH, SCOREPOLI_POS_Y - SCOREPOLI_HEIGHT, 0.0f );
	pVtx2D[ 1 ].pos = D3DXVECTOR3( SCOREPOLI_POS_X + SCOREPOLI_WIDTH, SCOREPOLI_POS_Y - SCOREPOLI_HEIGHT, 0.0f );
	pVtx2D[ 2 ].pos = D3DXVECTOR3( SCOREPOLI_POS_X - SCOREPOLI_WIDTH, SCOREPOLI_POS_Y + SCOREPOLI_HEIGHT, 0.0f );
	pVtx2D[ 3 ].pos = D3DXVECTOR3( SCOREPOLI_POS_X + SCOREPOLI_WIDTH, SCOREPOLI_POS_Y + SCOREPOLI_HEIGHT, 0.0f );

	pVtx2D[ 0 ].rhw = 1.0f;
	pVtx2D[ 1 ].rhw = 1.0f;
	pVtx2D[ 2 ].rhw = 1.0f;
	pVtx2D[ 3 ].rhw = 1.0f;

	pVtx2D[ 0 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );
	pVtx2D[ 1 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );
	pVtx2D[ 2 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );
	pVtx2D[ 3 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );

	pVtx2D[ 0 ].tex = D3DXVECTOR2( 0.0f, 0.0f );
	pVtx2D[ 1 ].tex = D3DXVECTOR2( 1.0f, 0.0f );
	pVtx2D[ 2 ].tex = D3DXVECTOR2( 0.0f, 1.0f );
	pVtx2D[ 3 ].tex = D3DXVECTOR2( 1.0f, 1.0f );

	g_pVtxBuffScore ->Unlock();

	// テクスチャの読み込み
	//------------------------------------
	D3DXCreateTextureFromFile(pDevice, SCORE_PASS, &g_pTextureScore);

	// スコアの初期化
	SetNumber( &g_score, D3DXVECTOR3( SCORE_POS_X, SCORE_POS_Y, 0.0f ), SCORE_DIGIT, SCORE_WIDTH, SCORE_HEIGHT );
}

//-----------------------------------------------
// 背景終了処理
//-----------------------------------------------
void UninitScore()
{
	// 終了処理
	UninitNumber( &g_score );

	// 使ったテクスチャの解放と初期化
	//------------------------------------
	if (g_pTextureScore != NULL)
	{ 
		g_pTextureScore ->Release();
		g_pTextureScore = NULL;
	}


	// 頂点バッファの解放と初期化
	//------------------------------------
	if (g_pVtxBuffScore != NULL)
	{ 
		g_pVtxBuffScore ->Release();
		g_pVtxBuffScore = NULL;
	}
}

//-----------------------------------------------
// 背景更新処理
//-----------------------------------------------
void UpdateScore()
{
	
}

//-----------------------------------------------
// 背景描画処理
//-----------------------------------------------
void DrawScore()
{
	DrawNumber( &g_score, g_nScore );

	DrawScorePoligon();
}

//-----------------------------------------------
// スコア設定
//-----------------------------------------------
void AddScore( int nScore )
{
	g_nScore += nScore;
}

//-----------------------------------------------
// スコア取得
//-----------------------------------------------
int GetScore( void )
{
	return g_nScore;
}

//-----------------------------------------------
// [ スコア ]ポリゴンの描画
//-----------------------------------------------
void DrawScorePoligon( void )
{
	// ローカル変数
	//------------------------------------
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスオブジェクト
	
	// 頂点バッファをデータストリームにバインド
	//-----------------------------------------
	pDevice ->SetStreamSource(0,
								g_pVtxBuffScore,
								0,
								sizeof(VERTEX_2D));	// 頂点データの間隔

	
	// 頂点フォーマットの設定
	//------------------------------------
	pDevice ->SetFVF( FVF_VERTEX_2D );

	// テクスチャの設定
	//------------------------------------
	pDevice -> SetTexture(0, g_pTextureScore);
			
	// ポリゴンの描画
	//------------------------------------
	pDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,	// プリミティブの種類
									0,				// 読み込む最初の番号
									2);				// 描画するプリミティブ数

}

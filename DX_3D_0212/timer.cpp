//===============================================
//
//  内容   ： タイマー処理
//  File   ： timer.h
//  制作者 ： 木崎　翔
//
//===============================================

//------ インクルードファイル ------
#include "timer.h"
#include "number.h"

//------ マクロ定義 ------
#define TIMER			( 45 )
#define TIMER_WIDTH		( 15.0f )
#define TIMER_HEIGHT	( 30.0f )
#define TIMER_DIG		( 3 )
#define TIMER_POS_X		( SCREEN_WIDTH / 2 - 25.0f )
#define TIMER_POS_Y		( 90.0f )

#define TIMERPOLI_WIDTH		( 50.0f )
#define TIMERPOLI_HEIGHT	( 30.0f )
#define TIMERPOLI_POS_X		( TIMER_POS_X + 35.0f )
#define TIMERPOLI_POS_Y		( TIMER_POS_Y - 60.0f )

//------ グローバル変数 ------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTimer = NULL;						// 頂点バッファへのポインタ
LPDIRECT3DTEXTURE9		g_pTextureTimer;		// テクスチャへのポインタ
int g_nTimer = TIMER;
NUMBER g_timer;

//===============================================
//
// タイマーに関する初期化
//
//===============================================
void InitTimer( void )
{
	g_nTimer = TIMER;

	SetNumber( &g_timer, D3DXVECTOR3( TIMER_POS_X, TIMER_POS_Y, 0.0f ), TIMER_DIG, TIMER_WIDTH, TIMER_HEIGHT );

	// ローカル変数
	//---------------------------------
	LPDIRECT3DDEVICE9 pDevice = NULL;		// デバイスオブジェクト
	VERTEX_2D *pVtx2D;

	// グローバル変数の初期化
	//---------------------------------
	
	// デバイス取得
	//------------------------------------
	pDevice = GetDevice();

	// 頂点バッファの確保
	//------------------------------------
	pDevice ->CreateVertexBuffer((sizeof(VERTEX_2D) * 4 ),		// 頂点データ用に確保するバッファ
								 D3DUSAGE_WRITEONLY,			// 頂点バッファの使用方法
								 FVF_VERTEX_2D,					// 使用する頂点フォーマット
								 D3DPOOL_MANAGED,				// バッファを保持するメモリクラスの指定
								 &g_pVtxBuffTimer,				// 頂点バッファへのポインタ
								 NULL);							// NULL固定	

	g_pVtxBuffTimer ->Lock( 0, 0, ( void** )&pVtx2D, 0 );

	pVtx2D[ 0 ].pos = D3DXVECTOR3( TIMERPOLI_POS_X - TIMERPOLI_WIDTH, TIMERPOLI_POS_Y - TIMERPOLI_HEIGHT, 0.0f );
	pVtx2D[ 1 ].pos = D3DXVECTOR3( TIMERPOLI_POS_X + TIMERPOLI_WIDTH, TIMERPOLI_POS_Y - TIMERPOLI_HEIGHT, 0.0f );
	pVtx2D[ 2 ].pos = D3DXVECTOR3( TIMERPOLI_POS_X - TIMERPOLI_WIDTH, TIMERPOLI_POS_Y + TIMERPOLI_HEIGHT, 0.0f );
	pVtx2D[ 3 ].pos = D3DXVECTOR3( TIMERPOLI_POS_X + TIMERPOLI_WIDTH, TIMERPOLI_POS_Y + TIMERPOLI_HEIGHT, 0.0f );

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

	g_pVtxBuffTimer ->Unlock();

	// テクスチャの読み込み
	//------------------------------------
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/timer.png", &g_pTextureTimer);
}

//===============================================
//
// タイマーに関する終了処理
//
//===============================================
void UninitTimer(void)
{
	UninitNumber( &g_timer );
}

//===============================================
//
// タイマーに関する更新処理
//
//===============================================
void UpdateTimer(void)
{
	// ローカル変数
	//------------------------------------
	static int nFrameCount = 0;

	// タイマー更新
	if ( ++nFrameCount % 60 == 0 )
	{
		g_nTimer -= 1;
		if ( g_nTimer <= 0 && GetFade() == FADE_NONE )
		{
			SetFade( FADE_OUT );
			SetMode( MODE_RANKING );
		}
	}
}

//===============================================
//
// タイマーに関する描画処理
//
//===============================================
void DrawTimer(void)
{
	// デバイスの取得
	//------------------------------------
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデータストリームにバインド
	//-----------------------------------------
	pDevice ->SetStreamSource(0,
								g_pVtxBuffTimer,
								0,
								sizeof(VERTEX_2D));	// 頂点データの間隔

	
	// 頂点フォーマットの設定
	//------------------------------------
	pDevice ->SetFVF( FVF_VERTEX_2D );

	// テクスチャの設定[ Timer ]
	//------------------------------------
	pDevice -> SetTexture(0, g_pTextureTimer);

	// ポリゴンの描画[ Timer ]
	//------------------------------------
	pDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,	// プリミティブの種類
									0,				// 読み込む最初の番号
									2);				// 描画するプリミティブ

	// タイマー描画
	DrawNumber( &g_timer, g_nTimer );

}

//===============================================
//
// タイマーの頂点情報セット
//
//===============================================
void SetupTimer(void)
{
	// ローカル変数
	//------------------------------------
	VERTEX_2D *pVtx2D;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	float fTimerDevide = 0.0f;
	int nDevide = 100;

	g_pVtxBuffTimer ->Lock( 0, 0, (void**)&pVtx2D, 0 );

	for ( int i = 0; i < TIMER_MAX; i++ )
	{
		fTimerDevide = ( g_nTimer / nDevide ) * 0.1f;

		pVtx2D[ 0 + i * 4 ].pos = D3DXVECTOR3( TIMER_POS_X + ( TIMER_WIDTH * 2 * (i + 1) ) - TIMER_WIDTH, TIMER_POS_Y - TIMER_HEIGHT, 0.0f );
		pVtx2D[ 1 + i * 4 ].pos = D3DXVECTOR3( TIMER_POS_X + ( TIMER_WIDTH * 2 * (i + 1) ) + TIMER_WIDTH, TIMER_POS_Y - TIMER_HEIGHT, 0.0f );
		pVtx2D[ 2 + i * 4 ].pos = D3DXVECTOR3( TIMER_POS_X + ( TIMER_WIDTH * 2 * (i + 1) ) - TIMER_WIDTH, TIMER_POS_Y + TIMER_HEIGHT, 0.0f );
		pVtx2D[ 3 + i * 4 ].pos = D3DXVECTOR3( TIMER_POS_X + ( TIMER_WIDTH * 2 * (i + 1) ) + TIMER_WIDTH, TIMER_POS_Y + TIMER_HEIGHT, 0.0f );

		pVtx2D[ 0 + i * 4 ].tex = D3DXVECTOR2( 0.0f + fTimerDevide, 0.0f );
		pVtx2D[ 1 + i * 4 ].tex = D3DXVECTOR2( 0.1f + fTimerDevide, 0.0f );
		pVtx2D[ 2 + i * 4 ].tex = D3DXVECTOR2( 0.0f + fTimerDevide, 1.0f );
		pVtx2D[ 3 + i * 4 ].tex = D3DXVECTOR2( 0.1f + fTimerDevide, 1.0f );

		nDevide /= 10;
	}

	g_pVtxBuffTimer ->Unlock();
}

//===============================================
//
// タイマーの頂点情報セット
//
//===============================================
int GetTimer( void )
{
	return g_nTimer;
}

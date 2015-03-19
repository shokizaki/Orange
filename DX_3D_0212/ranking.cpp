//-----------------------------------------------
//  内容：ランキング処理
//  File : ranking.cpp
//  制作者：木崎　翔
//-----------------------------------------------

//-----------------------------------------------
//  インクルードファイル
//-----------------------------------------------
#include "ranking.h"
#include "score.h"

//------ マクロ定義 ------
#define RANK_TEX_MAX	( 5 )

#define RANKING_PASS	"ranking.bin"
#define RANK_TEX_PASS	"data/TEXTURE/ranking.png"
#define RANK_TEX_1ST	"data/TEXTURE/1st.png"
#define RANK_TEX_2ND	"data/TEXTURE/2nd.png"
#define RANK_TEX_3RD	"data/TEXTURE/3rd.png"
#define RANKBACK_TEX	"data/TEXTURE/ringleDash_backTitle.jpg"
#define RANKPOS_X		( SCREEN_WIDTH / 2 - 125.0f )
#define RANKPOS_Y		( 190.0f )
#define RANK_WIDTH		( 30.0f )
#define RANK_HEIGHT		( 40.0f )
#define RANK_DIG		( 6 )

#define RANKPOLI_X		( SCREEN_WIDTH / 2 )
#define RANKPOLI_Y		( 70.0f )
#define RANKPOLI_WIDTH	( 200.0f )
#define RANKPOLI_HEIGHT	( 60.0f )

#define RANK1ST_POS_X	( 130.0f )
#define RANK1ST_POS_Y	( 190.0f )
#define RANK1ST_WIDTH	( 60.0f )
#define RANK1ST_HEIGHT	( 50.0f )

#define RANK2ND_POS_X	( 130.0f )
#define RANK2ND_POS_Y	( 350.0f )
#define RANK2ND_WIDTH	( 60.0f )
#define RANK2ND_HEIGHT	( 50.0f )

#define RANK3RD_POS_X	( 130.0f )
#define RANK3RD_POS_Y	( 510.0f )
#define RANK3RD_WIDTH	( 60.0f )
#define RANK3RD_HEIGHT	( 50.0f )

//------ グローバル変数 ------
RANKING ranking;
NUMBER rankingNumber[ RANK_MAX ];
bool g_bJudge = false;
int g_nIndex = 0;
int g_nRankingFrameCount = 0;

LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRanking = NULL;			// 頂点バッファへのポインタ
LPDIRECT3DTEXTURE9		g_pTextureRanking[ RANK_TEX_MAX ];			// テクスチャへのポインタ

// ランキング初期化
void InitRanking( void )
{
	// ローカル変数
	//---------------------------------
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// デバイスオブジェクト
	VERTEX_2D *pVtx2D;

	// グローバル変数初期化
	g_bJudge = false;
	g_nIndex = 0;
	g_nRankingFrameCount = 0;

	// ロード
	LoadRanking();

	//ranking.nScore[ 0 ] = 1000;
	//ranking.nScore[ 1 ] = 200;
	//ranking.nScore[ 2 ] = 50;

	//// ローカル変数
	////------------------------------------
	//FILE *fp;		// ファイルポインタ

	//// ファイルのオープン
	//fp = fopen("data/SCRIPT/ranking.bin", "wb");
	//if (fp == NULL)
	//{
	//	MessageBox( NULL, "ランキングテキストのオープンに失敗", "エラー", MB_OK );
	//	return;
	//}

	//// セーブ処理
	//fwrite( &ranking, sizeof( RANKING ), RANK_MAX, fp );

	//// ファイルのクローズ
	//fclose( fp );

	//rankingNumber[ 0 ].nNumber = ranking.nScore[ 0 ];
	//rankingNumber[ 1 ].nNumber = ranking.nScore[ 1 ];
	//rankingNumber[ 2 ].nNumber = ranking.nScore[ 2 ];

	// 頂点バッファの確保
	//------------------------------------
	pDevice ->CreateVertexBuffer(( sizeof(VERTEX_2D) * 4 * RANK_TEX_MAX ),				// 頂点データ用に確保するバッファ
								 D3DUSAGE_WRITEONLY,					// 頂点バッファの使用方法
								 FVF_VERTEX_2D,							// 使用する頂点フォーマット
								 D3DPOOL_MANAGED,						// バッファを保持するメモリクラスの指定
								 &g_pVtxBuffRanking,					// 頂点バッファへのポインタ
								 NULL);									// NULL固定	

	// ロック
	g_pVtxBuffRanking ->Lock( 0, 0, ( void** )&pVtx2D, 0 );

	// [ ランキング ]
	pVtx2D[ 0 ].pos = D3DXVECTOR3( RANKPOLI_X - RANKPOLI_WIDTH, RANKPOLI_Y - RANKPOLI_HEIGHT, 0.0f );
	pVtx2D[ 1 ].pos = D3DXVECTOR3( RANKPOLI_X + RANKPOLI_WIDTH, RANKPOLI_Y - RANKPOLI_HEIGHT, 0.0f );
	pVtx2D[ 2 ].pos = D3DXVECTOR3( RANKPOLI_X - RANKPOLI_WIDTH, RANKPOLI_Y + RANKPOLI_HEIGHT, 0.0f );
	pVtx2D[ 3 ].pos = D3DXVECTOR3( RANKPOLI_X + RANKPOLI_WIDTH, RANKPOLI_Y + RANKPOLI_HEIGHT, 0.0f );

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

	// [ 背景 ]
	pVtx2D[ 4 ].pos = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	pVtx2D[ 5 ].pos = D3DXVECTOR3( SCREEN_WIDTH, 0.0f, 0.0f );
	pVtx2D[ 6 ].pos = D3DXVECTOR3( 0.0f, SCREEN_HEIGHT, 0.0f );
	pVtx2D[ 7 ].pos = D3DXVECTOR3( SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f );

	pVtx2D[ 4 ].rhw = 1.0f;
	pVtx2D[ 5 ].rhw = 1.0f;
	pVtx2D[ 6 ].rhw = 1.0f;
	pVtx2D[ 7 ].rhw = 1.0f;

	pVtx2D[ 4 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );
	pVtx2D[ 5 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );
	pVtx2D[ 6 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );
	pVtx2D[ 7 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );

	pVtx2D[ 4 ].tex = D3DXVECTOR2( 0.0f, 0.0f );
	pVtx2D[ 5 ].tex = D3DXVECTOR2( 1.0f, 0.0f );
	pVtx2D[ 6 ].tex = D3DXVECTOR2( 0.0f, 1.0f );
	pVtx2D[ 7 ].tex = D3DXVECTOR2( 1.0f, 1.0f );

	// [ 1st ]
	pVtx2D[ 8 ].pos = D3DXVECTOR3( RANK1ST_POS_X - RANK1ST_WIDTH, RANK1ST_POS_Y - RANK1ST_HEIGHT, 0.0f );
	pVtx2D[ 9 ].pos = D3DXVECTOR3( RANK1ST_POS_X + RANK1ST_WIDTH, RANK1ST_POS_Y - RANK1ST_HEIGHT, 0.0f );
	pVtx2D[ 10 ].pos = D3DXVECTOR3( RANK1ST_POS_X - RANK1ST_WIDTH, RANK1ST_POS_Y + RANK1ST_HEIGHT, 0.0f );
	pVtx2D[ 11 ].pos = D3DXVECTOR3( RANK1ST_POS_X + RANK1ST_WIDTH, RANK1ST_POS_Y + RANK1ST_HEIGHT, 0.0f );

	pVtx2D[ 8 ].rhw = 1.0f;
	pVtx2D[ 9 ].rhw = 1.0f;
	pVtx2D[ 10 ].rhw = 1.0f;
	pVtx2D[ 11 ].rhw = 1.0f;

	pVtx2D[ 8 ].col = D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f );
	pVtx2D[ 9 ].col = D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f );
	pVtx2D[ 10 ].col = D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f );
	pVtx2D[ 11 ].col = D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f );

	pVtx2D[ 8 ].tex = D3DXVECTOR2( 0.0f, 0.0f );
	pVtx2D[ 9 ].tex = D3DXVECTOR2( 1.0f, 0.0f );
	pVtx2D[ 10 ].tex = D3DXVECTOR2( 0.0f, 1.0f );
	pVtx2D[ 11 ].tex = D3DXVECTOR2( 1.0f, 1.0f );

	// [ 2nd ]
	pVtx2D[ 12 ].pos = D3DXVECTOR3( RANK2ND_POS_X - RANK2ND_WIDTH, RANK2ND_POS_Y - RANK2ND_HEIGHT, 0.0f );
	pVtx2D[ 13 ].pos = D3DXVECTOR3( RANK2ND_POS_X + RANK2ND_WIDTH, RANK2ND_POS_Y - RANK2ND_HEIGHT, 0.0f );
	pVtx2D[ 14 ].pos = D3DXVECTOR3( RANK2ND_POS_X - RANK2ND_WIDTH, RANK2ND_POS_Y + RANK2ND_HEIGHT, 0.0f );
	pVtx2D[ 15 ].pos = D3DXVECTOR3( RANK2ND_POS_X + RANK2ND_WIDTH, RANK2ND_POS_Y + RANK2ND_HEIGHT, 0.0f );

	pVtx2D[ 12 ].rhw = 1.0f;
	pVtx2D[ 13 ].rhw = 1.0f;
	pVtx2D[ 14 ].rhw = 1.0f;
	pVtx2D[ 15 ].rhw = 1.0f;

	pVtx2D[ 12 ].col = D3DXCOLOR( 0.8f, 0.8f, 0.8f, 1.0f );
	pVtx2D[ 13 ].col = D3DXCOLOR( 0.8f, 0.8f, 0.8f, 1.0f );
	pVtx2D[ 14 ].col = D3DXCOLOR( 0.8f, 0.8f, 0.8f, 1.0f );
	pVtx2D[ 15 ].col = D3DXCOLOR( 0.8f, 0.8f, 0.8f, 1.0f );

	pVtx2D[ 12 ].tex = D3DXVECTOR2( 0.0f, 0.0f );
	pVtx2D[ 13 ].tex = D3DXVECTOR2( 1.0f, 0.0f );
	pVtx2D[ 14 ].tex = D3DXVECTOR2( 0.0f, 1.0f );
	pVtx2D[ 15 ].tex = D3DXVECTOR2( 1.0f, 1.0f );

	// [ 3rd ]
	pVtx2D[ 16 ].pos = D3DXVECTOR3( RANK3RD_POS_X - RANK3RD_WIDTH, RANK3RD_POS_Y - RANK3RD_HEIGHT, 0.0f );
	pVtx2D[ 17 ].pos = D3DXVECTOR3( RANK3RD_POS_X + RANK3RD_WIDTH, RANK3RD_POS_Y - RANK3RD_HEIGHT, 0.0f );
	pVtx2D[ 18 ].pos = D3DXVECTOR3( RANK3RD_POS_X - RANK3RD_WIDTH, RANK3RD_POS_Y + RANK3RD_HEIGHT, 0.0f );
	pVtx2D[ 19 ].pos = D3DXVECTOR3( RANK3RD_POS_X + RANK3RD_WIDTH, RANK3RD_POS_Y + RANK3RD_HEIGHT, 0.0f );

	pVtx2D[ 16 ].rhw = 1.0f;
	pVtx2D[ 17 ].rhw = 1.0f;
	pVtx2D[ 18 ].rhw = 1.0f;
	pVtx2D[ 19 ].rhw = 1.0f;

	pVtx2D[ 16 ].col = D3DXCOLOR( 1.0f, 0.7f, 0.0f, 1.0f );
	pVtx2D[ 17 ].col = D3DXCOLOR( 1.0f, 0.7f, 0.0f, 1.0f );
	pVtx2D[ 18 ].col = D3DXCOLOR( 1.0f, 0.7f, 0.0f, 1.0f );
	pVtx2D[ 19 ].col = D3DXCOLOR( 1.0f, 0.7f, 0.0f, 1.0f );

	pVtx2D[ 16 ].tex = D3DXVECTOR2( 0.0f, 0.0f );
	pVtx2D[ 17 ].tex = D3DXVECTOR2( 1.0f, 0.0f );
	pVtx2D[ 18 ].tex = D3DXVECTOR2( 0.0f, 1.0f );
	pVtx2D[ 19 ].tex = D3DXVECTOR2( 1.0f, 1.0f );

	// アンロック
	g_pVtxBuffRanking ->Unlock();

	// テクスチャポインタの初期化
	for ( int i = 0; i < RANK_TEX_MAX; i++ )
	{
		g_pTextureRanking[ i ] = NULL;
	}

	// テクスチャの読み込み
	//------------------------------------
	D3DXCreateTextureFromFile( pDevice, RANK_TEX_PASS, &g_pTextureRanking[ 0 ] );
	D3DXCreateTextureFromFile( pDevice, RANKBACK_TEX, &g_pTextureRanking[ 1 ] );
	D3DXCreateTextureFromFile( pDevice, RANK_TEX_1ST, &g_pTextureRanking[ 2 ] );
	D3DXCreateTextureFromFile( pDevice, RANK_TEX_2ND, &g_pTextureRanking[ 3 ] );
	D3DXCreateTextureFromFile( pDevice, RANK_TEX_3RD, &g_pTextureRanking[ 4 ] );

	// 描画の準備
	for ( int i = 0; i < RANK_MAX; i++ )
	{
		SetNumber( &rankingNumber[ i ], D3DXVECTOR3( RANKPOS_X, RANKPOS_Y + ( i * RANK_HEIGHT * 4 ), 0.0f ), RANK_DIG, RANK_WIDTH, RANK_HEIGHT );
	}

	// 音楽
	PlaySound( RESULT_BGM );
}

// ランキング終了処理
void UninitRanking( void )
{
	// ローカル変数
	//------------------------------------
	FILE *fp;		// ファイルポインタ

	// ファイルのオープン
	fp = fopen("data/SCRIPT/ranking.bin", "wb");
	if (fp == NULL)
	{
		//MessageBox( NULL, "ランキングテキストのオープンに失敗", "エラー", MB_OK );
		return;
	}

	// セーブ処理
	fwrite( &ranking, sizeof( RANKING ), RANK_MAX, fp );

	// ファイルのクローズ
	fclose( fp );

	// 数字描画に使ったバッファを解放
	for ( int i = 0; i < RANK_MAX; i++	 )
	{
		UninitNumber( &rankingNumber[ i ] );
	}

	for ( int i = 0; i < RANK_TEX_MAX; i++ )
	{
		// 使ったテクスチャの解放と初期化
		//------------------------------------
		if (g_pTextureRanking[ i ] != NULL)
		{ 
			g_pTextureRanking[ i ] ->Release();
			g_pTextureRanking[ i ] = NULL;
		}
	}


	// 頂点バッファの解放と初期化
	//------------------------------------
	if (g_pVtxBuffRanking != NULL)
	{ 
		g_pVtxBuffRanking ->Release();
		g_pVtxBuffRanking = NULL;
	}
}

// ランキング更新処理
void UpdateRanking( void )
{
	if ( ( GetKeyboardTrigger(DIK_RETURN) || GetPadElecomTrigger( PAD_4 ) || GetPadElecomTrigger( PAD_10 ) ) && GetFade() == FADE_NONE)
	{
		PlaySound( DESIDE4_SE );
		SetFade( FADE_OUT );
		SetMode( MODE_RESULT );
	}
}

// ランキング描画処理
void DrawRanking( void )
{
	// ローカル変数
	//------------------------------------
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスオブジェクト
	
	// 頂点バッファをデータストリームにバインド
	//-----------------------------------------
	pDevice ->SetStreamSource(0,
								g_pVtxBuffRanking,
								0,
								sizeof(VERTEX_2D));	// 頂点データの間隔

	
	// 頂点フォーマットの設定
	//------------------------------------
	pDevice ->SetFVF( FVF_VERTEX_2D );

	// テクスチャの設定[ 背景 ]
	//------------------------------------
	pDevice -> SetTexture(0, g_pTextureRanking[ 1 ]);
			
	// ポリゴンの描画[ ランキング ]
	//------------------------------------
	pDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,	// プリミティブの種類
									4,				// 読み込む最初の番号
									2);				// 描画するプリミティブ数

	// テクスチャの設定
	//------------------------------------
	pDevice -> SetTexture(0, g_pTextureRanking[ 0 ]);
			
	// ポリゴンの描画[ ランキング ]
	//------------------------------------
	pDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,	// プリミティブの種類
									0,				// 読み込む最初の番号
									2);				// 描画するプリミティブ数

	// テクスチャの設定[ 1st ]
	//------------------------------------
	pDevice -> SetTexture(0, g_pTextureRanking[ 2 ]);
			
	// ポリゴンの描画
	//------------------------------------
	pDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,	// プリミティブの種類
									8,				// 読み込む最初の番号
									2);				// 描画するプリミティブ数

	// テクスチャの設定[ 2nd ]
	//------------------------------------
	pDevice -> SetTexture(0, g_pTextureRanking[ 3 ]);
			
	// ポリゴンの描画
	//------------------------------------
	pDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,	// プリミティブの種類
									12,				// 読み込む最初の番号
									2);				// 描画するプリミティブ数

	// テクスチャの設定[ 3rd ]
	//------------------------------------
	pDevice -> SetTexture(0, g_pTextureRanking[ 4 ]);
			
	// ポリゴンの描画
	//------------------------------------
	pDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,	// プリミティブの種類
									16,				// 読み込む最初の番号
									2);				// 描画するプリミティブ数

	// 数字の描画
	if ( g_bJudge == true )
	{
		for ( int i = 0; i < RANK_MAX; i++ )
		{
			if ( i == g_nIndex )
			{
				if ( ( ++g_nRankingFrameCount % 60 ) < 40 )
				{
					DrawNumber( &rankingNumber[ i ], ranking.nScore[ i ] );
				}
			}
			else
			{
				DrawNumber( &rankingNumber[ i ], ranking.nScore[ i ] );
			}
		}
	}
	else
	{
		for ( int i = 0; i < RANK_MAX; i++ )
		{
			DrawNumber( &rankingNumber[ i ], ranking.nScore[ i ] );
		}
	}
}

// ランキングロード処理
void LoadRanking( void )
{
	// ローカル変数
	//------------------------------------
	FILE *fp;		// ファイルポインタ

	// ファイルのオープン
	fp = fopen("data/SCRIPT/ranking.bin", "rb");
	if (fp == NULL)
	{
		//MessageBox( NULL, "ランキングテキストのオープンに失敗", "エラー", MB_OK );
		return;
	}

	// ファイルの読み込み
	int size = fread( &ranking, sizeof( RANKING ), RANK_MAX, fp );

	// ファイルのクローズ
	fclose( fp );

	// プレイした時の得点を配列の最後に格納
	ranking.nScore[ RANK_MAX ] = GetScore();

	// ソート
	for ( int i = 0; i < RANK_MAX + 1; i++ )
	{
		int n1 = ranking.nScore[ i ];
		int n2 = ranking.nScore[ RANK_MAX ];

		if ( n1 < n2 )
		{
			if ( i == 0 )
			{
				int work = ranking.nScore[ 0 ];
				int work1 = ranking.nScore[ 1 ];
				int work2 = ranking.nScore[ 2 ];
				int work3 = ranking.nScore[ RANK_MAX ];

				ranking.nScore[ 0 ] = work3;
				ranking.nScore[ 1 ] = work;
				ranking.nScore[ 2 ] = work1;
				
				g_bJudge = true;
				
				break;
			}
			else if ( i == 1 )
			{
				int work1 = ranking.nScore[ 1 ];
				int work2 = ranking.nScore[ 2 ];
				int work3 = ranking.nScore[ RANK_MAX ];

				ranking.nScore[ 1 ] = work3;
				ranking.nScore[ 2 ] = work1;

				g_bJudge = true;

				break;
			}
			else if ( i == 2 )
			{
				ranking.nScore[ 2 ] = ranking.nScore[ RANK_MAX ];
				g_bJudge = true;

				break;
			}
		}
	}

	if ( g_bJudge == true )
	{
		for ( int i = 0; i < RANK_MAX; i++ )
		{
			if ( ranking.nScore[ i ] == GetScore() )
			{
				g_nIndex = i;

				break;
			}
		}
	}
}
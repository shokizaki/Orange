//-----------------------------------------------
//  内容：リザルト処理
//  Fail:result.cpp
//  制作者：木崎　翔
//-----------------------------------------------

//-----------------------------------------------
//  インクルードファイル
//-----------------------------------------------
#include "result.h"
#include "main.h"
#include "input.h"
#include "fade.h"
//#include "resultbg.h"

//-----------------------------------------------
//　マクロ定義
//-----------------------------------------------
#define RESULT_MAX  ( 2 )			 // 頂点の数
#define TEXTURE_MAX ( 2 )			 // 使用するテクスチャの数

#define THANKS_POS_X	( SCREEN_WIDTH / 2 )
#define THANKS_POS_Y	( SCREEN_HEIGHT - 100.0f )
#define THANKS_WIDTH	( 300.0f )
#define THANKS_HEIGHT	( 40.0f )

//-----------------------------------------------
//  グローバル変数
//-----------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResult = NULL;		// 頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureResult[TEXTURE_MAX];		// テクスチャへのポインタ
int g_nFrameCount = 0;
bool g_bResultFade = false;

//-----------------------------------------------
// 背景初期化処理
//-----------------------------------------------
void InitResult()
{
	// 初期化
	//------------------------------------
	g_nFrameCount = 0;
	g_bResultFade = false;

	// ローカル変数
	//---------------------------------
	LPDIRECT3DDEVICE9 pDevice;		// デバイスオブジェクト
	VERTEX_2D *pVtx;

	// デバイス取得
	//------------------------------------
	pDevice = GetDevice();

	pDevice ->CreateVertexBuffer((sizeof( VERTEX_2D ) * 4 * RESULT_MAX),	// 頂点データ用に確保するバッファ
								 D3DUSAGE_WRITEONLY,				// 頂点バッファの使用方法
								 FVF_VERTEX_2D,						// 使用する頂点フォーマット
								 D3DPOOL_MANAGED,					// バッファを保持するメモリクラスの指定
								 &g_pVtxBuffResult,					// 頂点バッファへのポインタ
								 NULL);								// NULL固定
	// タイトル背景初期化処理
	//------------------------------------
	//InitResultBG();

		// データの位置を確保する（ロック）
	//------------------------------------
	g_pVtxBuffResult ->Lock(0,
						0,
						(void**)&pVtx,
						0);

	// 
	//------------------------------------
	pVtx[ 0 ].pos.x = 0.0f;
	pVtx[ 0 ].pos.y = 0.0f;	// 頂点の位置
	pVtx[ 0 ].pos.z = 0.0f;												// 頂点の位置

	pVtx[ 1 ].pos.x = SCREEN_WIDTH;
	pVtx[ 1 ].pos.y = 0.0f;	// 頂点の位置
	pVtx[ 1 ].pos.z = 0.0f;												// 頂点の位置

	pVtx[ 2 ].pos.x = 0.0f;
	pVtx[ 2 ].pos.y = SCREEN_HEIGHT;	// 頂点の位置
	pVtx[ 2 ].pos.z = 0.0f;												// 頂点の位置

	pVtx[ 3 ].pos.x = SCREEN_WIDTH;
	pVtx[ 3 ].pos.y = SCREEN_HEIGHT;	// 頂点の位置
	pVtx[ 3 ].pos.z = 0.0f;												// 頂点の位置

	// 法線
	pVtx[ 0 ].rhw = 1.0f;
	pVtx[ 1 ].rhw = 1.0f;
	pVtx[ 2 ].rhw = 1.0f;
	pVtx[ 3 ].rhw = 1.0f;			

	// フェードの色
	//------------------------------------
	pVtx[ 0 ].col = D3DXCOLOR( 1.0f, 0.0f, 0.0f, 1.0f );			// 頂点ごとの色の設定
	pVtx[ 1 ].col = D3DXCOLOR( 1.0f, 0.0f, 0.0f, 1.0f );			//			↓
	pVtx[ 2 ].col = D3DXCOLOR( 1.0f, 0.0f, 0.0f, 1.0f );			//			↓
	pVtx[ 3 ].col = D3DXCOLOR( 1.0f, 0.0f, 0.0f, 1.0f );			//			↓

	pVtx[ 0 ].tex = D3DXVECTOR2( 0.0f, 0.0f );
	pVtx[ 1 ].tex = D3DXVECTOR2( 1.0f, 0.0f );
	pVtx[ 2 ].tex = D3DXVECTOR2( 0.0f, 1.0f );
	pVtx[ 3 ].tex = D3DXVECTOR2( 1.0f, 1.0f );

	// 頂点情報セット
	//------------------------------------
	pVtx[ 4 ].pos.x = SCREEN_WIDTH / 2 - 200.0f;
	pVtx[ 4 ].pos.y = 500.0f - 40.0f;	// 頂点の位置
	pVtx[ 4 ].pos.z = 0.0f;												// 頂点の位置

	pVtx[ 5 ].pos.x = SCREEN_WIDTH / 2 + 200.0f;
	pVtx[ 5 ].pos.y = 500.0f - 40.0f;	// 頂点の位置
	pVtx[ 5 ].pos.z = 0.0f;												// 頂点の位置

	pVtx[ 6 ].pos.x = SCREEN_WIDTH / 2 - 200.0f;
	pVtx[ 6 ].pos.y = 500.0f + 40.0f;	// 頂点の位置
	pVtx[ 6 ].pos.z = 0.0f;												// 頂点の位置

	pVtx[ 7 ].pos.x = SCREEN_WIDTH / 2 + 200.0f;
	pVtx[ 7 ].pos.y = 500.0f + 40.0f;	// 頂点の位置
	pVtx[ 7 ].pos.z = 0.0f;												// 頂点の位置

	// パースペクティブコレクト
	pVtx[ 4 ].rhw = 1.0f;
	pVtx[ 5 ].rhw = 1.0f;
	pVtx[ 6 ].rhw = 1.0f;
	pVtx[ 7 ].rhw = 1.0f;	

	// 色
	//------------------------------------
	pVtx[ 4 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );			// 頂点ごとの色の設定
	pVtx[ 5 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );			//			↓
	pVtx[ 6 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );			//			↓
	pVtx[ 7 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );			//			↓

	pVtx[ 4 ].tex = D3DXVECTOR2( 0.0f, 0.0f );
	pVtx[ 5 ].tex = D3DXVECTOR2( 1.0f, 0.0f );
	pVtx[ 6 ].tex = D3DXVECTOR2( 0.0f, 1.0f );
	pVtx[ 7 ].tex = D3DXVECTOR2( 1.0f, 1.0f );

	// 解放
	//------------------------------------
	g_pVtxBuffResult -> Unlock();

	// テクスチャの読み込み
	//------------------------------------
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/result.jpg",	&g_pTextureResult[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/thanks.png", &g_pTextureResult[ 1 ]);
}

//-----------------------------------------------
// 背景終了処理
//-----------------------------------------------
void UninitResult()
{
	for ( int nCnt = 0; nCnt < 4; nCnt++ )
	{
		if (g_pTextureResult[ nCnt ] != NULL)
		{ // 使ったテクスチャの解放と初期化
		  //------------------------------------
			g_pTextureResult[ nCnt ] -> Release();
			g_pTextureResult[ nCnt ] = NULL;
		}
	}

	if (g_pVtxBuffResult != NULL)
	{ // 頂点バッファの解放と初期化
	  //------------------------------------
		g_pVtxBuffResult->Release();
		g_pVtxBuffResult = NULL;
	}

	//StopSound();
}

//-----------------------------------------------
// リザルト更新処理
//-----------------------------------------------
void UpdateResult()
{
	// エンターを押したら
	//------------------------------------
	// 遷移判定
	if ( ( GetKeyboardTrigger(DIK_RETURN) /*|| GetGamePadTrigger( GAMEPAD_START ) || GetGamePadTrigger( GAMEPAD_A )*/ || GetPadElecomTrigger( PAD_4 ) || GetPadElecomTrigger( PAD_10 ) ) && GetFade() == FADE_NONE )
	{
		PlaySound( DESIDE4_SE );
		SetFade( FADE_OUT, 60 );
		SetMode( MODE_TITLE );

		g_bResultFade = true;
	}
}

//-----------------------------------------------
// 背景描画処理
//-----------------------------------------------
void DrawResult()
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
								g_pVtxBuffResult,
								0,
								sizeof(VERTEX_2D));	// 頂点データの間隔

	// 頂点フォーマットの設定
	//------------------------------------
	pDevice -> SetFVF(FVF_VERTEX_2D);

	for ( int i = 0; i < RESULT_MAX; i++ )
	{
		//// テクスチャの設定
		////------------------------------------
		//pDevice -> SetTexture(0, g_pTextureResult[ i ]);

		// テクスチャの設定
		//------------------------------------
		pDevice -> SetTexture( 0, NULL );

		// ポリゴンの描画
		//------------------------------------
		pDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,	// プリミティブの種類
										i * 4,				// 読み込む最初の番号
										2);				// 描画するプリミティブ数
	}
}

//-----------------------------------------------
// 頂点情報セット
//-----------------------------------------------
void SetupResult()
{
}
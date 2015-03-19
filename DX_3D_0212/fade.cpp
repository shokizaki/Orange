//=======================================================================================
//
//	クラス：AT11C272 30623 13
//	制作者：木崎　翔
//	内容：フェードイン・アウト
//
//=======================================================================================

//-----------------------------------------------
//
//  インクルードファイル
//
//-----------------------------------------------
#include "fade.h"
#include "mode.h"
#include "camera.h"

//-----------------------------------------------
//　マクロ定義
//-----------------------------------------------
#define FADE_VTX_MAX	( 4 )		 // 頂点の数
#define TEXTURE_MAX		( 1 )		 // 使用するテクスチャの数
#define FADE_FRAME		( 60 )

//-----------------------------------------------
//
//  グローバル変数 
//
//-----------------------------------------------
static float fadeR = 1.0f;
static float fadeG = 1.0f;
static float fadeB = 1.0f;
static float fadeA = 1.0f;

FADE g_fade = FADE_NONE;
int fadeFrame = 0;
int fadeCount = 0;
float g_fAlphaPlus = 0;

bool fadeEnable = false;
bool fadeExecute = false;
bool fadeIn = false;

LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFade = NULL;		// 頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureFade[TEXTURE_MAX];		// テクスチャへのポインタ

D3DXVECTOR3 posFade;			// 位置
D3DXVECTOR3 rotFade;			// 向き
D3DXVECTOR3 sclFade;			// 大きさ

D3DXMATRIX mtxViewFade;			// ワールドビュー
D3DXMATRIX mtxWorldFade;		// ワールドマトリックス
D3DXMATRIX g_mtxProjectionFade;	// プロジェクションマトリックス

//-----------------------------------------------
//
//   関数プロトタイプ宣言
//
//-----------------------------------------------

//-----------------------------------------------
// フェードカラーセット
//-----------------------------------------------
static void SetFadeColor( float r, float g, float b);	// 内部関数

//-----------------------------------------------
// フェードカラーセット
//-----------------------------------------------
static void SetFadeColor(float r, float g, float b)
{
	fadeR = r;
	fadeG = g;
	fadeB = b;
}

//-----------------------------------------------
//
//　フェード初期化
//　
//-----------------------------------------------
void InitFade()
{
	// ローカル変数
	//---------------------------------
	LPDIRECT3DDEVICE9 pDevice;		// デバイスオブジェクト

	// 初期化
	//------------------------------------
	fadeFrame = 0;
	fadeCount = 0;
	g_fAlphaPlus = 0;
	fadeIn = true;
	fadeR = 1.0f;
	fadeG = 1.0f;
	fadeB = 1.0f;
	fadeA = 1.0f;

	g_fade = FADE_NONE;

	// デバイス取得
	//------------------------------------
	pDevice = GetDevice();

	pDevice ->CreateVertexBuffer((sizeof( VERTEX_2D ) * FADE_VTX_MAX),	// 頂点データ用に確保するバッファ
								 D3DUSAGE_WRITEONLY,				// 頂点バッファの使用方法
								 FVF_VERTEX_2D,						// 使用する頂点フォーマット
								 D3DPOOL_MANAGED,					// バッファを保持するメモリクラスの指定
								 &g_pVtxBuffFade,					// 頂点バッファへのポインタ
								 NULL);								// NULL固定

	posFade = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	rotFade = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	sclFade = D3DXVECTOR3( 1.0f, 1.0f, 1.0f );

	// 背景の頂点情報セット
	//---------------------------------
	SetupFade();
}

//-----------------------------------------------
//
//　フェードセットアップ
//　
//-----------------------------------------------
void SetupFade()
{
	// ローカル変数
	//------------------------------------
	VERTEX_2D *pVtx;				// 頂点の構造体ポインタ
	LPDIRECT3DDEVICE9 pDevice;		// デバイスオブジェクト

	// デバイス取得
	//------------------------------------
	pDevice = GetDevice();

	// データの位置を確保する（ロック）
	//------------------------------------
	g_pVtxBuffFade ->Lock(0,
						0,
						(void**)&pVtx,
						0);

	// フェードポリゴンの大きさ
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
	pVtx[ 0 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, fadeA );			// 頂点ごとの色の設定
	pVtx[ 1 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, fadeA );			//			↓
	pVtx[ 2 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, fadeA );			//			↓
	pVtx[ 3 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, fadeA );			//			↓

	pVtx[ 0 ].tex = D3DXVECTOR2( 0.0f, 0.0f );
	pVtx[ 1 ].tex = D3DXVECTOR2( 1.0f, 0.0f );
	pVtx[ 2 ].tex = D3DXVECTOR2( 0.0f, 1.0f );
	pVtx[ 3 ].tex = D3DXVECTOR2( 1.0f, 1.0f );

	// 解放
	//------------------------------------
	g_pVtxBuffFade -> Unlock();
}

//-----------------------------------------------
//
//　フェード終了処理
//　
//-----------------------------------------------
void UninitFade()
{
	// ローカル変数
	//------------------------------------
	int nCnt;

	for (nCnt = 0; nCnt < TEXTURE_MAX; nCnt++)
	{
		if (g_pTextureFade[nCnt] != NULL)
		{ // 使ったテクスチャの解放と初期化
		  //------------------------------------
			g_pTextureFade[nCnt] -> Release();
			g_pTextureFade[nCnt] = NULL;
		}
	}

	if (g_pVtxBuffFade != NULL)
	{ // 頂点バッファの解放と初期化
	  //------------------------------------
		g_pVtxBuffFade->Release();
		g_pVtxBuffFade = NULL;
	}
}

//-----------------------------------------------
//
//　フェードイン関数
//　フェードインを指定色でスタートする
//  r,g,b　・・・　フェード色　
//　frame　・・・　フェード時間（フレーム数）
//　
//-----------------------------------------------
void FadeIn( float r, float g, float b, int frame )
{
	SetFadeColor( r, g, b );		// フェード色を指定
	fadeFrame = frame;				// フェードするフレーム数を入れる
	fadeCount = 0;					// カウント初期化
	fadeExecute = true;				// フェードを有効
	fadeEnable = true;				// フェード実行中
	fadeIn = true;					// フェードイン
	g_fAlphaPlus = 1.0f / fadeFrame;	// 加算されるアルファの量
}

//-----------------------------------------------
//
//　フェードアウト関数
//　フェードアウトを指定色でスタートする
//  r,g,b　・・・　フェード色　
//　frame　・・・　フェード時間（フレーム数）
//　
//-----------------------------------------------
void FadeOut( float r, float g, float b, int frame )
{
	SetFadeColor( r, g, b );		// フェード色を指定
	fadeFrame = frame;				// フェードするフレーム数を入れる
	fadeCount = 0;					// カウント初期化
	fadeExecute = true;				// フェード実行中
	fadeEnable = true;				// フェードを有効
	fadeIn = false;					// フェードアウト
	g_fAlphaPlus = 1.0f / fadeFrame;	// 加算されるアルファの量
}

//-----------------------------------------------
//
//　フェード更新処理関数
//　
//-----------------------------------------------
void UpdateFade(void)
{
	CAMERA *pCamera = GetCamera();

	// フェード処理が無効だったら
	//----------------------------------------------
	if ( g_fade == FADE_NONE )
	{
		return;
	}

	// フェード計算
	//------------------------------------
	//fadeA = (int)( 255 * (fadeIn ? 1 - (float)fadeCount / fadeFrame : (float)fadeCount / fadeFrame) );

	// フェードイン処理
	//----------------------------------------------
	if ( g_fade == FADE_IN )
	{
		g_fAlphaPlus = 1.0f / FADE_FRAME;

		fadeA -= g_fAlphaPlus;
		if (fadeCount == FADE_FRAME)
		{
			fadeA = 0.0f;

			fadeCount = 0;

			g_fade = FADE_NONE;			// フェードの終了
		}
	}

	// フェードアウト処理
	//----------------------------------------------
	if ( g_fade == FADE_OUT )
	{
		g_fAlphaPlus = 1.0f / FADE_FRAME;

		fadeA += g_fAlphaPlus;
		if (fadeCount == FADE_FRAME)
		{
			fadeA = 1.0f;

			fadeCount = 0;

			g_fade = FADE_NONE;			// フェードの終了
			
			// モード遷移
			//-----------------------------------
			ChangeScene();
		}
	}

	// フェードカウントアップ
	//----------------------------------------------
	fadeCount += 1;

	// フェードポリゴンの位置をカメラの目の前にする
	//posFade = D3DXVECTOR3( pCamera->posCameraP.x, pCamera->posCameraP.y, pCamera->posCameraP.z + ( 22.0f * cosf( pCamera->rotCamera.y ) ) );

	SetupFade();

	// フェードカウントがフェードフレームより多くなったら
	//---------------------------------------------------
	//if (FADE_FRAME < fadeCount)
	//{
	//	fadeCount = fadeFrame;		// フェードフレームを入れる
	//	fadeExecute = false;		// フェード終了
	//	return;
	//}
}

//-----------------------------------------------
//
//　フェード描画処理関数
//　
//-----------------------------------------------
void DrawFade(void)
{
	// ローカル変数
	//------------------------------------
	LPDIRECT3DDEVICE9 pDevice;						// デバイスオブジェクト
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;		// 計算用
	CAMERA *pCamera = GetCamera();					// カメラ取得
	
	// デバイスの取得
	//------------------------------------
	pDevice = GetDevice();

	// 頂点バッファをデータストリームにバインド
	//-----------------------------------------
	pDevice ->SetStreamSource(0,
								g_pVtxBuffFade,
								0,
								sizeof(VERTEX_2D));	// 頂点データの間隔

	// 頂点フォーマットの設定
	//------------------------------------
	pDevice -> SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	//------------------------------------
	pDevice -> SetTexture(0, NULL);

	// ポリゴンの描画
	//------------------------------------
	pDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,	// プリミティブの種類
									0,				// 読み込む最初の番号
									2);				// 描画するプリミティブ数

	switch ( g_fade )
	{
	case FADE_NONE:
		PrintDebugProc("FADE_NONE\n");
		break;

	case FADE_IN:
		PrintDebugProc("FADE_IN\n");
		break;

	case FADE_OUT:
		PrintDebugProc("FADE_OUT\n");
		break;

	}
	
}

//-----------------------------------------------
//
//　現在のフェード取得
//　
//-----------------------------------------------
FADE GetFade( void )
{
	return g_fade;
}

//-----------------------------------------------
//
//　フェードのセット
//　
//-----------------------------------------------
void SetFade( FADE fadeSet )
{
	g_fade = fadeSet;
}

//-----------------------------------------------
//
//　フェードの位置セット
//　
//-----------------------------------------------
void SetFadePos( D3DXVECTOR3 pos, D3DXVECTOR3 rot )
{
	posFade.x = pos.x;
	posFade.x = pos.y;
	posFade.x = pos.z + ( 21.0f * cosf( rot.y ) );
}
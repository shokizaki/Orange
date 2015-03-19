//-----------------------------------------------
//  内容：スコアに関する処理
//  Fail:score.cpp
//  制作者：木崎　翔
//-----------------------------------------------

//-----------------------------------------------
//  インクルードファイル
//-----------------------------------------------
#include "number.h"

//-----------------------------------------------
//　マクロ定義
//-----------------------------------------------
#define NUMBER_TEX		(2)				// テクスチャの使用数
#define NUMBER_MAX		(5)				// プリミティブ数
#define WAVE_NUM		(2)				// 残敵数の描画開始プリミティブ番号

//-----------------------------------------------
//  グローバル変数
//-----------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffNumber = NULL;		// 頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureNumber = NULL;		// テクスチャへのポインタ

void InitNumber( void )
{
	// ローカル変数
	//---------------------------------
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// デバイスオブジェクト

	// テクスチャへのポインタを初期化
	//------------------------------------
	g_pTextureNumber = NULL;

	// テクスチャの読み込み
	//------------------------------------
	D3DXCreateTextureFromFile( pDevice, "data/TEXTURE/number000.png", &g_pTextureNumber );
}

// 終了
void UninitNumber( NUMBER *pNumber )
{
	// 頂点バッファの解放と初期化
	//------------------------------------
	if ( pNumber ->pVtxBuffer != NULL)
	{ 
		pNumber ->pVtxBuffer ->Release();
		pNumber ->pVtxBuffer = NULL;
	}
}

// テクスチャの解放処理
void UninitNumberTexture( void )
{
	// 使ったテクスチャの解放と初期化
	//------------------------------------
	if ( g_pTextureNumber != NULL )
	{ 
		g_pTextureNumber ->Release();
		g_pTextureNumber = NULL;
	}
}

// 数字のセット
void SetNumber( NUMBER *pNumber, D3DXVECTOR3 pos, int nDig, float fWidth, float fHeight )
{
	// ローカル変数
	//---------------------------------
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// デバイスオブジェクト
	VERTEX_2D *pVtx2D = NULL;

	// 構造体に格納
	//---------------------------------
	pNumber ->pos = pos;		// 位置
	pNumber ->nDigit = nDig;	// 分割数

	// 頂点バッファの確保
	//------------------------------------
	pDevice ->CreateVertexBuffer((sizeof(VERTEX_2D) * 4 * nDig),		// 頂点データ用に確保するバッファ
								 D3DUSAGE_WRITEONLY,					// 頂点バッファの使用方法
								 FVF_VERTEX_2D,							// 使用する頂点フォーマット
								 D3DPOOL_MANAGED,						// バッファを保持するメモリクラスの指定
								 &pNumber ->pVtxBuffer,					// 頂点バッファへのポインタ
								 NULL);									// NULL固定	
	
	// ロック
	pNumber ->pVtxBuffer ->Lock( 0, 0, ( void** )&pVtx2D, 0 );

	// 頂点情報をセット
	for ( int i = 0; i < nDig; i++ )
	{
		pVtx2D[ 0 + i * 4 ].pos = D3DXVECTOR3( ( pos.x + i * fWidth * 2 ) - fWidth, pos.y - fHeight, 0.0f );
		pVtx2D[ 1 + i * 4 ].pos = D3DXVECTOR3( ( pos.x + i * fWidth * 2 ) + fWidth, pos.y - fHeight, 0.0f );
		pVtx2D[ 2 + i * 4 ].pos = D3DXVECTOR3( ( pos.x + i * fWidth * 2 ) - fWidth, pos.y + fHeight, 0.0f );
		pVtx2D[ 3 + i * 4 ].pos = D3DXVECTOR3( ( pos.x + i * fWidth * 2 ) + fWidth, pos.y + fHeight, 0.0f );

		pVtx2D[ 0 + i * 4 ].rhw = 1.0f;
		pVtx2D[ 1 + i * 4 ].rhw = 1.0f;
		pVtx2D[ 2 + i * 4 ].rhw = 1.0f;
		pVtx2D[ 3 + i * 4 ].rhw = 1.0f;

		pVtx2D[ 0 + i * 4 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );
		pVtx2D[ 1 + i * 4 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );
		pVtx2D[ 2 + i * 4 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );
		pVtx2D[ 3 + i * 4 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );
	}

	// アンロック
	pNumber ->pVtxBuffer ->Unlock();
}

// 数字の描画
void DrawNumber( NUMBER *pNumber, int nNumber )
{
	// ローカル変数
	//------------------------------------
	LPDIRECT3DDEVICE9 pDevice = GetDevice();						// デバイスオブジェクト
	VERTEX_2D *pVtx2D = NULL;
	int nDevide = 1;

	// 描画する桁数を計算
	for ( int i = 0; i < pNumber ->nDigit - 1; i++ )
	{
		nDevide *= 10;
	}

	// ロック
	pNumber ->pVtxBuffer ->Lock( 0, 0, ( void** )&pVtx2D, 0 );

	// テクスチャ座標の計算
	for ( int i = 0; i < pNumber ->nDigit; i++ )
	{
		float fNumber = ( nNumber / nDevide ) * 0.1f;

		pVtx2D[ 0 + i * 4 ].tex = D3DXVECTOR2( 0.0f + fNumber, 0.0f );
		pVtx2D[ 1 + i * 4 ].tex = D3DXVECTOR2( 0.1f + fNumber, 0.0f );
		pVtx2D[ 2 + i * 4 ].tex = D3DXVECTOR2( 0.0f + fNumber, 1.0f );
		pVtx2D[ 3 + i * 4 ].tex = D3DXVECTOR2( 0.1f + fNumber, 1.0f );

		nDevide /= 10;
	}

	// アンロック
	pNumber ->pVtxBuffer ->Unlock();

	// 頂点バッファをデータストリームにバインド
	//-----------------------------------------
	pDevice ->SetStreamSource( 0, pNumber ->pVtxBuffer, 0, sizeof( VERTEX_2D ) );

	// 頂点フォーマットの設定
	//------------------------------------
	pDevice ->SetFVF( FVF_VERTEX_2D );

	// テクスチャの設定[ フレーム ]
	//------------------------------------
	pDevice -> SetTexture( 0, g_pTextureNumber );

	for ( int i = 0; i < pNumber ->nDigit; i++ )
	{
		// ポリゴンの描画
		//------------------------------------
		pDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,	// プリミティブの種類
										i * 4,			// 読み込む最初の番号
										2);				// 描画するプリミティブ数
	}
}
/*
|   @Brief  : タイトル画面のボタンを押せ表示
|
|   @File   : TitlePreBtn.cpp
|
|   @Auhtor : Ayumi Yasui
|
|   @Note   : Press 4 or Enter
|
*------------------------------------------------------------------------*/

/*--- インクルード ------------------------------------------------------*/

#include "TitlePreBtn.h"

/*--- 定数定義 ----------------------------------------------------------*/

// 位置
#define TX      ( SCREEN_WIDTH * 0.5f )
#define TY      ( 500.0f )

// 大きさ
#define WIDTH   ( 534 )
#define HEIGHT  ( 162 )

/*--- 変数型定義 --------------------------------------------------------*/

typedef struct tagTITLEPREBTN
{
    LPDIRECT3DVERTEXBUFFER9 vtx;        // 頂点バッファ
    LPDIRECT3DTEXTURE9      tex;        // テクスチャ
    float                   fAlpha;     // 点滅用：アルファ加速値
    D3DXCOLOR               col;        // 色
}TITLEPREBTN;

/*--- プロトタイプ宣言 --------------------------------------------------*/

/*--- グローバル変数 ----------------------------------------------------*/

TITLEPREBTN g_TitlePreBtn;

/*************************************************************************
|
|   @Brief  : 初期化
|
|   @Param  : なし
|
|   @Return : なし
|
*------------------------------------------------------------------------*/
VOID InitTitlePreBtn( VOID )
{
    // アルファ加速値
    g_TitlePreBtn.fAlpha = 0.0f;

    // 現在のアルファ値
    g_TitlePreBtn.col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 0.0f );

    // デバイス取得
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice ->CreateVertexBuffer((sizeof( VERTEX_2D ) * 4),	// 頂点データ用に確保するバッファ
								 D3DUSAGE_WRITEONLY,				// 頂点バッファの使用方法
								 FVF_VERTEX_2D,						// 使用する頂点フォーマット
								 D3DPOOL_MANAGED,					// バッファを保持するメモリクラスの指定
								 &g_TitlePreBtn.vtx,					// 頂点バッファへのポインタ
								 NULL);								// NULL固定

	// 頂点情報の編集
	//------------------------------------
    VERTEX_2D *pVtx;
	g_TitlePreBtn.vtx->Lock( 0, 0, (void**)&pVtx, 0 );
    {
        pVtx[ 0 ].pos.x = -WIDTH  * 0.5f + TX;
        pVtx[ 0 ].pos.y = -HEIGHT * 0.5f + TY;
        pVtx[ 0 ].pos.z = 0.0f;

        pVtx[ 1 ].pos.x =  WIDTH  * 0.5f + TX;
        pVtx[ 1 ].pos.y = -HEIGHT * 0.5f + TY;
        pVtx[ 1 ].pos.z = 0.0f;

        pVtx[ 2 ].pos.x = -WIDTH  * 0.5f + TX;
        pVtx[ 2 ].pos.y =  HEIGHT * 0.5f + TY;
        pVtx[ 2 ].pos.z = 0.0f;

        pVtx[ 3 ].pos.x =  WIDTH  * 0.5f + TX;
        pVtx[ 3 ].pos.y =  HEIGHT * 0.5f + TY;
        pVtx[ 3 ].pos.z = 0.0f;

        // 法線
        pVtx[ 0 ].rhw = 1.0f;
        pVtx[ 1 ].rhw = 1.0f;
        pVtx[ 2 ].rhw = 1.0f;
        pVtx[ 3 ].rhw = 1.0f;

        // 色
        pVtx[ 0 ].col
            =  pVtx[ 1 ].col
            = pVtx[ 2 ].col
            = pVtx[ 3 ].col
            = g_TitlePreBtn.col;

        // テクスチャ
        pVtx[ 0 ].tex = D3DXVECTOR2( 0.0f, 0.0f );
        pVtx[ 1 ].tex = D3DXVECTOR2( 1.0f, 0.0f );
        pVtx[ 2 ].tex = D3DXVECTOR2( 0.0f, 1.0f );
        pVtx[ 3 ].tex = D3DXVECTOR2( 1.0f, 1.0f );
    }
    g_TitlePreBtn.vtx -> Unlock();

    // テクスチャの読み込み
    D3DXCreateTextureFromFile( pDevice, "data/TEXTURE/Title/PressButton.png", &g_TitlePreBtn.tex );
}

/*************************************************************************
|
|   @Brief  : 終了
|
|   @Param  : なし
|
|   @Return : なし
|
*------------------------------------------------------------------------*/
VOID UninitTitlePreBtn( VOID )
{
    // テクスチャバッファの解放
    if( g_TitlePreBtn.tex != NULL )
    {
        g_TitlePreBtn.tex->Release();
        g_TitlePreBtn.tex = NULL;
    }

    // 頂点バッファの解放
    if( g_TitlePreBtn.vtx != NULL )
    {
        g_TitlePreBtn.vtx->Release();
        g_TitlePreBtn.vtx = NULL;
    }
}

/*************************************************************************
|
|   @Brief  : 更新
|
|   @Param  : なし
|
|   @Return : なし
|
*------------------------------------------------------------------------*/
VOID UpdateTitlePreBtn( VOID )
{
    if( GetFade() == FADE_OUT )
    {
        g_TitlePreBtn.fAlpha += ( 1.0f - g_TitlePreBtn.col.a ) * 0.15f;
    }
    else
    {
        g_TitlePreBtn.fAlpha += ( 1.0f - g_TitlePreBtn.col.a ) * 0.01f;
    }

    g_TitlePreBtn.col.a += g_TitlePreBtn.fAlpha;

    VERTEX_2D *pVtx;
    g_TitlePreBtn.vtx->Lock( 0, 0, (void**)&pVtx, 0 );
    {
        pVtx[ 0 ].col
            = pVtx[ 1 ].col
            = pVtx[ 2 ].col
            = pVtx[ 3 ].col
            = g_TitlePreBtn.col;
    }
    g_TitlePreBtn.vtx -> Unlock();
}

/*************************************************************************
|
|   @Brief  : 描画
|
|   @Param  : なし
|
|   @Return : なし
|
*------------------------------------------------------------------------*/
VOID DrawTitlePreBtn( VOID )
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();
    pDevice->SetStreamSource( 0, g_TitlePreBtn.vtx, 0, sizeof( VERTEX_2D ) );
    pDevice->SetFVF( FVF_VERTEX_2D );
    pDevice->SetTexture( 0, g_TitlePreBtn.tex );
    pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );
}

/*************************************************************************
|   End of File
**************************************************************************/
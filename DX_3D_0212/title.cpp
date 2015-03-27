//-----------------------------------------------
//  内容：タイトル処理
//  Fail:title.cpp
//  制作者：木崎　翔
//-----------------------------------------------

//-----------------------------------------------
//  インクルードファイル
//-----------------------------------------------
#include "title.h"
#include "main.h" 
#include "input.h"
#include "fade.h"
#include "camera.h"
#include "TitlePreBtn.h"

//-----------------------------------------------
//　マクロ定義
//-----------------------------------------------

//-----------------------------------------------
//　構造体定義
//-----------------------------------------------

typedef struct tagTITLE
{
    LPDIRECT3DVERTEXBUFFER9 vtx;		// 頂点バッファへのポインタ
    LPDIRECT3DTEXTURE9      tex;		// テクスチャへのポインタ
    unsigned int            nCntFrame;  // 経過フレーム
}TITLE;

//-----------------------------------------------
//  関数プロトタイプ宣言
//-----------------------------------------------

//-----------------------------------------------
//　マクロ定義
//-----------------------------------------------

//-----------------------------------------------
//  グローバル変数
//-----------------------------------------------
TITLE g_Title;  // タイトルのセット
bool g_bTitleFade = false;

//-----------------------------------------------
// 背景初期化処理
//-----------------------------------------------
void InitTitle()
{
    // 経過フレームのリセット
    g_Title.nCntFrame = 0;

    // BGM
	PlaySound( TITLE_BGM );

	// デバイス取得
	//------------------------------------
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice ->CreateVertexBuffer((sizeof( VERTEX_2D ) * 4),	// 頂点データ用に確保するバッファ
								 D3DUSAGE_WRITEONLY,				// 頂点バッファの使用方法
								 FVF_VERTEX_2D,						// 使用する頂点フォーマット
								 D3DPOOL_MANAGED,					// バッファを保持するメモリクラスの指定
								 &g_Title.vtx,					// 頂点バッファへのポインタ
								 NULL);								// NULL固定

	// データの位置を確保する（ロック）
	//------------------------------------
    VERTEX_2D *pVtx;
	g_Title.vtx->Lock( 0, 0, (void**)&pVtx, 0 );

	// 頂点バッファの編集
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

	// 色
	pVtx[ 0 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );			// 頂点ごとの色の設定
	pVtx[ 1 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );			//			↓
	pVtx[ 2 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );			//			↓
	pVtx[ 3 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );			//			↓

    // テクスチャ
	pVtx[ 0 ].tex = D3DXVECTOR2( 0.0f, 0.0f );
	pVtx[ 1 ].tex = D3DXVECTOR2( 1.0f, 0.0f );
	pVtx[ 2 ].tex = D3DXVECTOR2( 0.0f, 1.0f );
	pVtx[ 3 ].tex = D3DXVECTOR2( 1.0f, 1.0f );

	// 編集終了
	//------------------------------------
	g_Title.vtx -> Unlock();

	// テクスチャの読み込み
	//------------------------------------
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Title/Title.jpg",	&g_Title.tex );


    // PreBtn
    InitTitlePreBtn();
}

//-----------------------------------------------
// 背景終了処理
//-----------------------------------------------
void UninitTitle()
{
    // PreBtn
    UninitTitlePreBtn();

    // BGM
	StopSound();

    // テクスチャバッファの解放
    if( g_Title.tex != NULL )
    {
        g_Title.tex->Release();
        g_Title.tex = NULL;
    }

    // 頂点バッファの解放
    if( g_Title.vtx != NULL )
    {
        g_Title.vtx->Release();
        g_Title.vtx = NULL;
    }
}

//-----------------------------------------------
// 背景更新処理
//-----------------------------------------------
void UpdateTitle()
{
    UpdateTitlePreBtn();

    g_Title.nCntFrame++;
    if( g_Title.nCntFrame >= 60 * 60 )
    {
		SetFade( FADE_OUT, 60 );
		SetMode( MODE_RANKING );
    }

	// 遷移判定
	if ( ( GetKeyboardTrigger(DIK_RETURN) || GetPadElecomTrigger( PAD_4 ) ) && GetFade() == FADE_NONE)
	{
		PlaySound( DESIDE4_SE );
		SetFade( FADE_OUT, 60 );
		SetMode( MODE_GAME );
	}
}

//-----------------------------------------------
// 背景描画処理
//-----------------------------------------------
void DrawTitle()
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();
    pDevice->SetStreamSource( 0, g_Title.vtx, 0, sizeof( VERTEX_2D ) );
    pDevice->SetFVF( FVF_VERTEX_2D );
    pDevice->SetTexture( 0, g_Title.tex );
    pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );

    DrawTitlePreBtn();
}

//-----------------------------------------------
// フェード中かどうか
//-----------------------------------------------
bool GetTitleFade( void )
{
	return g_bTitleFade;
}


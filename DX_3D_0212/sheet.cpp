//-----------------------------------------------
//  内容：タイトル処理
//  Fail:title.cpp
//  制作者：木崎　翔
//-----------------------------------------------

//-----------------------------------------------
//  インクルードファイル
//-----------------------------------------------
#include "main.h" 
#include "input.h"
#include "sheet.h"

//-----------------------------------------------
//　マクロ定義
//-----------------------------------------------

//-----------------------------------------------
//  関数プロトタイプ宣言
//-----------------------------------------------

//-----------------------------------------------
//　マクロ定義
//-----------------------------------------------

//-----------------------------------------------
//  グローバル変数
//-----------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSheet = NULL;		// 頂点バッファへのポインタ
bool g_bSheetUse = false;
D3DXCOLOR g_nSheet;

//-----------------------------------------------
// 背景初期化処理
//-----------------------------------------------
void InitSheet()
{
		LPDIRECT3DDEVICE9 pDevice;					//デバイスポインタ
	pDevice = GetDevice();

	//頂点バッファの作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*SHEET_VERTEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffSheet, NULL);

		VERTEX_2D *pVtx;
	g_pVtxBuffSheet->Lock(0, 0, (void**)&pVtx, 0);

			//頂点座標初期化
	pVtx[ 0 ].pos = D3DXVECTOR3 ( 0.0f, 0.0f, 0.0f );
	pVtx[ 1 ].pos = D3DXVECTOR3 ( SCREEN_WIDTH, 0.0f, 0.0f );
	pVtx[ 2 ].pos = D3DXVECTOR3 ( 0.0f, SCREEN_HEIGHT, 0.0f );
	pVtx[ 3 ].pos = D3DXVECTOR3 ( SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f );

		//テクスチャのパースペクティブ初期化
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラー初期化 

		g_nSheet = D3DCOLOR_RGBA( 255, 255, 255, 0 );
		pVtx[0].col = g_nSheet;
		pVtx[1].col = g_nSheet;
		pVtx[2].col = g_nSheet;
		pVtx[3].col = g_nSheet;

		//頂点テクスチャ初期化	
		pVtx[0].tex = D3DXVECTOR2(0, 0);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0);
		pVtx[2].tex = D3DXVECTOR2(0, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		g_pVtxBuffSheet->Unlock();		//頂点データをアンロック

}

//-----------------------------------------------
// 背景終了処理
//-----------------------------------------------
void UninitSheet()
{
		//頂点バッファへのポインタの廃棄
	if (g_pVtxBuffSheet != NULL){
		g_pVtxBuffSheet->Release();
		g_pVtxBuffSheet = NULL;
	}

}

//-----------------------------------------------
// 背景更新処理
//-----------------------------------------------
void UpdateSheet()
{
			LPDIRECT3DDEVICE9 pDevice;					//デバイスポインタ
	pDevice = GetDevice();

	if( GetKeyboardTrigger( DIK_L ) == true)
	{
		if( g_bSheetUse == false )
		{
			g_nSheet = D3DCOLOR_RGBA( 255, 0, 0, 100 );
			g_bSheetUse = true;
		}
		else if( g_bSheetUse == true )
		{
			g_nSheet = D3DCOLOR_RGBA( 255, 255, 255, 0 );
			g_bSheetUse = false;
		}

	VERTEX_2D *pVtx;
	g_pVtxBuffSheet->Lock(0, 0, (void**)&pVtx, 0);

		pVtx[0].col = g_nSheet;
		pVtx[1].col = g_nSheet;
		pVtx[2].col = g_nSheet;
		pVtx[3].col = g_nSheet;

		g_pVtxBuffSheet->Unlock();		//頂点データをアンロック

	}

}

//-----------------------------------------------
// 背景描画処理
//-----------------------------------------------
void DrawSheet()
{
		LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();

	pDevice->SetStreamSource(0, g_pVtxBuffSheet, 0, sizeof(VERTEX_2D));
	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice ->SetTexture( 0, NULL );

	//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//-----------------------------------------------
// フェード中かどうか
//-----------------------------------------------
bool GetSheet( void )
{
	return g_bSheetUse;
}


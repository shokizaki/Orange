//-----------------------------------------------
//  内容：弾に関する処理
//  Fail:bullet.cpp
//  制作者：木崎　翔
//-----------------------------------------------

//-----------------------------------------------
//  インクルードファイル
//-----------------------------------------------
#include "main.h"
#include "bullet.h"
#include "effect.h"
#include "enemy.h"
#include "collision.h"

//-----------------------------------------------
//　マクロ定義
//-----------------------------------------------
#define VERTEX_MAX  (8)			// 頂点の数
#define TEXTURE_MAX ( 3 )			// 使用するテクスチャの数
#define UP_LIMIT (0)			// 上の限界値
#define EFFECT_WIDTH (50)		// 弾の幅
#define EFFECT_HEIGHT (50)		// 弾の高さ

//-----------------------------------------------
//  グローバル変数
//-----------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffectEx = NULL;		// 頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureEffectEx[TEXTURE_MAX];		// テクスチャへのポインタ
EFFECT g_aEffectEx[EFFECTEX_MAX];						// EFFECT構造体型変数

//-----------------------------------------------
// バレットに関する初期化
//-----------------------------------------------
void InitEffectEx()
{
	// ローカル変数
	//---------------------------------
	LPDIRECT3DDEVICE9 pDevice = NULL;		// デバイスオブジェクト
	VERTEX_2D *pVtx;

	// デバイス取得
	//------------------------------------
	pDevice = GetDevice();
	
	pDevice ->CreateVertexBuffer((sizeof(VERTEX_2D) * (EFFECTEX_MAX * 4)),	// 頂点データ用に確保するバッファ
								 D3DUSAGE_WRITEONLY,				// 頂点バッファの使用方法
								 FVF_VERTEX_2D,						// 使用する頂点フォーマット
								 D3DPOOL_MANAGED,					// バッファを保持するメモリクラスの指定
								 &g_pVtxBuffEffectEx,					// 頂点バッファへのポインタ
								 NULL);								// NULL固定

	// データの位置を確保する（ロック）
	//------------------------------------
	g_pVtxBuffEffectEx ->Lock(0,
						0,
						(void**)&pVtx,
						0);

	// テクスチャへのポインタを初期化
	//------------------------------------
	for (int nCnt = 0; nCnt < TEXTURE_MAX; nCnt++)				
	{
		g_pTextureEffectEx[nCnt] = NULL;
	}

	// テクスチャの読み込み
	//------------------------------------
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ef000.png", &g_pTextureEffectEx[ 0 ]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ef001.png", &g_pTextureEffectEx[ 1 ]);

	// 解放
	//------------------------------------
	g_pVtxBuffEffectEx -> Unlock();		

	// バレットの頂点情報セット
	//------------------------------------
	SetupEffectEx();
}

// バレットの頂点情報セット
void SetupEffectEx(void)
{
	// ローカル変数
	//---------------------------------
	LPDIRECT3DDEVICE9 pDevice;		// デバイスオブジェクト
	int nCnt;						// カウンタ変数
	VERTEX_2D *pVtx;				// 頂点の構造体ポインタ

	// デバイス取得
	//------------------------------------
	pDevice = GetDevice();

	// データの位置を確保する（ロック）
	//------------------------------------
	g_pVtxBuffEffectEx ->Lock(0,
						0,
						(void**)&pVtx,
						0);

	for (nCnt = 0; nCnt < EFFECTEX_MAX; nCnt++)
	{
		if ( g_aEffectEx[ nCnt ].bUse == true )
		{
			// 弾
			//------------------------------------
			pVtx[0 + (nCnt * 4)].pos.x = g_aEffectEx[nCnt].pos.x - g_aEffectEx[nCnt].fWidth;	// 頂点の位置
			pVtx[0 + (nCnt * 4)].pos.y = g_aEffectEx[nCnt].pos.y - g_aEffectEx[nCnt].fHeight;
			pVtx[0 + (nCnt * 4)].pos.z = 0.0f;
			pVtx[1 + (nCnt * 4)].pos.x = g_aEffectEx[nCnt].pos.x + g_aEffectEx[nCnt].fWidth;	// 頂点の位置
			pVtx[1 + (nCnt * 4)].pos.y = g_aEffectEx[nCnt].pos.y - g_aEffectEx[nCnt].fHeight;
			pVtx[1 + (nCnt * 4)].pos.z = 0.0f;
			pVtx[2 + (nCnt * 4)].pos.x = g_aEffectEx[nCnt].pos.x - g_aEffectEx[nCnt].fWidth;	// 頂点の位置
			pVtx[2 + (nCnt * 4)].pos.y = g_aEffectEx[nCnt].pos.y + g_aEffectEx[nCnt].fHeight;
			pVtx[2 + (nCnt * 4)].pos.z = 0.0f;
			pVtx[3 + (nCnt * 4)].pos.x = g_aEffectEx[nCnt].pos.x + g_aEffectEx[nCnt].fWidth;	// 頂点の位置
			pVtx[3 + (nCnt * 4)].pos.y = g_aEffectEx[nCnt].pos.y + g_aEffectEx[nCnt].fHeight;
			pVtx[3 + (nCnt * 4)].pos.z = 0.0f;

			pVtx[0 + nCnt * 4].rhw = 1.0f;			// テクスチャのパースペクティブコレクトを1.0fで固定
			pVtx[1 + nCnt * 4].rhw = 1.0f;			// テクスチャのパースペクティブコレクトを1.0fで固定
			pVtx[2 + nCnt * 4].rhw = 1.0f;			// テクスチャのパースペクティブコレクトを1.0fで固定
			pVtx[3 + nCnt * 4].rhw = 1.0f;			// テクスチャのパースペクティブコレクトを1.0fで固定

			// 弾
			//------------------------------------
			pVtx[0 + (nCnt * 4)].col = D3DXCOLOR(g_aEffectEx[nCnt].r, g_aEffectEx[nCnt].g, g_aEffectEx[nCnt].b, g_aEffectEx[nCnt].a);			// 頂点ごとの色の設定
			pVtx[1 + (nCnt * 4)].col = D3DXCOLOR(g_aEffectEx[nCnt].r, g_aEffectEx[nCnt].g, g_aEffectEx[nCnt].b, g_aEffectEx[nCnt].a);			//			↓
			pVtx[2 + (nCnt * 4)].col = D3DXCOLOR(g_aEffectEx[nCnt].r, g_aEffectEx[nCnt].g, g_aEffectEx[nCnt].b, g_aEffectEx[nCnt].a);			//			↓
			pVtx[3 + (nCnt * 4)].col = D3DXCOLOR(g_aEffectEx[nCnt].r, g_aEffectEx[nCnt].g, g_aEffectEx[nCnt].b, g_aEffectEx[nCnt].a);			//			↓
		
			//switch ( g_aEffectEx[ nCnt ].effectTex )
			//{
			//case EFFECT_000:
			//	// 弾
			//	//------------------------------------
			//	pVtx[0 + (nCnt * 4)].tex = D3DXVECTOR2(0.0f,0.0f);			// 頂点の位置に対するテクスチャ座標の位置
			//	pVtx[1 + (nCnt * 4)].tex = D3DXVECTOR2(1.0f,0.0f);			//					↓
			//	pVtx[2 + (nCnt * 4)].tex = D3DXVECTOR2(0.0f,1.0f);			//					↓
			//	pVtx[3 + (nCnt * 4)].tex = D3DXVECTOR2(1.0f,1.0f);			//					↓

			//	break;

			//case EFFECT_001:
			//	// 弾
			//	//------------------------------------
			//	pVtx[0 + (nCnt * 4)].tex = D3DXVECTOR2(0.0f,0.0f);			// 頂点の位置に対するテクスチャ座標の位置
			//	pVtx[1 + (nCnt * 4)].tex = D3DXVECTOR2(1.0f,0.0f);			//					↓
			//	pVtx[2 + (nCnt * 4)].tex = D3DXVECTOR2(0.0f,1.0f);			//					↓
			//	pVtx[3 + (nCnt * 4)].tex = D3DXVECTOR2(1.0f,1.0f);			//					↓

			//	break;

			//case EFFECT_002:
			//	// 弾
			//	//------------------------------------
			//	pVtx[0 + (nCnt * 4)].tex = D3DXVECTOR2(0.0f,0.0f);			// 頂点の位置に対するテクスチャ座標の位置
			//	pVtx[1 + (nCnt * 4)].tex = D3DXVECTOR2(1.0f,0.0f);			//					↓
			//	pVtx[2 + (nCnt * 4)].tex = D3DXVECTOR2(0.0f,1.0f);			//					↓
			//	pVtx[3 + (nCnt * 4)].tex = D3DXVECTOR2(1.0f,1.0f);			//					↓

			//	break;
			//}

			// 弾
			//------------------------------------
			pVtx[0 + (nCnt * 4)].tex = D3DXVECTOR2(0.0f,0.0f);			// 頂点の位置に対するテクスチャ座標の位置
			pVtx[1 + (nCnt * 4)].tex = D3DXVECTOR2(1.0f,0.0f);			//					↓
			pVtx[2 + (nCnt * 4)].tex = D3DXVECTOR2(0.0f,1.0f);			//					↓
			pVtx[3 + (nCnt * 4)].tex = D3DXVECTOR2(1.0f,1.0f);			//					↓

		}
	}

	// 解放
	//------------------------------------
	g_pVtxBuffEffectEx -> Unlock();		
}

//-----------------------------------------------
// バレットに関する終了処理
//-----------------------------------------------
void UninitEffectEx()
{
	// ローカル変数
	//------------------------------------
	int nCnt;

	for (nCnt = 0; nCnt < TEXTURE_MAX; nCnt++)
	{
		if (g_pTextureEffectEx[nCnt] != NULL)
		{ // 使ったテクスチャの解放と初期化
		  //------------------------------------
			g_pTextureEffectEx[nCnt] -> Release();
			g_pTextureEffectEx[nCnt] = NULL;
		}
	}

	if (g_pVtxBuffEffectEx != NULL)
	{ // 頂点バッファの解放と初期化
	  //------------------------------------
		g_pVtxBuffEffectEx->Release();
		g_pVtxBuffEffectEx = NULL;
	}

	// エフェクトの初期化
	//-------------------------------------------
	for (nCnt = 0; nCnt < EFFECTEX_MAX; nCnt++)
	{
		g_aEffectEx[nCnt].pos.x = 0;
		g_aEffectEx[nCnt].pos.y = 0;
		g_aEffectEx[nCnt].nLife = 0;
		g_aEffectEx[nCnt].bUse = false;
		g_aEffectEx[nCnt].bCol = false;
	}
}	

//-----------------------------------------------
// バレットに関する更新処理
//-----------------------------------------------
void UpdateEffectEx()
{
	// ローカル変数
	//------------------------------------
	int nCnt = 0;

	for (nCnt = 0; nCnt < EFFECTEX_MAX; nCnt++)
	{	
		if (g_aEffectEx[nCnt].bUse == true)
		{
			//g_aEffectEx[nCnt].nLife -= 1;

			g_aEffectEx[ nCnt ].a -= 0.02f;

			if (g_aEffectEx[nCnt].a <= 0)
			{
				g_aEffectEx[nCnt].bUse = false;
			}
		}
	}

	// バレットの頂点情報セット
	//------------------------------------
	SetupEffectEx();
}

//-----------------------------------------------
// バレットに関する描画処理
//-----------------------------------------------
void DrawEffectEx()
{
	int nCnt = 0;
	LPDIRECT3DDEVICE9 pDevice;		// デバイスオブジェクト
	
	// デバイスの取得
	//------------------------------------
	pDevice = GetDevice();

	// 加算合成
	//------------------------------------
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// 頂点バッファをデータストリームにバインド
	//-----------------------------------------
	pDevice ->SetStreamSource(0,
								g_pVtxBuffEffectEx,
								0,
								sizeof(VERTEX_2D));	// 頂点データの間隔

	// 頂点フォーマットの設定
	//------------------------------------
	pDevice -> SetFVF(FVF_VERTEX_2D);

	for (nCnt = 0; nCnt < EFFECTEX_MAX; nCnt++)
	{
		if (g_aEffectEx[nCnt].bUse == true)
		{		

			//switch ( g_aEffectEx[ nCnt ].effectTex )
			//{
			//case EFFECT_000:
			//	// テクスチャの設定（背景）
			//	//------------------------------------
			//	pDevice -> SetTexture(0, g_pTextureEffectEx[ 0 ]);

			//	break;

			//case EFFECT_001:
			//	// テクスチャの設定（背景）
			//	//------------------------------------
			//	pDevice -> SetTexture(0, g_pTextureEffectEx[ 0 ]);

			//	break;

			//case EFFECT_002:
			//	// テクスチャの設定（背景）
			//	//------------------------------------
			//	pDevice -> SetTexture(0, g_pTextureEffectEx[ 0 ]);

			//	break;
			//}

			// テクスチャの設定（背景）
			//------------------------------------
			pDevice -> SetTexture(0, g_pTextureEffectEx[ 0 ]);

			// ポリゴンの描画
			//------------------------------------
			pDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,	// プリミティブの種類
										  nCnt * 4,			// ロードする最初の頂点のインデックス
										  2);				// 描画するプリミティブ数
		}
	}

	// レンダーステートの設定
	////------------------------------------
	//pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// カリングの設定
	//pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// αブレンドを行う
	//pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// 透明度の割合
	//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// 透明度の割合
	
	pDevice-> SetRenderState(D3DRS_SRCBLEND	, D3DBLEND_SRCALPHA);
	pDevice-> SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//=====================================================================
//
// エフェクトの生成
//
// （引数）
//  表示位置、消えるまでのフレーム時間、幅、高さ
//
//=====================================================================
void SetEffectEx(D3DXVECTOR3 pos, int nLife, float fWidth, float fHeight )
{
	int nCnt = 0;

	for (nCnt = 0; nCnt < EFFECTEX_MAX; nCnt++)
	{
		if (g_aEffectEx[nCnt].bUse == false)
		{
			g_aEffectEx[nCnt].pos.x = pos.x;
			g_aEffectEx[nCnt].pos.y = pos.y;
			g_aEffectEx[nCnt].nLife = nLife;
			g_aEffectEx[nCnt].fWidth = fWidth;
			g_aEffectEx[nCnt].fHeight = fHeight;
			g_aEffectEx[nCnt].r = 1.0f;
			g_aEffectEx[nCnt].g = 1.0f;
			g_aEffectEx[nCnt].b = 1.0f;
			g_aEffectEx[nCnt].a = 1.0f;
			g_aEffectEx[nCnt].bUse = true;

			// バレットの頂点情報セット
			//------------------------------------
			SetupEffectEx();

			break;	// 見つけたら出る
		}
	}
}	

EFFECT *GetEffectEx()
{
	return (&g_aEffectEx[0]);
}
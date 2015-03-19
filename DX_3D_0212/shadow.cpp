//===============================================
//
//  内容   ： インデックス化した
//				メッシュ影に関する処理
//  File   ： shadow.cpp
//  制作者 ： 木崎　翔
//
//===============================================

//===============================================
// 
//  インクルードファイル
//
//===============================================
#include "shadow.h"
#include "model.h"
#include "input.h"
#include "camera.h"
#include "billboard.h"
#include "enemy.h"

//===============================================
//
//　マクロ定義
//
//===============================================
#define TEXTURE_MAX 	( 1 )			// 使用するテクスチャの数
#define SHADOW_MAX		( 100 )			// 影の最大数
#define TEX_PASS		"data/TEXTURE/shadow000.jpg"

//-----------------------------------------------
//  グローバル変数
//-----------------------------------------------
LPDIRECT3DTEXTURE9		g_pTextureShadow[TEXTURE_MAX];			// テクスチャへのポインタ

LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffShadow = NULL;				// 頂点バッファへのポインタ
SHADOW g_shadow[SHADOW_MAX];

static D3DXMATRIX g_mtxProjection;								// プロジェクションマトリックス

//===============================================
//
// 影に関する初期化
//
//===============================================
void InitShadow()	
{
	// ローカル変数
	//---------------------------------
	LPDIRECT3DDEVICE9 pDevice = NULL;		// デバイスオブジェクト
	
	// デバイス取得
	//------------------------------------
	pDevice = GetDevice();

	// 頂点バッファの確保
	//------------------------------------
	pDevice ->CreateVertexBuffer((sizeof(VERTEX_3D) * SHADOW_MAX * 4),	// 頂点データ用に確保するバッファ
								 D3DUSAGE_WRITEONLY,					// 頂点バッファの使用方法
								 FVF_VERTEX_3D,							// 使用する頂点フォーマット
								 D3DPOOL_MANAGED,						// バッファを保持するメモリクラスの指定
								 &g_pVtxBuffShadow,						// 頂点バッファへのポインタ
								 NULL);									// NULL固定

	// 影初期化
	//------------------------------------
	g_shadow[0].posShadow = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_shadow[0].rotShadow = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_shadow[0].sclShadow = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	/*for ( int i = 0; i < SHADOW_MAX; i++ )
	{
		g_shadow[ i ].bUse = false;
	}*/

	// テクスチャへのポインタを初期化
	//------------------------------------
	for (int nCnt = 0; nCnt < TEXTURE_MAX; nCnt++)				
	{
		g_pTextureShadow[nCnt] = NULL;
	}

	// テクスチャの読み込み
	//------------------------------------
	D3DXCreateTextureFromFile(pDevice, TEX_PASS, &g_pTextureShadow[ 0 ]);
}

//===============================================
//
// 影の頂点情報セット(インデックス指定)
//
//===============================================
void SetupShadowEx( int num )
{
	// ローカル変数
	//------------------------------------
	VERTEX_3D *pVtx = NULL;

	// データの位置を確保する（ロック）
	//------------------------------------
	g_pVtxBuffShadow ->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0 + num * 4].pos = D3DXVECTOR3(-g_shadow[num].fWidth, 0.1f, g_shadow[num].fHeight);
	pVtx[1 + num * 4].pos = D3DXVECTOR3(g_shadow[num].fWidth,  0.1f, g_shadow[num].fHeight);
	pVtx[2 + num * 4].pos = D3DXVECTOR3(-g_shadow[num].fWidth, 0.1f, -g_shadow[num].fHeight);
	pVtx[3 + num * 4].pos = D3DXVECTOR3(g_shadow[num].fWidth,  0.1f, -g_shadow[num].fHeight);
	
	pVtx[0 + num * 4].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1 + num * 4].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2 + num * 4].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3 + num * 4].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	pVtx[0 + num * 4].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, g_shadow[ num ].a );
	pVtx[1 + num * 4].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, g_shadow[ num ].a );
	pVtx[2 + num * 4].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, g_shadow[ num ].a );
	pVtx[3 + num * 4].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, g_shadow[ num ].a );

	pVtx[0 + num * 4].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1 + num * 4].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2 + num * 4].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3 + num * 4].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点情報のアンロック
	//------------------------------------
 	g_pVtxBuffShadow -> Unlock();
}

//===============================================
//
// 影に関する終了処理
//
//===============================================
void UninitShadow()
{
	// ローカル変数
	//------------------------------------
	int nCnt;

	for (nCnt = 0; nCnt < TEXTURE_MAX; nCnt++)
	{
		if (g_pTextureShadow[nCnt] != NULL)
		{ 
			// 使ったテクスチャの解放と初期化
			//------------------------------------
			g_pTextureShadow[nCnt] -> Release();
			g_pTextureShadow[nCnt] = NULL;
		}
	}

	if (g_pVtxBuffShadow != NULL)
	{ 
		// 頂点バッファの解放と初期化
		//------------------------------------
		g_pVtxBuffShadow -> Release();
		g_pVtxBuffShadow = NULL;
	}

	for (int nCnt = 0; nCnt < SHADOW_MAX; nCnt++)	
	{
		g_shadow[nCnt].bUse = false;
	}
}	

//===============================================
//
// 影に関する更新処理
//
//===============================================
void UpdateShadowEx( int num, D3DXVECTOR3 pos )
{
	//// ポジションの更新
	//g_shadow[ num ].posShadow = D3DXVECTOR3( pos.x, 0.1f, pos.z );

	//// 頂点情報のセット
	//SetupShadowEx( num );
	
}

//===============================================
//
// 影に関する描画処理
//
//===============================================
void DrawShadow( void )
{
	// ローカル変数
	//------------------------------------
	LPDIRECT3DDEVICE9 pDevice;						// デバイスオブジェクト
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;		// 計算用
	CAMERA *pCamera = GetCamera();					// カメラ取得
	
	// デバイスの取得
	//------------------------------------
	pDevice = GetDevice();

	// カメラの設定
	//------------------------------------
	SetCamera();

	// レンダーステートの設定（減算合成）
	//------------------------------------
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// アルファテストの有効化
	pDevice ->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice ->SetRenderState(D3DRS_ALPHAREF,		10);
	pDevice ->SetRenderState(D3DRS_ALPHAFUNC,		D3DCMP_GREATER);

	// Zバッファ処理
	//pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_GREATER);
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	for (int nCnt = 0; nCnt < SHADOW_MAX; nCnt++)
	{
		if (g_shadow[nCnt].bUse == true)
		{
			// ワールドマトリックスの設定
			//------------------------------------
			D3DXMatrixIdentity(&g_shadow[nCnt].mtxWorld);			// フォーマットの初期化

			//==============================================
			// 
			// 反映する順番は厳守！！
			// 
			//==============================================

			// スケールを反映
			D3DXMatrixScaling(&mtxScl, g_shadow[nCnt].sclShadow.x, g_shadow[nCnt].sclShadow.y, g_shadow[nCnt].sclShadow.z);
			D3DXMatrixMultiply(&g_shadow[nCnt].mtxWorld, &g_shadow[nCnt].mtxWorld, &mtxScl);

			// 向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_shadow[nCnt].rotShadow.y, g_shadow[nCnt].rotShadow.x, g_shadow[nCnt].rotShadow.z);
			D3DXMatrixMultiply(&g_shadow[nCnt].mtxWorld, &g_shadow[nCnt].mtxWorld, &mtxRot);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTranslate, g_shadow[nCnt].posShadow.x, g_shadow[nCnt].posShadow.y, g_shadow[nCnt].posShadow.z);
			D3DXMatrixMultiply(&g_shadow[nCnt].mtxWorld, &g_shadow[nCnt].mtxWorld, &mtxTranslate);

			// 設定
			pDevice->SetTransform(D3DTS_WORLD, &g_shadow[nCnt].mtxWorld);

			// 頂点バッファをデータストリームにバインド
			//-----------------------------------------
			pDevice ->SetStreamSource(0,
										g_pVtxBuffShadow,
										0,
										sizeof(VERTEX_3D));	// 頂点データの間隔

			// 頂点フォーマットの設定
			//------------------------------------
			pDevice -> SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			//------------------------------------
			pDevice -> SetTexture(0, g_pTextureShadow[ 0 ]);

			// ポリゴンの描画
			//------------------------------------
			pDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,	// プリミティブの種類
											nCnt * 4,		// 
											2);				// 描画するプリミティブ数
		}
	}

	// 通常ブレンドに戻す
	//------------------------------------
	pDevice-> SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice-> SetRenderState(D3DRS_SRCBLEND	, D3DBLEND_SRCALPHA);
	pDevice-> SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//===============================================
//
// 影のセット(インデックスを返す)
//
//===============================================
int SetShadowEx(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scl, float fWidth, float fHeight, float a)
{
	int nCnt;

	for (nCnt = 0; nCnt < SHADOW_MAX; nCnt++)	
	{
		if (g_shadow[nCnt].bUse == false)
		{
			g_shadow[nCnt].posShadow = pos;
			g_shadow[nCnt].rotShadow = rot;
			g_shadow[nCnt].sclShadow = scl;
			
			g_shadow[nCnt].a = a;

			g_shadow[nCnt].fWidth = fWidth;
			g_shadow[nCnt].fHeight = fHeight;

			g_shadow[nCnt].bUse = true;

			break;
		}
	}

	// インデックスを返す
	return nCnt;
}

//===============================================
//
// 影の情報を取得する
//
//===============================================
SHADOW *GetShadow()
{
	return &g_shadow[0];
}

//===============================================
//
// 影の使用Off
//
//===============================================
void UseShadow( int num )
{
	g_shadow[ num ].bUse = false;
}


void UpdateShadow( void )
{
	for ( int i = 0; i < SHADOW_MAX; i++ )
	{
		if ( g_shadow[ i ].bUse == true )
		{
			g_shadow[ i ].bUse = false;
		}
	}
}

void SetShadow(D3DXVECTOR3 pos, float fWidth, float fHeight, float a)
{
	int nCnt = 0;
	for (nCnt = 0; nCnt < SHADOW_MAX; nCnt++)	
	{
		if (g_shadow[nCnt].bUse == false)
		{
			g_shadow[nCnt].posShadow = pos;
			g_shadow[nCnt].rotShadow = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
			g_shadow[nCnt].sclShadow = D3DXVECTOR3( 1.0f, 1.0f, 1.0f );
			
			g_shadow[nCnt].a = a;

			g_shadow[nCnt].fWidth = fWidth;
			g_shadow[nCnt].fHeight = fHeight;

			g_shadow[nCnt].bUse = true;

			break;
		}
	}

	SetupShadowEx( nCnt );
}
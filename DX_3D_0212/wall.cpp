//===============================================
//
//  内容   ： インデックス化した
//				メッシュフィールドに関する処理
//  File   ： meshField.cpp
//  制作者 ： 木崎　翔
//
//===============================================

//===============================================
// 
//  インクルードファイル
//
//===============================================
#include "meshField.h"
#include "input.h"
#include "camera.h"
#include "wall.h"

//===============================================
//
//　マクロ定義
//
//===============================================
#define TEXTURE_MAX 	(1)			// 使用するテクスチャの数
#define WALL_MAX		(4)			// フィールドの最大数
#define TEX_PASS		"data/TEXTURE/wall000.jpg"

#define NUM_X (4)
#define NUM_Z (2)

#define WIDTH (750.0f)
#define HEIGHT (100.0f)

//-----------------------------------------------
//  グローバル変数
//-----------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWall = NULL;				// 頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffWall = NULL;
LPDIRECT3DTEXTURE9		g_pTextureWall[TEXTURE_MAX];		// テクスチャへのポインタ

WALL g_wall[WALL_MAX];

static D3DXMATRIX g_mtxProjection;		// プロジェクションマトリックス
static D3DXMATRIX g_mtxView;			// ビューマトリックス

static int g_nNumVtx = 0;
static int g_nNumIdx = 0;
static int g_nMaxVtx = 0;
static int g_nDrawPrimitiveNum;

//===============================================
//
// フィールドに関する初期化
//
//===============================================
void InitWall()	
{
	// ローカル変数
	//---------------------------------
	LPDIRECT3DDEVICE9 pDevice = NULL;		// デバイスオブジェクト
	g_nNumVtx = 0;
	g_nNumIdx = 0;
	g_nMaxVtx = (NUM_X + 1) * (NUM_Z + 1);
	g_nDrawPrimitiveNum = (NUM_X * NUM_Z * 2) + ( (NUM_Z - 1) * 4 );

	// デバイス取得
	//------------------------------------
	pDevice = GetDevice();

	// 頂点バッファの確保
	//------------------------------------
	pDevice ->CreateVertexBuffer((sizeof(VERTEX_3D) * g_nMaxVtx),		// 頂点データ用に確保するバッファ
								 D3DUSAGE_WRITEONLY,					// 頂点バッファの使用方法
								 FVF_VERTEX_3D,							// 使用する頂点フォーマット
								 D3DPOOL_MANAGED,						// バッファを保持するメモリクラスの指定
								 &g_pVtxBuffWall,					// 頂点バッファへのポインタ
								 NULL);									// NULL固定	

	// インデックスバッファの確保
	//------------------------------------
	pDevice->CreateIndexBuffer(( sizeof(WORD) * ( g_nDrawPrimitiveNum + 2 ) ),	// 確保するバッファサイズ
							   D3DUSAGE_WRITEONLY,								// 頂点バッファの使用方法
							   D3DFMT_INDEX16,									// 使用する頂点フォーマット
							   D3DPOOL_MANAGED,									// バッファを保持するメモリクラスの指定
							   &g_pIdxBuffWall,							// インデックスバッファへのポインタ
							   NULL);

	// フィールド初期化
	//------------------------------------
	g_wall[0].posWall = D3DXVECTOR3(-750.0f, 0.0f, 1500.0f);
	g_wall[0].rotWall = D3DXVECTOR3(-(D3DX_PI / 2), 0.0f, 0.0f);
	g_wall[0].sclWall = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	g_wall[1].posWall = D3DXVECTOR3(-1500.0f, 0.0f, -750.0f);
	g_wall[1].rotWall = D3DXVECTOR3(-(D3DX_PI / 2), -(D3DX_PI / 2), 0.0f);
	g_wall[1].sclWall = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	g_wall[2].posWall = D3DXVECTOR3(1500.0f, 0.0f, 750.0f);
	g_wall[2].rotWall = D3DXVECTOR3(-(D3DX_PI / 2),  (D3DX_PI / 2), 0.0f);
	g_wall[2].sclWall = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	g_wall[3].posWall = D3DXVECTOR3(750.0f, 0.0f, -1500.0f);
	g_wall[3].rotWall = D3DXVECTOR3(-(D3DX_PI / 2), (D3DX_PI), 0.0f);
	g_wall[3].sclWall = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	// テクスチャへのポインタを初期化
	//------------------------------------
	for (int nCnt = 0; nCnt < TEXTURE_MAX; nCnt++)				
	{
		g_pTextureWall[nCnt] = NULL;
	}

	// テクスチャの読み込み
	//------------------------------------
	D3DXCreateTextureFromFile(pDevice, TEX_PASS, &g_pTextureWall[0]);

	// フィールドの頂点情報とインデックスのセット
	//-------------------------------------------
	SetupWall();
}

//===============================================
//
// フィールドの頂点情報セット
//
//===============================================
void SetupWall()
{
	VERTEX_3D *pVtx;

	// データの位置を確保する（ロック）
	//------------------------------------
	g_pVtxBuffWall ->Lock(0, 0, (void**)&pVtx, 0);

	//==============================================
	// 
	// メッシュフィールド情報設定
	// 
	//==============================================

	// ループ用変数
	int nCntX = 0;
	int nCntZ = 0;

	// 頂点情報に使用するワーク
	float fStartX = 0 - (NUM_Z / 2) * WIDTH;
	float fStartZ = 0 + (NUM_X / 2) * HEIGHT;

	// 頂点情報の設定
	//------------------------------------
	for (nCntZ = 0; nCntZ < (NUM_Z + 1); nCntZ++)
	{
		for (nCntX = 0; nCntX < (NUM_X + 1); nCntX++)
		{
			// 頂点座標
			pVtx[g_nNumVtx].pos = D3DXVECTOR3( fStartX + (nCntX * WIDTH), 0.0f, fStartZ - (nCntZ * HEIGHT));

			// 法線
			pVtx[g_nNumVtx].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

			// 色
			pVtx[g_nNumVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// テクスチャ座標
			pVtx[g_nNumVtx].tex = D3DXVECTOR2((float)nCntX, (float)nCntZ);

			// 頂点情報の個数をインクリメント
			g_nNumVtx++;
		}
	}

	// インデックスに使用する変数
	//------------------------------------
	WORD *pIdx = NULL;

	// インデックスバッファのロック
	//------------------------------------
	g_pIdxBuffWall->Lock(0, 0, (void**)&pIdx, 0);

	// インデックスの設定
	//------------------------------------
	for (nCntZ = 0; nCntZ < NUM_Z; nCntZ++)
	{
		for (nCntX = 0; nCntX < (NUM_X + 1); nCntX++)
		{
			pIdx[g_nNumIdx] = (NUM_X + 1) + nCntX + nCntZ * (NUM_X + 1);
			g_nNumIdx++;

			pIdx[g_nNumIdx] = nCntX + nCntZ * (NUM_X + 1);
			g_nNumIdx++;
		}

		// 折り返す必要があったら
		//-----------------------------------
		if (nCntZ != NUM_Z - 1)
		{
			pIdx[g_nNumIdx] = pIdx[g_nNumIdx - 1];
			g_nNumIdx++;

			pIdx[g_nNumIdx] = nCntX + ((nCntZ + 1) * nCntX);
			g_nNumIdx++;
		}
	}

	// インデックスバッファのアンロック
	//------------------------------------
	g_pIdxBuffWall->Unlock();

	// 頂点情報のアンロック
	//------------------------------------
	g_pVtxBuffWall -> Unlock();
}

//===============================================
//
// フィールドに関する終了処理
//
//===============================================
void UninitWall()
{
	// ローカル変数
	//------------------------------------
	int nCnt;

	for (nCnt = 0; nCnt < TEXTURE_MAX; nCnt++)
	{
		if (g_pTextureWall[nCnt] != NULL)
		{ 
			// 使ったテクスチャの解放と初期化
			//------------------------------------
			g_pTextureWall[nCnt] -> Release();
			g_pTextureWall[nCnt] = NULL;
		}
	}

	if (g_pVtxBuffWall != NULL)
	{ 
		// 頂点バッファの解放と初期化
		//------------------------------------
		g_pVtxBuffWall -> Release();
		g_pVtxBuffWall = NULL;
	}

	if (g_pIdxBuffWall != NULL)
	{
		g_pIdxBuffWall -> Release();
		g_pIdxBuffWall = NULL;
	}
}	

//===============================================
//
// フィールドに関する更新処理
//
//===============================================
void UpdateWall()
{
	
}

//===============================================
//
// フィールドに関する描画処理
//
//===============================================
void DrawWall()
{
	// ローカル変数
	//------------------------------------
	LPDIRECT3DDEVICE9 pDevice;						// デバイスオブジェクト
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;		// 計算用
	CAMERA *pCamera = GetCamera();
	
	// デバイスの取得
	//------------------------------------
	pDevice = GetDevice();

	// カメラの設定
	//------------------------------------
	SetCamera();

	for (int nCnt = 0; nCnt < WALL_MAX; nCnt++)
	{
		// ワールドマトリックスの設定
		//------------------------------------
		D3DXMatrixIdentity(&g_wall[nCnt].mtxWorldWall);			// フォーマットの初期化

		//==============================================
		// 
		// 反映する順番は厳守！！
		// 
		//==============================================

		// スケールを反映
		D3DXMatrixScaling(&mtxScl, g_wall[nCnt].sclWall.x, g_wall[nCnt].sclWall.y, g_wall[nCnt].sclWall.z);
		D3DXMatrixMultiply(&g_wall[nCnt].mtxWorldWall, &g_wall[nCnt].mtxWorldWall, &mtxScl);

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_wall[nCnt].rotWall.y, g_wall[nCnt].rotWall.x, g_wall[nCnt].rotWall.z);
		D3DXMatrixMultiply(&g_wall[nCnt].mtxWorldWall, &g_wall[nCnt].mtxWorldWall, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTranslate, g_wall[nCnt].posWall.x, g_wall[nCnt].posWall.y, g_wall[nCnt].posWall.z);
		D3DXMatrixMultiply(&g_wall[nCnt].mtxWorldWall, &g_wall[nCnt].mtxWorldWall, &mtxTranslate);

		// 設定
		pDevice->SetTransform(D3DTS_WORLD, &g_wall[nCnt].mtxWorldWall);

		// 頂点バッファをデータストリームにバインド
		//-----------------------------------------
		pDevice ->SetStreamSource(0,
									g_pVtxBuffWall,
									0,
									sizeof(VERTEX_3D));	// 頂点データの間隔

		// インデックスバッファのバインド
		//------------------------------------
		pDevice->SetIndices(g_pIdxBuffWall);

		// 頂点フォーマットの設定
		//------------------------------------
		pDevice -> SetFVF(FVF_VERTEX_3D);

		// テクスチャの設定
		//------------------------------------
		pDevice -> SetTexture(0, g_pTextureWall[0]);

		// ポリゴンの描画
		//------------------------------------
		pDevice -> DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,	// プリミティブの種類
										0,						// 
										0,						// 
										g_nMaxVtx,				// 頂点数
										0,						// 
										g_nDrawPrimitiveNum);	// 描画するプリミティブ数
	}
}

//===============================================
//
// 壁の情報を取得
//
//===============================================
WALL *GetWall(void)
{
	return &g_wall[0];
}
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
#include "camera.h"

//===============================================
//
//　マクロ定義
//
//===============================================
#define TEXTURE_MAX 	(1)			// 使用するテクスチャの数
#define FIELD_MAX		(4)			// フィールドの最大数
#define TEX_PASS		"data/TEXTURE/field001.jpg"

//-----------------------------------------------
//  グローバル変数
//-----------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshField = NULL;				// 頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshField = NULL;
LPDIRECT3DTEXTURE9		g_pTextureMeshField[TEXTURE_MAX];		// テクスチャへのポインタ

MESHFIELD g_meshField;

D3DXVECTOR3 g_posMeshField;			// 位置
D3DXVECTOR3 g_rotMeshField;			// 向き
D3DXVECTOR3 g_sclMeshField;			// 大きさ

static D3DXMATRIX g_mtxProjection;		// プロジェクションマトリックス
static D3DXMATRIX g_mtxView;			// ビューマトリックス
static D3DXMATRIX g_mtxWorld;			// ワールドマトリックス

int g_nNumVtx = 0;
int g_nNumIdx = 0;
int g_nMaxVtx = 0;
int g_nDrawPrimitiveNum;

//===============================================
//
// フィールドに関する初期化
//
//===============================================
void InitMeshField(int numX, int numZ, float fWidth, float fHeight)	
{
	// ローカル変数
	//---------------------------------
	LPDIRECT3DDEVICE9 pDevice = NULL;		// デバイスオブジェクト

	// グローバル変数の初期化
	//------------------------------------
	g_nNumVtx = 0;
	g_nNumIdx = 0;
	g_nMaxVtx = (numX + 1) * (numZ + 1);
	g_nDrawPrimitiveNum = (numX * numZ * 2) + ( (numZ - 1) * 4 );

	// デバイス取得
	//------------------------------------
	pDevice = GetDevice();

	// 頂点バッファの確保
	//------------------------------------
	pDevice ->CreateVertexBuffer((sizeof(VERTEX_3D) * g_nMaxVtx),		// 頂点データ用に確保するバッファ
								 D3DUSAGE_WRITEONLY,					// 頂点バッファの使用方法
								 FVF_VERTEX_3D,							// 使用する頂点フォーマット
								 D3DPOOL_MANAGED,						// バッファを保持するメモリクラスの指定
								 &g_pVtxBuffMeshField,					// 頂点バッファへのポインタ
								 NULL);									// NULL固定	

	// インデックスバッファの確保
	//------------------------------------
	pDevice->CreateIndexBuffer(( sizeof(WORD) * ( g_nDrawPrimitiveNum + 2 ) ),	// 確保するバッファサイズ
							   D3DUSAGE_WRITEONLY,								// 頂点バッファの使用方法
							   D3DFMT_INDEX16,									// 使用する頂点フォーマット
							   D3DPOOL_MANAGED,									// バッファを保持するメモリクラスの指定
							   &g_pIdxBuffMeshField,							// インデックスバッファへのポインタ
							   NULL);

	// フィールド初期化
	//------------------------------------
	g_posMeshField = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_rotMeshField = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_sclMeshField = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	g_meshField.nNumX = numX;
	g_meshField.nNumZ = numZ;
	g_meshField.fWidth = fWidth;
	g_meshField.fHeight = fHeight;

	// テクスチャへのポインタを初期化
	//------------------------------------
	for (int nCnt = 0; nCnt < TEXTURE_MAX; nCnt++)				
	{
		g_pTextureMeshField[nCnt] = NULL;
	}

	// テクスチャの読み込み
	//------------------------------------
	D3DXCreateTextureFromFile(pDevice, TEX_PASS, &g_pTextureMeshField[0]);

	// フィールドの頂点情報とインデックスのセット
	//-------------------------------------------
	SetupMeshField(numX, numZ, fWidth, fHeight);
}

//===============================================
//
// フィールドの頂点情報セット
//
//===============================================
void SetupMeshField(int numX, int numZ, float fWidth, float fHeight)
{
	VERTEX_3D *pVtx;

	// データの位置を確保する（ロック）
	//------------------------------------
	g_pVtxBuffMeshField ->Lock(0, 0, (void**)&pVtx, 0);

	//==============================================
	// 
	// メッシュフィールド情報設定
	// 
	//==============================================

	// ループ用変数
	int nCntX = 0;
	int nCntZ = 0;
	float fRot = 0.0f;

	// 頂点情報に使用するワーク
	float fStartX = 0 - ((float)numZ / 2) * fWidth;
	float fStartZ = 0 + ((float)numX / 2) * fHeight;

	// 頂点情報の設定
	//------------------------------------
	for (nCntZ = 0; nCntZ < (numZ + 1); nCntZ++)
	{
		for (nCntX = 0; nCntX < (numX + 1); nCntX++)
		{
			// 頂点座標
			pVtx[g_nNumVtx].pos = D3DXVECTOR3( ( fStartX + (nCntX * fWidth) ), 0.0f, ( fStartZ - (nCntZ * fHeight)));

			// 法線
			pVtx[g_nNumVtx].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// 色
			pVtx[g_nNumVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// テクスチャ座標
			pVtx[g_nNumVtx].tex = D3DXVECTOR2((float)nCntX, (float)nCntZ);

			// 頂点情報の個数をインクリメント
			g_nNumVtx++;

			//fRot += D3DX_PI / 4;
		}
	}

	// インデックスに使用する変数
	//------------------------------------
	WORD *pIdx = NULL;

	// インデックスバッファのロック
	//------------------------------------
	g_pIdxBuffMeshField->Lock(0, 0, (void**)&pIdx, 0);

	// インデックスの設定
	//------------------------------------
	for (nCntZ = 0; nCntZ < numZ; nCntZ++)
	{
		for (nCntX = 0; nCntX < (numX + 1); nCntX++)
		{
			pIdx[g_nNumIdx] = (numX + 1) + nCntX + nCntZ * (numX + 1);
			g_nNumIdx++;

			pIdx[g_nNumIdx] = nCntX + nCntZ * (numX + 1);
			g_nNumIdx++;
		}

		// 折り返す必要があったら
		//-----------------------------------
		if (nCntZ != numZ - 1)
		{
			pIdx[g_nNumIdx] = pIdx[g_nNumIdx - 1];
			g_nNumIdx++;

			pIdx[g_nNumIdx] = nCntX + ((nCntZ + 1) * nCntX);
			g_nNumIdx++;
		}
	}

	// インデックスバッファのアンロック
	//------------------------------------
	g_pIdxBuffMeshField->Unlock();

	// 頂点情報のアンロック
	//------------------------------------
	g_pVtxBuffMeshField -> Unlock();
}

//===============================================
//
// フィールドに関する終了処理
//
//===============================================
void UninitMeshField()
{
	// ローカル変数
	//------------------------------------
	int nCnt;

	for (nCnt = 0; nCnt < TEXTURE_MAX; nCnt++)
	{
		if (g_pTextureMeshField[nCnt] != NULL)
		{ 
			// 使ったテクスチャの解放と初期化
			//------------------------------------
			g_pTextureMeshField[nCnt] -> Release();
			g_pTextureMeshField[nCnt] = NULL;
		}
	}

	if (g_pVtxBuffMeshField != NULL)
	{ 
		// 頂点バッファの解放と初期化
		//------------------------------------
		g_pVtxBuffMeshField -> Release();
		g_pVtxBuffMeshField = NULL;
	}

	if (g_pIdxBuffMeshField != NULL)
	{
		g_pIdxBuffMeshField -> Release();
		g_pIdxBuffMeshField = NULL;
	}
}	

//===============================================
//
// フィールドに関する更新処理
//
//===============================================
void UpdateMeshField()
{
	
}

//===============================================
//
// フィールドに関する描画処理
//
//===============================================
void DrawMeshField()
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

	// ワールドマトリックスの設定
	//------------------------------------
	D3DXMatrixIdentity(&g_mtxWorld);			// フォーマットの初期化

	//==============================================
	// 
	// 反映する順番は厳守！！
	// 
	//==============================================

	// スケールを反映
	D3DXMatrixScaling(&mtxScl, g_sclMeshField.x, g_sclMeshField.y, g_sclMeshField.z);
	D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorld, &mtxScl);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotMeshField.y, g_rotMeshField.x, g_rotMeshField.z);
	D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTranslate, g_posMeshField.x, g_posMeshField.y, g_posMeshField.z);
	D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorld, &mtxTranslate);

	// 設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorld);

	// 頂点バッファをデータストリームにバインド
	//-----------------------------------------
	pDevice ->SetStreamSource(0,
								g_pVtxBuffMeshField,
								0,
								sizeof(VERTEX_3D));	// 頂点データの間隔

	// インデックスバッファのバインド
	//------------------------------------
	pDevice->SetIndices(g_pIdxBuffMeshField);

	// 頂点フォーマットの設定
	//------------------------------------
	pDevice -> SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	//------------------------------------
	pDevice -> SetTexture(0, g_pTextureMeshField[ 0 ]);

	// ポリゴンの描画
	//------------------------------------
	pDevice -> DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,	// プリミティブの種類
									0,						// 
									0,						// 
									g_nMaxVtx,				// 頂点数
									0,						// 
									g_nDrawPrimitiveNum);	// 描画するプリミティブ数
}

//===============================================
//
// フィールドの情報取得
//
//===============================================
MESHFIELD *GetMeshField(void)
{
	return &g_meshField;
}
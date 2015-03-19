//===============================================
//
//  内容   ： フィールドに関する処理
//  File   ： field.cpp
//  制作者 ： 木崎　翔
//
//===============================================

//===============================================
// 
//  インクルードファイル
//
//===============================================
#include "field.h"
#include "input.h"
#include "camera.h"

//===============================================
//
//　マクロ定義
//
//===============================================
#define TEXTURE_MAX 	(1)			// 使用するテクスチャの数
#define FIELD_MAX		(4)			// フィールドの最大数
#define TEX_PASS		"data/TEXTURE/buis.jpg"

//-----------------------------------------------
//  グローバル変数
//-----------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffField = NULL;						// 頂点バッファへのポインタ
LPDIRECT3DTEXTURE9		g_pTextureField[TEXTURE_MAX] = {NULL};		// テクスチャへのポインタ

FIELD g_field[FIELD_MAX];

D3DXVECTOR3 g_posField;			// 位置
D3DXVECTOR3 g_rotField;			// 向き
D3DXVECTOR3 g_sclField;			// 大きさ

static D3DXMATRIX g_mtxProjection;		// プロジェクションマトリックス
static D3DXMATRIX g_mtxView;			// ビューマトリックス
static D3DXMATRIX g_mtxWorld;			// ワールドマトリックス

//===============================================
//
// フィールドに関する初期化
//
//===============================================
void InitField(void)	
{
	// ローカル変数
	//---------------------------------
	LPDIRECT3DDEVICE9 pDevice = NULL;		// デバイスオブジェクト
	//VERTEX_3D *pVtx;

	// デバイス取得
	//------------------------------------
	pDevice = GetDevice();

	// デバイスの設定
	//------------------------------------
	pDevice ->CreateVertexBuffer((sizeof(VERTEX_3D) * 4),				// 頂点データ用に確保するバッファ
								 D3DUSAGE_WRITEONLY,					// 頂点バッファの使用方法
								 FVF_VERTEX_3D,							// 使用する頂点フォーマット
								 D3DPOOL_MANAGED,						// バッファを保持するメモリクラスの指定
								 &g_pVtxBuffField,						// 頂点バッファへのポインタ
								 NULL);									// NULL固定	

	// フィールド初期化
	//------------------------------------
	g_posField = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_rotField = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_sclField = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	// テクスチャへのポインタを初期化
	//------------------------------------
	for (int nCnt = 0; nCnt < TEXTURE_MAX; nCnt++)				
	{
		g_pTextureField[nCnt] = NULL;
	}

	// テクスチャの読み込み
	//------------------------------------
	D3DXCreateTextureFromFile(pDevice, TEX_PASS, &g_pTextureField[0]);

	// フィールドの頂点情報セット
	//------------------------------------
	SetupField();
}

//===============================================
//
// フィールドの頂点情報セット
//
//===============================================
void SetupField(void)
{
	// ローカル変数
	//---------------------------------
	LPDIRECT3DDEVICE9 pDevice;		// デバイスオブジェクト
	VERTEX_3D *pVtx;				// 頂点の構造体ポインタ

	// デバイス取得
	//------------------------------------
	pDevice = GetDevice();

	// データの位置を確保する（ロック）
	//------------------------------------
	g_pVtxBuffField ->Lock(0,
						0,
						(void**)&pVtx,
						0);

	// 位置
	//------------------------------------
	//pVtx[0].pos.x = g_posField.x - 100.0f;
	//pVtx[0].pos.y = g_posField.y - 0.0f;
	//pVtx[0].pos.z = g_posField.z + 100.0f;

	pVtx[0].pos = D3DXVECTOR3(-100.0f, 0.0f, 100.0f);

	pVtx[1].pos.x = g_posField.x + 100.0f;
	pVtx[1].pos.y = g_posField.y - 0.0f;
	pVtx[1].pos.z = g_posField.z + 100.0f;

	pVtx[2].pos.x = g_posField.x - 100.0f;
	pVtx[2].pos.y = g_posField.y - 0.0f;
	pVtx[2].pos.z = g_posField.z - 100.0f;

	pVtx[3].pos.x = g_posField.x + 100.0f;
	pVtx[3].pos.y = g_posField.y - 0.0f;
	pVtx[3].pos.z = g_posField.z - 100.0f;

	// 法線ベクトルの向き
	//------------------------------------
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// 色
	//------------------------------------
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャ座標
	//------------------------------------
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	
	// 解放
	//------------------------------------
	g_pVtxBuffField -> Unlock();		
}

//===============================================
//
// フィールドに関する終了処理
//
//===============================================
void UninitField()
{
	// ローカル変数
	//------------------------------------
	int nCnt;

	for (nCnt = 0; nCnt < TEXTURE_MAX; nCnt++)
	{
		if (g_pTextureField[nCnt] != NULL)
		{ 
			// 使ったテクスチャの解放と初期化
			//------------------------------------
			g_pTextureField[nCnt] -> Release();
			g_pTextureField[nCnt] = NULL;
		}
	}

	if (g_pVtxBuffField != NULL)
	{ 
		// 頂点バッファの解放と初期化
		//------------------------------------
		g_pVtxBuffField->Release();
		g_pVtxBuffField = NULL;
	}
}	

//===============================================
//
// フィールドに関する更新処理
//
//===============================================
void UpdateField()
{
	
}

//===============================================
//
// フィールドに関する描画処理
//
//===============================================
void DrawField()
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
	D3DXMatrixScaling(&mtxScl, g_sclField.x, g_sclField.y, g_sclField.z);
	D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorld, &mtxScl);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotField.y, g_rotField.x, g_rotField.z);
	D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTranslate, g_posField.x, g_posField.y, g_posField.z);
	D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorld, &mtxTranslate);

	// 設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorld);

	// 頂点バッファをデータストリームにバインド
	//-----------------------------------------
	pDevice ->SetStreamSource(0,
								g_pVtxBuffField,
								0,
								sizeof(VERTEX_3D));	// 頂点データの間隔

	// 頂点フォーマットの設定
	//------------------------------------
	pDevice -> SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	//------------------------------------
	pDevice -> SetTexture(0, g_pTextureField[0]);

	// ポリゴンの描画
	//------------------------------------
	pDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,	// プリミティブの種類
									0,				// ロードする最初の頂点のインデックス
									2);				// 描画するプリミティブ数
}
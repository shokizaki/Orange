//===============================================
//
//  内容   ： 空処理
//  File   ： sky.cpp
//  制作者 ： 木崎　翔
//
//===============================================

// インクルードファイル
#include "object.h"
#include "model.h"
#include "enemy.h"

// マクロ定義

// グローバル変数
D3DXMATRIX mtxWorldSky;			// ワールドマトリックス
D3DXVECTOR3 posSky;				// 位置
D3DXVECTOR3 rotSky;				// 角度
D3DXVECTOR3 sclSky;				// スケール

LPD3DXMESH pMeshSky;			// メッシュ情報へのポインタ
LPD3DXBUFFER pBuffMatSky;	// マテリアル情報へのポインタ
DWORD numMatSky;				// マテリアルの数
LPDIRECT3DTEXTURE9 pTextureSky;		// テクスチャへのポインタ

//===============================================
//
// オブジェクトに関する初期化
//
//===============================================
void InitSky(void)
{
	// ローカル変数
	//------------------------------------
	LPDIRECT3DDEVICE9 pDevice;						// デバイスオブジェクト

	// デバイスの取得
	//------------------------------------
	pDevice = GetDevice();

	pTextureSky = NULL;

	posSky = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	rotSky = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	sclSky = D3DXVECTOR3( 1.0f, 1.0f, 1.0f );
	
	// xファイルの読み込み
	//------------------------------------
	D3DXLoadMeshFromX("data/MODEL/sky.x",		// 読み込むファイル名
						D3DXMESH_SYSTEMMEM,		// 
						pDevice,					// 
						NULL,						// 
						&pBuffMatSky,			// 
						NULL,						// 
						&numMatSky,			// 
						&pMeshSky);

	// テクスチャの読み込み
	//------------------------------------
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/sky000.jpg", &pTextureSky);
}

//===============================================
//
// オブジェクトに関する終了処理
//
//===============================================
void UninitSky(void)
{
	if ( pMeshSky != NULL )
	{ 
		// メッシュオブジェクトの解放
		//------------------------------------
		pMeshSky -> Release();
		pMeshSky = NULL;
	}
	// 
	//------------------------------------
	if ( pBuffMatSky != NULL )
	{ 
		// オブジェクトバッファの解放
		pBuffMatSky -> Release();
		pBuffMatSky = NULL;
	}
	
	if ( pTextureSky != NULL )
	{ 
		// 使ったテクスチャの解放と初期化
		//------------------------------------
		pTextureSky ->Release();
		pTextureSky = NULL;
	}
}

//===============================================
//
// オブジェクトに関する更新処理
//
//===============================================
void UpdateSky(void)
{
	// 空の回転
	rotSky.y += 0.0003f;
}

//===============================================
//
// オブジェクトに関する描画処理
//
//===============================================
void DrawSky(void)
{
	// ローカル変数
	//------------------------------------
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// デバイスオブジェクト
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;		// 計算用
	D3DXMATRIX pMtxWorld;
	D3DXMATERIAL *pMat;								// 
	D3DMATERIAL9 matDef;							// 元のマテリアル情報


	// ワールドマトリックスの設定
	//------------------------------------
	D3DXMatrixIdentity(&mtxWorldSky);			// フォーマットの初期化
	D3DXMatrixIdentity(&mtxScl);
	D3DXMatrixIdentity(&mtxRot);
	D3DXMatrixIdentity(&mtxTranslate);

	//==============================================
	// 
	// 反映する順番は厳守！！
	// 
	//==============================================

	// スケールを反映
	D3DXMatrixScaling(&mtxScl, sclSky.x, sclSky.y, sclSky.z);
	D3DXMatrixMultiply(&mtxWorldSky, &mtxWorldSky, &mtxScl);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rotSky.y, rotSky.x, rotSky.z);
	D3DXMatrixMultiply(&mtxWorldSky, &mtxWorldSky, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTranslate, posSky.x, posSky.y, posSky.z);
	D3DXMatrixMultiply(&mtxWorldSky, &mtxWorldSky, &mtxTranslate);
		
	// 設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorldSky);

	// 現在のマテリアル情報を保存
	//------------------------------------
	pDevice->GetMaterial(&matDef);

	pMat = (D3DXMATERIAL*)pBuffMatSky ->GetBufferPointer();

	/*strSrc = strrchr(pMat ->pTextureFilename, '\\') + 1;

	strcat(strDest, strSrc);

	D3DXCreateTextureFromFile(pDevice, strDest, &g_pTextureSky[1]);*/

	// マテリアルの数だけループ
	for (int nCntMat = 0; nCntMat < (int)numMatSky; nCntMat++)
	{
		pDevice ->SetMaterial(&pMat[ nCntMat ].MatD3D);		// マテリアルの設定
		pDevice ->SetTexture(0, pTextureSky);						// テクスチャのセット
		pMeshSky->DrawSubset( nCntMat );	// 描画
	}

	// マテリアルを元に戻す
	//------------------------------------
	pDevice ->SetMaterial( &matDef );
}
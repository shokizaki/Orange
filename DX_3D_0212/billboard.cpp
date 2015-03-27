//===============================================
//
//  内容   ： ビルボード処理
//  File   ： billboard.cpp
//  制作者 ： 木崎　翔
//
//===============================================

//===============================================
// 
//  インクルードファイル
//
//===============================================
#include "billboard.h"
#include "input.h"
#include "camera.h"
#include "meshField.h"
#include "collision.h"

//===============================================
//
//　マクロ定義
//
//===============================================
#define TEXTURE_MAX 	( 3 )			// 使用するテクスチャの数
#define BILL_WIDTH		( 15.0f )
#define BILL_HEIGHT		( 15.0f )

#define TEX_EMOTION		"data/TEXTURE/emotion.png"
#define TEX_CHARGE		"data/TEXTURE/ef002.png"
#define TEX_TITLE		"data/TEXTURE/ringleDash_backTitle.jpg"

//-----------------------------------------------
//  グローバル変数
//-----------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBillboard = NULL;				// 頂点バッファへのポインタ
LPDIRECT3DTEXTURE9		g_pTextureBillboard[TEXTURE_MAX];		// テクスチャへのポインタ

BILLBOARD g_aBillboard[ BILLBOARD_MAX ];		// ビルボードの構造体変数
COL_RECT rectBillboard[ BILLBOARD_MAX ];
int nShadow[ 10 ];

D3DXMATRIX g_mtxProjectionBillboard;		// プロジェクションマトリックス

//===============================================
//
// バレットに関する初期化
//
//===============================================
void InitBillboard( void )
{
	// ローカル変数
	//---------------------------------
	LPDIRECT3DDEVICE9 pDevice = NULL;		// デバイスオブジェクト
	
	// デバイス取得
	//------------------------------------
	pDevice = GetDevice();

	// 頂点バッファの確保
	//------------------------------------
	pDevice ->CreateVertexBuffer((sizeof(VERTEX_3D) * 4 * BILLBOARD_MAX),		// 頂点データ用に確保するバッファ
								 D3DUSAGE_WRITEONLY,					// 頂点バッファの使用方法
								 FVF_VERTEX_3D,							// 使用する頂点フォーマット
								 D3DPOOL_MANAGED,						// バッファを保持するメモリクラスの指定
								 &g_pVtxBuffBillboard,					// 頂点バッファへのポインタ
								 NULL);									// NULL固定	

	// ビルボード初期化
	//------------------------------------
	for ( int i = 0; i < BILLBOARD_MAX; i++ )
	{
		g_aBillboard[ i ].bUse = false;
	}

	// テクスチャへのポインタを初期化
	//------------------------------------
	for (int nCnt = 0; nCnt < TEXTURE_MAX; nCnt++)				
	{
		g_pTextureBillboard[ nCnt ] = NULL;
	}

	// テクスチャの読み込み
	//------------------------------------
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/player.jpg", &g_pTextureBillboard[ 0 ]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/player.jpg", &g_pTextureBillboard[ 1 ]);
}

//===============================================
//
// バレットに関する終了処理
//
//===============================================
void UninitBillboard( void )
{
	// 使ったテクスチャの解放と初期化
	//------------------------------------
	for (int nCnt = 0; nCnt < TEXTURE_MAX; nCnt++)
	{
		if (g_pTextureBillboard[ nCnt ] != NULL)
		{ 
			g_pTextureBillboard[ nCnt ] ->Release();
			g_pTextureBillboard[ nCnt ] = NULL;
		}
	}

	// 頂点バッファの解放と初期化
	//------------------------------------
	if (g_pVtxBuffBillboard != NULL)
	{ 
		g_pVtxBuffBillboard ->Release();
		g_pVtxBuffBillboard = NULL;
	}
}

//===============================================
//
// バレットに関する更新処理
//
//===============================================
void UpdateBillboard( void )
{
	// ローカル変数
	//------------------------------------
	VERTEX_3D *pVtx3D;

	for ( int i = 0; i < BILLBOARD_MAX; i++ )
	{
		if ( g_aBillboard[ i ].bUse == true )
		{
			g_aBillboard[ i ].a -= 0.02f;
			if ( g_aBillboard[ i ].a < 0.0f )
			{
				g_aBillboard[ i ].bUse = false;
			}
		}
	}

	// ビルボードの頂点情報とインデックスのセット
	//-------------------------------------------
	SetupBillboard();
}

//===============================================
//
// バレットに関する描画処理
//
//===============================================
void DrawBillboard( void )
{
	// ローカル変数
	//------------------------------------
	LPDIRECT3DDEVICE9 pDevice;						// デバイスオブジェクト
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;		// 計算用
	CAMERA *pCamera;					// カメラ取得
	
	// デバイスの取得
	//------------------------------------
	pDevice = GetDevice();

	// ライトの無効化
	//------------------------------------
	pDevice ->LightEnable(0, FALSE);
	pDevice ->LightEnable(2, FALSE);

	// カメラの設定
	//------------------------------------
	SetCamera();

	pCamera = GetCamera();

	// アルファテストの有効化
	//------------------------------------
	pDevice ->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//pDevice ->SetRenderState(D3DRS_ALPHAREF,		255);
	//pDevice ->SetRenderState(D3DRS_ALPHAFUNC,		D3DCMP_EQUAL);

	for (int nCnt = 0; nCnt < BILLBOARD_MAX; nCnt++)
	{
		if ( g_aBillboard[nCnt].bUse == true )
		{
			// ワールドマトリックスの設定
			//------------------------------------
			D3DXMatrixIdentity(&g_aBillboard[nCnt].mtxWorldBillboard);			// フォーマットの初期化

			// 逆行列を求める
			D3DXMatrixInverse(&g_aBillboard[nCnt].mtxWorldBillboard, NULL, &pCamera->mtxView);

			// 念のための初期化
			g_aBillboard[nCnt].mtxWorldBillboard._41 = 0.0f;
			g_aBillboard[nCnt].mtxWorldBillboard._42 = 0.0f;
			g_aBillboard[nCnt].mtxWorldBillboard._43 = 0.0f;

			//==============================================
			// 
			// 反映する順番は厳守！！
			// 
			//==============================================

			// スケールを反映
			D3DXMatrixScaling(&mtxScl, g_aBillboard[nCnt].sclBillboard.x, g_aBillboard[nCnt].sclBillboard.y, g_aBillboard[nCnt].sclBillboard.z);
			D3DXMatrixMultiply(&g_aBillboard[nCnt].mtxWorldBillboard, &g_aBillboard[nCnt].mtxWorldBillboard, &mtxScl);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTranslate, g_aBillboard[nCnt].posBillboard.x, g_aBillboard[nCnt].posBillboard.y, g_aBillboard[nCnt].posBillboard.z);
			D3DXMatrixMultiply(&g_aBillboard[nCnt].mtxWorldBillboard, &g_aBillboard[nCnt].mtxWorldBillboard, &mtxTranslate);

			// 設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aBillboard[nCnt].mtxWorldBillboard);

			// 頂点バッファをデータストリームにバインド
			//-----------------------------------------
			pDevice ->SetStreamSource(0,
										g_pVtxBuffBillboard,
										0,
										sizeof(VERTEX_3D));	// 頂点データの間隔

			// 頂点フォーマットの設定
			//------------------------------------
			pDevice -> SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			//------------------------------------
  			pDevice -> SetTexture( 0, NULL/*g_pTextureBillboard[ ( int )g_aBillboard[ nCnt ].type ]*/ );

			// ポリゴンの描画
			//------------------------------------
			pDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,	// プリミティブの種類
											nCnt * 4,		// 読み込む最初の番号
											2);				// 描画するプリミティブ数
		}
	}

	// アルファテストの無効化
	pDevice ->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	// ライトの有効化
	pDevice ->LightEnable(0, TRUE);
	pDevice ->LightEnable(2, TRUE);
}

//===============================================
//
// バレットの頂点情報セット
//
//===============================================
void SetupBillboard( void )
{
	// ローカル変数
	//------------------------------------
	VERTEX_3D *pVtx;

	// データの位置を確保する（ロック）
	//------------------------------------
	g_pVtxBuffBillboard ->Lock(0, 0, (void**)&pVtx, 0);

	for ( int num = 0; num < BILLBOARD_MAX; num++ )
	{
		if (g_aBillboard[ num ].bUse == true)
		{
			pVtx[ 0 + num * 4 ].pos = D3DXVECTOR3(-g_aBillboard[ num ].fWidth, g_aBillboard[ num ].fHeight, 0.0f);
			pVtx[ 1 + num * 4 ].pos = D3DXVECTOR3(g_aBillboard[ num ].fWidth, g_aBillboard[ num ].fHeight, 0.0f);
			pVtx[ 2 + num * 4 ].pos = D3DXVECTOR3(-g_aBillboard[ num ].fWidth, -g_aBillboard[ num ].fHeight, 0.0f);
			pVtx[ 3 + num * 4 ].pos = D3DXVECTOR3(g_aBillboard[ num ].fWidth, -g_aBillboard[ num ].fHeight, 0.0f);
	
			pVtx[ 0 + num * 4 ].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[ 1 + num * 4 ].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[ 2 + num * 4 ].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[ 3 + num * 4 ].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

			pVtx[ 0 + num * 4 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, g_aBillboard[ num ].a );
			pVtx[ 1 + num * 4 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, g_aBillboard[ num ].a );
			pVtx[ 2 + num * 4 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, g_aBillboard[ num ].a );
			pVtx[ 3 + num * 4 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, g_aBillboard[ num ].a );

			/*pVtx[ 0 + num * 4 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );
			pVtx[ 1 + num * 4 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );
			pVtx[ 2 + num * 4 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );
			pVtx[ 3 + num * 4 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );*/

			pVtx[ 0 + num * 4 ].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[ 1 + num * 4 ].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[ 2 + num * 4 ].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[ 3 + num * 4 ].tex = D3DXVECTOR2(1.0f, 1.0f);
		}
	}

	// 頂点情報のアンロック
	//------------------------------------
	g_pVtxBuffBillboard -> Unlock();
}

//===============================================
//
// バレットのセット
//
//===============================================
int SetBillboard( D3DXVECTOR3 pos, float fHeight, float fWidth, TEXTURE_TYPE type )
{
	VERTEX_3D *pVtx3D;

	for ( int i = 0; i < BILLBOARD_MAX; i++ )
	{
		if ( g_aBillboard[ i ].bUse == false )
		{
			g_aBillboard[ i ].posBillboard.x = pos.x;								// 位置
			g_aBillboard[ i ].posBillboard.y = pos.y + 20.0f;								// 位置
			g_aBillboard[ i ].posBillboard.z = pos.z;								// 位置
			g_aBillboard[ i ].rotBillboard = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );	// 角度
			g_aBillboard[ i ].sclBillboard = D3DXVECTOR3( 1.0f, 1.0f, 1.0f );	// スケール

			g_aBillboard[ i ].fWidth = fWidth;			// 幅
			g_aBillboard[ i ].fHeight = fHeight;		// 高さ

			g_aBillboard[ i ].nFrameCount = 0;

			g_aBillboard[ i ].type = type;

			g_aBillboard[ i ].a = 1.0f;

			g_aBillboard[ i ].bUse = true;	// 使用中にする

			// ビルボードの頂点情報とインデックスのセット
			//-------------------------------------------
			SetupBillboard();

			return i;	// インデックスを返す
		}
	}

	return NULL;
}

//===============================================
//
// バレットの情報取得
//
//===============================================
BILLBOARD *GetBillboard( void )
{
	return &g_aBillboard[ 0 ];
}

//===============================================
//
// バレットの当たり判定情報取得
//
//===============================================
COL_RECT *GetRectBillboard( void )
{
	return &rectBillboard[ 0 ];
}

//===============================================
//
// ビルボードの位置を設定
//
//===============================================
void SetPosBillboard( int num, D3DXVECTOR3 pos )
{
	g_aBillboard[ num ].posBillboard.x = pos.x;
	g_aBillboard[ num ].posBillboard.y = pos.y + g_aBillboard[ num ].fHeightFromParent;
	g_aBillboard[ num ].posBillboard.z = pos.z;
}

//===============================================
//
// ビルボードのOFF
//
//===============================================
void SetUseBillboard( int num )
{
	g_aBillboard[ num ].bUse = false;
}
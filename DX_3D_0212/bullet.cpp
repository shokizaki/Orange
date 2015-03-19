//===============================================
//
//  内容   ： ビルボード処理
//  File   ： bullet.cpp
//  制作者 ： 木崎　翔
//
//===============================================

//===============================================
// 
//  インクルードファイル
//
//===============================================
#include "bullet.h"
#include "shadow.h"
#include "meshField.h"
#include "camera.h"

//===============================================
//
//　マクロ定義
//
//===============================================
#define BULLET_TEX_MAX	( 1 )
#define BULLET_WIDTH	( 15.0f )
#define BULLET_HEIGHT	( 15.0f )

#define TEX_PASS_A		"data/TEXTURE/onp001_white.png"

//-----------------------------------------------
//  グローバル変数
//-----------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;				// 頂点バッファへのポインタ
LPDIRECT3DTEXTURE9		g_pTextureBullet[ BULLET_TEX_MAX ];		// テクスチャへのポインタ

BULLET g_aBullet[ BULLET_MAX ];
COL_RECT rectBullet[ BULLET_MAX ];

//===============================================
//
// バレットに関する初期化
//
//===============================================
void InitBullet( void )
{
	// ローカル変数
	//---------------------------------
	LPDIRECT3DDEVICE9 pDevice = NULL;		// デバイスオブジェクト
	
	// デバイス取得
	//------------------------------------
	pDevice = GetDevice();

	// 頂点バッファの確保
	//------------------------------------
	pDevice ->CreateVertexBuffer((sizeof(VERTEX_3D) * 4 * BULLET_MAX),		// 頂点データ用に確保するバッファ
								 D3DUSAGE_WRITEONLY,					// 頂点バッファの使用方法
								 FVF_VERTEX_3D,							// 使用する頂点フォーマット
								 D3DPOOL_MANAGED,						// バッファを保持するメモリクラスの指定
								 &g_pVtxBuffBullet,					// 頂点バッファへのポインタ
								 NULL);									// NULL固定	

	// ビルボード初期化
	//------------------------------------
	for ( int i = 0; i < BULLET_MAX; i++ )
	{
		g_aBullet[ i ].bUse = false;
	}

	// テクスチャへのポインタを初期化
	//------------------------------------
	for (int nCnt = 0; nCnt < BULLET_TEX_MAX; nCnt++)				
	{
		g_pTextureBullet[ nCnt ] = NULL;
	}

	// テクスチャの読み込み
	//------------------------------------
	D3DXCreateTextureFromFile(pDevice, TEX_PASS_A, &g_pTextureBullet[ 0 ]);
}

//===============================================
//
// バレットに関する終了処理
//
//===============================================
void UninitBullet( void )
{
	// 使ったテクスチャの解放と初期化
	//------------------------------------
	for (int nCnt = 0; nCnt < BULLET_TEX_MAX; nCnt++)
	{
		if (g_pTextureBullet[ nCnt ] != NULL)
		{ 
			g_pTextureBullet[ nCnt ] ->Release();
			g_pTextureBullet[ nCnt ] = NULL;
		}
	}

	// 頂点バッファの解放と初期化
	//------------------------------------
	if (g_pVtxBuffBullet != NULL)
	{ 
		g_pVtxBuffBullet ->Release();
		g_pVtxBuffBullet = NULL;
	}
}

//===============================================
//
// バレットに関する更新処理
//
//===============================================
void UpdateBullet( void )
{
	// ローカル変数
	//------------------------------------
	MESHFIELD *pMeshField = GetMeshField();		// メッシュフィールドの情報を取得

	for ( int i = 0; i < BULLET_MAX; i++ )
	{
		if ( g_aBullet[ i ].bUse == true )
		{
			// 移動処理
			g_aBullet[ i ].posBullet.x -= g_aBullet[ i ].move.x * 10.0f;
			g_aBullet[ i ].posBullet.z += g_aBullet[ i ].move.z * 10.0f;	
		
			// 当たり判定用変数を更新
			rectBullet[ i ].pos = g_aBullet[ i ].posBullet;

			// 壁に当たったら
			//------------------------------------
			if (g_aBullet[ i ].posBullet.x - g_aBullet[ i ].fWidth < -(pMeshField ->nNumX * pMeshField ->fWidth) / 2 
			||	g_aBullet[ i ].posBullet.x + g_aBullet[ i ].fWidth > (pMeshField ->nNumX * pMeshField ->fWidth) / 2)
			{
				g_aBullet[ i ].bUse = false;		// 使用OFF
				UseShadow( g_aBullet[ i ].nShadowNum );
			}
			if (g_aBullet[ i ].posBullet.z - g_aBullet[ i ].fHeight < -(pMeshField ->nNumZ * pMeshField ->fHeight) / 2
			||	g_aBullet[ i ].posBullet.z + g_aBullet[ i ].fHeight > (pMeshField ->nNumZ * pMeshField ->fHeight) / 2)
			{
				g_aBullet[ i ].bUse = false;		// 使用OFF
				UseShadow( g_aBullet[ i ].nShadowNum );
			}

			// 影の更新（インデックス指定）
			//UpdateShadowEx( g_aBullet[ i ].nShadowNum, g_aBullet[ i ].posBullet );
			SetShadow( D3DXVECTOR3( g_aBullet[ i ].posBullet.x, 0.1f, g_aBullet[ i ].posBullet.z ), g_aBullet[ i ].fWidth, g_aBullet[ i ].fHeight, 0.7f );
		}
	}

	// ビルボードの頂点情報とインデックスのセット
	//-------------------------------------------
	SetupBullet();
}

//===============================================
//
// バレットに関する描画処理
//
//===============================================
void DrawBullet( void )
{
	// ローカル変数
	//------------------------------------
	LPDIRECT3DDEVICE9 pDevice;						// デバイスオブジェクト
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;		// 計算用
	CAMERA *pCamera = GetCamera();					// カメラ取得
	
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

	// アルファテストの有効化
	//------------------------------------
	pDevice ->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice ->SetRenderState(D3DRS_ALPHAREF,		255);
	pDevice ->SetRenderState(D3DRS_ALPHAFUNC,		D3DCMP_EQUAL);

	for (int nCnt = 0; nCnt < BULLET_MAX; nCnt++)
	{
		if (g_aBullet[nCnt].bUse == true)
		{
			// ワールドマトリックスの設定
			//------------------------------------
			D3DXMatrixIdentity(&g_aBullet[nCnt].mtxWorldBullet);			// フォーマットの初期化

			// 逆行列を求める
			D3DXMatrixInverse(&g_aBullet[nCnt].mtxWorldBullet, NULL, &pCamera->mtxView);

			// 念のための初期化
			g_aBullet[nCnt].mtxWorldBullet._41 = 0.0f;
			g_aBullet[nCnt].mtxWorldBullet._42 = 0.0f;
			g_aBullet[nCnt].mtxWorldBullet._43 = 0.0f;

			//==============================================
			// 
			// 反映する順番は厳守！！
			// 
			//==============================================

			// スケールを反映
			D3DXMatrixScaling(&mtxScl, g_aBullet[nCnt].sclBullet.x, g_aBullet[nCnt].sclBullet.y, g_aBullet[nCnt].sclBullet.z);
			D3DXMatrixMultiply(&g_aBullet[nCnt].mtxWorldBullet, &g_aBullet[nCnt].mtxWorldBullet, &mtxScl);

			// 向きを反映
			//D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aBullet[nCnt].rotBullet.y, g_aBullet[nCnt].rotBullet.x, g_aBullet[nCnt].rotBullet.z);
			//D3DXMatrixMultiply(&g_aBullet[nCnt].mtxWorldBullet, &g_aBullet[nCnt].mtxWorldBullet, &mtxRot);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTranslate, g_aBullet[nCnt].posBullet.x, g_aBullet[nCnt].posBullet.y, g_aBullet[nCnt].posBullet.z);
			D3DXMatrixMultiply(&g_aBullet[nCnt].mtxWorldBullet, &g_aBullet[nCnt].mtxWorldBullet, &mtxTranslate);

			// 設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aBullet[nCnt].mtxWorldBullet);

			// 頂点バッファをデータストリームにバインド
			//-----------------------------------------
			pDevice ->SetStreamSource(0,
										g_pVtxBuffBullet,
										0,
										sizeof(VERTEX_3D));	// 頂点データの間隔

			// 頂点フォーマットの設定
			//------------------------------------
			pDevice -> SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			//------------------------------------
			pDevice -> SetTexture( 0, g_pTextureBullet[ 0 ] );

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
void SetupBullet( void )
{
	// ローカル変数
	//------------------------------------
	VERTEX_3D *pVtx;

	// データの位置を確保する（ロック）
	//------------------------------------
	g_pVtxBuffBullet ->Lock(0, 0, (void**)&pVtx, 0);

	for ( int nCnt = 0; nCnt < BULLET_MAX; nCnt++ )
	{
		if (g_aBullet[ nCnt ].bUse == true)
		{
			pVtx[ 0 + nCnt * 4 ].pos = D3DXVECTOR3(-g_aBullet[ nCnt ].fWidth, g_aBullet[ nCnt ].fHeight, 0.0f);
			pVtx[ 1 + nCnt * 4 ].pos = D3DXVECTOR3(g_aBullet[ nCnt ].fWidth, g_aBullet[ nCnt ].fHeight, 0.0f);
			pVtx[ 2 + nCnt * 4 ].pos = D3DXVECTOR3(-g_aBullet[ nCnt ].fWidth, -g_aBullet[ nCnt ].fHeight, 0.0f);
			pVtx[ 3 + nCnt * 4 ].pos = D3DXVECTOR3(g_aBullet[ nCnt ].fWidth, -g_aBullet[ nCnt ].fHeight, 0.0f);
	
			pVtx[ 0 + nCnt * 4 ].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[ 1 + nCnt * 4 ].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[ 2 + nCnt * 4 ].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[ 3 + nCnt * 4 ].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			pVtx[ 0 + nCnt * 4 ].col = g_aBullet[ nCnt ].color;
			pVtx[ 1 + nCnt * 4 ].col = g_aBullet[ nCnt ].color;
			pVtx[ 2 + nCnt * 4 ].col = g_aBullet[ nCnt ].color;
			pVtx[ 3 + nCnt * 4 ].col = g_aBullet[ nCnt ].color;

			pVtx[ 0 + nCnt * 4 ].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[ 1 + nCnt * 4 ].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[ 2 + nCnt * 4 ].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[ 3 + nCnt * 4 ].tex = D3DXVECTOR2(1.0f, 1.0f);
		}
	}

	// 頂点情報のアンロック
	//------------------------------------
	g_pVtxBuffBullet -> Unlock();
}

//===============================================
//
// バレットのセット
//
//===============================================
void SetBullet( D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scl, D3DXVECTOR3 move, BULLETTYPE type )
{
	for ( int i = 0; i < BULLET_MAX; i++ )
	{
		if (g_aBullet[ i ].bUse == false)
		{
			g_aBullet[ i ].posBullet.x = pos.x;
			g_aBullet[ i ].posBullet.y = pos.y + 20.0f;
			g_aBullet[ i ].posBullet.z = pos.z;
			g_aBullet[ i ].rotBullet = rot;
			g_aBullet[ i ].sclBullet = scl;

			g_aBullet[ i ].move = move;
			g_aBullet[ i ].fWidth = BULLET_WIDTH;
			g_aBullet[ i ].fHeight = BULLET_HEIGHT;

			g_aBullet[ i ].type = type;

			switch ( g_aBullet[ i ].type )
			{
			case BULLETTYPE_ONP_RED:
				g_aBullet[ i ].color = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

				break;

			case BULLETTYPE_ONP_YELLOW:
				g_aBullet[ i ].color = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);

				break;

			case BULLETTYPE_ONP_BLUE:
				g_aBullet[ i ].color = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);

				break;
			}

			g_aBullet[ i ].bUse = true;

			rectBullet[ i ].pos = g_aBullet[ i ].posBullet;
			rectBullet[ i ].harfSize.x = g_aBullet[ i ].fWidth;
			rectBullet[ i ].harfSize.z = g_aBullet[ i ].fHeight;

			// 影の生成（インデックス指定）
			g_aBullet[ i ].nShadowNum = SetShadowEx( pos, rot, scl, 5.0f, 5.0f, 1.0f );

			break;
		}
	}
}

//===============================================
//
// バレットの情報取得
//
//===============================================
BULLET *GetBullet( void )
{
	return &g_aBullet[ 0 ];
}

//===============================================
//
// バレットの当たり判定情報取得
//
//===============================================
COL_RECT *GetRectBullet( void )
{
	return &rectBullet[ 0 ];
}
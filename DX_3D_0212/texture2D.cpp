//===============================================
//
//  内容   ： ユーザーインターフェース処理
//  File   ： Texture2D.h
//  制作者 ： 木崎　翔
//
//===============================================

//------ インクルードファイル ------
#include "texture2D.h"

//------ マクロ定義 ------
#define TEXTURE2D_TEX_MAX	( 5 )

#define TEXTURE2D_EF000		"data/TEXTURE/ef000.png"
#define TEXTURE2D_EF001		"data/TEXTURE/ef001.png"
#define TEXTURE2D_EMOTION	"data/TEXTURE/emotion.png"
#define TEXTURE2D_HINTBOX	"data/TEXTURE/hintBox.png"


//------ グローバル変数 ------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTexture2D = NULL;			// 頂点バッファへのポインタ
LPDIRECT3DTEXTURE9		g_pTextureTexture2D[ TEXTURE2D_TEX_MAX ];		// テクスチャへのポインタ

TEXTURE2D g_aTexture2D[ TEXTURE2D_MAX ];

D3DXMATRIX g_mtxProjectionTexture2D;		// プロジェクションマトリックス

//===============================================
//
// Texture2Dに関する初期化
//
//===============================================
void InitTexture2D( void )
{
	// ローカル変数
	//---------------------------------
	LPDIRECT3DDEVICE9 pDevice = NULL;		// デバイスオブジェクト

	// グローバル変数の初期化
	//---------------------------------


	// デバイス取得
	//------------------------------------
	pDevice = GetDevice();

	// 頂点バッファの確保
	//------------------------------------
	pDevice ->CreateVertexBuffer((sizeof(VERTEX_2D) * 4 * TEXTURE2D_MAX),		// 頂点データ用に確保するバッファ
								 D3DUSAGE_WRITEONLY,					// 頂点バッファの使用方法
								 FVF_VERTEX_2D,							// 使用する頂点フォーマット
								 D3DPOOL_MANAGED,						// バッファを保持するメモリクラスの指定
								 &g_pVtxBuffTexture2D,							// 頂点バッファへのポインタ
								 NULL);									// NULL固定	

	// テクスチャへのポインタを初期化
	//------------------------------------
	for (int nCnt = 0; nCnt < TEXTURE2D_TEX_MAX; nCnt++)				
	{
		g_pTextureTexture2D[ nCnt ] = NULL;
	}

	// テクスチャの読み込み
	//------------------------------------
	D3DXCreateTextureFromFile(pDevice, TEXTURE2D_EF000, &g_pTextureTexture2D[ 0 ]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE2D_EF001, &g_pTextureTexture2D[ 1 ]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE2D_EMOTION, &g_pTextureTexture2D[ 2 ]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE2D_HINTBOX, &g_pTextureTexture2D[ 3 ]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE2D_HINTBOX, &g_pTextureTexture2D[ 4 ]);

	// 頂点情報のセット
	//------------------------------------
	SetupTexture2D();
}

//===============================================
//
// Texture2Dの頂点情報セット
//
//===============================================
void SetupTexture2D( void )
{
	// ローカル変数
	//---------------------------------
	LPDIRECT3DDEVICE9 pDevice = NULL;		// デバイスオブジェクト
	VERTEX_2D *pVtx2D;

	// デバイス取得
	//------------------------------------
	pDevice = GetDevice();

	g_pVtxBuffTexture2D ->Lock( 0, 0, (void**)&pVtx2D, 0 );

	for ( int i = 0; i < TEXTURE2D_MAX; i++ )
	{
		// 位置
		pVtx2D[ 0 + i * 4 ].pos = D3DXVECTOR3( g_aTexture2D[ i ].pos.x - g_aTexture2D[ i ].fWidth, g_aTexture2D[ i ].pos.y - g_aTexture2D[ i ].fHeight, 0.0f );
		pVtx2D[ 1 + i * 4 ].pos = D3DXVECTOR3( g_aTexture2D[ i ].pos.x + g_aTexture2D[ i ].fWidth, g_aTexture2D[ i ].pos.y - g_aTexture2D[ i ].fHeight, 0.0f );
		pVtx2D[ 2 + i * 4 ].pos = D3DXVECTOR3( g_aTexture2D[ i ].pos.x - g_aTexture2D[ i ].fWidth, g_aTexture2D[ i ].pos.y + g_aTexture2D[ i ].fHeight, 0.0f );
		pVtx2D[ 3 + i * 4 ].pos = D3DXVECTOR3( g_aTexture2D[ i ].pos.x + g_aTexture2D[ i ].fWidth, g_aTexture2D[ i ].pos.y + g_aTexture2D[ i ].fHeight, 0.0f );

		// パースペクティブコレクト＆色
		for ( int j = 0; j < 4; j++ )
		{
			pVtx2D[ j + i * 4 ].rhw = 1.0f;
			pVtx2D[ j + i * 4 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );
		}

		// テクスチャ座標
		//pVtx2D[ 0 + i * 4  ].tex = D3DXVECTOR2( 0.0f, 0.0f );
		//pVtx2D[ 1 + i * 4  ].tex = D3DXVECTOR2( 1.0f, 0.0f );
		//pVtx2D[ 2 + i * 4  ].tex = D3DXVECTOR2( 0.0f, 1.0f );
		//pVtx2D[ 3 + i * 4  ].tex = D3DXVECTOR2( 1.0f, 1.0f );
	}

	g_pVtxBuffTexture2D ->Unlock();
}

//===============================================
//
// Texture2Dに関する終了処理
//
//===============================================
void UninitTexture2D( void )
{
	// 使ったテクスチャの解放と初期化
	//------------------------------------
	for (int nCnt = 0; nCnt < TEXTURE2D_TEX_MAX; nCnt++)
	{
		if (g_pTextureTexture2D[ nCnt ] != NULL)
		{ 
			g_pTextureTexture2D[ nCnt ] ->Release();
			g_pTextureTexture2D[ nCnt ] = NULL;
		}
	}

	// 頂点バッファの解放と初期化
	//------------------------------------
	if (g_pVtxBuffTexture2D != NULL)
	{ 
		g_pVtxBuffTexture2D ->Release();
		g_pVtxBuffTexture2D = NULL;
	}
}

//===============================================
//
// Texture2Dに関する更新処理
//
//===============================================
void UpdateTexture2D( void )
{
	VERTEX_2D *pVtx2D;

	g_pVtxBuffTexture2D ->Lock( 0, 0, (void**)&pVtx2D, 0 );

	for ( int i = 0; i < TEXTURE2D_MAX; i++ )
	{
		if ( g_aTexture2D[ i ].bUse == true )
		{
			// テクスチャ座標の更新
			g_aTexture2D[ i ].nFrameCount++;

			if ( ( ++g_aTexture2D[ i ].nFrameCount % g_aTexture2D[ i ].nFrame ) == 0 )
			{
				g_aTexture2D[ i ].nTexNumX++;

				if ( g_aTexture2D[ i ].nTexNumX == g_aTexture2D[ i ].nTexDevideX )
				{
					if ( g_aTexture2D[ i ].bLoop == true )
					{
						g_aTexture2D[ i ].nTexNumX = 0;
					}
					else
					{
						g_aTexture2D[ i ].bUse = false;
					}
				}
			} 

			switch ( g_aTexture2D[ i ].type )
			{
				// [ エフェクト000 ]
			case TEXTURE2DTYPE_000:
			
				pVtx2D[ 0 + i * 4 ].tex = D3DXVECTOR2( 0.0f + ( ( float )g_aTexture2D[ i ].nTexNumX / g_aTexture2D[ i ].nTexDevideX ),									   0.0f );
				pVtx2D[ 1 + i * 4 ].tex = D3DXVECTOR2( ( 1.0f / g_aTexture2D[ i ].nTexDevideX ) + ( ( float )g_aTexture2D[ i ].nTexNumX / g_aTexture2D[ i ].nTexDevideX ), 0.0f );
				pVtx2D[ 2 + i * 4 ].tex = D3DXVECTOR2( 0.0f + ( ( float )g_aTexture2D[ i ].nTexNumX / g_aTexture2D[ i ].nTexDevideX ),									   1.0f );
				pVtx2D[ 3 + i * 4 ].tex = D3DXVECTOR2( ( 1.0f / g_aTexture2D[ i ].nTexDevideX ) + ( ( float )g_aTexture2D[ i ].nTexNumX / g_aTexture2D[ i ].nTexDevideX ), 1.0f );

				break;

				// [ エフェクト001 ]
			case TEXTURE2DTYPE_001:

				pVtx2D[ 0 + i * 4 ].tex = D3DXVECTOR2( 0.0f + ( ( float )g_aTexture2D[ i ].nTexNumX / g_aTexture2D[ i ].nTexDevideX ),									   0.0f );
				pVtx2D[ 1 + i * 4 ].tex = D3DXVECTOR2( ( 1.0f / g_aTexture2D[ i ].nTexDevideX ) + ( ( float )g_aTexture2D[ i ].nTexNumX / g_aTexture2D[ i ].nTexDevideX ), 0.0f );
				pVtx2D[ 2 + i * 4 ].tex = D3DXVECTOR2( 0.0f + ( ( float )g_aTexture2D[ i ].nTexNumX / g_aTexture2D[ i ].nTexDevideX ),									   1.0f );
				pVtx2D[ 3 + i * 4 ].tex = D3DXVECTOR2( ( 1.0f / g_aTexture2D[ i ].nTexDevideX ) + ( ( float )g_aTexture2D[ i ].nTexNumX / g_aTexture2D[ i ].nTexDevideX ), 1.0f );

				break;

			case TEXTURE2DTYPE_HASTY2:

				pVtx2D[ 0 + i * 4 ].tex = D3DXVECTOR2( ( 1.0f / 30 * 6 ) + ( ( float )g_aTexture2D[ i ].nTexNumX / 30 ), ( 1.0f / 9 ) * 2 );
				pVtx2D[ 1 + i * 4 ].tex = D3DXVECTOR2( ( 1.0f / 30 * 7 ) + ( ( float )g_aTexture2D[ i ].nTexNumX / 30 ), ( 1.0f / 9 ) * 2 );
				pVtx2D[ 2 + i * 4 ].tex = D3DXVECTOR2( ( 1.0f / 30 * 6 ) + ( ( float )g_aTexture2D[ i ].nTexNumX / 30 ), ( 1.0f / 9 ) * 3 );
				pVtx2D[ 3 + i * 4 ].tex = D3DXVECTOR2( ( 1.0f / 30 * 7 ) + ( ( float )g_aTexture2D[ i ].nTexNumX / 30 ), ( 1.0f / 9 ) * 3 );

				break;

			case TEXTURE2DTYPE_BOX:

				pVtx2D[ 0 + i * 4 ].tex = D3DXVECTOR2( 0.0f, 0.0f );
				pVtx2D[ 1 + i * 4 ].tex = D3DXVECTOR2( 1.0f, 0.0f );
				pVtx2D[ 2 + i * 4 ].tex = D3DXVECTOR2( 0.0f, 1.0f );
				pVtx2D[ 3 + i * 4 ].tex = D3DXVECTOR2( 1.0f, 1.0f );

				break;

			case TEXTURE2DTYPE_BOXREVERS:

				pVtx2D[ 0 + i * 4 ].tex = D3DXVECTOR2( 1.0f, 0.0f );
				pVtx2D[ 1 + i * 4 ].tex = D3DXVECTOR2( 0.0f, 0.0f );
				pVtx2D[ 2 + i * 4 ].tex = D3DXVECTOR2( 1.0f, 1.0f );
				pVtx2D[ 3 + i * 4 ].tex = D3DXVECTOR2( 0.0f, 1.0f );

				break;

			default:

				break;
			}
		}
	}

	g_pVtxBuffTexture2D ->Unlock();

	// 頂点情報セット
	SetupTexture2D();
}

//===============================================
//
// Texture2Dに関する描画処理
//
//===============================================
void DrawTexture2D( void )
{
	// ローカル変数
	//------------------------------------
	LPDIRECT3DDEVICE9 pDevice;						// デバイスオブジェクト
	
	// デバイスの取得
	//------------------------------------
	pDevice = GetDevice();

	// 頂点バッファをデータストリームにバインド
	//-----------------------------------------
	pDevice ->SetStreamSource(0,
								g_pVtxBuffTexture2D,
								0,
								sizeof(VERTEX_2D));	// 頂点データの間隔

	
	// 頂点フォーマットの設定
	//------------------------------------
	pDevice ->SetFVF( FVF_VERTEX_2D );

	for ( int i = 0; i < TEXTURE2D_MAX; i++ )
	{
		if ( g_aTexture2D[ i ].bUse == true )
		{
			switch ( g_aTexture2D[ i ].type )
			{
				// [ エフェクト000 ]
			case TEXTURE2DTYPE_000:
			
				// 加算合成
				//------------------------------------
				pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
				pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

				break;

				// [ エフェクト001 ]
			case TEXTURE2DTYPE_001:
			
				// 加算合成
				//------------------------------------
				pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
				pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

				break;

			default:

				break;
			}

			// テクスチャの設定
			//------------------------------------
			pDevice -> SetTexture(0, g_pTextureTexture2D[ ( int )g_aTexture2D[ i ].type ]);
			
			// ポリゴンの描画
			//------------------------------------
			pDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,	// プリミティブの種類
											i * 4,				// 読み込む最初の番号
											2);				// 描画するプリミティブ数

			// 元に戻す
			pDevice-> SetRenderState(D3DRS_SRCBLEND	, D3DBLEND_SRCALPHA);
			pDevice-> SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		}
	}
}


//===============================================
//
// Texture2Dのセット
//
//===============================================
int SetTexture2D( D3DXVECTOR3 pos, float fWidth, float fHeight, TEXTURE2DTYPE type )
{
	for ( int i = 0; i < TEXTURE2D_MAX; i++ )
	{
		if ( g_aTexture2D[ i ].bUse == false )
		{
			g_aTexture2D[ i ].pos = pos;				// 位置

			g_aTexture2D[ i ].fWidth = fWidth;			// 幅
			g_aTexture2D[ i ].fHeight = fHeight;		// 高さ

			g_aTexture2D[ i ].type = type;				// テクスチャの種類
			g_aTexture2D[ i ].nFrameCount = 0;			// フレームカウンタ

			// 使いたいテクスチャの種類
			switch ( type )
			{
				// [ エフェクト ]
			case TEXTURE2DTYPE_000:
				g_aTexture2D[ i ].nFrame = 10;		// アニメーションの切り替わるタイミング（フレーム）
				g_aTexture2D[ i ].nTexDevideX = 10;	// テクスチャに横分割数
				g_aTexture2D[ i ].nTexDevideY = 1;	// テクスチャの縦分割数

				g_aTexture2D[ i ].nTexNumX = 0;		// ズラす分
				g_aTexture2D[ i ].nTexNumY = 0;		// ズラす分

				g_aTexture2D[ i ].bLoop = false;	// ループ

				break;

				// [ エフェクト ]
			case TEXTURE2DTYPE_001:
				g_aTexture2D[ i ].nFrame = 10;		// アニメーションの切り替わるタイミング（フレーム）
				g_aTexture2D[ i ].nTexDevideX = 10;	// テクスチャに横分割数
				g_aTexture2D[ i ].nTexDevideY = 1;	// テクスチャの縦分割数

				g_aTexture2D[ i ].nTexNumX = 0;		// ズラす分
				g_aTexture2D[ i ].nTexNumY = 0;		// ズラす分

				g_aTexture2D[ i ].bLoop = false;	// ループ

				break;

			case TEXTURE2DTYPE_HASTY2:
				g_aTexture2D[ i ].nFrame = 15;		// アニメーションの切り替わるタイミング（フレーム）
				g_aTexture2D[ i ].nTexDevideX = 2;	// テクスチャに横分割数
				g_aTexture2D[ i ].nTexDevideY = 1;	// テクスチャの縦分割数

				g_aTexture2D[ i ].nTexNumX = 0;		// ズラす分
				g_aTexture2D[ i ].nTexNumY = 0;		// ズラす分

				g_aTexture2D[ i ].bLoop = true;	// ループ

				break;

			// [ エフェクト ]
			case TEXTURE2DTYPE_BOXREVERS:
				g_aTexture2D[ i ].nFrame = 10;		// アニメーションの切り替わるタイミング（フレーム）
				g_aTexture2D[ i ].nTexDevideX = 1;	// テクスチャに横分割数
				g_aTexture2D[ i ].nTexDevideY = 1;	// テクスチャの縦分割数

				g_aTexture2D[ i ].nTexNumX = 0;		// ズラす分
				g_aTexture2D[ i ].nTexNumY = 0;		// ズラす分

				g_aTexture2D[ i ].bLoop = true;	// ループ

				break;

			// [ エフェクト ]
			case TEXTURE2DTYPE_BOX:
				g_aTexture2D[ i ].nFrame = 10;		// アニメーションの切り替わるタイミング（フレーム）
				g_aTexture2D[ i ].nTexDevideX = 1;	// テクスチャに横分割数
				g_aTexture2D[ i ].nTexDevideY = 1;	// テクスチャの縦分割数

				g_aTexture2D[ i ].nTexNumX = 0;		// ズラす分
				g_aTexture2D[ i ].nTexNumY = 0;		// ズラす分

				g_aTexture2D[ i ].bLoop = true;	// ループ

				break;
			}

			g_aTexture2D[ i ].bUse = true;	// 使用中にする

			// 頂点情報セット
			SetupTexture2D();

			return i;
		}
	}

	return 0;
}

//===============================================
//
// Texture2Dの使用OFF
//
//===============================================
void SetTexture2DUse( int num )
{
	if ( g_aTexture2D[ num ].bUse == true )
	{
		g_aTexture2D[ num ].bUse = false;
	}
}
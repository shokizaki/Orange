//-----------------------------------------------
//  内容：ミニマップ処理
//  File : miniMap.cpp
//  制作者：木崎　翔
//-----------------------------------------------

//------ インクルードファイル ------
#include "miniMap.h"

//------ マクロ定義 ------
#define MINIMAP_TEX_MAX ( 5 )
#define TEX_PLAYER		"data/TEXTURE/miniMapIconPlayer.png"
#define TEX_ENEMY		"data/TEXTURE/miniMapIconEnemy.png"
#define TEX_BACK_A		"data/TEXTURE/rader001.png"
#define TEX_BACK_B		"data/TEXTURE/rader002.png"
#define TEX_OBJECT		"data/TEXTURE/miniMapIconObject.png"

//#define MINIMAP_POS_X	( SCREEN_WIDTH - 80.0f )
#define MINIMAP_POS_X	( SCREEN_WIDTH / 2 )
//#define MINIMAP_POS_Y	( 80.0f )
#define MINIMAP_POS_Y	( SCREEN_HEIGHT - 80.0f )
#define MINIMAP_WIDTH	( 80.0f )
#define MINIMAP_HEIGHT	( 80.0f )

#define MINIMAP_ICON_WIDTH	( 10.0f )
#define MINIMAP_ICON_HEIGHT ( 10.0f )

//------ グローバル変数 ------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMiniMap = NULL;				// 頂点バッファへのポインタ
LPDIRECT3DTEXTURE9		g_pTextureMiniMap[ MINIMAP_TEX_MAX ];	// テクスチャへのポインタ
MINIMAP g_aMiniMap[ MINIMAP_MAX ];

//------ 内部関数宣言 --------------------------

//-----------------------------------------------
// 頂点情報セット
//-----------------------------------------------
void SetupVertexMiniMap( void );

//-----------------------------------------------

//-----------------------------------------------
// ミニマップ初期化
//-----------------------------------------------
void InitMiniMap( void )
{
	// ローカル変数
	//---------------------------------
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスオブジェクト

	for ( int i = 0; i < MINIMAP_TEX_MAX; i++ )
	{
		g_pTextureMiniMap[ i ] = NULL;
	}

	// 頂点バッファの確保
	//------------------------------------
	pDevice ->CreateVertexBuffer((sizeof(VERTEX_2D) * 4 * MINIMAP_MAX),	// 頂点データ用に確保するバッファ
								 D3DUSAGE_WRITEONLY,					// 頂点バッファの使用方法
								 FVF_VERTEX_2D,							// 使用する頂点フォーマット
								 D3DPOOL_MANAGED,						// バッファを保持するメモリクラスの指定
								 &g_pVtxBuffMiniMap,					// 頂点バッファへのポインタ
								 NULL);									// NULL固定	
	
	// ミニマップセット
	SetMiniMap( D3DXVECTOR3( MINIMAP_POS_X, MINIMAP_POS_Y, 0.0f ), MINIMAPTYPE_BACK, 0.0f );

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEX_PLAYER, &g_pTextureMiniMap[ 0 ]);
	D3DXCreateTextureFromFile(pDevice, TEX_ENEMY, &g_pTextureMiniMap[ 1 ]);
	D3DXCreateTextureFromFile(pDevice, TEX_BACK_B, &g_pTextureMiniMap[ 2 ]);
	D3DXCreateTextureFromFile(pDevice, TEX_OBJECT, &g_pTextureMiniMap[ 3 ]);
	D3DXCreateTextureFromFile(pDevice, TEX_BACK_A, &g_pTextureMiniMap[ 4 ]);
}

//-----------------------------------------------
// ミニマップ終了処理
//-----------------------------------------------
void UninitMiniMap( void )
{
	// 使ったテクスチャの解放と初期化
	//------------------------------------
	for (int nCnt = 0; nCnt < MINIMAP_TEX_MAX; nCnt++)
	{
		if ( g_pTextureMiniMap[ nCnt ] != NULL )
		{ 
			g_pTextureMiniMap[ nCnt ] ->Release();
			g_pTextureMiniMap[ nCnt ] = NULL;
		}
	}

	// 頂点バッファの解放と初期化
	//------------------------------------
	if (g_pVtxBuffMiniMap != NULL)
	{ 
		g_pVtxBuffMiniMap ->Release();
		g_pVtxBuffMiniMap = NULL;
	}
}

//-----------------------------------------------
// ミニマップ更新処理
//-----------------------------------------------
void UpdateMiniMap( void )
{
	for ( int i = 1; i < MINIMAP_MAX; i++ )
	{
		if ( g_aMiniMap[ i ].bUse == true )
		{
			if ( i != MINIMAP_MAX - 1 )
			{
				g_aMiniMap[ i ].bUse = false;
			}
		}
	}

	// 頂点情報セット
	SetupVertexMiniMap();
}

//-----------------------------------------------
// ミニマップ描画処理
//-----------------------------------------------
void DrawMiniMap( void )
{
	// ローカル変数
	//------------------------------------
	LPDIRECT3DDEVICE9 pDevice = GetDevice();						// デバイスオブジェクト
	
	// 頂点バッファをデータストリームにバインド
	//-----------------------------------------
	pDevice ->SetStreamSource(0,
								g_pVtxBuffMiniMap,
								0,
								sizeof(VERTEX_2D));	// 頂点データの間隔

	
	// 頂点フォーマットの設定
	//------------------------------------
	pDevice ->SetFVF( FVF_VERTEX_2D );

	for ( int i = 0; i < MINIMAP_MAX; i++ )
	{
		if ( g_aMiniMap[ i ].bUse == true )
		{
			if ( i == MINIMAP_MAX - 1 )
			{
				// テクスチャの設定
				//------------------------------------
				pDevice -> SetTexture( 0, g_pTextureMiniMap[ 4 ] );
			
				// ポリゴンの描画
				//------------------------------------
				pDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,	// プリミティブの種類
													i * 4,			// 読み込む最初の番号
													2);				// 描画するプリミティブ数
			}
			else
			{
				// テクスチャの設定
				//------------------------------------
				pDevice -> SetTexture( 0, g_pTextureMiniMap[ (int)g_aMiniMap[ i ].type ] );
			
				// ポリゴンの描画
				//------------------------------------
				pDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,	// プリミティブの種類
													i * 4,			// 読み込む最初の番号
													2);				// 描画するプリミティブ数
			}
		}
	}
}

//-----------------------------------------------
// 頂点情報セット
//-----------------------------------------------
void SetupVertexMiniMap( void )
{
	// ローカル変数
	//------------------------------------
	VERTEX_2D *pVtx2D;

	g_pVtxBuffMiniMap ->Lock( 0, 0, ( void** )&pVtx2D, 0 );

	for ( int i = 0; i < MINIMAP_MAX; i++ )
	{
		if ( g_aMiniMap[ i ].bUse == true )
		{
			if ( g_aMiniMap[ i ].type == MINIMAPTYPE_PLAYER )
			{
				// 位置
				pVtx2D[ 0 + i * 4 ].pos = D3DXVECTOR3( g_aMiniMap[ i ].pos.x - g_aMiniMap[ i ].fWidth * -sinf( atan2f( g_aMiniMap[ i ].pos.y, g_aMiniMap[ i ].pos.x ) - g_aMiniMap[ i ].fRotate ), g_aMiniMap[ i ].pos.y - g_aMiniMap[ i ].fHeight * -cosf( atan2f( g_aMiniMap[ i ].pos.y, g_aMiniMap[ i ].pos.x ) - g_aMiniMap[ i ].fRotate ), 0.0f );
				pVtx2D[ 1 + i * 4 ].pos = D3DXVECTOR3( g_aMiniMap[ i ].pos.x + g_aMiniMap[ i ].fWidth * -sinf( atan2f( g_aMiniMap[ i ].pos.y, g_aMiniMap[ i ].pos.x ) + g_aMiniMap[ i ].fRotate ), g_aMiniMap[ i ].pos.y - g_aMiniMap[ i ].fHeight * -cosf( atan2f( g_aMiniMap[ i ].pos.y, g_aMiniMap[ i ].pos.x ) + g_aMiniMap[ i ].fRotate ), 0.0f );
				pVtx2D[ 2 + i * 4 ].pos = D3DXVECTOR3( g_aMiniMap[ i ].pos.x - g_aMiniMap[ i ].fWidth * -sinf( atan2f( g_aMiniMap[ i ].pos.y, g_aMiniMap[ i ].pos.x ) + g_aMiniMap[ i ].fRotate ), g_aMiniMap[ i ].pos.y + g_aMiniMap[ i ].fHeight * -cosf( atan2f( g_aMiniMap[ i ].pos.y, g_aMiniMap[ i ].pos.x ) + g_aMiniMap[ i ].fRotate ), 0.0f );
				pVtx2D[ 3 + i * 4 ].pos = D3DXVECTOR3( g_aMiniMap[ i ].pos.x + g_aMiniMap[ i ].fWidth * -sinf( atan2f( g_aMiniMap[ i ].pos.y, g_aMiniMap[ i ].pos.x ) - g_aMiniMap[ i ].fRotate ), g_aMiniMap[ i ].pos.y + g_aMiniMap[ i ].fHeight * -cosf( atan2f( g_aMiniMap[ i ].pos.y, g_aMiniMap[ i ].pos.x ) - g_aMiniMap[ i ].fRotate ), 0.0f );
			}
			else
			{
				// 位置
				pVtx2D[ 0 + i * 4 ].pos = D3DXVECTOR3( g_aMiniMap[ i ].pos.x - g_aMiniMap[ i ].fWidth, g_aMiniMap[ i ].pos.y - g_aMiniMap[ i ].fHeight, 0.0f );
				pVtx2D[ 1 + i * 4 ].pos = D3DXVECTOR3( g_aMiniMap[ i ].pos.x + g_aMiniMap[ i ].fWidth, g_aMiniMap[ i ].pos.y - g_aMiniMap[ i ].fHeight, 0.0f );
				pVtx2D[ 2 + i * 4 ].pos = D3DXVECTOR3( g_aMiniMap[ i ].pos.x - g_aMiniMap[ i ].fWidth, g_aMiniMap[ i ].pos.y + g_aMiniMap[ i ].fHeight, 0.0f );
				pVtx2D[ 3 + i * 4 ].pos = D3DXVECTOR3( g_aMiniMap[ i ].pos.x + g_aMiniMap[ i ].fWidth, g_aMiniMap[ i ].pos.y + g_aMiniMap[ i ].fHeight, 0.0f );
			}

			// パースペクティブコレクト
			pVtx2D[ 0 + i * 4 ].rhw = 1.0f;		
			pVtx2D[ 1 + i * 4 ].rhw = 1.0f;		
			pVtx2D[ 2 + i * 4 ].rhw = 1.0f;		
			pVtx2D[ 3 + i * 4 ].rhw = 1.0f;	
	
			// 色
			pVtx2D[ 0 + i * 4 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f);		
			pVtx2D[ 1 + i * 4 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f);		
			pVtx2D[ 2 + i * 4 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f);		
			pVtx2D[ 3 + i * 4 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f);		
			
			// テクスチャ座標
			pVtx2D[ 0 + i * 4 ].tex = D3DXVECTOR2( 0.0f, 0.0f);
			pVtx2D[ 1 + i * 4 ].tex = D3DXVECTOR2( 1.0f, 0.0f);
			pVtx2D[ 2 + i * 4 ].tex = D3DXVECTOR2( 0.0f, 1.0f);
			pVtx2D[ 3 + i * 4 ].tex = D3DXVECTOR2( 1.0f, 1.0f);
		}
	}

	g_pVtxBuffMiniMap ->Unlock();
}

//-----------------------------------------------
// ミニマップセット
//-----------------------------------------------
void SetMiniMap( D3DXVECTOR3 pos, MINIMAPTYPE type, float fRot )
{
	int nPlayer;

	for ( int i = 0; i < MINIMAP_MAX; i++ )
	{
		if ( g_aMiniMap[ i ].bUse == true )
		{
			if ( g_aMiniMap[ i ].type == MINIMAPTYPE_PLAYER )
			{
				nPlayer = i;
			}
		}

		if ( g_aMiniMap[ i ].bUse == false )
		{
			g_aMiniMap[ i ].worldPos = pos;		// ３D空間の座標を取得
			g_aMiniMap[ i ].type = type;		// 表示するタイプを指定

			// タイプ判定
			switch ( type )
			{
			case MINIMAPTYPE_PLAYER:
				g_aMiniMap[ i ].fWidth = MINIMAP_ICON_WIDTH;
				g_aMiniMap[ i ].fHeight = MINIMAP_ICON_HEIGHT;
				g_aMiniMap[ i ].fRotate = fRot;

				// ミニマップ座標の真ん中に固定
				g_aMiniMap[ i ].pos = D3DXVECTOR3( MINIMAP_POS_X, MINIMAP_POS_Y, 0.0f );

				break;

			case MINIMAPTYPE_ENEMY:

				// ミニマップ座標に落とし込む
				float x, y;
				x = ( g_aMiniMap[ nPlayer ].worldPos.x - g_aMiniMap[ i ].worldPos.x ) / ( SCREEN_WIDTH / MINIMAP_WIDTH );
				y = ( g_aMiniMap[ nPlayer ].worldPos.z - g_aMiniMap[ i ].worldPos.z ) / ( SCREEN_HEIGHT / MINIMAP_HEIGHT );

				// ミニマップ外だったら描画しない
				if ( g_aMiniMap[ nPlayer ].pos.x - x < MINIMAP_POS_X - MINIMAP_WIDTH + 5.0f
				||	 g_aMiniMap[ nPlayer ].pos.x - x > MINIMAP_POS_X + MINIMAP_WIDTH - 5.0f
				||	 g_aMiniMap[ nPlayer ].pos.y + y < MINIMAP_POS_Y - MINIMAP_HEIGHT + 5.0f
				||	 g_aMiniMap[ nPlayer ].pos.y + y > MINIMAP_POS_Y + MINIMAP_HEIGHT - 5.0f)
				{
					return;
				}

				// 座標を決定
				g_aMiniMap[ i ].pos = D3DXVECTOR3( g_aMiniMap[ nPlayer ].pos.x - x, g_aMiniMap[ nPlayer ].pos.y + y, 0.0f );
				g_aMiniMap[ i ].fWidth = MINIMAP_ICON_WIDTH;
				g_aMiniMap[ i ].fHeight = MINIMAP_ICON_HEIGHT;

				break;

			case MINIMAPTYPE_BACK:
				g_aMiniMap[ i ].pos = pos;
				g_aMiniMap[ i ].fWidth = MINIMAP_WIDTH;
				g_aMiniMap[ i ].fHeight = MINIMAP_HEIGHT;

				g_aMiniMap[ MINIMAP_MAX - 1 ].type = type;		// 表示するタイプを指定
				g_aMiniMap[ MINIMAP_MAX - 1 ].pos = pos;
				g_aMiniMap[ MINIMAP_MAX - 1 ].fWidth = MINIMAP_WIDTH;
				g_aMiniMap[ MINIMAP_MAX - 1 ].fHeight = MINIMAP_HEIGHT;
				g_aMiniMap[ MINIMAP_MAX - 1 ].bUse = true;
				
				break;

			case MINIMAPTYPE_OBJECT:

				// ミニマップ座標に落とし込む
				float ox, oy;
				ox = ( g_aMiniMap[ nPlayer ].worldPos.x - g_aMiniMap[ i ].worldPos.x ) / ( SCREEN_WIDTH / MINIMAP_WIDTH );
				oy = ( g_aMiniMap[ nPlayer ].worldPos.z - g_aMiniMap[ i ].worldPos.z ) / ( SCREEN_HEIGHT / MINIMAP_HEIGHT );

				// ミニマップ外だったら描画しない
				if ( g_aMiniMap[ nPlayer ].pos.x - ox < MINIMAP_POS_X - MINIMAP_WIDTH + 5.0f
				||	 g_aMiniMap[ nPlayer ].pos.x - ox > MINIMAP_POS_X + MINIMAP_WIDTH - 5.0f
				||	 g_aMiniMap[ nPlayer ].pos.y + oy < MINIMAP_POS_Y - MINIMAP_HEIGHT + 5.0f
				||	 g_aMiniMap[ nPlayer ].pos.y + oy > MINIMAP_POS_Y + MINIMAP_HEIGHT - 5.0f)
				{
					return;
				}

				// 座標を決定
				g_aMiniMap[ i ].pos = D3DXVECTOR3( g_aMiniMap[ nPlayer ].pos.x - ox, g_aMiniMap[ nPlayer ].pos.y + oy, 0.0f );
				g_aMiniMap[ i ].fWidth = MINIMAP_ICON_WIDTH;
				g_aMiniMap[ i ].fHeight = MINIMAP_ICON_HEIGHT;

				break;
			}

			g_aMiniMap[ i ].bUse = true;
			break;
		}
	}

	// 頂点情報のセット
	SetupVertexMiniMap();
}

//-----------------------------------------------
// ミニマップセット
//-----------------------------------------------
MINIMAP *GetMiniMap( void )
{
	return &g_aMiniMap[ 0 ];
}
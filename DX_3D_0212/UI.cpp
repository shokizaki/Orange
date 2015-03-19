//===============================================
//
//  内容   ： ユーザーインターフェース処理
//  File   ： UI.h
//  制作者 ： 木崎　翔
//
//===============================================

//------ インクルードファイル ------
#include "UI.h"
#include "bullet.h"
#include "model.h"
#include "texture2D.h"
#include "stringDraw.h"
#include "number.h"

//------ マクロ定義 ------
#define UI_TEX_MAX	( 5 )

#define BELL_TEX		"data/TEXTURE/suzu000.png"
#define CATCHENEMY_TEX	"data/TEXTURE/absorbNum.png"

// 音符の色を示すUI
#define BELL_WIDTH	( 70.0f )
#define BELL_HEIGHT ( 70.0f )
#define BELL_POS_X	( 80.0f )
#define BELL_POS_Y	( SCREEN_HEIGHT - 80.0f )

// 吸収率を示すUI
#define BELLSTATUS_WIDTH	( 120.0f )
#define BELLSTATUS_HEIGHT	( 120.0f )
#define BELLSTATUS_POS_X	( SCREEN_WIDTH - 100.0f )
#define BELLSTATUS_POS_Y	( SCREEN_HEIGHT - 100.0f )

// 吸収した数を示すUI
#define CATCHENEMY_WIDTH	( 90.0f )
#define CATCHENEMY_HEIGHT	( 40.0f )
#define CATCHENEMY_POS_X	( 100.0f )
#define CATCHENEMY_POS_Y	( 50.0f )
#define CATCHNUM_WIDTH		( 15.0f )
#define CATCHNUM_HEIGHT		( 30.0f )
#define CATCHNUM_POS_X		( CATCHENEMY_POS_X + 40.0f )
#define CATCHNUM_POS_Y		( CATCHENEMY_POS_Y + 35.0f )

//------ グローバル変数 ------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUI = NULL;			// 頂点バッファへのポインタ
LPDIRECT3DTEXTURE9		g_pTextureUI[ UI_TEX_MAX ];		// テクスチャへのポインタ

UI g_aUI[ UI_MAX ];		// UIの情報
bool g_bHasty = false;

NUMBER g_absorbNum;		// 吸収した数を描画するための変数

//D3DXMATRIX g_mtxProjectionUI;		// プロジェクションマトリックス

//===============================================
//
// UIに関する初期化
//
//===============================================
void InitUI( void )
{
	// ローカル変数
	//---------------------------------
	LPDIRECT3DDEVICE9 pDevice = NULL;		// デバイスオブジェクト

	// グローバル変数の初期化
	//---------------------------------

	// [ 鈴 ]
	g_aUI[ 0 ].pos = D3DXVECTOR3( BELL_POS_X, BELL_POS_Y, 0.0f );
	g_aUI[ 0 ].fWidth = BELL_WIDTH;
	g_aUI[ 0 ].fHeight = BELL_HEIGHT;
	g_aUI[ 0 ].type = UITYPE_BELL;

	// [ 鈴ステータス ]
	g_aUI[ 1 ].pos = D3DXVECTOR3( BELLSTATUS_POS_X, BELLSTATUS_POS_Y, 0.0f );
	g_aUI[ 1 ].fWidth = BELLSTATUS_WIDTH;
	g_aUI[ 1 ].fHeight = BELLSTATUS_HEIGHT;
	g_aUI[ 1 ].type = UITYPE_BELL;

	// [ 敵を吸収した数 ]
	g_aUI[ 2 ].pos = D3DXVECTOR3( CATCHENEMY_POS_X, CATCHENEMY_POS_Y, 0.0f );
	g_aUI[ 2 ].fWidth = CATCHENEMY_WIDTH;
	g_aUI[ 2 ].fHeight = CATCHENEMY_HEIGHT;
	g_aUI[ 2 ].type = UITYPE_CATCHENEMY;
	SetNumber( &g_absorbNum, D3DXVECTOR3( CATCHNUM_POS_X, CATCHNUM_POS_Y, 0.0f ), 2, CATCHNUM_WIDTH, CATCHNUM_HEIGHT );

	g_bHasty = false;

	// デバイス取得
	//------------------------------------
	pDevice = GetDevice();

	// 頂点バッファの確保
	//------------------------------------
	pDevice ->CreateVertexBuffer((sizeof(VERTEX_2D) * 4 * UI_MAX),		// 頂点データ用に確保するバッファ
								 D3DUSAGE_WRITEONLY,					// 頂点バッファの使用方法
								 FVF_VERTEX_2D,							// 使用する頂点フォーマット
								 D3DPOOL_MANAGED,						// バッファを保持するメモリクラスの指定
								 &g_pVtxBuffUI,							// 頂点バッファへのポインタ
								 NULL);									// NULL固定	

	// テクスチャへのポインタを初期化
	//------------------------------------
	for (int nCnt = 0; nCnt < UI_TEX_MAX; nCnt++)				
	{
		g_pTextureUI[ nCnt ] = NULL;
	}

	// テクスチャの読み込み
	//------------------------------------
	D3DXCreateTextureFromFile(pDevice, BELL_TEX, &g_pTextureUI[ 0 ]);
	D3DXCreateTextureFromFile(pDevice, CATCHENEMY_TEX, &g_pTextureUI[ 1 ]);

	// 頂点情報のセット
	//------------------------------------
	SetupUI();
}

//===============================================
//
// UIの頂点情報セット
//
//===============================================
void SetupUI( void )
{
	// ローカル変数
	//---------------------------------
	LPDIRECT3DDEVICE9 pDevice = NULL;		// デバイスオブジェクト
	VERTEX_2D *pVtx2D;

	// デバイス取得
	//------------------------------------
	pDevice = GetDevice();

	g_pVtxBuffUI ->Lock( 0, 0, (void**)&pVtx2D, 0 );

	for ( int i = 0; i < UI_MAX; i++ )
	{
		// 位置
		pVtx2D[ 0 + i * 4 ].pos = D3DXVECTOR3( g_aUI[ i ].pos.x - g_aUI[ i ].fWidth, g_aUI[ i ].pos.y - g_aUI[ i ].fHeight, 0.0f );
		pVtx2D[ 1 + i * 4 ].pos = D3DXVECTOR3( g_aUI[ i ].pos.x + g_aUI[ i ].fWidth, g_aUI[ i ].pos.y - g_aUI[ i ].fHeight, 0.0f );
		pVtx2D[ 2 + i * 4 ].pos = D3DXVECTOR3( g_aUI[ i ].pos.x - g_aUI[ i ].fWidth, g_aUI[ i ].pos.y + g_aUI[ i ].fHeight, 0.0f );
		pVtx2D[ 3 + i * 4 ].pos = D3DXVECTOR3( g_aUI[ i ].pos.x + g_aUI[ i ].fWidth, g_aUI[ i ].pos.y + g_aUI[ i ].fHeight, 0.0f );

		// パースペクティブコレクト＆色
		for ( int j = 0; j < 4; j++ )
		{
			pVtx2D[ j + i * 4 ].rhw = 1.0f;
			pVtx2D[ j + i * 4 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );
		}

		// テクスチャ座標
		pVtx2D[ 0 + i * 4  ].tex = D3DXVECTOR2( 0.0f, 0.0f );
		pVtx2D[ 1 + i * 4  ].tex = D3DXVECTOR2( 1.0f, 0.0f );
		pVtx2D[ 2 + i * 4  ].tex = D3DXVECTOR2( 0.0f, 1.0f );
		pVtx2D[ 3 + i * 4  ].tex = D3DXVECTOR2( 1.0f, 1.0f );
	}

	g_pVtxBuffUI ->Unlock();
}

//===============================================
//
// UIに関する終了処理
//
//===============================================
void UninitUI( void )
{
	// 使ったテクスチャの解放と初期化
	//------------------------------------
	for (int nCnt = 0; nCnt < UI_TEX_MAX; nCnt++)
	{
		if (g_pTextureUI[ nCnt ] != NULL)
		{ 
			g_pTextureUI[ nCnt ] ->Release();
			g_pTextureUI[ nCnt ] = NULL;
		}
	}

	// 頂点バッファの解放と初期化
	//------------------------------------
	if (g_pVtxBuffUI != NULL)
	{ 
		g_pVtxBuffUI ->Release();
		g_pVtxBuffUI = NULL;
	}

	UninitNumber( &g_absorbNum );
}

//===============================================
//
// UIに関する更新処理
//
//===============================================
void UpdateUI( void )
{
	// バレットUIの更新
	UpdateUIBullet();

	// 吸収率UIの更新
	UpdateUIAbsorb();

	// 吸収した数UIの更新
	UpdateUICatchEnemy();
}

//===============================================
//
// UIに関する描画処理
//
//===============================================
void DrawUI( void )
{
	// ローカル変数
	//------------------------------------
	LPDIRECT3DDEVICE9 pDevice;						// デバイスオブジェクト
	
	// デバイスの取得
	//------------------------------------
	pDevice = GetDevice();

	// 数字の描画
	DrawNumber( &g_absorbNum, GetCatchEnemy() );

	// 頂点バッファをデータストリームにバインド
	//-----------------------------------------
	pDevice ->SetStreamSource(0,
								g_pVtxBuffUI,
								0,
								sizeof(VERTEX_2D));	// 頂点データの間隔

	
	// 頂点フォーマットの設定
	//------------------------------------
	pDevice ->SetFVF( FVF_VERTEX_2D );

	{
		for ( int i = 0; i < UI_MAX; i++ )
		{
			switch ( g_aUI[ i ].type )
			{
			case UITYPE_BELL:
			
				// テクスチャの設定
				//------------------------------------
				pDevice -> SetTexture(0, g_pTextureUI[ ( int )g_aUI[ i ].type ]);
				break;

			case UITYPE_CATCHENEMY:
			
				// テクスチャの設定
				//------------------------------------
				pDevice -> SetTexture(0, g_pTextureUI[ ( int )g_aUI[ i ].type ]);

				break;

			case UITYPE_MAX:

				// テクスチャの設定
				//------------------------------------
				pDevice -> SetTexture(0, NULL);
				break;
			}

			// ポリゴンの描画
			//------------------------------------
			pDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,	// プリミティブの種類
											i * 4,				// 読み込む最初の番号
											2);				// 描画するプリミティブ数
		}
	}
}

//===============================================
//
// バレット関連のUI更新
//
//===============================================
void UpdateUIBullet( void )
{
	VERTEX_2D *pVtx2D;

	g_pVtxBuffUI ->Lock( 0, 0, (void**)&pVtx2D, 0 );

	if ( ( GetKeyboardTrigger( DIK_UP ) || GetKeyboardTrigger( DIK_DOWN ) || GetGamePadTrigger( GAMEPAD_X ) || GetGamePadTrigger( GAMEPAD_B ) )
	&&	 ( ( GetMode() == MODE_TUTORIAL && GetEnterFlag() == true ) || GetMode() == MODE_GAME ) )
	{
		PlaySound( SUZU001_SE );
		
		// テクスチャのセット
		SetTexture2D( g_aUI[ 0 ].pos, 100.0f, 100.0f, TEXTURE2DTYPE_000 );
	}

	// 音符の判定
	switch ( GetBulletType() )
	{
	case BULLETTYPE_ONP_RED:

		// 赤色
		pVtx2D[ 0 ].col = D3DXCOLOR( 1.0f, 0.0f, 0.0f, 1.0f );
		pVtx2D[ 1 ].col = D3DXCOLOR( 1.0f, 0.0f, 0.0f, 1.0f );
		pVtx2D[ 2 ].col = D3DXCOLOR( 1.0f, 0.0f, 0.0f, 1.0f );
		pVtx2D[ 3 ].col = D3DXCOLOR( 1.0f, 0.0f, 0.0f, 1.0f );

		break;

	case BULLETTYPE_ONP_YELLOW:

		// 黄色
		pVtx2D[ 0 ].col = D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f );
		pVtx2D[ 1 ].col = D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f );
		pVtx2D[ 2 ].col = D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f );
		pVtx2D[ 3 ].col = D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f );

		break;

	case BULLETTYPE_ONP_BLUE:

		// 青色
		pVtx2D[ 0 ].col = D3DXCOLOR( 0.0f, 0.0f, 1.0f, 1.0f );
		pVtx2D[ 1 ].col = D3DXCOLOR( 0.0f, 0.0f, 1.0f, 1.0f );
		pVtx2D[ 2 ].col = D3DXCOLOR( 0.0f, 0.0f, 1.0f, 1.0f );
		pVtx2D[ 3 ].col = D3DXCOLOR( 0.0f, 0.0f, 1.0f, 1.0f );

		break;
	}

	g_pVtxBuffUI ->Unlock();
}

//===============================================
//
// 吸収率関連のUI更新
//
//===============================================
void UpdateUIAbsorb( void )
{
	// ローカル変数
	//------------------------------------
	VERTEX_2D *pVtx2D;

	g_pVtxBuffUI ->Lock( 0, 0, (void**)&pVtx2D, 0 );

	if ( GetCatchEnemy() >= 0 &&  GetCatchEnemy() < 5 )
	{
		// 緑色
		pVtx2D[ 4 ].col = D3DXCOLOR( 0.0f, 1.0f, 0.0f, 1.0f );
		pVtx2D[ 5 ].col = D3DXCOLOR( 0.0f, 1.0f, 0.0f, 1.0f );
		pVtx2D[ 6 ].col = D3DXCOLOR( 0.0f, 1.0f, 0.0f, 1.0f );
		pVtx2D[ 7 ].col = D3DXCOLOR( 0.0f, 1.0f, 0.0f, 1.0f );

		if ( g_bHasty == true )
		{
			g_bHasty = false;
			SetTexture2DUse( g_aUI[ 1 ].nNumTexture2D );
		}
	}
	else if ( GetCatchEnemy() >= 5 &&  GetCatchEnemy() < 8 )
	{
		// 黄色
		pVtx2D[ 4 ].col = D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f );
		pVtx2D[ 5 ].col = D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f );
		pVtx2D[ 6 ].col = D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f );
		pVtx2D[ 7 ].col = D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f );

		if ( g_bHasty == true )
		{
			g_bHasty = false;
			SetTexture2DUse( g_aUI[ 1 ].nNumTexture2D );
		}
	}
	else if ( GetCatchEnemy() >= 10 )
	{
		// 赤色
		pVtx2D[ 4 ].col = D3DXCOLOR( 1.0f, 0.0f, 0.0f, 1.0f );
		pVtx2D[ 5 ].col = D3DXCOLOR( 1.0f, 0.0f, 0.0f, 1.0f );
		pVtx2D[ 6 ].col = D3DXCOLOR( 1.0f, 0.0f, 0.0f, 1.0f );
		pVtx2D[ 7 ].col = D3DXCOLOR( 1.0f, 0.0f, 0.0f, 1.0f );

		if ( g_bHasty == false )
		{
			g_bHasty = true;
			g_aUI[ 1 ].nNumTexture2D = SetTexture2D( D3DXVECTOR3( g_aUI[ 1 ].pos.x, g_aUI[ 1 ].pos.y - g_aUI[ 1 ].fHeight - 20.0f, 0.0f ), 30.0f, 30.0f, TEXTURE2DTYPE_HASTY2 );
		}
	}

	g_pVtxBuffUI ->Unlock();
}

//===============================================
//
// 敵を吸収した数関連のUI更新
//
//===============================================
void UpdateUICatchEnemy(void)
{

}
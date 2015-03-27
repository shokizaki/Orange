//-----------------------------------------------
//  内容：リザルト処理
//  Fail:result.cpp
//  制作者：木崎　翔
//-----------------------------------------------

//-----------------------------------------------
//  インクルードファイル
//-----------------------------------------------
#include "result.h"
#include "main.h"
#include "input.h"
#include "fade.h"

//-----------------------------------------------
//　マクロ定義
//-----------------------------------------------

enum tagRESULT_TEX
{
    RESULT_TEX_BG = 0,
    RESULT_TEX_PLAYER,

    RESULT_TEX_MAX
};

//-----------------------------------------------
//　構造体定義
//-----------------------------------------------

typedef struct tagRESULT
{
    LPDIRECT3DVERTEXBUFFER9 vtx;                    // 頂点バッファ
    LPDIRECT3DTEXTURE9      tex[RESULT_TEX_MAX];    // テクスチャバッファ
    LPD3DXMESH              pMeshPlayer;            // メッシュ情報へのポインタ
    LPD3DXBUFFER            pBuffMatPlayer;         // マテリアル情報へのポインタ
    DWORD                   nMatPlayer;             // マテリアルの数
}RESULT;

//-----------------------------------------------
//  グローバル変数
//-----------------------------------------------
RESULT g_Result;
int    g_nFrameCount = 0;
bool   g_bResultFade = false;

//-----------------------------------------------
// 背景初期化処理
//-----------------------------------------------
void InitResult()
{
    // BGM
    PlaySound(RESULT_BGM);

	g_nFrameCount = 0;
	g_bResultFade = false;

	LPDIRECT3DDEVICE9 pDevice;		// デバイスオブジェクト

	// デバイス取得
	//------------------------------------
	pDevice = GetDevice();

	pDevice ->CreateVertexBuffer(( sizeof( VERTEX_2D ) * 4 ),	// 頂点データ用に確保するバッファ
								 D3DUSAGE_WRITEONLY,				// 頂点バッファの使用方法
								 FVF_VERTEX_2D,						// 使用する頂点フォーマット
								 D3DPOOL_MANAGED,					// バッファを保持するメモリクラスの指定
								 &g_Result.vtx,					// 頂点バッファへのポインタ
								 NULL);								// NULL固定
		// データの位置を確保する（ロック）
	//------------------------------------
	VERTEX_2D *pVtx;
	g_Result.vtx ->Lock( 0, 0, (void**)&pVtx, 0 );

    pVtx[ 0 ].pos.x = 0.0f;
	pVtx[ 0 ].pos.y = 0.0f;	// 頂点の位置
	pVtx[ 0 ].pos.z = 1.0f;												// 頂点の位置

	pVtx[ 1 ].pos.x = SCREEN_WIDTH;
	pVtx[ 1 ].pos.y = 0.0f;	// 頂点の位置
	pVtx[ 1 ].pos.z = 1.0f;												// 頂点の位置

	pVtx[ 2 ].pos.x = 0.0f;
	pVtx[ 2 ].pos.y = SCREEN_HEIGHT;	// 頂点の位置
	pVtx[ 2 ].pos.z = 1.0f;												// 頂点の位置

	pVtx[ 3 ].pos.x = SCREEN_WIDTH;
	pVtx[ 3 ].pos.y = SCREEN_HEIGHT;	// 頂点の位置
	pVtx[ 3 ].pos.z = 1.0f;												// 頂点の位置

	// 法線
	pVtx[ 0 ].rhw = 1.0f;
	pVtx[ 1 ].rhw = 1.0f;
	pVtx[ 2 ].rhw = 1.0f;
	pVtx[ 3 ].rhw = 1.0f;			

	// フェードの色
	//------------------------------------
	pVtx[ 0 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );			// 頂点ごとの色の設定
	pVtx[ 1 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );			//			↓
	pVtx[ 2 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );			//			↓
	pVtx[ 3 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );			//			↓

	pVtx[ 0 ].tex = D3DXVECTOR2( 0.0f, 0.0f );
	pVtx[ 1 ].tex = D3DXVECTOR2( 1.0f, 0.0f );
	pVtx[ 2 ].tex = D3DXVECTOR2( 0.0f, 1.0f );
	pVtx[ 3 ].tex = D3DXVECTOR2( 1.0f, 1.0f );


	// 解放
	//------------------------------------
	g_Result.vtx -> Unlock();

	// テクスチャの読み込み
	//------------------------------------
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/result.jpg", &g_Result.tex[RESULT_TEX_BG]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Player.jpg", &g_Result.tex[RESULT_TEX_PLAYER]);

	// xファイルの読み込み
	//------------------------------------
	D3DXLoadMeshFromX("data/MODEL/PlayerU.x",		// 読み込むファイル名
						D3DXMESH_SYSTEMMEM,							// 
						pDevice,										// 
						NULL,											// 
						&g_Result.pBuffMatPlayer,			// 
						NULL,											// 
						&g_Result.nMatPlayer,			// 
						&g_Result.pMeshPlayer );
}

//-----------------------------------------------
// 背景終了処理
//-----------------------------------------------
void UninitResult()
{
    // プレイヤーデータ終了
    // メッシュ情報の解放と初期化
	//------------------------------------
	if ( g_Result.pMeshPlayer != NULL)
	{ 
		g_Result.pMeshPlayer ->Release();
		g_Result.pMeshPlayer = NULL;
	}
	
	// マテリアル情報の解放と初期化
	//------------------------------------
	if ( g_Result.pBuffMatPlayer != NULL)
	{ 
		g_Result.pBuffMatPlayer ->Release();
		g_Result.pBuffMatPlayer = NULL;
	}


    // 背景終了
	for ( int nCnt = 0; nCnt < RESULT_TEX_MAX; nCnt++ )
	{
		if (g_Result.tex[ nCnt ] != NULL)
		{ // 使ったテクスチャの解放と初期化
		  //------------------------------------
			g_Result.tex[ nCnt ] -> Release();
			g_Result.tex[ nCnt ] = NULL;
		}
	}

	if (g_Result.vtx != NULL)
	{ // 頂点バッファの解放と初期化
	  //------------------------------------
		g_Result.vtx->Release();
		g_Result.vtx = NULL;
	}

    // BGM
	StopSound();
}

//-----------------------------------------------
// リザルト更新処理
//-----------------------------------------------
void UpdateResult()
{
    g_nFrameCount++;

	// 遷移判定
	if ( ( GetKeyboardTrigger(DIK_RETURN)  || GetPadElecomTrigger( PAD_4 ) || GetPadElecomTrigger( PAD_10 ) || g_nFrameCount > 60 * 8 ) && GetFade() == FADE_NONE )
	{
		PlaySound( DESIDE4_SE );
		SetFade( FADE_OUT, 60 );
		SetMode( MODE_RANKING );

		g_bResultFade = true;
	}
}

//-----------------------------------------------
// 背景描画処理
//-----------------------------------------------
void DrawResult()
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
								g_Result.vtx,
								0,
								sizeof(VERTEX_2D));	// 頂点データの間隔

	// 頂点フォーマットの設定
	//------------------------------------
	pDevice -> SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	//------------------------------------
	pDevice -> SetTexture( 0, g_Result.tex[RESULT_TEX_BG] );

	// ポリゴンの描画
	//------------------------------------
	pDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,	// プリミティブの種類
									0,				// 読み込む最初の番号
									2);				// 描画するプリミティブ数

	// ワールド行列設定
    D3DXMATRIX mtxWorld;
    D3DXMatrixIdentity( &mtxWorld );
	pDevice ->SetTransform(D3DTS_WORLD, &mtxWorld );

	// 現在のマテリアル情報を保存
    D3DMATERIAL9 matDef;
	pDevice ->GetMaterial( &matDef );

	// バッファへのポインタを取得
	D3DXMATERIAL *pMat = (D3DXMATERIAL*)g_Result.pBuffMatPlayer ->GetBufferPointer();

	// マテリアルの数だけループ
	for (int nCntMat = 0; nCntMat < (int)g_Result.nMatPlayer; nCntMat++)
	{
		pDevice ->SetMaterial( &pMat[ nCntMat ].MatD3D );			// マテリアルの設定
        if( pMat[nCntMat].pTextureFilename == NULL )
        {
		    pDevice ->SetTexture( 0, NULL );							// テクスチャのセット
        }
        else
        {
		    pDevice ->SetTexture( 0, g_Result.tex[RESULT_TEX_PLAYER] );							// テクスチャのセット
        }
		g_Result.pMeshPlayer ->DrawSubset( nCntMat );		// 描画
	}
			
	// マテリアルを元に戻す
	//------------------------------------
	pDevice ->SetMaterial( &matDef );
}

//-----------------------------------------------
// 頂点情報セット
//-----------------------------------------------
void SetupResult()
{
}
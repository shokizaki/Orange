//-----------------------------------------------
//  内容：歯車描画処理
//  File : gear.cpp
//  制作者：木崎　翔
//-----------------------------------------------

//------ インクルードファイル ------
#include "gear.h"
#include "camera.h"

//------ マクロ定義 ------
#define GEAR_WIDTH ( 15.0f )
#define GEAR_HEIGHT ( 15.0f )

//------ グローバル変数 ------
GEAR g_aGear[ GEAR_MAX ];

bool g_bMoveGear = false;	// 選択しているかどうか
bool g_bGearSelect = false;	// 選択しているかどうか（生成する時の判定）
int g_nMoveGear = 0;		// 選択している歯車のインデックス
int g_nCreateGearNum = 0;	// 生成された歯車の数

//------ 内部関数宣言 ------

//-----------------------------------------------
// 歯車描画初期化
//-----------------------------------------------
void InitGear( void )
{
	// ローカル変数
	//---------------------------------
	LPDIRECT3DDEVICE9 pDevice = NULL;		// デバイスオブジェクト
	
	// デバイス取得
	//------------------------------------
	pDevice = GetDevice();

	for ( int i = 0; i < GEAR_MAX; i++ )
	{
		g_aGear[ i ].bUse = false;
		g_aGear[ i ].bRotation = false;
	}

	//{
	//	// xファイルの読み込み
	//	//------------------------------------
	//	D3DXLoadMeshFromX("data/MODEL/gear.x",		// 読み込むファイル名
	//					  D3DXMESH_SYSTEMMEM,							// 
	//					  pDevice,										// 
	//					  NULL,											// 
	//					  &pBuffMatModelInit,			// 
	//					  NULL,											// 
	//					  &numMatModelInit,			// 
	//					  &pMeshModelInit );
	//}

	// グローバル変数の初期化
	g_bMoveGear = false;
	g_nMoveGear = 0;
	g_nCreateGearNum = 0;
	g_bGearSelect = false;
}

//-----------------------------------------------
// 歯車描画終了処理
//-----------------------------------------------
void UninitGear( void )
{
	for (int nCnt = 0; nCnt < GEAR_MAX; nCnt++)
	{
		if ( g_aGear[ nCnt ].bUse == true )
		{
			// メッシュ情報の解放と初期化
			//------------------------------------
			if (g_aGear[ nCnt ].pMeshModel != NULL)
			{ 
				g_aGear[ nCnt ].pMeshModel ->Release();
				g_aGear[ nCnt ].pMeshModel = NULL;
			}
	
			// マテリアル情報の解放と初期化
			//------------------------------------
			if (g_aGear[ nCnt ].pBuffMatModel != NULL)
			{ 
				g_aGear[ nCnt ].pBuffMatModel ->Release();
				g_aGear[ nCnt ].pBuffMatModel = NULL;
			}
		}
	}
}

//-----------------------------------------------
// 歯車描画更新処理
//-----------------------------------------------
void UpdateGear( void )
{
	g_aGear[ 0 ].move = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );

	{
		// 歯車位置の保存
		if ( GetKeyboardTrigger( DIK_F4 ) == true )
		{
			// ファイルのオープン
			FILE *fp = fopen( "GearPositionInfo.txt", "wt" );

			for (int nCnt = 0; nCnt < GEAR_MAX; nCnt++)
			{
				if ( g_aGear[ nCnt ].bUse == true )
				{
					char workChar[ 256 ] = "POS = ";
					char workFloatTrans[ 256 ];

					// 位置をワークに突っ込む
					sprintf( workFloatTrans, "%.2f", g_aGear[ nCnt ].pos.x );
					strcat( workChar, workFloatTrans );		// 連結
					strcat( workChar, " " );				// 連結
					sprintf( workFloatTrans, "%.2f", g_aGear[ nCnt ].pos.y );
					strcat( workChar, workFloatTrans );		// 連結
					strcat( workChar, " " );				// 連結
					sprintf( workFloatTrans, "%.2f", g_aGear[ nCnt ].pos.z );
					strcat( workChar, workFloatTrans );		// 連結
					strcat( workChar, "\n" );				// 連結

					// 書き出し
					fputs( workChar, fp );
				}
			}

			// ファイルのクローズ
			fclose( fp );
		}

		// 回転処理
		for ( int i = 0; i < GEAR_MAX; i++ )
		{
			if ( g_aGear[ i ].bUse == true )
			{
				if ( g_aGear[ i ].bRotation == true )
				{
					g_aGear[ i ].rot.z += 0.025f;
				}
			}
		}
	}
}

//-----------------------------------------------
// 歯車描画描画処理
//-----------------------------------------------
void DrawGear( void )
{
	// ローカル変数
	//------------------------------------
	LPDIRECT3DDEVICE9 pDevice;						// デバイスオブジェクト
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;		// 計算用
	D3DXMATERIAL *pMat;								// マテリアル情報を取得する変数
	D3DMATERIAL9 matDef;							// 元のマテリアル情報

	// デバイスの取得
	//------------------------------------
	pDevice = GetDevice();

	// カメラのセット
	SetCamera();

	// それぞれのパーツの行列計算と描画開始
	for (int nCnt = 0; nCnt < GEAR_MAX; nCnt++)
	{
		if ( g_aGear[ nCnt ].bUse == true )
		{
			D3DXMatrixIdentity( &g_aGear[ nCnt ].mtxWorld );	// フォーマットの初期化
			D3DXMatrixIdentity( &mtxScl );						// 行列の初期化
			D3DXMatrixIdentity( &mtxRot );						// 行列の初期化
			D3DXMatrixIdentity( &mtxTranslate );				// 行列の初期化

			// スケールを反映
			D3DXMatrixScaling(&mtxScl, g_aGear[ nCnt ].scl.x, g_aGear[ nCnt ].scl.y, g_aGear[ nCnt ].scl.z);
			D3DXMatrixMultiply(&g_aGear[ nCnt ].mtxWorld, &g_aGear[ nCnt ].mtxWorld, &mtxScl);

			// 向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aGear[ nCnt ].rot.y, g_aGear[ nCnt ].rot.x, g_aGear[ nCnt ].rot.z);
			D3DXMatrixMultiply(&g_aGear[ nCnt ].mtxWorld, &g_aGear[ nCnt ].mtxWorld, &mtxRot);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTranslate, g_aGear[ nCnt ].pos.x, g_aGear[ nCnt ].pos.y, g_aGear[ nCnt ].pos.z);
			D3DXMatrixMultiply(&g_aGear[ nCnt ].mtxWorld, &g_aGear[ nCnt ].mtxWorld, &mtxTranslate);

			// 設定
			pDevice ->SetTransform(D3DTS_WORLD, &g_aGear[ nCnt ].mtxWorld);

			// 現在のマテリアル情報を保存
			pDevice ->GetMaterial( &matDef );

			// バッファへのポインタを取得
			pMat = (D3DXMATERIAL*)g_aGear[ nCnt ].pBuffMatModel ->GetBufferPointer();

			/*strSrc = strrchr(pMat ->pTextureFilename, '\\') + 1;

			strcat(strDest, strSrc);

			D3DXCreateTextureFromFile(pDevice, strDest, &g_pTextureModel[1]);*/
			
			// マテリアルの数だけループ
			for (int nCntMat = 0; nCntMat < (int)g_aGear[ nCnt ].numMatModel; nCntMat++)
			{
				pDevice ->SetMaterial( &pMat[ nCntMat ].MatD3D );			// マテリアルの設定
				pDevice ->SetTexture( 0, NULL );							// テクスチャのセット
				g_aGear[ nCnt ].pMeshModel ->DrawSubset( nCntMat );		// 描画
			}
			
			// マテリアルを元に戻す
			//------------------------------------
			pDevice ->SetMaterial( &matDef );
		}
	}
}

//-----------------------------------------------
// 歯車セット
//-----------------------------------------------
int SetGear( D3DXVECTOR3 pos )
{
	// ローカル変数
	//---------------------------------
	LPDIRECT3DDEVICE9 pDevice = NULL;		// デバイスオブジェクト
	
	// デバイス取得
	//------------------------------------
	pDevice = GetDevice();

	for ( int i = 0; i < GEAR_MAX; i++ )
	{
		if ( g_aGear[ i ].bUse == false )
		{
			// 位置の設定
			g_aGear[ i ].pos = pos;
			g_aGear[ i ].rot = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
			g_aGear[ i ].scl = D3DXVECTOR3( 1.0f, 1.0f, 1.0f );

			g_aGear[ i ].move = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );

			// 当たり判定用
			g_aGear[ i ].rect.pos = pos;
			g_aGear[ i ].rect.harfSize = D3DXVECTOR3( GEAR_WIDTH, GEAR_HEIGHT, GEAR_HEIGHT );

			// モデル情報をコピー
			//g_aGear[ i ].numMatModel = numMatModelInit;
			//g_aGear[ i ].pBuffMatModel = pBuffMatModelInit;
			//g_aGear[ i ].pMeshModel = pMeshModelInit;

			{
				// xファイルの読み込み
				//------------------------------------
				D3DXLoadMeshFromX("data/MODEL/gear.x",		// 読み込むファイル名
								  D3DXMESH_SYSTEMMEM,							// 
								  pDevice,										// 
								  NULL,											// 
								  &g_aGear[ i ].pBuffMatModel,			// 
								  NULL,											// 
								  &g_aGear[ i ].numMatModel,			// 
								  &g_aGear[ i ].pMeshModel );
			}

			// 回転フラグ
			g_aGear[ i ].bRotation = false;

			g_aGear[ i ].bUse = true;
			
			// 生成した数を増やす
			g_nCreateGearNum++;

			// インデックスを返す
			return i;
		}
	}

	return GEAR_MAX;
}

//-----------------------------------------------
// エディット更新
//-----------------------------------------------
void EditGear( void )
{
	if ( GetKeyboardTrigger( DIK_B ) == true && g_bGearSelect == false )
	{
		// 角度を初期化
		g_aGear[ g_nMoveGear ].rot.y = 0.0f;

		// 歯車の生成
		g_nMoveGear = SetGear( D3DXVECTOR3( 0.0f, 20.0f, 0.0f ) );
	}

	//if ( g_bMoveGear == true )
	{
		// 移動処理
		if ( GetKeyboardTrigger( DIK_A ) == true )
		{
			g_aGear[ g_nMoveGear ].pos.x += -40.0f;
		}
		if ( GetKeyboardTrigger( DIK_D ) == true )
		{
			g_aGear[ g_nMoveGear ].pos.x += 40.0f;
		}
		if ( GetKeyboardTrigger( DIK_W ) == true )
		{
			g_aGear[ g_nMoveGear ].pos.y += 40.0f;
		}
		if ( GetKeyboardTrigger( DIK_S ) == true )
		{
			g_aGear[ g_nMoveGear ].pos.y += -40.0f;
		}

		g_aGear[ g_nMoveGear ].rect.pos = g_aGear[ g_nMoveGear ].pos;
		g_aGear[ g_nMoveGear ].rot.y += 0.025f;
	}

	// 選択している歯車の変更
	if ( GetKeyboardTrigger( DIK_UP ) == true )
	{
		// 角度を初期化
		g_aGear[ g_nMoveGear ].rot.y = 0.0f;
			
		g_nMoveGear--;
		if ( g_nMoveGear < 0 )
		{
			g_nMoveGear = g_nCreateGearNum - 1;
		}
	}
	if ( GetKeyboardTrigger( DIK_DOWN ) == true )
	{
		// 角度を初期化
		g_aGear[ g_nMoveGear ].rot.y = 0.0f;

		g_nMoveGear++;
		if ( g_nMoveGear > g_nCreateGearNum - 1 )
		{
			g_nMoveGear = 0;
		}
	}

	// 歯車の削除
	if ( GetKeyboardTrigger( DIK_SPACE ) == true )
	{
		g_nCreateGearNum--;
		if ( g_nCreateGearNum < 0 )
		{
			g_nCreateGearNum = 0;
		}

		for ( int i = g_nMoveGear; i < g_nCreateGearNum; i++ )
		{
			g_aGear[ i ].pos = g_aGear[ i + 1 ].pos;
		}

		g_aGear[ g_nCreateGearNum ].bUse = false;
	}

	// 歯車位置の保存
	if ( GetKeyboardTrigger( DIK_F4 ) == true )
	{
		// ファイルのオープン
		FILE *fp = fopen( "GearPositionInfo.txt", "wt" );

		for (int nCnt = 0; nCnt < GEAR_MAX; nCnt++)
		{
			if ( g_aGear[ nCnt ].bUse == true )
			{
				char workChar[ 256 ] = "POS = ";
				char workFloatTrans[ 256 ];

				// 位置をワークに突っ込む
				sprintf( workFloatTrans, "%.2f", g_aGear[ nCnt ].pos.x );
				strcat( workChar, workFloatTrans );		// 連結
				strcat( workChar, " " );				// 連結
				sprintf( workFloatTrans, "%.2f", g_aGear[ nCnt ].pos.y );
				strcat( workChar, workFloatTrans );		// 連結
				strcat( workChar, " " );				// 連結
				sprintf( workFloatTrans, "%.2f", g_aGear[ nCnt ].pos.z );
				strcat( workChar, workFloatTrans );		// 連結
				strcat( workChar, "\n" );				// 連結

				// 書き出し
				fputs( workChar, fp );
			}
		}

		// ファイルのクローズ
		fclose( fp );
	}

	PrintDebugProc("[ Bキーで歯車生成 ]\n");
	PrintDebugProc("[ SPACEキーで選択している歯車を削除 ]\n");
	PrintDebugProc("歯車の個数 : %d\n", g_nCreateGearNum);
	PrintDebugProc("↑、↓ *** 歯車番号の選択");
	PrintDebugProc("選択している歯車番号 : %d\n", g_nMoveGear);

	int nNum = g_nMoveGear - 10;
	if ( nNum < 0 )
	{
		nNum = 0;
	}
	for ( int i = nNum; i < nNum+ 11; i++ )
	{
		if ( g_aGear[ i ].bUse == true )
		{
			if ( i == g_nMoveGear )
			{
				PrintDebugProc("->* [ %d ] * %f %f %f\n", i, g_aGear[ i ].pos.x, g_aGear[ i ].pos.y, g_aGear[ i ].pos.z);
			}
			else
			{
				PrintDebugProc("* [ %d ] * %f %f %f\n", i, g_aGear[ i ].pos.x, g_aGear[ i ].pos.y, g_aGear[ i ].pos.z);
			}
		}
	}
}

//-----------------------------------------------
// 当たり判定の取得
//-----------------------------------------------
GEAR *GetGear( void )
{
	return &g_aGear[ 0 ];
}

//-----------------------------------------------
// 回転させる判定
//-----------------------------------------------
void SetGearRotation( int num, bool use )
{
	g_aGear[ num ].bRotation = use;
}

//-----------------------------------------------
// 回転させる判定の取得
//-----------------------------------------------
bool GetGearRotation( int num )
{
	return g_aGear[ num ].bRotation;
}

//-----------------------------------------------
// 角度直し
//-----------------------------------------------
void ZeroGearRotation( void )
{
	for (int nCnt = 0; nCnt < GEAR_MAX; nCnt++)
	{
		if ( g_aGear[ nCnt ].bUse == true )
		{
			g_aGear[ nCnt ].rot = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
		}
	}
}
//-----------------------------------------------
//  内容：四角描画処理
//  File : cube.cpp
//  制作者：木崎　翔
//-----------------------------------------------

//------ インクルードファイル ------
#include "cube.h"
#include "camera.h"
#include "player.h"
#include "sheet.h"

//------ マクロ定義 ------
#define CUBE_WIDTH ( 15.0f )
#define CUBE_HEIGHT ( 15.0f )

//------ グローバル変数 ------
CUBE g_aCube[ CUBE_MAX ];

bool g_bMoveCube = false;	// 選択しているかどうか
bool g_bCubeSelect = false;	// 選択しているかどうか（生成する時の判定）
int g_nMoveCube = 0;		// 選択しているキューブのインデックス
int g_nCreateCubeNum = 0;	// 生成されたキューブの数

//------ 内部関数宣言 ------

//-----------------------------------------------
// 四角描画初期化
//-----------------------------------------------
void InitCube( void )
{
	// ローカル変数
	//---------------------------------
	LPDIRECT3DDEVICE9 pDevice = NULL;		// デバイスオブジェクト
	
	// デバイス取得
	//------------------------------------
	pDevice = GetDevice();

	for ( int i = 0; i < CUBE_MAX; i++ )
	{
		g_aCube[ i ].bUse = false;
	}

	// グローバル変数の初期化
	g_bMoveCube = false;
	g_nMoveCube = 0;
	//g_bEdit = false;
	g_nCreateCubeNum = 0;
	g_bCubeSelect = false;

	// 位置読み込み
	// ファイルの読み込み開始
	FILE *fp = fopen( "CubePos.txt", "rt" );
	char strWork[ 256 ];
	D3DXVECTOR3 fWork;
	while ( strcmp( strWork, "END_SCRIPT" ) != 0 )
	{
		// 一行読み込む
		fscanf( fp, "%s = %f %f %f" , strWork, &fWork.x, &fWork.y, &fWork.z );

		if ( strcmp( strWork, "POS" ) == 0 )
		{
			// 白キューブ生成
			SetCube( fWork );
		}
	}
}

//-----------------------------------------------
// 四角描画終了処理
//-----------------------------------------------
void UninitCube( void )
{
	for (int nCnt = 0; nCnt < CUBE_MAX; nCnt++)
	{
		if ( g_aCube[ nCnt ].bUse == true )
		{
			// メッシュ情報の解放と初期化
			//------------------------------------
			if (g_aCube[ nCnt ].pMeshModel != NULL)
			{ 
				g_aCube[ nCnt ].pMeshModel ->Release();
				g_aCube[ nCnt ].pMeshModel = NULL;
			}
	
			// マテリアル情報の解放と初期化
			//------------------------------------
			if (g_aCube[ nCnt ].pBuffMatModel != NULL)
			{ 
				g_aCube[ nCnt ].pBuffMatModel ->Release();
				g_aCube[ nCnt ].pBuffMatModel = NULL;
			}
		}
	}
}

//-----------------------------------------------
// 四角描画更新処理
//-----------------------------------------------
void UpdateCube( void )
{
	g_aCube[ 0 ].move = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );

	{
		//// キューブ位置の保存
		//if ( GetKeyboardTrigger( DIK_F2 ) == true )
		//{
		//	// ファイルのオープン
		//	FILE *fp = fopen( "CubePositionInfo.txt", "wt" );

		//	for (int nCnt = 0; nCnt < CUBE_MAX; nCnt++)
		//	{
		//		if ( g_aCube[ nCnt ].bUse == true )
		//		{
		//			char workChar[ 256 ] = "POS = ";
		//			char workFloatTrans[ 256 ];

		//			// 位置をワークに突っ込む
		//			sprintf( workFloatTrans, "%.2f", g_aCube[ nCnt ].pos.x );
		//			strcat( workChar, workFloatTrans );		// 連結
		//			strcat( workChar, " " );				// 連結
		//			sprintf( workFloatTrans, "%.2f", g_aCube[ nCnt ].pos.y );
		//			strcat( workChar, workFloatTrans );		// 連結
		//			strcat( workChar, " " );				// 連結
		//			sprintf( workFloatTrans, "%.2f", g_aCube[ nCnt ].pos.z );
		//			strcat( workChar, workFloatTrans );		// 連結
		//			strcat( workChar, "\n" );				// 連結

		//			// 書き出し
		//			fputs( workChar, fp );
		//		}
		//	}

		//	// ファイルのクローズ
		//	fclose( fp );
		//}

		// 移動処理（デバッグ用）
		/*if ( g_aCube[ 0 ].bUse == true )
		{
			if ( GetKeyboardPress( DIK_I ) == true )
			{
				g_aCube[ 0 ].move.y = 3.0f;
				g_aCube[ 0 ].pos.y += g_aCube[ 0 ].move.y;
				g_aCube[ 0 ].rect.pos = g_aCube[ 0 ].pos;	
			}
			if ( GetKeyboardPress( DIK_K ) == true )
			{
				g_aCube[ 0 ].move.y = -3.0f;
				g_aCube[ 0 ].pos.y += g_aCube[ 0 ].move.y;
				g_aCube[ 0 ].rect.pos = g_aCube[ 0 ].pos;	
			}
			if ( GetKeyboardPress( DIK_J ) == true )
			{
				g_aCube[ 0 ].move.x = -3.0f;
				g_aCube[ 0 ].pos.x += g_aCube[ 0 ].move.x;
				g_aCube[ 0 ].rect.pos = g_aCube[ 0 ].pos;	
			}
			if ( GetKeyboardPress( DIK_L ) == true )
			{
				g_aCube[ 0 ].move.x = 3.0f;
				g_aCube[ 0 ].pos.x += g_aCube[ 0 ].move.x;
				g_aCube[ 0 ].rect.pos = g_aCube[ 0 ].pos;	
			}
		}*/
	}
}

//-----------------------------------------------
// 四角描画描画処理
//-----------------------------------------------
void DrawCube( void )
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

	// シートが張られてたら
	if ( GetSheet() == true )
	{
		// それぞれのパーツの行列計算と描画開始
		for (int nCnt = 0; nCnt < CUBE_MAX; nCnt++)
		{
			if ( g_aCube[ nCnt ].bUse == true )
			{
				D3DXMatrixIdentity( &g_aCube[ nCnt ].mtxWorld );	// フォーマットの初期化
				D3DXMatrixIdentity( &mtxScl );						// 行列の初期化
				D3DXMatrixIdentity( &mtxRot );						// 行列の初期化
				D3DXMatrixIdentity( &mtxTranslate );				// 行列の初期化

				// スケールを反映
				D3DXMatrixScaling(&mtxScl, g_aCube[ nCnt ].scl.x, g_aCube[ nCnt ].scl.y, g_aCube[ nCnt ].scl.z);
				D3DXMatrixMultiply(&g_aCube[ nCnt ].mtxWorld, &g_aCube[ nCnt ].mtxWorld, &mtxScl);

				// 向きを反映
				D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aCube[ nCnt ].rot.y, g_aCube[ nCnt ].rot.x, g_aCube[ nCnt ].rot.z);
				D3DXMatrixMultiply(&g_aCube[ nCnt ].mtxWorld, &g_aCube[ nCnt ].mtxWorld, &mtxRot);

				// 位置を反映
				D3DXMatrixTranslation(&mtxTranslate, g_aCube[ nCnt ].pos.x, g_aCube[ nCnt ].pos.y, g_aCube[ nCnt ].pos.z);
				D3DXMatrixMultiply(&g_aCube[ nCnt ].mtxWorld, &g_aCube[ nCnt ].mtxWorld, &mtxTranslate);

				// 設定
				pDevice ->SetTransform(D3DTS_WORLD, &g_aCube[ nCnt ].mtxWorld);

				// 現在のマテリアル情報を保存
				pDevice ->GetMaterial( &matDef );

				// バッファへのポインタを取得
				pMat = (D3DXMATERIAL*)g_aCube[ nCnt ].pBuffMatModel ->GetBufferPointer();

				/*strSrc = strrchr(pMat ->pTextureFilename, '\\') + 1;

				strcat(strDest, strSrc);

				D3DXCreateTextureFromFile(pDevice, strDest, &g_pTextureModel[1]);*/
			
				// マテリアルの数だけループ
				for (int nCntMat = 0; nCntMat < (int)g_aCube[ nCnt ].numMatModel; nCntMat++)
				{
					pDevice ->SetMaterial( &pMat[ nCntMat ].MatD3D );			// マテリアルの設定
					pDevice ->SetTexture( 0, g_aCube[ nCnt ].pTexture );							// テクスチャのセット
					g_aCube[ nCnt ].pMeshModel ->DrawSubset( nCntMat );		// 描画
				}
			
				// マテリアルを元に戻す
				//------------------------------------
				pDevice ->SetMaterial( &matDef );
			}
		}
	}
}

//-----------------------------------------------
// 四角セット
//-----------------------------------------------
int SetCube( D3DXVECTOR3 pos )
{
	// ローカル変数
	//---------------------------------
	LPDIRECT3DDEVICE9 pDevice = NULL;		// デバイスオブジェクト
	
	// デバイス取得
	//------------------------------------
	pDevice = GetDevice();

	for ( int i = 0; i < CUBE_MAX; i++ )
	{
		if ( g_aCube[ i ].bUse == false )
		{
			// 位置の設定
			g_aCube[ i ].pos = pos;
			g_aCube[ i ].rot = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
			g_aCube[ i ].scl = D3DXVECTOR3( 1.0f, 1.0f, 1.0f );

			g_aCube[ i ].move = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );

			// 当たり判定用
			g_aCube[ i ].rect.pos = pos;
			g_aCube[ i ].rect.harfSize = D3DXVECTOR3( CUBE_WIDTH, CUBE_HEIGHT, CUBE_HEIGHT );

			// モデル情報をコピー
			//g_aCube[ i ].numMatModel = numMatModelInit;
			//g_aCube[ i ].pBuffMatModel = pBuffMatModelInit;
			//g_aCube[ i ].pMeshModel = pMeshModelInit;

			{
				// xファイルの読み込み
				//------------------------------------
				D3DXLoadMeshFromX("data/MODEL/Block_Gre.x",		// 読み込むファイル名
								  D3DXMESH_SYSTEMMEM,							// 
								  pDevice,										// 
								  NULL,											// 
								  &g_aCube[ i ].pBuffMatModel,			// 
								  NULL,											// 
								  &g_aCube[ i ].numMatModel,			// 
								  &g_aCube[ i ].pMeshModel );
			}

			// テクスチャの読み込み
			//------------------------------------
			D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Block.jpg", &g_aCube[ i ].pTexture);

			g_aCube[ i ].bUse = true;
			
			// 生成した数を増やす
			g_nCreateCubeNum++;

			// インデックスを返す
			return i;
		}
	}

	return CUBE_MAX;
}

//-----------------------------------------------
// エディット更新
//-----------------------------------------------
void EditCube( void )
{
	if ( GetKeyboardTrigger( DIK_B ) == true && g_bCubeSelect == false )
	{
		// 角度を初期化
		g_aCube[ g_nMoveCube ].rot.y = 0.0f;

		// キューブの生成
		if ( g_nCreateCubeNum == 0 )
		{
			g_nMoveCube = SetCube( D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );
		}
		else
		{
			g_nMoveCube = SetCube( D3DXVECTOR3( g_aCube[ g_nMoveCube ].pos.x + 40.0f, g_aCube[ g_nMoveCube ].pos.y, 0.0f ) );
		}
	}

	//if ( g_bMoveCube == true )
	{
		// 移動処理
		if ( GetKeyboardTrigger( DIK_A ) == true )
		{
			g_aCube[ g_nMoveCube ].pos.x += -40.0f;
		}
		if ( GetKeyboardTrigger( DIK_D ) == true )
		{
			g_aCube[ g_nMoveCube ].pos.x += 40.0f;
		}
		if ( GetKeyboardTrigger( DIK_W ) == true )
		{
			g_aCube[ g_nMoveCube ].pos.y += 40.0f;
		}
		if ( GetKeyboardTrigger( DIK_S ) == true )
		{
			g_aCube[ g_nMoveCube ].pos.y += -40.0f;
		}

		g_aCube[ g_nMoveCube ].rect.pos = g_aCube[ g_nMoveCube ].pos;
		g_aCube[ g_nMoveCube ].rot.y += 0.025f;
	}

	// 選択しているキューブの変更
	if ( GetKeyboardTrigger( DIK_UP ) == true )
	{
		// 角度を初期化
		g_aCube[ g_nMoveCube ].rot.y = 0.0f;
			
		g_nMoveCube--;
		if ( g_nMoveCube < 0 )
		{
			g_nMoveCube = g_nCreateCubeNum - 1;
		}
	}
	if ( GetKeyboardTrigger( DIK_DOWN ) == true )
	{
		// 角度を初期化
		g_aCube[ g_nMoveCube ].rot.y = 0.0f;

		g_nMoveCube++;
		if ( g_nMoveCube > g_nCreateCubeNum - 1 )
		{
			g_nMoveCube = 0;
		}
	}

	// キューブの削除
	if ( GetKeyboardTrigger( DIK_SPACE ) == true )
	{
		g_nCreateCubeNum--;
		if ( g_nCreateCubeNum < 0 )
		{
			g_nCreateCubeNum = 0;
		}

		for ( int i = g_nMoveCube; i < g_nCreateCubeNum; i++ )
		{
			g_aCube[ i ] = g_aCube[ i + 1 ];
		}

		g_aCube[ g_nCreateCubeNum ].bUse = false;
			
	}

	// キューブ位置の保存
	if ( GetKeyboardTrigger( DIK_F2 ) == true )
	{
		// ファイルのオープン
		FILE *fp = fopen( "CubePositionInfo.txt", "wt" );

		for (int nCnt = 0; nCnt < CUBE_MAX; nCnt++)
		{
			if ( g_aCube[ nCnt ].bUse == true )
			{
				char workChar[ 256 ] = "POS = ";
				char workFloatTrans[ 256 ];

				// 位置をワークに突っ込む
				sprintf( workFloatTrans, "%.2f", g_aCube[ nCnt ].pos.x );
				strcat( workChar, workFloatTrans );		// 連結
				strcat( workChar, " " );				// 連結
				sprintf( workFloatTrans, "%.2f", g_aCube[ nCnt ].pos.y );
				strcat( workChar, workFloatTrans );		// 連結
				strcat( workChar, " " );				// 連結
				sprintf( workFloatTrans, "%.2f", g_aCube[ nCnt ].pos.z );
				strcat( workChar, workFloatTrans );		// 連結
				strcat( workChar, "\n" );				// 連結

				// 書き出し
				fputs( workChar, fp );
			}
		}

		// ファイルのクローズ
		fclose( fp );
	}

	// デバッグ表示
	PrintDebugProc("[ Bキーでキューブ生成 ]\n");
	PrintDebugProc("[ SPACEキーで選択しているキューブを削除 ]\n");
	PrintDebugProc("キューブの個数 : %d\n", g_nCreateCubeNum);
	PrintDebugProc("↑、↓ *** キューブ番号の選択");
	PrintDebugProc("選択しているキューブ番号 : %d\n", g_nMoveCube);

	int nNum = g_nMoveCube - 10;
	if ( nNum < 0 )
	{
		nNum = 0;
	}
	for ( int i = nNum; i < nNum + 11; i++ )
	{
		if ( g_aCube[ i ].bUse == true )
		{
			if ( i == g_nMoveCube )
			{
				PrintDebugProc("->* [ %d ] * %f %f %f\n", i, g_aCube[ i ].pos.x, g_aCube[ i ].pos.y, g_aCube[ i ].pos.z);
			}
			else
			{
				PrintDebugProc("* [ %d ] * %f %f %f\n", i, g_aCube[ i ].pos.x, g_aCube[ i ].pos.y, g_aCube[ i ].pos.z);
			}
		}
	}
}

//-----------------------------------------------
// 当たり判定の取得
//-----------------------------------------------
CUBE *GetCube( void )
{
	return &g_aCube[ 0 ];
}

//-----------------------------------------------
// 角度直し
//-----------------------------------------------
void ZeroCubeRotation( void )
{
	for (int nCnt = 0; nCnt < CUBE_MAX; nCnt++)
	{
		if ( g_aCube[ nCnt ].bUse == true )
		{
			g_aCube[ nCnt ].rot = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
		}
	}
}
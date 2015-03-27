//-----------------------------------------------
//  内容：歯車描画処理
//  File : goal.cpp
//  制作者：木崎　翔
//-----------------------------------------------

//------ インクルードファイル ------
#include "goal.h"
#include "camera.h"
#include "player.h"
#include "sheet.h"

//------ マクロ定義 ------
#define GOAL_WIDTH ( 15.0f )
#define GOAL_HEIGHT ( 15.0f )

//------ グローバル変数 ------
GOAL g_aGoal[ GOAL_MAX ];

bool g_bMoveGoal = false;	// 選択しているかどうか
bool g_bGoalSelect = false;	// 選択しているかどうか（生成する時の判定）
int g_nMoveGoal = 0;		// 選択している歯車のインデックス
int g_nCreateGoalNum = 0;	// 生成された歯車の数

//------ 内部関数宣言 ------

//-----------------------------------------------
// 歯車描画初期化
//-----------------------------------------------
void InitGoal( void )
{
	// ローカル変数
	//---------------------------------
	LPDIRECT3DDEVICE9 pDevice = NULL;		// デバイスオブジェクト
	
	// デバイス取得
	//------------------------------------
	pDevice = GetDevice();

	for ( int i = 0; i < GOAL_MAX; i++ )
	{
		g_aGoal[ i ].bUse = false;
		g_aGoal[ i ].bRotation = false;
	}

	//{
	//	// xファイルの読み込み
	//	//------------------------------------
	//	D3DXLoadMeshFromX("data/MODEL/goal.x",		// 読み込むファイル名
	//					  D3DXMESH_SYSTEMMEM,							// 
	//					  pDevice,										// 
	//					  NULL,											// 
	//					  &pBuffMatModelInit,			// 
	//					  NULL,											// 
	//					  &numMatModelInit,			// 
	//					  &pMeshModelInit );
	//}

	// グローバル変数の初期化
	g_bMoveGoal = false;
	g_nMoveGoal = 0;
	g_nCreateGoalNum = 0;
	g_bGoalSelect = false;

	// 位置読み込み処理開始	
	FILE *fp = fopen( "GoalPos.txt", "rt" );
	char strWork[ 256 ];
	D3DXVECTOR3 fWork;

	// ファイルの読み込み開始
	while ( strcmp( strWork, "END_SCRIPT" ) != 0 )
	{
		// 一行読み込む
		fscanf( fp, "%s = %f %f %f" , strWork, &fWork.x, &fWork.y, &fWork.z );

		if ( strcmp( strWork, "POS" ) == 0 )
		{
			// 白キューブ生成
			SetGoal( fWork );
		}
	}
}

//-----------------------------------------------
// 歯車描画終了処理
//-----------------------------------------------
void UninitGoal( void )
{
	for (int nCnt = 0; nCnt < GOAL_MAX; nCnt++)
	{
		if ( g_aGoal[ nCnt ].bUse == true )
		{
			// メッシュ情報の解放と初期化
			//------------------------------------
			if (g_aGoal[ nCnt ].pMeshModel != NULL)
			{ 
				g_aGoal[ nCnt ].pMeshModel ->Release();
				g_aGoal[ nCnt ].pMeshModel = NULL;
			}
	
			// マテリアル情報の解放と初期化
			//------------------------------------
			if (g_aGoal[ nCnt ].pBuffMatModel != NULL)
			{ 
				g_aGoal[ nCnt ].pBuffMatModel ->Release();
				g_aGoal[ nCnt ].pBuffMatModel = NULL;
			}
		}
	}
}

//-----------------------------------------------
// 歯車描画更新処理
//-----------------------------------------------
void UpdateGoal( void )
{
	g_aGoal[ 0 ].move = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );

	{
		// 歯車位置の保存
		if ( GetKeyboardTrigger( DIK_F4 ) == true )
		{
			// ファイルのオープン
			FILE *fp = fopen( "GoalPositionInfo.txt", "wt" );

			for (int nCnt = 0; nCnt < GOAL_MAX; nCnt++)
			{
				if ( g_aGoal[ nCnt ].bUse == true )
				{
					char workChar[ 256 ] = "POS = ";
					char workFloatTrans[ 256 ];

					// 位置をワークに突っ込む
					sprintf( workFloatTrans, "%.2f", g_aGoal[ nCnt ].pos.x );
					strcat( workChar, workFloatTrans );		// 連結
					strcat( workChar, " " );				// 連結
					sprintf( workFloatTrans, "%.2f", g_aGoal[ nCnt ].pos.y );
					strcat( workChar, workFloatTrans );		// 連結
					strcat( workChar, " " );				// 連結
					sprintf( workFloatTrans, "%.2f", g_aGoal[ nCnt ].pos.z );
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
		for ( int i = 0; i < GOAL_MAX; i++ )
		{
			if ( g_aGoal[ i ].bUse == true )
			{
				if ( g_aGoal[ i ].bRotation == true )
				{
					g_aGoal[ i ].rot.z += 0.025f;
				}
			}
		}
	}
}

//-----------------------------------------------
// 歯車描画描画処理
//-----------------------------------------------
void DrawGoal( void )
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
	//if ( GetSheet() == true )
	{
		// それぞれのパーツの行列計算と描画開始
		for (int nCnt = 0; nCnt < GOAL_MAX; nCnt++)
		{
			if ( g_aGoal[ nCnt ].bUse == true )
			{
				D3DXMatrixIdentity( &g_aGoal[ nCnt ].mtxWorld );	// フォーマットの初期化
				D3DXMatrixIdentity( &mtxScl );						// 行列の初期化
				D3DXMatrixIdentity( &mtxRot );						// 行列の初期化
				D3DXMatrixIdentity( &mtxTranslate );				// 行列の初期化

				// スケールを反映
				D3DXMatrixScaling(&mtxScl, g_aGoal[ nCnt ].scl.x, g_aGoal[ nCnt ].scl.y, g_aGoal[ nCnt ].scl.z);
				D3DXMatrixMultiply(&g_aGoal[ nCnt ].mtxWorld, &g_aGoal[ nCnt ].mtxWorld, &mtxScl);

				// 向きを反映
				D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aGoal[ nCnt ].rot.y, g_aGoal[ nCnt ].rot.x, g_aGoal[ nCnt ].rot.z);
				D3DXMatrixMultiply(&g_aGoal[ nCnt ].mtxWorld, &g_aGoal[ nCnt ].mtxWorld, &mtxRot);

				// 位置を反映
				D3DXMatrixTranslation(&mtxTranslate, g_aGoal[ nCnt ].pos.x, g_aGoal[ nCnt ].pos.y, g_aGoal[ nCnt ].pos.z);
				D3DXMatrixMultiply(&g_aGoal[ nCnt ].mtxWorld, &g_aGoal[ nCnt ].mtxWorld, &mtxTranslate);

				// 設定
				pDevice ->SetTransform(D3DTS_WORLD, &g_aGoal[ nCnt ].mtxWorld);

				// 現在のマテリアル情報を保存
				pDevice ->GetMaterial( &matDef );

				// バッファへのポインタを取得
				pMat = (D3DXMATERIAL*)g_aGoal[ nCnt ].pBuffMatModel ->GetBufferPointer();

				/*strSrc = strrchr(pMat ->pTextureFilename, '\\') + 1;

				strcat(strDest, strSrc);

				D3DXCreateTextureFromFile(pDevice, strDest, &g_pTextureModel[1]);*/
			
				// マテリアルの数だけループ
				for (int nCntMat = 0; nCntMat < (int)g_aGoal[ nCnt ].numMatModel; nCntMat++)
				{
					pDevice ->SetMaterial( &pMat[ nCntMat ].MatD3D );			// マテリアルの設定
					pDevice ->SetTexture( 0, g_aGoal[ nCnt ].pTexture );							// テクスチャのセット
					g_aGoal[ nCnt ].pMeshModel ->DrawSubset( nCntMat );		// 描画
				}
			
				// マテリアルを元に戻す
				//------------------------------------
				pDevice ->SetMaterial( &matDef );
			}
		}
	}
}

//-----------------------------------------------
// 歯車セット
//-----------------------------------------------
int SetGoal( D3DXVECTOR3 pos )
{
	// ローカル変数
	//---------------------------------
	LPDIRECT3DDEVICE9 pDevice = NULL;		// デバイスオブジェクト
	
	// デバイス取得
	//------------------------------------
	pDevice = GetDevice();

	for ( int i = 0; i < GOAL_MAX; i++ )
	{
		if ( g_aGoal[ i ].bUse == false )
		{
			// 位置の設定
			g_aGoal[ i ].pos = pos;
			g_aGoal[ i ].rot = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
			g_aGoal[ i ].scl = D3DXVECTOR3( 1.0f, 1.0f, 1.0f );

			g_aGoal[ i ].move = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );

			// 当たり判定用
			g_aGoal[ i ].rect.pos = pos;
			g_aGoal[ i ].rect.harfSize = D3DXVECTOR3( GOAL_WIDTH, GOAL_HEIGHT, GOAL_HEIGHT );

			// モデル情報をコピー
			//g_aGoal[ i ].numMatModel = numMatModelInit;
			//g_aGoal[ i ].pBuffMatModel = pBuffMatModelInit;
			//g_aGoal[ i ].pMeshModel = pMeshModelInit;

			{
				// xファイルの読み込み
				//------------------------------------
				D3DXLoadMeshFromX("data/MODEL/gear.x",		// 読み込むファイル名
								  D3DXMESH_SYSTEMMEM,			// 
								  pDevice,						// 
								  NULL,							// 
								  &g_aGoal[ i ].pBuffMatModel,	// 
								  NULL,							// 
								  &g_aGoal[ i ].numMatModel,	// 
								  &g_aGoal[ i ].pMeshModel );
			}

			// テクスチャの読み込み
			//------------------------------------
			D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Block.jpg", &g_aGoal[ i ].pTexture);

			// 回転フラグ
			g_aGoal[ i ].bRotation = false;

			g_aGoal[ i ].bUse = true;
			
			// 生成した数を増やす
			g_nCreateGoalNum++;

			// インデックスを返す
			return i;
		}
	}

	return GOAL_MAX;
}

//-----------------------------------------------
// エディット更新
//-----------------------------------------------
void EditGoal( void )
{
	if ( GetKeyboardTrigger( DIK_B ) == true && g_bGoalSelect == false )
	{
		// 角度を初期化
		g_aGoal[ g_nMoveGoal ].rot.y = 0.0f;

		// 歯車の生成
		g_nMoveGoal = SetGoal( D3DXVECTOR3( g_aGoal[ g_nMoveGoal ].pos.x + 40.0f, g_aGoal[ g_nMoveGoal ].pos.y, g_aGoal[ g_nMoveGoal ].pos.z ) );
	}

	//if ( g_bMoveGoal == true )
	{
		// 移動処理
		if ( GetKeyboardTrigger( DIK_A ) == true )
		{
			g_aGoal[ g_nMoveGoal ].pos.x += -40.0f;
		}
		if ( GetKeyboardTrigger( DIK_D ) == true )
		{
			g_aGoal[ g_nMoveGoal ].pos.x += 40.0f;
		}
		if ( GetKeyboardTrigger( DIK_W ) == true )
		{
			g_aGoal[ g_nMoveGoal ].pos.y += 40.0f;
		}
		if ( GetKeyboardTrigger( DIK_S ) == true )
		{
			g_aGoal[ g_nMoveGoal ].pos.y += -40.0f;
		}

		g_aGoal[ g_nMoveGoal ].rect.pos = g_aGoal[ g_nMoveGoal ].pos;
		g_aGoal[ g_nMoveGoal ].rot.y += 0.025f;
	}

	// 選択している歯車の変更
	if ( GetKeyboardTrigger( DIK_UP ) == true )
	{
		// 角度を初期化
		g_aGoal[ g_nMoveGoal ].rot.y = 0.0f;
			
		g_nMoveGoal--;
		if ( g_nMoveGoal < 0 )
		{
			g_nMoveGoal = g_nCreateGoalNum - 1;
		}
	}
	if ( GetKeyboardTrigger( DIK_DOWN ) == true )
	{
		// 角度を初期化
		g_aGoal[ g_nMoveGoal ].rot.y = 0.0f;

		g_nMoveGoal++;
		if ( g_nMoveGoal > g_nCreateGoalNum - 1 )
		{
			g_nMoveGoal = 0;
		}
	}

	// 歯車の削除
	if ( GetKeyboardTrigger( DIK_SPACE ) == true )
	{
		g_nCreateGoalNum--;
		if ( g_nCreateGoalNum < 0 )
		{
			g_nCreateGoalNum = 0;
		}

		for ( int i = g_nMoveGoal; i < g_nCreateGoalNum; i++ )
		{
			g_aGoal[ i ].pos = g_aGoal[ i + 1 ].pos;
			g_aGoal[ i ].rect.pos = g_aGoal[ i + 1 ].pos;
		}

		g_aGoal[ g_nCreateGoalNum ].bUse = false;
	}

	// 歯車位置の保存
	if ( GetKeyboardTrigger( DIK_F4 ) == true )
	{
		// ファイルのオープン
		FILE *fp = fopen( "GoalPositionInfo.txt", "wt" );

		for (int nCnt = 0; nCnt < GOAL_MAX; nCnt++)
		{
			if ( g_aGoal[ nCnt ].bUse == true )
			{
				char workChar[ 256 ] = "POS = ";
				char workFloatTrans[ 256 ];

				// 位置をワークに突っ込む
				sprintf( workFloatTrans, "%.2f", g_aGoal[ nCnt ].pos.x );
				strcat( workChar, workFloatTrans );		// 連結
				strcat( workChar, " " );				// 連結
				sprintf( workFloatTrans, "%.2f", g_aGoal[ nCnt ].pos.y );
				strcat( workChar, workFloatTrans );		// 連結
				strcat( workChar, " " );				// 連結
				sprintf( workFloatTrans, "%.2f", g_aGoal[ nCnt ].pos.z );
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
	PrintDebugProc("歯車の個数 : %d\n", g_nCreateGoalNum);
	PrintDebugProc("↑、↓ *** 歯車番号の選択");
	PrintDebugProc("選択している歯車番号 : %d\n", g_nMoveGoal);

	int nNum = g_nMoveGoal - 10;
	if ( nNum < 0 )
	{
		nNum = 0;
	}
	for ( int i = nNum; i < nNum+ 11; i++ )
	{
		if ( g_aGoal[ i ].bUse == true )
		{
			if ( i == g_nMoveGoal )
			{
				PrintDebugProc("->* [ %d ] * %f %f %f\n", i, g_aGoal[ i ].pos.x, g_aGoal[ i ].pos.y, g_aGoal[ i ].pos.z);
			}
			else
			{
				PrintDebugProc("* [ %d ] * %f %f %f\n", i, g_aGoal[ i ].pos.x, g_aGoal[ i ].pos.y, g_aGoal[ i ].pos.z);
			}
		}
	}
}

//-----------------------------------------------
// 当たり判定の取得
//-----------------------------------------------
GOAL *GetGoal( void )
{
	return &g_aGoal[ 0 ];
}

//-----------------------------------------------
// 回転させる判定
//-----------------------------------------------
void SetGoalRotation( int num, bool use )
{
	g_aGoal[ num ].bRotation = use;
}

//-----------------------------------------------
// 回転させる判定の取得
//-----------------------------------------------
bool GetGoalRotation( int num )
{
	return g_aGoal[ num ].bRotation;
}

//-----------------------------------------------
// 角度直し
//-----------------------------------------------
void ZeroGoalRotation( void )
{
	for (int nCnt = 0; nCnt < GOAL_MAX; nCnt++)
	{
		if ( g_aGoal[ nCnt ].bUse == true )
		{
			g_aGoal[ nCnt ].rot = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
		}
	}
}
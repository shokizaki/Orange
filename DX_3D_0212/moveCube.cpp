//-----------------------------------------------
//  内容：四角描画処理
//  File : cube.cpp
//  制作者：木崎　翔
//-----------------------------------------------

//------ インクルードファイル ------
#include "moveCube.h"
#include "camera.h"
#include "gear.h"
#include "player.h"
#include "sheet.h"

//------ マクロ定義 ------
#define MOVECUBE_WIDTH ( 15.0f )
#define MOVECUBE_HEIGHT ( 15.0f )

//------ グローバル変数 ------
MOVECUBE g_aMoveCube[ MOVECUBE_MAX ];

bool g_bMoveMoveCube = false;	// 選択しているかどうか
bool g_bMoveCubeSelect = false;	// 選択しているかどうか（生成する時の判定）
int g_nMoveMoveCube = 0;		// 選択している赤キューブのインデックス
int g_nCreateMoveCubeNum = 0;	// 生成された赤キューブの数

//------ 内部関数宣言 ------

//-----------------------------------------------
// 四角描画初期化
//-----------------------------------------------
void InitMoveCube( void )
{
	// ローカル変数
	//---------------------------------
	LPDIRECT3DDEVICE9 pDevice = NULL;		// デバイスオブジェクト
	
	// デバイス取得
	//------------------------------------
	pDevice = GetDevice();

	for ( int i = 0; i < MOVECUBE_MAX; i++ )
	{
		g_aMoveCube[ i ].bUse = false;
	}

	// グローバル変数の初期化
	g_bMoveMoveCube = false;
	g_nMoveMoveCube = 0;
	//g_bEdit = false;
	g_nCreateMoveCubeNum = 0;
	g_bMoveCubeSelect = false;

	// 位置読み込み
	// ファイルの読み込み開始
	FILE *fp = fopen( "RedCubePos.txt", "rt" );
    if( fp == NULL )
    {
        return ;
    }
	char strWork[ 256 ];
	D3DXVECTOR3 fWork;
	while ( strcmp( strWork, "END_SCRIPT" ) != 0 )
	{
		// 一行読み込む
		fscanf( fp, "%s = %f %f %f" , strWork, &fWork.x, &fWork.y, &fWork.z );

		if ( strcmp( strWork, "POS" ) == 0 )
		{
			// 白キューブ生成
			SetMoveCube( fWork );
		}
	}
    fclose(fp);
}

//-----------------------------------------------
// 四角描画終了処理
//-----------------------------------------------
void UninitMoveCube( void )
{
	for (int nCnt = 0; nCnt < MOVECUBE_MAX; nCnt++)
	{
		if ( g_aMoveCube[ nCnt ].bUse == true )
		{
			// メッシュ情報の解放と初期化
			//------------------------------------
			if (g_aMoveCube[ nCnt ].pMeshModel != NULL)
			{ 
				g_aMoveCube[ nCnt ].pMeshModel ->Release();
				g_aMoveCube[ nCnt ].pMeshModel = NULL;
			}
	
			// マテリアル情報の解放と初期化
			//------------------------------------
			if (g_aMoveCube[ nCnt ].pBuffMatModel != NULL)
			{ 
				g_aMoveCube[ nCnt ].pBuffMatModel ->Release();
				g_aMoveCube[ nCnt ].pBuffMatModel = NULL;
			}
		}
	}
}

//-----------------------------------------------
// 四角描画更新処理
//-----------------------------------------------
void UpdateMoveCube( void )
{
	// ローカル変数
	GEAR *pGear = GetGear();

	{
		// 赤キューブ位置の保存
		if ( GetKeyboardTrigger( DIK_F5 ) == true )
		{
			// ファイルのオープン
			FILE *fp = fopen( "MoveCubePositionInfo.txt", "wt" );

			for (int nCnt = 0; nCnt < MOVECUBE_MAX; nCnt++)
			{
				if ( g_aMoveCube[ nCnt ].bUse == true )
				{
					char workChar[ 256 ] = "POS = ";
					char workFloatTrans[ 256 ];

					// 位置をワークに突っ込む
					sprintf( workFloatTrans, "%.2f", g_aMoveCube[ nCnt ].pos.x );
					strcat( workChar, workFloatTrans );		// 連結
					strcat( workChar, " " );				// 連結
					sprintf( workFloatTrans, "%.2f", g_aMoveCube[ nCnt ].pos.y );
					strcat( workChar, workFloatTrans );		// 連結
					strcat( workChar, " " );				// 連結
					sprintf( workFloatTrans, "%.2f", g_aMoveCube[ nCnt ].pos.z );
					strcat( workChar, workFloatTrans );		// 連結
					strcat( workChar, "\n" );				// 連結

					// 書き出し
					fputs( workChar, fp );
				}
			}

			// ファイルのクローズ
			fclose( fp );
		}

		for (int nCnt = 0; nCnt < MOVECUBE_MAX; nCnt++)
		{
			if ( g_aMoveCube[ nCnt ].bUse == true )
			{
				if ( g_aMoveCube[ nCnt ].nGearIndex != GEAR_MAX )
				{
					for ( int i = 0; i < GEAR_MAX; i++ )
					{
						if ( ( pGear + g_aMoveCube[ nCnt ].nGearIndex ) ->bRotation == true )
						{

						}
					}
				}
			}
		}		
	}
}

//-----------------------------------------------
// 四角描画描画処理
//-----------------------------------------------
void DrawMoveCube( void )
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

	if ( GetSheet() == false )
	{
		// それぞれのパーツの行列計算と描画開始
		for (int nCnt = 0; nCnt < MOVECUBE_MAX; nCnt++)
		{
			if ( g_aMoveCube[ nCnt ].bUse == true )
			{
				D3DXMatrixIdentity( &g_aMoveCube[ nCnt ].mtxWorld );	// フォーマットの初期化
				D3DXMatrixIdentity( &mtxScl );						// 行列の初期化
				D3DXMatrixIdentity( &mtxRot );						// 行列の初期化
				D3DXMatrixIdentity( &mtxTranslate );				// 行列の初期化

				// スケールを反映
				D3DXMatrixScaling(&mtxScl, g_aMoveCube[ nCnt ].scl.x, g_aMoveCube[ nCnt ].scl.y, g_aMoveCube[ nCnt ].scl.z);
				D3DXMatrixMultiply(&g_aMoveCube[ nCnt ].mtxWorld, &g_aMoveCube[ nCnt ].mtxWorld, &mtxScl);

				// 向きを反映
				D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aMoveCube[ nCnt ].rot.y, g_aMoveCube[ nCnt ].rot.x, g_aMoveCube[ nCnt ].rot.z);
				D3DXMatrixMultiply(&g_aMoveCube[ nCnt ].mtxWorld, &g_aMoveCube[ nCnt ].mtxWorld, &mtxRot);

				// 位置を反映
				D3DXMatrixTranslation(&mtxTranslate, g_aMoveCube[ nCnt ].pos.x, g_aMoveCube[ nCnt ].pos.y, g_aMoveCube[ nCnt ].pos.z);
				D3DXMatrixMultiply(&g_aMoveCube[ nCnt ].mtxWorld, &g_aMoveCube[ nCnt ].mtxWorld, &mtxTranslate);

				// 設定
				pDevice ->SetTransform(D3DTS_WORLD, &g_aMoveCube[ nCnt ].mtxWorld);

				// 現在のマテリアル情報を保存
				pDevice ->GetMaterial( &matDef );

				// バッファへのポインタを取得
				pMat = (D3DXMATERIAL*)g_aMoveCube[ nCnt ].pBuffMatModel ->GetBufferPointer();

				/*strSrc = strrchr(pMat ->pTextureFilename, '\\') + 1;

				strcat(strDest, strSrc);

				D3DXCreateTextureFromFile(pDevice, strDest, &g_pTextureModel[1]);*/
			
				// マテリアルの数だけループ
				for (int nCntMat = 0; nCntMat < (int)g_aMoveCube[ nCnt ].numMatModel; nCntMat++)
				{
					pDevice ->SetMaterial( &pMat[ nCntMat ].MatD3D );			// マテリアルの設定
					pDevice ->SetTexture( 0, g_aMoveCube[ nCnt ].pTexture );							// テクスチャのセット
					g_aMoveCube[ nCnt ].pMeshModel ->DrawSubset( nCntMat );		// 描画
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
int SetMoveCube( D3DXVECTOR3 pos )
{
	// ローカル変数
	//---------------------------------
	LPDIRECT3DDEVICE9 pDevice = NULL;		// デバイスオブジェクト
	
	// デバイス取得
	//------------------------------------
	pDevice = GetDevice();

	for ( int i = 0; i < MOVECUBE_MAX; i++ )
	{
		if ( g_aMoveCube[ i ].bUse == false )
		{
			// 位置の設定
			g_aMoveCube[ i ].pos = pos;
			g_aMoveCube[ i ].initPos = pos;
			g_aMoveCube[ i ].targetPos = pos;
			g_aMoveCube[ i ].rot = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
			g_aMoveCube[ i ].scl = D3DXVECTOR3( 1.0f, 1.0f, 1.0f );

			g_aMoveCube[ i ].move = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );

			g_aMoveCube[ i ].nGearIndex = GEAR_MAX;

			// 当たり判定用
			g_aMoveCube[ i ].rect.pos = pos;
			g_aMoveCube[ i ].rect.harfSize = D3DXVECTOR3( MOVECUBE_WIDTH, MOVECUBE_HEIGHT, MOVECUBE_HEIGHT );

			// モデル情報をコピー
			//g_aMoveCube[ i ].numMatModel = numMatModelInit;
			//g_aMoveCube[ i ].pBuffMatModel = pBuffMatModelInit;
			//g_aMoveCube[ i ].pMeshModel = pMeshModelInit;

			{
				// xファイルの読み込み
				//------------------------------------
				D3DXLoadMeshFromX("data/MODEL/Block_Red.x",		// 読み込むファイル名
								  D3DXMESH_SYSTEMMEM,							// 
								  pDevice,										// 
								  NULL,											// 
								  &g_aMoveCube[ i ].pBuffMatModel,			// 
								  NULL,											// 
								  &g_aMoveCube[ i ].numMatModel,			// 
								  &g_aMoveCube[ i ].pMeshModel );
			}

			// テクスチャの読み込み
			//------------------------------------
			D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Block.jpg", &g_aMoveCube[ i ].pTexture);

			g_aMoveCube[ i ].bUse = true;
			
			// 生成した数を増やす
			g_nCreateMoveCubeNum++;

			// インデックスを返す
			return i;
		}
	}

	return MOVECUBE_MAX;
}

//-----------------------------------------------
// エディット更新
//-----------------------------------------------
void EditMoveCube( void )
{
	if ( GetKeyboardTrigger( DIK_B ) == true && g_bMoveCubeSelect == false )
	{
		// 角度を初期化
		g_aMoveCube[ g_nMoveMoveCube ].rot.y = 0.0f;

		// 赤キューブの生成
		g_nMoveMoveCube = SetMoveCube( D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );
	}

	//if ( g_bMoveMoveCube == true )
	{
		// 移動処理
		if ( GetKeyboardTrigger( DIK_A ) == true )
		{
			g_aMoveCube[ g_nMoveMoveCube ].pos.x += -40.0f;
		}
		if ( GetKeyboardTrigger( DIK_D ) == true )
		{
			g_aMoveCube[ g_nMoveMoveCube ].pos.x += 40.0f;
		}
		if ( GetKeyboardTrigger( DIK_W ) == true )
		{
			g_aMoveCube[ g_nMoveMoveCube ].pos.y += 40.0f;
		}
		if ( GetKeyboardTrigger( DIK_S ) == true )
		{
			g_aMoveCube[ g_nMoveMoveCube ].pos.y += -40.0f;
		}

		g_aMoveCube[ g_nMoveMoveCube ].rect.pos = g_aMoveCube[ g_nMoveMoveCube ].pos;
		g_aMoveCube[ g_nMoveMoveCube ].rot.y += 0.025f;
	}

	// 選択している赤キューブの変更
	if ( GetKeyboardTrigger( DIK_UP ) == true )
	{
		// 角度を初期化
		g_aMoveCube[ g_nMoveMoveCube ].rot.y = 0.0f;
			
		g_nMoveMoveCube--;
		if ( g_nMoveMoveCube < 0 )
		{
			g_nMoveMoveCube = g_nCreateMoveCubeNum - 1;
		}
	}
	if ( GetKeyboardTrigger( DIK_DOWN ) == true )
	{
		// 角度を初期化
		g_aMoveCube[ g_nMoveMoveCube ].rot.y = 0.0f;

		g_nMoveMoveCube++;
		if ( g_nMoveMoveCube > g_nCreateMoveCubeNum - 1 )
		{
			g_nMoveMoveCube = 0;
		}
	}

	// 赤キューブの削除
	if ( GetKeyboardTrigger( DIK_SPACE ) == true )
	{
		g_nCreateMoveCubeNum--;
		if ( g_nCreateMoveCubeNum < 0 )
		{
			g_nCreateMoveCubeNum = 0;
		}

		for ( int i = g_nMoveMoveCube; i < g_nCreateMoveCubeNum; i++ )
		{
			g_aMoveCube[ i ].pos = g_aMoveCube[ i + 1 ].pos;
			g_aMoveCube[ i ].rect.pos = g_aMoveCube[ i + 1 ].pos;
		}

		g_aMoveCube[ g_nCreateMoveCubeNum ].bUse = false;
			
	}

	// 赤キューブ位置の保存
	if ( GetKeyboardTrigger( DIK_F5 ) == true )
	{
		// ファイルのオープン
		FILE *fp = fopen( "MoveCubePositionInfo.txt", "wt" );

		for (int nCnt = 0; nCnt < MOVECUBE_MAX; nCnt++)
		{
			if ( g_aMoveCube[ nCnt ].bUse == true )
			{
				char workChar[ 256 ] = "POS = ";
				char workFloatTrans[ 256 ];

				// 位置をワークに突っ込む
				sprintf( workFloatTrans, "%.2f", g_aMoveCube[ nCnt ].pos.x );
				strcat( workChar, workFloatTrans );		// 連結
				strcat( workChar, " " );				// 連結
				sprintf( workFloatTrans, "%.2f", g_aMoveCube[ nCnt ].pos.y );
				strcat( workChar, workFloatTrans );		// 連結
				strcat( workChar, " " );				// 連結
				sprintf( workFloatTrans, "%.2f", g_aMoveCube[ nCnt ].pos.z );
				strcat( workChar, workFloatTrans );		// 連結
				strcat( workChar, "\n" );				// 連結

				// 書き出し
				fputs( workChar, fp );
			}
		}

		// ファイルのクローズ
		fclose( fp );
	}

	PrintDebugProc("[ Bキーで赤キューブ生成 ]\n");
	PrintDebugProc("[ SPACEキーで選択している赤キューブを削除 ]\n");
	PrintDebugProc("赤キューブの個数 : %d\n", g_nCreateMoveCubeNum);
	PrintDebugProc("↑、↓ *** 赤キューブ番号の選択");
	PrintDebugProc("選択している赤キューブ番号 : %d\n", g_nMoveMoveCube);

	int nNum = g_nMoveMoveCube - 10;
	if ( nNum < 0 )
	{
		nNum = 0;
	}
	for ( int i = nNum; i < nNum + 11; i++ )
	{
		if ( g_aMoveCube[ i ].bUse == true )
		{
			if ( i == g_nMoveMoveCube )
			{
				PrintDebugProc("->* [ %d ] * %f %f %f\n", i, g_aMoveCube[ i ].pos.x, g_aMoveCube[ i ].pos.y, g_aMoveCube[ i ].pos.z);
			}
			else
			{
				PrintDebugProc("* [ %d ] * %f %f %f\n", i, g_aMoveCube[ i ].pos.x, g_aMoveCube[ i ].pos.y, g_aMoveCube[ i ].pos.z);
			}
		}
	}
}

//-----------------------------------------------
// 当たり判定の取得
//-----------------------------------------------
MOVECUBE *GetMoveCube( void )
{
	return &g_aMoveCube[ 0 ];
}

//-----------------------------------------------
// 角度直し
//-----------------------------------------------
void ZeroMoveCubeRotation( void )
{
	for (int nCnt = 0; nCnt < MOVECUBE_MAX; nCnt++)
	{
		if ( g_aMoveCube[ nCnt ].bUse == true )
		{
			g_aMoveCube[ nCnt ].rot = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
		}
	}
}
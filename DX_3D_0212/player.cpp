//-----------------------------------------------
//  内容：プレイヤー処理
//  File : player.cpp
//  制作者：木崎　翔
//-----------------------------------------------

//------ インクルードファイル ------
#include "player.h"
#include "camera.h"
#include "collision.h"
#include "cube.h"
#include "Edit.h"
#include "gear.h"
#include "moveCube.h"

//------ マクロ定義 ------
#define PLAYER_WIDTH ( 20.0f )
#define PLAYER_HEIGHT ( 20.0f )
#define PLAYER_POS_X ( 0.0f )
#define PLAYER_POS_Y ( 30.0f )
#define PLAYER_POS_Z ( 0.0f )

#define GRAVITY_VAL ( 0.5f )

//------ グローバル変数 ------
D3DXMATRIX mtxWorldPlayer;				// ワールドマトリックス
LPD3DXMESH pMeshModelPlayer;			// メッシュ情報へのポインタ
LPD3DXBUFFER pBuffMatModelPlayer;		// マテリアル情報へのポインタ
DWORD numMatModelPlayer;				// マテリアルの数

D3DXVECTOR3 posPlayer;	
D3DXVECTOR3 posPlayerOld;	
D3DXVECTOR3 rotPlayer;	
D3DXVECTOR3 sclPlayer;	
COL_RECT rectPlayer;
float fJumpVal = 0.0f;
int g_nCubeIndex = 0;
int g_nRedCubeIndex = 0;
bool g_bJump = false;

bool g_bDirect = true;

//------ 内部関数宣言 ------

//-----------------------------------------------
// プレイヤー初期化
//-----------------------------------------------
void InitPlayer( void )
{
	// ローカル変数
	//---------------------------------
	LPDIRECT3DDEVICE9 pDevice = NULL;		// デバイスオブジェクト
	
	// デバイス取得
	//------------------------------------
	pDevice = GetDevice();

	{
		// xファイルの読み込み
		//------------------------------------
		D3DXLoadMeshFromX("data/MODEL/sphere.x",		// 読み込むファイル名
						  D3DXMESH_SYSTEMMEM,							// 
						  pDevice,										// 
						  NULL,											// 
						  &pBuffMatModelPlayer,			// 
						  NULL,											// 
						  &numMatModelPlayer,			// 
						  &pMeshModelPlayer );
	}

	// グローバル変数の初期化
	sclPlayer = D3DXVECTOR3( 1.0f, 1.0f, 1.0f ); 
	rotPlayer = D3DXVECTOR3( 0.0f, 0.0f, 0.0f ); 
	posPlayer = D3DXVECTOR3( PLAYER_POS_X, PLAYER_POS_Y, PLAYER_POS_Z ); 
	posPlayerOld = posPlayer;

	rectPlayer.pos = posPlayer;
	rectPlayer.harfSize = D3DXVECTOR3( PLAYER_WIDTH, PLAYER_HEIGHT, PLAYER_HEIGHT );

	fJumpVal = 0.0f;
	g_nCubeIndex = CUBE_MAX;
	g_nRedCubeIndex = 0;
	g_bDirect = true;
	g_bJump = false;
}

//-----------------------------------------------
// プレイヤー終了処理
//-----------------------------------------------
void UninitPlayer( void )
{
	// メッシュ情報の解放と初期化
	//------------------------------------
	if (pMeshModelPlayer != NULL)
	{ 
		pMeshModelPlayer ->Release();
		pMeshModelPlayer = NULL;
	}
	
	// マテリアル情報の解放と初期化
	//------------------------------------
	if (pBuffMatModelPlayer != NULL)
	{ 
		pBuffMatModelPlayer ->Release();
		pBuffMatModelPlayer = NULL;
	}
}

//-----------------------------------------------
// プレイヤー更新処理
//-----------------------------------------------
void UpdatePlayer( void )
{
	// ローカル変数
	CUBE *pCube = GetCube();
	GEAR *pGear = GetGear();
	MOVECUBE *pMoveCube = GetMoveCube();

	if ( GetEdit() == false )
	{
		// 前回位置を保存
		posPlayerOld = posPlayer;
		g_nCubeIndex = CUBE_MAX;

		// 重力をかける
		fJumpVal += GRAVITY_VAL;
		posPlayer.y -= fJumpVal;
		rectPlayer.pos.y = posPlayer.y;

		// キューブとの当たり判定
		for ( int i = 0; i < CUBE_MAX; i++ )
		{
			if ( ( pCube + i ) ->bUse == true )
			{
				if ( ColRectXY( &rectPlayer, &( pCube + i ) ->rect ) == true )
				{
					posPlayer.y = posPlayerOld.y;
					rectPlayer.pos.y = posPlayer.y;
					fJumpVal = 0.0f;
					g_bJump = false;

					g_nCubeIndex = i;
				}
			}
		}

		// 赤キューブとの当たり判定
		for ( int i = 0; i < MOVECUBE_MAX; i++ )
		{
			if ( ( pMoveCube + i ) ->bUse == true )
			{
				if ( ColRectXY( &rectPlayer, &( pMoveCube + i ) ->rect ) == true )
				{
					posPlayer.y = posPlayerOld.y;
					rectPlayer.pos.y = posPlayer.y;
					fJumpVal = 0.0f;
					g_bJump = false;

					g_nCubeIndex = i;
				}
			}
		}

		// 地面判定
		if ( posPlayer.y + PLAYER_HEIGHT <= PLAYER_HEIGHT )
		{
			posPlayer.y = posPlayerOld.y;
			rectPlayer.pos.y = posPlayer.y;
			fJumpVal = 0.0f;			
			g_bJump = false;
		}

		// 移動処理
		if ( GetKeyboardPress( DIK_A ) == true )
		{
			posPlayer.x -= 3.0f;
			rectPlayer.pos.x = posPlayer.x;
			g_bDirect = false;
		}
		if ( GetKeyboardPress( DIK_D ) == true )
		{
			posPlayer.x += 3.0f;
			rectPlayer.pos.x = posPlayer.x;
			g_bDirect = true;
		}

		// キューブとの当たり判定
		for ( int i = 0; i < CUBE_MAX; i++ )
		{
			if ( ( pCube + i ) ->bUse == true )
			{
				if ( ColRectXY( &rectPlayer, &( pCube + i ) ->rect ) == true )
				{
					posPlayer.x = posPlayerOld.x;
					rectPlayer.pos.x = posPlayer.x;
				}
			}
		}

		// 赤キューブとの当たり判定
		for ( int i = 0; i < MOVECUBE_MAX; i++ )
		{
			if ( ( pMoveCube + i ) ->bUse == true )
			{
				if ( ColRectXY( &rectPlayer, &( pMoveCube + i ) ->rect ) == true )
				{
					posPlayer.x = posPlayerOld.x;
					rectPlayer.pos.x = posPlayer.x;
				}
			}
		}

		// 赤キューブを取る処理
		if ( GetKeyboardTrigger( DIK_RETURN ) == true )
		{
			if ( g_nRedCubeIndex == 0 )
			{
				// 赤キューブとの当たり判定
				for ( int i = 0; i < MOVECUBE_MAX; i++ )
				{
					if ( ( pMoveCube + i ) ->bUse == true )
					{
						if ( g_bDirect == true )
						{
							// 作業用変数
							COL_RECT work;

							// 現在の状態を保存
							work = rectPlayer;

							work.harfSize.x += 10.0f;
							
							if ( ColRectXY( &work, &( pMoveCube + i ) ->rect ) == true )
							{
								g_nRedCubeIndex = 1;
								( pMoveCube + i ) ->bUse = false;
							}

							break;	// ループを抜ける
						}
						else
						{
							// 作業用変数
							COL_RECT work;

							// 現在の状態を保存
							work = rectPlayer;

							work.pos.x -= 10.0f;
							
							if ( ColRectXY( &work, &( pMoveCube + i ) ->rect ) == true )
							{
								g_nRedCubeIndex = 1;
								( pMoveCube + i ) ->bUse = false;
							}

							break;	// ループを抜ける
						}
					}
				}
			}
		}

		// 赤キューブの生成
		// 横に生成
		if ( GetKeyboardTrigger( DIK_P ) == true && GetKeyboardPress( DIK_S ) == false )
		{
			// キューブを所持していたら
			if ( g_nRedCubeIndex == 1 )
			{
				// 右向きの場合
				if ( g_bDirect == true )
				{
					if ( ( (int)( posPlayer.x + 40.0f ) % 40 ) != 0 )
					{
						// 作業用変数
						int nWorkX = ( (int)( posPlayer.x + 40.0f ) / 40 );
						
						// 生成する場所を計算
						if ( ( (int)( posPlayer.x + 40.0f ) % 40 ) < 10 )
						{
							nWorkX = nWorkX - 1;
						}
						int nWorkY = ( (int)( posPlayer.y + 30.0f ) / 40 );

						// 赤キューブの生成
						SetMoveCube( D3DXVECTOR3( 40.0f + 40.0f * nWorkX, 40.0f * nWorkY, posPlayer.z ) );
					}
					else
					{
						// 赤キューブの生成
						SetMoveCube( D3DXVECTOR3( posPlayer.x + 40.0f, posPlayer.y, posPlayer.z ) );
					}
				}

				// 左向きの場合
				else
				{
					if ( ( (int)( posPlayer.x - 40.0f ) % 40 ) != 0 )
					{
						// 生成する場所を計算
						int nWorkX = ( (int)( posPlayer.x - 40.0f ) / 40 );
						if ( ( (int)( posPlayer.x - 40.0f ) % 40 ) > -10 )
						{
							nWorkX = nWorkX + 1;
						}
						int nWorkY = ( (int)( posPlayer.y + 30.0f ) / 40 );

						// 赤キューブの生成
						SetMoveCube( D3DXVECTOR3( -40.0f + 40.0f * nWorkX, 40.0f * nWorkY, posPlayer.z ) );
					}
					else
					{
						// 赤キューブの生成
						SetMoveCube( D3DXVECTOR3( posPlayer.x - 40.0f, posPlayer.y, posPlayer.z ) );
					}
				}

				// 所持フラグを折る
				g_nRedCubeIndex = 0;
			}
		}

		// 斜め下に生成
		else if ( GetKeyboardTrigger( DIK_P ) == true && GetKeyboardPress( DIK_S ) == true )
		{
			// 所持していたら
			if ( g_nRedCubeIndex == 1 )
			{
				// 右向きの場合
				if ( g_bDirect == true )
				{
					{
						// 生成する場所を計算
						int nWorkX = ( (int)( posPlayer.x + 40.0f ) / 40 );
						if ( ( (int)( posPlayer.x + 40.0f ) % 40 ) < 10 )
						{
							nWorkX = nWorkX - 1;
						}
						int nWorkY = ( (int)( posPlayer.y + 30.0f ) / 40 );

						// 赤キューブの生成
						SetMoveCube( D3DXVECTOR3( 40.0f + 40.0f * nWorkX, 40.0f * ( nWorkY - 1 ), posPlayer.z ) );
					}
				}

				// 左向きの場合
				else
				{
					{
						// 生成する場所を計算
						int nWorkX = ( (int)( posPlayer.x - 40.0f ) / 40 );
						if ( ( (int)( posPlayer.x - 40.0f ) % 40 ) > -10 )
						{
							nWorkX = nWorkX + 1;
						}
						int nWorkY = ( (int)( posPlayer.y + 30.0f ) / 40 );

						// 赤キューブの生成
						SetMoveCube( D3DXVECTOR3( -40.0f + 40.0f * nWorkX, 40.0f * ( nWorkY - 1 ), posPlayer.z ) );
					}
				}

				// 所持フラグを折る
				g_nRedCubeIndex = 0;
			}
		}

		// ジャンプ処理
		if ( GetKeyboardTrigger( DIK_SPACE ) == true && g_bJump == false )
		{
			fJumpVal -= 8.0f;
			g_bJump = true;
		}

		// 移動床を判定するやつ
		if ( g_nCubeIndex < CUBE_MAX )
		{
			posPlayer += ( pCube + g_nCubeIndex ) ->move;
			rectPlayer.pos = posPlayer;
		}

		// 歯車との当たり判定
		for ( int i = 0; i < GEAR_MAX; i++ )
		{
			if ( ( pGear + i ) ->bUse == true )
			{
				if ( ColRectXY( &rectPlayer, &( pGear + i ) ->rect ) == true )
				{
					if ( GetKeyboardTrigger( DIK_RETURN ) == true )
					{
						if ( ( pGear + i ) ->bRotation == false )
						{
							SetGearRotation( i, true );
						}
						else
						{
							SetGearRotation( i, false );
						}
					}
				}
			}
		}

		PrintDebugProc("[ プレイヤー位置 ] %f %f %f\n", posPlayer.x, posPlayer.y, posPlayer.z);
	}
}

//-----------------------------------------------
// プレイヤー描画処理
//-----------------------------------------------
void DrawPlayer( void )
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

	D3DXMatrixIdentity( &mtxWorldPlayer );	// フォーマットの初期化
	D3DXMatrixIdentity( &mtxScl );						// 行列の初期化
	D3DXMatrixIdentity( &mtxRot );						// 行列の初期化
	D3DXMatrixIdentity( &mtxTranslate );				// 行列の初期化

	// スケールを反映
	D3DXMatrixScaling(&mtxScl, sclPlayer.x, sclPlayer.y, sclPlayer.z);
	D3DXMatrixMultiply(&mtxWorldPlayer, &mtxWorldPlayer, &mtxScl);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rotPlayer.y, rotPlayer.x, rotPlayer.z);
	D3DXMatrixMultiply(&mtxWorldPlayer, &mtxWorldPlayer, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTranslate, posPlayer.x, posPlayer.y, posPlayer.z);
	D3DXMatrixMultiply(&mtxWorldPlayer, &mtxWorldPlayer, &mtxTranslate);

	// 設定
	pDevice ->SetTransform(D3DTS_WORLD, &mtxWorldPlayer);

	// 現在のマテリアル情報を保存
	pDevice ->GetMaterial( &matDef );

	// バッファへのポインタを取得
	pMat = (D3DXMATERIAL*)pBuffMatModelPlayer ->GetBufferPointer();

	/*strSrc = strrchr(pMat ->pTextureFilename, '\\') + 1;

	strcat(strDest, strSrc);

	D3DXCreateTextureFromFile(pDevice, strDest, &g_pTextureModel[1]);*/
			
	// マテリアルの数だけループ
	for (int nCntMat = 0; nCntMat < (int)numMatModelPlayer; nCntMat++)
	{
		pDevice ->SetMaterial( &pMat[ nCntMat ].MatD3D );			// マテリアルの設定
		pDevice ->SetTexture( 0, NULL );							// テクスチャのセット
		pMeshModelPlayer ->DrawSubset( nCntMat );		// 描画
	}
			
	// マテリアルを元に戻す
	//------------------------------------
	pDevice ->SetMaterial( &matDef );
}
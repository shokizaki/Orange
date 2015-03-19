//===============================================
//
//  内容   ： オブジェクト処理
//  File   ： object.cpp
//  制作者 ： 木崎　翔
//
//===============================================

// インクルードファイル
#include "object.h"
#include "model.h"
#include "enemy.h"
#include "billboard.h"
#include "score.h"
#include "stringDraw.h"
#include "camera.h"
#include "texture2D.h"
#include "miniMap.h"

// マクロ定義
#define OBJECT_A_FILEPASS	"data/MODEL/box000.x"
#define OBJECT_A_TEXPASS	"data/TEXTURE/exclamation000.jpg"
#define OBJECT_A_WIDTH		( 25.0f )
#define OBJECT_A_HEIGHT_Z	( 25.0f )
#define OBJECT_A_HEIGHT		( 25.0f )

#define OBJECT_B_FILEPASS	"data/MODEL/box000.x"
#define OBJECT_B_TEXPASS	"data/TEXTURE/question000.jpg"
#define OBJECT_B_WIDTH		( 25.0f )
#define OBJECT_B_HEIGHT_Z	( 25.0f )

#define OBJECT_C_FILEPASS	"data/MODEL/wood001.x"
#define OBJECT_C_WIDTH		( 15.0f )
#define OBJECT_C_HEIGHT_Z	( 15.0f )

#define OBJECT_D_FILEPASS	"data/MODEL/wood002.x"
#define OBJECT_D_WIDTH		( 50.0f )
#define OBJECT_D_HEIGHT_Z	( 50.0f )

#define OBJECT_E_FILEPASS	"data/MODEL/wood003.x"
#define OBJECT_E_WIDTH		( 40.0f )
#define OBJECT_E_HEIGHT_Z	( 40.0f )

#define DIAMETER			( 100 )

// グローバル変数
OBJECT g_object[ MAX_OBJECT ];
COL_RECT rectObject[ MAX_OBJECT ];
bool g_bDirection = false;
int g_nIndexObject = 0;

//===============================================
//
// オブジェクトに関する初期化
//
//===============================================
void InitObject(void)
{
	// テクスチャポインタの初期化
	for ( int i = 0; i < MAX_OBJECT; i++ )
	{
		g_object[ i ].pTextureObject = NULL;
		g_object[ i ].bUse = false;
	}

	// オブジェクトのセット

	// ボックス
	SetObject( D3DXVECTOR3( 0.0f, 0.0f, -100.0f ), OBJECTTYPE_A );
	
	// 奥の列
	SetObject( D3DXVECTOR3( -1000.0f, 0.0f, 1000.0f ), OBJECTTYPE_E );
	SetObject( D3DXVECTOR3( -800.0f, 0.0f, 800.0f ),  OBJECTTYPE_E );
	SetObject( D3DXVECTOR3( -600.0f, 0.0f, 1000.0f ), OBJECTTYPE_E );
	SetObject( D3DXVECTOR3( -400.0f, 0.0f, 800.0f ),  OBJECTTYPE_E );
	SetObject( D3DXVECTOR3( -200.0f, 0.0f, 1000.0f ), OBJECTTYPE_E );
	SetObject( D3DXVECTOR3( 0.0f,    0.0f, 800.0f ),  OBJECTTYPE_E );
	SetObject( D3DXVECTOR3( 1000.0f, 0.0f, 1000.0f ), OBJECTTYPE_E );
	SetObject( D3DXVECTOR3( 800.0f,  0.0f, 800.0f ),  OBJECTTYPE_E );
	SetObject( D3DXVECTOR3( 600.0f,  0.0f, 1000.0f ), OBJECTTYPE_E );
	SetObject( D3DXVECTOR3( 400.0f,  0.0f, 800.0f ),  OBJECTTYPE_E );
	SetObject( D3DXVECTOR3( 200.0f,  0.0f, 1000.0f ), OBJECTTYPE_E );

	// 手前の列
	SetObject( D3DXVECTOR3( -1000.0f, 0.0f, -1000.0f ), OBJECTTYPE_E );
	SetObject( D3DXVECTOR3( -800.0f, 0.0f, -800.0f ),  OBJECTTYPE_E );
	SetObject( D3DXVECTOR3( -600.0f, 0.0f, -1000.0f ), OBJECTTYPE_E );
	SetObject( D3DXVECTOR3( -400.0f, 0.0f, -800.0f ),  OBJECTTYPE_E );
	SetObject( D3DXVECTOR3( -200.0f, 0.0f, -1000.0f ), OBJECTTYPE_E );
	SetObject( D3DXVECTOR3( 0.0f,    0.0f, -800.0f ),  OBJECTTYPE_E );
	SetObject( D3DXVECTOR3( 1000.0f, 0.0f, -1000.0f ), OBJECTTYPE_E );
	SetObject( D3DXVECTOR3( 800.0f,  0.0f, -800.0f ),  OBJECTTYPE_E );
	SetObject( D3DXVECTOR3( 600.0f,  0.0f, -1000.0f ), OBJECTTYPE_E );
	SetObject( D3DXVECTOR3( 400.0f,  0.0f, -800.0f ),  OBJECTTYPE_E );
	SetObject( D3DXVECTOR3( 200.0f,  0.0f, -1000.0f ), OBJECTTYPE_E );
}

//===============================================
//
// オブジェクトに関する終了処理
//
//===============================================
void UninitObject(void)
{
	for ( int i = 0; i < MAX_OBJECT; i++ )
	{
		if ( g_object[ i ].pMeshObject != NULL )
		{ 
			// メッシュオブジェクトの解放
			//------------------------------------
			g_object[ i ].pMeshObject -> Release();
			g_object[ i ].pMeshObject = NULL;
		}
				// 
		//------------------------------------
		if ( g_object[ i ].pBuffMatObject != NULL )
		{ 
			// オブジェクトバッファの解放
			g_object[ i ].pBuffMatObject -> Release();
			g_object[ i ].pBuffMatObject = NULL;
		}
	}
	
	for (int i = 0; i < MAX_OBJECT; i++)
	{
		if ( g_object[ i ].pTextureObject != NULL )
		{ 
			// 使ったテクスチャの解放と初期化
			//------------------------------------
			g_object[ i ].pTextureObject ->Release();
			g_object[ i ].pTextureObject = NULL;
		}
	}
}

//===============================================
//
// オブジェクトに関する更新処理
//
//===============================================
void UpdateObject(void)
{
	// ローカル変数
	MODEL_INFO *pModelPlayer = GetModelInfo();
	MODEL_INFO *pModelEnemy = GetEnemyInfo();
	COL_RECT *pRectPlayer = GetRectPlayer();
	COL_RECT *pRectEnemy = GetRectEnemy();
	CAMERA *pCamera = GetCamera();

	for ( int i = 0; i < MAX_OBJECT; i++ )
	{
		if ( g_object[ i ].bUse == true )
		{
			if ( pModelPlayer ->direction == DIRECTION_DEPTH || pModelPlayer ->direction == DIRECTION_FRONT )
			{
				// 当たり判定
				if ( ColRect( &rectObject[ i ], pRectPlayer ) == true )
				{
					if ( ( GetKeyboardTrigger( DIK_K ) == true /*|| GetGamePadTrigger( GAMEPAD_A )*/ || GetPadElecomTrigger( PAD_4 ) ) && g_object[ i ].type == OBJECTTYPE_A )
					{
						if ( GetCatchEnemy() > 0 )
						{
							int nCatchEnemy = GetCatchEnemy();
							AddScore( nCatchEnemy * ( DIAMETER + nCatchEnemy ) );
							InitCatchEnemy();
							SetBillboard( D3DXVECTOR3( g_object[ i ].pos.x, g_object[ i ].pos.y + OBJECT_A_HEIGHT, g_object[ i ].pos.z ), TEXTUREANIM_CHARGE, 0.0f, 75.0f, 75.0f );
						
							if ( GetStringMessage() == RELEASETRY )
							{
								SetStringMessage( ATTENTIONSPEED );
							}
						}

						PlaySound( ABSORB_SE );
					}

					if ( ColRectZ_Depth( pRectPlayer, &rectObject[ i ] ) == true )
					{
						pModelPlayer ->posParts[ 0 ].z = pModelPlayer ->posOld.z;
						pModelPlayer ->move.z = -pModelPlayer ->move.z * 0.1f;

						// あたり判定用変数を更新
						pRectPlayer ->pos.z = pModelPlayer ->posParts[ 0 ].z;
					}
					if ( ColRectZ_Front( pRectPlayer, &rectObject[ i ] ) == true )
					{
						pModelPlayer ->posParts[ 0 ].z = pModelPlayer ->posOld.z;
						pModelPlayer ->move.z = -pModelPlayer ->move.z * 0.1f;

						// あたり判定用変数を更新
						pRectPlayer ->pos.z = pModelPlayer ->posParts[ 0 ].z;
					}
				}

				// 当たり判定
				if ( ColRect( &rectObject[ i ], pRectPlayer ) == true )
				{
					if (( GetKeyboardTrigger( DIK_K ) == true /*|| GetGamePadTrigger( GAMEPAD_A )*/ || GetPadElecomTrigger( PAD_4 ) ) && g_object[ i ].type == OBJECTTYPE_A )
					{
						if ( GetCatchEnemy() > 0 )
						{
							int nCatchEnemy = GetCatchEnemy();
							AddScore( nCatchEnemy * ( DIAMETER + nCatchEnemy ) );
							InitCatchEnemy();
							SetBillboard( D3DXVECTOR3( g_object[ i ].pos.x, g_object[ i ].pos.y + OBJECT_A_HEIGHT, g_object[ i ].pos.z ), TEXTUREANIM_CHARGE, 0.0f, 75.0f, 75.0f );
						
							if ( GetStringMessage() == RELEASETRY )
							{
								SetStringMessage( ATTENTIONSPEED );
							}
						}

						PlaySound( ABSORB_SE );
					}
				
					if ( ColRectX_Right( pRectPlayer, &rectObject[ i ] ) == true )
					{
						pModelPlayer ->posParts[ 0 ].x = pModelPlayer ->posOld.x;
						pModelPlayer ->move.x = -pModelPlayer ->move.x * 0.1f;

						// あたり判定用変数を更新
						pRectPlayer ->pos.x = pModelPlayer ->posParts[ 0 ].x;
					}
					if ( ColRectX_Left( pRectPlayer, &rectObject[ i ] ) == true )
					{
						pModelPlayer ->posParts[ 0 ].x = pModelPlayer ->posOld.x;
						pModelPlayer ->move.x = -pModelPlayer ->move.x * 0.1f;

						// あたり判定用変数を更新
						pRectPlayer ->pos.x = pModelPlayer ->posParts[ 0 ].x;
					}
				}

				
			}
			else
			{
				// 当たり判定
				if ( ColRect( &rectObject[ i ], pRectPlayer ) == true )
				{
					if ( ( GetKeyboardTrigger( DIK_K ) == true /*|| GetGamePadTrigger( GAMEPAD_A )*/ || GetPadElecomTrigger( PAD_4 ) ) && g_object[ i ].type == OBJECTTYPE_A )
					{
						if ( GetCatchEnemy() > 0 )
						{
							int nCatchEnemy = GetCatchEnemy();
							AddScore( nCatchEnemy * ( DIAMETER + nCatchEnemy ) );
							InitCatchEnemy();
							SetBillboard( D3DXVECTOR3( g_object[ i ].pos.x, g_object[ i ].pos.y + OBJECT_A_HEIGHT, g_object[ i ].pos.z ), TEXTUREANIM_CHARGE, 0.0f, 75.0f, 75.0f );
						
							if ( GetStringMessage() == RELEASETRY )
							{
								SetStringMessage( ATTENTIONSPEED );
							}
						}

						PlaySound( ABSORB_SE );
					}
				
					if ( ColRectX_Right( pRectPlayer, &rectObject[ i ] ) == true )
					{
						pModelPlayer ->posParts[ 0 ].x = pModelPlayer ->posOld.x;
						pModelPlayer ->move.x = -pModelPlayer ->move.x * 0.1f;

						// あたり判定用変数を更新
						pRectPlayer ->pos.x = pModelPlayer ->posParts[ 0 ].x;
					}
					if ( ColRectX_Left( pRectPlayer, &rectObject[ i ] ) == true )
					{
						pModelPlayer ->posParts[ 0 ].x = pModelPlayer ->posOld.x;
						pModelPlayer ->move.x = -pModelPlayer ->move.x * 0.1f;

						// あたり判定用変数を更新
						pRectPlayer ->pos.x = pModelPlayer ->posParts[ 0 ].x;
					}
				}

				// 当たり判定
				if ( ColRect( &rectObject[ i ], pRectPlayer ) == true )
				{
					if ( ( GetKeyboardTrigger( DIK_K ) == true /*|| GetGamePadTrigger( GAMEPAD_A )*/ || GetPadElecomTrigger( PAD_4 ) ) && g_object[ i ].type == OBJECTTYPE_A )
					{
						if ( GetCatchEnemy() > 0 )
						{
							int nCatchEnemy = GetCatchEnemy();
							AddScore( nCatchEnemy * ( DIAMETER + nCatchEnemy ) );
							InitCatchEnemy();
							SetBillboard( D3DXVECTOR3( g_object[ i ].pos.x, g_object[ i ].pos.y + OBJECT_A_HEIGHT, g_object[ i ].pos.z ), TEXTUREANIM_CHARGE, 0.0f, 75.0f, 75.0f );
						
							if ( GetStringMessage() == RELEASETRY )
							{
								SetStringMessage( ATTENTIONSPEED );
							}
						}

						PlaySound( ABSORB_SE );
					}

					if ( ColRectZ_Depth( pRectPlayer, &rectObject[ i ] ) == true )
					{
						pModelPlayer ->posParts[ 0 ].z = pModelPlayer ->posOld.z;
						pModelPlayer ->move.z = -pModelPlayer ->move.z * 0.1f;

						// あたり判定用変数を更新
						pRectPlayer ->pos.z = pModelPlayer ->posParts[ 0 ].z;
					}
					if ( ColRectZ_Front( pRectPlayer, &rectObject[ i ] ) == true )
					{
						pModelPlayer ->posParts[ 0 ].z = pModelPlayer ->posOld.z;
						pModelPlayer ->move.z = -pModelPlayer ->move.z * 0.1f;

						// あたり判定用変数を更新
						pRectPlayer ->pos.z = pModelPlayer ->posParts[ 0 ].z;
					}
				}
			}

			if ( g_object[ i ].type == OBJECTTYPE_A )
			{
				// ミニマップセット
				SetMiniMap( D3DXVECTOR3( g_object[ i ].pos.x, g_object[ i ].pos.y, g_object[ i ].pos.z ), MINIMAPTYPE_OBJECT, 0.0f );
			}
		}
	}

	// ボックスの位置ヒント
	//if ( g_object[ 0 ].pos.x >= pCamera ->posCameraP.x + SCREEN_WIDTH / 2 )
	//{
	//	if ( g_bDirection == false )
	//	{
	//		g_nIndexObject = SetTexture2D( D3DXVECTOR3( SCREEN_WIDTH - 40.0f, SCREEN_HEIGHT / 2 - 50.0f, 0.0f ), 40.0f, 40.0f, TEXTURE2DTYPE_BOX );
	//		g_bDirection = true;
	//	}
	//}
	//
	//else if ( g_object[ 0 ].pos.x <= pCamera ->posCameraP.x - SCREEN_WIDTH / 2 )
	//{
	//	if ( g_bDirection == false )
	//	{
	//		g_nIndexObject = SetTexture2D( D3DXVECTOR3( 40.0f, SCREEN_HEIGHT / 2 - 50.0f, 0.0f ), 40.0f, 40.0f, TEXTURE2DTYPE_BOXREVERS );
	//		g_bDirection = true;
	//	}	
	//}

	//else
	//{
	//	if ( g_bDirection == true )
	//	{
	//		g_bDirection = false;
	//		SetTexture2DUse( g_nIndexObject );
	//	}
	//}

	PrintDebugProc( "%f %f", g_object[ 0 ].pos.x, g_object[ 0 ].pos.z );
}

//===============================================
//
// オブジェクトに関する描画処理
//
//===============================================
void DrawObject(void)
{
	// ローカル変数
	//------------------------------------
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// デバイスオブジェクト
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;		// 計算用
	D3DXMATRIX pMtxWorld;
	D3DXMATERIAL *pMat;								// 
	D3DMATERIAL9 matDef;							// 元のマテリアル情報

	for (int nCnt = 0; nCnt < MAX_OBJECT; nCnt++)
	{
		if ( g_object[ nCnt ].bUse == true )
		{
			// ワールドマトリックスの設定
			//------------------------------------
			D3DXMatrixIdentity(&g_object[ nCnt ].mtxWorld);			// フォーマットの初期化
			D3DXMatrixIdentity(&mtxScl);
			D3DXMatrixIdentity(&mtxRot);
			D3DXMatrixIdentity(&mtxTranslate);

			//==============================================
			// 
			// 反映する順番は厳守！！
			// 
			//==============================================

			// スケールを反映
			D3DXMatrixScaling(&mtxScl, g_object[ nCnt ].scl.x, g_object[ nCnt ].scl.y, g_object[ nCnt ].scl.z);
			D3DXMatrixMultiply(&g_object[ nCnt ].mtxWorld, &g_object[ nCnt ].mtxWorld, &mtxScl);

			// 向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_object[ nCnt ].rot.y, g_object[ nCnt ].rot.x, g_object[ nCnt ].rot.z);
			D3DXMatrixMultiply(&g_object[ nCnt ].mtxWorld, &g_object[ nCnt ].mtxWorld, &mtxRot);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTranslate, g_object[ nCnt ].pos.x, g_object[ nCnt ].pos.y, g_object[ nCnt ].pos.z);
			D3DXMatrixMultiply(&g_object[ nCnt ].mtxWorld, &g_object[ nCnt ].mtxWorld, &mtxTranslate);
		
			// 設定
			pDevice->SetTransform(D3DTS_WORLD, &g_object[ nCnt ].mtxWorld);

			// 現在のマテリアル情報を保存
			//------------------------------------
			pDevice->GetMaterial(&matDef);

			pMat = (D3DXMATERIAL*)g_object[ nCnt ].pBuffMatObject ->GetBufferPointer();

			/*strSrc = strrchr(pMat ->pTextureFilename, '\\') + 1;

			strcat(strDest, strSrc);

			D3DXCreateTextureFromFile(pDevice, strDest, &g_pTextureObject[1]);*/

			// マテリアルの数だけループ
			for (int nCntMat = 0; nCntMat < (int)g_object[ nCnt ].numMatObject; nCntMat++)
			{
				pDevice ->SetMaterial(&pMat[ nCntMat ].MatD3D);		// マテリアルの設定
				pDevice ->SetTexture(0, g_object[ nCnt ].pTextureObject);						// テクスチャのセット
				g_object[ nCnt ].pMeshObject->DrawSubset( nCntMat );	// 描画
			}

			// マテリアルを元に戻す
			//------------------------------------
			pDevice ->SetMaterial( &matDef );
		}
	}
}

//===============================================
//
// オブジェクトのセット
//
//===============================================
void SetObject( D3DXVECTOR3 pos, OBJECTTYPE type )
{
	// ローカル変数
	LPDIRECT3DDEVICE9 pDevice = GetDevice();						// デバイスオブジェクト

	for ( int i = 0; i < MAX_OBJECT; i++ )
	{
		// 使われていなかったら
		if ( g_object[ i ].bUse == false )
		{
			// それぞれ格納
			g_object[ i ].type = type;
			g_object[ i ].pos = pos;
			g_object[ i ].rot = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
			g_object[ i ].scl = D3DXVECTOR3( 1.0f, 1.0f, 1.0f );

			// タイプ判定
			switch ( g_object[ i ].type )
			{
			case OBJECTTYPE_A:
				g_object[ i ].harfSize = D3DXVECTOR3( OBJECT_A_WIDTH * g_object[ i ].scl.x, 0.0f, OBJECT_A_HEIGHT_Z * g_object[ i ].scl.z );
				D3DXCreateTextureFromFile(pDevice, OBJECT_A_TEXPASS, &g_object[ i ].pTextureObject);

				{
				
					// xファイルの読み込み
					//------------------------------------
					D3DXLoadMeshFromX(OBJECT_A_FILEPASS,		// 読み込むファイル名
									  D3DXMESH_SYSTEMMEM,			// 
									  pDevice,						// 
									  NULL,							// 
									  &g_object[ i ].pBuffMatObject,// 
									  NULL,							// 
									  &g_object[ i ].numMatObject,	// 
									  &g_object[ i ].pMeshObject);
				}

				break;

			case OBJECTTYPE_B:
				g_object[ i ].harfSize = D3DXVECTOR3( OBJECT_B_WIDTH * g_object[ i ].scl.x, 0.0f, OBJECT_B_HEIGHT_Z * g_object[ i ].scl.z );
				D3DXCreateTextureFromFile(pDevice, OBJECT_B_TEXPASS, &g_object[ i ].pTextureObject);

				{
				
					// xファイルの読み込み
					//------------------------------------
					D3DXLoadMeshFromX(OBJECT_B_FILEPASS,		// 読み込むファイル名
									  D3DXMESH_SYSTEMMEM,			// 
									  pDevice,						// 
									  NULL,							// 
									  &g_object[ i ].pBuffMatObject,// 
									  NULL,							// 
									  &g_object[ i ].numMatObject,	// 
									  &g_object[ i ].pMeshObject);
				}

				break;

			case OBJECTTYPE_C:
				g_object[ i ].harfSize = D3DXVECTOR3( OBJECT_C_WIDTH * g_object[ i ].scl.x, 0.0f, OBJECT_C_HEIGHT_Z * g_object[ i ].scl.z );
				D3DXCreateTextureFromFile(pDevice, NULL, &g_object[ i ].pTextureObject);

				{
				
					// xファイルの読み込み
					//------------------------------------
					D3DXLoadMeshFromX(OBJECT_C_FILEPASS,		// 読み込むファイル名
									  D3DXMESH_SYSTEMMEM,			// 
									  pDevice,						// 
									  NULL,							// 
									  &g_object[ i ].pBuffMatObject,// 
									  NULL,							// 
									  &g_object[ i ].numMatObject,	// 
									  &g_object[ i ].pMeshObject);
				}

				break;

			case OBJECTTYPE_D:
				g_object[ i ].harfSize = D3DXVECTOR3( OBJECT_D_WIDTH * g_object[ i ].scl.x, 0.0f, OBJECT_D_HEIGHT_Z * g_object[ i ].scl.z );
				D3DXCreateTextureFromFile(pDevice, NULL, &g_object[ i ].pTextureObject);

				{
				
					// xファイルの読み込み
					//------------------------------------
					D3DXLoadMeshFromX(OBJECT_D_FILEPASS,		// 読み込むファイル名
									  D3DXMESH_SYSTEMMEM,			// 
									  pDevice,						// 
									  NULL,							// 
									  &g_object[ i ].pBuffMatObject,// 
									  NULL,							// 
									  &g_object[ i ].numMatObject,	// 
									  &g_object[ i ].pMeshObject);
				}

				break;

			case OBJECTTYPE_E:
				g_object[ i ].harfSize = D3DXVECTOR3( OBJECT_E_WIDTH * g_object[ i ].scl.x, 0.0f, OBJECT_E_HEIGHT_Z * g_object[ i ].scl.z );
				D3DXCreateTextureFromFile(pDevice, NULL, &g_object[ i ].pTextureObject);

				{
				
					// xファイルの読み込み
					//------------------------------------
					D3DXLoadMeshFromX(OBJECT_E_FILEPASS,		// 読み込むファイル名
									  D3DXMESH_SYSTEMMEM,			// 
									  pDevice,						// 
									  NULL,							// 
									  &g_object[ i ].pBuffMatObject,// 
									  NULL,							// 
									  &g_object[ i ].numMatObject,	// 
									  &g_object[ i ].pMeshObject);
				}

				break;
			}

			// 当たり判定用変数に格納
			rectObject[ i ].pos = g_object[ i ].pos;
			rectObject[ i ].harfSize = g_object[ i ].harfSize;

			g_object[ i ].bUse = true;		// 使用ON
			break;
		}
	}
}

//===============================================
//
// オブジェクト構造体変数取得
//
//===============================================
OBJECT *GetObject( void )
{
	return &g_object[ 0 ];
}

//===============================================
//
// 当たり判定の情報を取得
//
//===============================================
COL_RECT *GetRectObject( void )
{
	return &rectObject[ 0 ];
}

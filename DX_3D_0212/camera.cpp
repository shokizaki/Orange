//===============================================
//
//  内容   ： カメラに関する処理
//  File   ： camera.cpp
//  制作者 ： 木崎　翔
//
//===============================================

//===============================================
// 
//  インクルードファイル
//
//===============================================
#include "camera.h"
#include "model.h"
#include "input.h"
#include "billboard.h"
#include "pause.h"
#include "stringDraw.h"
#include "enemy.h"

//===============================================
//
//　マクロ定義
//
//===============================================
#define ROT_VAL (D3DX_PI / 180)	// 回転角度
#define MOVE_VAL (3.0f)			// 移動量
#define ROTATE_VAL (0.08f)

#define INTERVAL_CAMERA ( 800.0f )	// カメラとモデルの距離
#define HEIGHT_CAMERA	( 200.0f )	// カメラの視点の高さ

#define INTERVAL_BATTLECAMERA	( 600.0f )	// カメラとモデルの距離
#define HEIGHT_BATTLECAMERA		( 250.0f )	// カメラの視点の高さ

//-----------------------------------------------
//  グローバル変数
//-----------------------------------------------
CAMERA g_camera;				// CAMERA型構造体変数
D3DXMATRIX g_mtxProjection;		// プロジェクションマトリックス
bool bRotate = false;

//===============================================
//
// カメラに関する初期化
//
//===============================================
void InitCamera(void)
{
	// カメラに関する初期化
	//------------------------------------
	if ( GetMode() == MODE_TITLE )
	{
		g_camera.posCameraP = D3DXVECTOR3(0.0f, 100.0f, -500.0f);	// 視点の初期化
		g_camera.posCameraR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 注視点の初期化
		g_camera.vecCameraU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		// 上方向ベクトルの初期化

		g_camera.rotCamera = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// カメラの角度
		g_camera.fLengthIntervalCamera = sqrtf((g_camera.posCameraP.x * g_camera.posCameraP.x) + (g_camera.posCameraP.z * g_camera.posCameraP.z));
		g_camera.fLengthHeightCamera = HEIGHT_BATTLECAMERA;
	}
	else
	{
		g_camera.posCameraP = D3DXVECTOR3(0.0f, 300.0f, -600.0f);	// 視点の初期化
		g_camera.posCameraR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 注視点の初期化
		g_camera.vecCameraU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		// 上方向ベクトルの初期化

		g_camera.rotCamera = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// カメラの角度
		g_camera.fLengthIntervalCamera = sqrtf((g_camera.posCameraP.x * g_camera.posCameraP.x) + (g_camera.posCameraP.z * g_camera.posCameraP.z));
		g_camera.fLengthHeightCamera = HEIGHT_BATTLECAMERA;
		g_camera.fLengthIntervalCamera = INTERVAL_BATTLECAMERA;
	}

	g_camera.posCameraP_Old = g_camera.posCameraP;				// 視点のリセット位置の保存
	g_camera.posCameraR_Old = g_camera.posCameraR;				// 注視点ｎリセット位置の保存

	bRotate = false;

	//SetFadePos( g_camera.posCameraP, D3DXVECTOR3(0.0f, g_camera.rotCamera.y, 0.0f) );
}

//===============================================
//
// カメラに関する終了処理
//
//===============================================
void UninitCamera(void)
{

}

//===============================================
//
// カメラに関する更新処理
//
//===============================================
void UpdateCamera(void)
{
	MODEL_INFO *pModelInfo = GetModelInfo();
	MODEL_INFO *pEnemyInfo = GetEnemyInfo();
	bool *bEnemy = GetEnemyUse();

	if ( GetPauseDraw() == false )
	{
		// 注視点移動
		if (GetKeyboardPress(DIK_Q))
		{
			g_camera.rotCamera.y += 0.05f;
		}
		if (GetKeyboardPress(DIK_E))
		{
			g_camera.rotCamera.y -= 0.05f;
		}
		// 上移動
		if (GetKeyboardPress(DIK_Y))
		{
			g_camera.posCameraR.y += 5.0f;
		}
		// 下移動
		if (GetKeyboardPress(DIK_N))
		{
			g_camera.posCameraR.y -= 5.0f;
		}

		// 視点移動
		if (GetKeyboardPress(DIK_T))
		{
			g_camera.posCameraP.x -= sinf( g_camera.rotCamera.y ) * 5.0f;
			g_camera.posCameraP.z += cosf( g_camera.rotCamera.y ) * 5.0f;
		}
		if (GetKeyboardPress(DIK_G))
		{
			g_camera.posCameraP.x += sinf( g_camera.rotCamera.y ) * 5.0f;
			g_camera.posCameraP.z -= cosf( g_camera.rotCamera.y ) * 5.0f;
		}
		if (GetKeyboardPress(DIK_F))
		{
			g_camera.posCameraP.x -= cosf( g_camera.rotCamera.y ) * 5.0f;
			g_camera.posCameraP.z -= sinf( g_camera.rotCamera.y ) * 5.0f;
		}
		if (GetKeyboardPress(DIK_H))
		{
			g_camera.posCameraP.x += cosf( g_camera.rotCamera.y ) * 5.0f;
			g_camera.posCameraP.z += sinf( g_camera.rotCamera.y ) * 5.0f;
		}

		// 上移動
		if (GetKeyboardPress(DIK_U))
		{
			g_camera.posCameraP.y += 5.0f;
		}
		// 下移動
		if (GetKeyboardPress(DIK_M))
		{
			g_camera.posCameraP.y -= 5.0f;
		}

		// 移動した結果を注視点に反映
		g_camera.posCameraR.x = g_camera.posCameraP.x + sinf(-g_camera.rotCamera.y) * g_camera.fLengthIntervalCamera;
		g_camera.posCameraR.z = g_camera.posCameraP.z + cosf(g_camera.rotCamera.y) * g_camera.fLengthIntervalCamera;

		return;
	}

	//==============================================
	// 
	// カメラ移動処理
	// 
	//==============================================

	// 上移動
	if (GetKeyboardPress(DIK_Y))
	{
		g_camera.posCameraP.y += 2.0f;
	}

	// 下移動
	if (GetKeyboardPress(DIK_N))
	{
		g_camera.posCameraP.y -= 2.0f;
	}

	//==============================================
	// 
	// 注視点処理
	// 
	//==============================================

	// 上移動
	if (GetKeyboardPress(DIK_T))
	{
		g_camera.posCameraR.y += 5.0f;
	}

	// 下移動
	if (GetKeyboardPress(DIK_B))
	{
		g_camera.posCameraR.y -= 5.0f;
	}

	if ( ( GetKeyboardTrigger( DIK_L ) == true || GetGamePadTrigger( GAMEPAD_L ) ) || bRotate == true )
	{
		bRotate = true;
		
		// カメラの回り込み処理
		RotationCamera();
	}

	// カメラの回り込み処理（手動）
	// ゲームパッドを使用している場合
	//if ( GetGamePadUse() == true )
	{
		if ( GetPadElecomStickRX() == -1 )
		{
			g_camera.rotCamera.y += 0.03f;
		}

		if ( GetPadElecomStickRX() == 1 )
		{
			g_camera.rotCamera.y -= 0.03f;
		}
	}

	// キーボードの場合
	//else
	{
		if ( GetKeyboardPress( DIK_U ) == true )
		{
			g_camera.rotCamera.y += 0.03f;
		}

		if ( GetKeyboardPress( DIK_O ) == true )
		{
			g_camera.rotCamera.y -= 0.03f;
		}
	}

	// 規定値を超えたら丸め込む
	if (g_camera.rotCamera.y > D3DX_PI)
	{
		g_camera.rotCamera.y = ( ( g_camera.rotCamera.y ) - ( D3DX_PI * 2 ) );
	}
	if (g_camera.rotCamera.y < -D3DX_PI)
	{
		g_camera.rotCamera.y = ( ( g_camera.rotCamera.y ) + ( D3DX_PI * 2 ) );
	}

	if ( GetStringMessage() == GOTOUCH )
	{
		for ( int i = 0; i < ENEMY_MAX; i++ )
		{
			if ( *( bEnemy + i ) == true )
			{
				g_camera.posCameraR.x = ( pEnemyInfo + i ) ->posParts[ 0 ].x;
				break;
			}
		}
	}
	else
	{
		if ( GetMode() == MODE_TITLE )
		{
			// 視点の位置を設定
			g_camera.posCameraP.x = pModelInfo->posParts[ 0 ].x + 400.0f * sinf( g_camera.rotCamera.y );
			g_camera.posCameraP.y = pModelInfo->posParts[ 0 ].y + 200.0f;
			g_camera.posCameraP.z = pModelInfo->posParts[ 0 ].z + 500.0f * -cosf( g_camera.rotCamera.y );

			// 注視点の位置を設定
			g_camera.posCameraR.x += ((pModelInfo ->posParts[ 0 ].x + -sinf(pModelInfo ->rotParts[ 0 ].y) * 30.0f + (pModelInfo ->move.x * 15.0f)) - g_camera.posCameraR.x) * 0.1f;
			g_camera.posCameraR.z += ((pModelInfo ->posParts[ 0 ].z + -cosf(pModelInfo ->rotParts[ 0 ].y) * 50.0f + (pModelInfo ->move.z * 15.0f)) - g_camera.posCameraR.z) * 0.1f;	
		}
		else
		{
			// 視点の位置を設定
			g_camera.posCameraP.x = pModelInfo->posParts[ 0 ].x + g_camera.fLengthIntervalCamera * sinf( g_camera.rotCamera.y );
			g_camera.posCameraP.y = pModelInfo->posParts[ 0 ].y + g_camera.fLengthHeightCamera;
			g_camera.posCameraP.z = pModelInfo->posParts[ 0 ].z + g_camera.fLengthIntervalCamera * -cosf( g_camera.rotCamera.y );

			// 注視点の位置を設定
			g_camera.posCameraR.x += ((pModelInfo ->posParts[ 0 ].x + -sinf(pModelInfo ->rotParts[ 0 ].y) * 30.0f + (pModelInfo ->move.x * 15.0f)) - g_camera.posCameraR.x) * 0.1f;
			g_camera.posCameraR.z += ((pModelInfo ->posParts[ 0 ].z + -cosf(pModelInfo ->rotParts[ 0 ].y) * 50.0f + (pModelInfo ->move.z * 15.0f)) - g_camera.posCameraR.z) * 0.1f;	
		}
	}

		
	// デバッグ表示
	PrintDebugProc("/----- カメラ情報 -------------/\n");
	PrintDebugProc("[ 視点 ]\nX : %f  Y : %f  Z : %f\n", g_camera.posCameraP.x, g_camera.posCameraP.y, g_camera.posCameraP.z);
	//PrintDebugProc("[ 注視点 ]\nX : %f  Y : %f  Z : %f\n", g_camera.posCameraR.x, g_camera.posCameraR.y, g_camera.posCameraR.z);
	//PrintDebugProc("[ 向き ]\nY : %f\n", g_camera.rotCamera.y);
	//PrintDebugProc("[ 目標の向き ]\nY : %f\n", g_camera.rotCameraGoal.y);
	PrintDebugProc("/------------------------------/\n\n");
}

//===============================================
//
// カメラに関する描画処理
//
//===============================================
void SetCamera(void)
{
	// ローカル変数
	//------------------------------------
	LPDIRECT3DDEVICE9 pDevice;						// デバイスオブジェクト

	// デバイスの取得
	//------------------------------------
	pDevice = GetDevice();

	// プロジェクションマトリックスの設定
	//------------------------------------
	D3DXMatrixIdentity(&g_mtxProjection);		// フォーマットの初期化

	D3DXMatrixPerspectiveFovLH(&g_mtxProjection,							// 作成開始
								(float)DEGTOR(45),							// 視野角（画角）
								(float)SCREEN_WIDTH / SCREEN_HEIGHT,		// アスペクト比
								20.0f,										// Near値
								10000.0f);									// Far値

	pDevice ->SetTransform(D3DTS_PROJECTION, &g_mtxProjection);				// 設定

	// ビューマトリックスの設定
	//------------------------------------
	D3DXMatrixIdentity(&g_camera.mtxView);			// フォーマットの初期化

	D3DXMatrixLookAtLH(&g_camera.mtxView,					// 作成開始
						&g_camera.posCameraP,		// カメラの視点
						&g_camera.posCameraR,		// カメラの注視点
						&g_camera.vecCameraU);		// カメラの上方向ベクトル

	pDevice->SetTransform(D3DTS_VIEW, &g_camera.mtxView);	// 設定
}

//===============================================
//
// カメラの回り込み処理
//
//===============================================
void RotationCamera(void)
{
	// ローカル変数
	MODEL_INFO *pModelInfo = GetModelInfo();
	static bool bSave = false;

	// カメラの目標地点を格納
	if ( bSave == false )
	{
		g_camera.rotCameraGoal.y = D3DX_PI - pModelInfo ->targetRotParts.y;
		bSave = true;
	}

	if (g_camera.rotCameraGoal.y > D3DX_PI)
	{
		g_camera.rotCameraGoal.y = ((g_camera.rotCameraGoal.y) - (D3DX_PI * 2));
	}
	else if (g_camera.rotCameraGoal.y < -D3DX_PI)
	{
		g_camera.rotCameraGoal.y = ((g_camera.rotCameraGoal.y) + (D3DX_PI * 2));
	}

	if ( fabs( g_camera.rotCamera.y - g_camera.rotCameraGoal.y ) < 0.02f )
	{
		bRotate = false;
		bSave = false;
		return;
	}

	// カメラの回り込み処理
	//if (pModel ->bMove == false)
	{
		if (g_camera.rotCameraGoal.y - g_camera.rotCamera.y > D3DX_PI)
		{
			g_camera.rotCamera.y += ((g_camera.rotCameraGoal.y - g_camera.rotCamera.y) - (D3DX_PI * 2)) * ROTATE_VAL;

			if (g_camera.rotCamera.y > D3DX_PI)
			{
				g_camera.rotCamera.y = ((g_camera.rotCamera.y) - (D3DX_PI * 2));
			}
			else if (g_camera.rotCamera.y < -D3DX_PI)
			{
				g_camera.rotCamera.y = ((g_camera.rotCamera.y) + (D3DX_PI * 2));
			}
		}
		else if (g_camera.rotCameraGoal.y - g_camera.rotCamera.y < -D3DX_PI)
		{
			g_camera.rotCamera.y += ((g_camera.rotCameraGoal.y - g_camera.rotCamera.y) + (D3DX_PI * 2)) * ROTATE_VAL;

			if (g_camera.rotCamera.y > D3DX_PI)
			{
				g_camera.rotCamera.y = (g_camera.rotCamera.y) - (D3DX_PI * 2);
			}
			else if (g_camera.rotCamera.y < -D3DX_PI)
			{
				g_camera.rotCamera.y = (g_camera.rotCamera.y) + (D3DX_PI * 2);
			}
		}
		else
		{
			g_camera.rotCamera.y += (g_camera.rotCameraGoal.y - g_camera.rotCamera.y) * ROTATE_VAL;
		}
	}
}

//===============================================
//
// カメラ情報取得
//
//===============================================
CAMERA *GetCamera()
{
	return &g_camera;
}
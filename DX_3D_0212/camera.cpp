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
#include "input.h"
#include "cube.h"
#include "Edit.h"
#include "player.h"

//===============================================
//
//　マクロ定義
//
//===============================================
#define ROT_VAL (D3DX_PI / 180)	// 回転角度
#define MOVE_VAL (3.0f)			// 移動量
#define ROTATE_VAL (0.08f)
#define INTERVAL_CAMERA ( 1000.0f )	// カメラとモデルの距離
#define HEIGHT_CAMERA	( 200.0f )	// カメラの視点の高さ

#define INTERVAL_BATTLECAMERA	( 700.0f )	// カメラとモデルの距離
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
	g_camera.posCameraP = D3DXVECTOR3(0.0f, 100.0f, -500.0f);	// 視点の初期化
	g_camera.posCameraR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 注視点の初期化
	g_camera.vecCameraU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		// 上方向ベクトルの初期化

	{
		g_camera.rotCamera = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// カメラの角度
		g_camera.fLengthIntervalCamera = INTERVAL_CAMERA;
		g_camera.fLengthHeightCamera = HEIGHT_CAMERA;
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
	D3DXVECTOR3 pPosPlayer = GetPlayerPos();

	if ( GetEdit() == false )
	{
		g_camera.posCameraP.x = pPosPlayer.x;
		g_camera.posCameraP.y = pPosPlayer.y + 100.0f;
		g_camera.posCameraP.z = pPosPlayer.z - 500.0f;
		
		g_camera.posCameraR.y = pPosPlayer.y + 100.0f;

		// 移動した結果を注視点に反映
		g_camera.posCameraR.x = g_camera.posCameraP.x + sinf(-g_camera.rotCamera.y) * g_camera.fLengthIntervalCamera;
		g_camera.posCameraR.z = g_camera.posCameraP.z + cosf(g_camera.rotCamera.y) * g_camera.fLengthIntervalCamera;
	}
	else
	{
		// 旋回
		if (GetKeyboardPress(DIK_Q))
		{
			g_camera.rotCamera.y += 0.05f;
		}
		if (GetKeyboardPress(DIK_E))
		{
			g_camera.rotCamera.y -= 0.05f;
		}

		// 注視点移動
		// 上移動
		if (GetKeyboardPress(DIK_R))
		{
			g_camera.posCameraP.y += 5.0f;
		}
		// 下移動
		if (GetKeyboardPress(DIK_V))
		{
			g_camera.posCameraP.y -= 5.0f;
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

		if ( GetKeyboardPress(DIK_N) )
		{
			g_camera.posCameraP.y -= 2.0f;
			g_camera.posCameraR.y -= 2.0f;
		}
		if ( GetKeyboardPress(DIK_M) )
		{
			g_camera.posCameraP.y += 2.0f;
			g_camera.posCameraR.y += 2.0f;
		}
	}

	// カメラの回り込み処理（自動）
	/*if ( ( GetKeyboardTrigger( DIK_L ) == true || GetGamePadTrigger( GAMEPAD_L ) ) || bRotate == true )
	{
		bRotate = true;
		
		RotationCamera();
	}*/
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
// カメラのバトル時に関する初期化
//
//===============================================
void InitBattleCamera( void )
{
	// カメラに関する初期化
	//------------------------------------
	g_camera.posCameraP = D3DXVECTOR3(0.0f, HEIGHT_BATTLECAMERA, -500.0f);	// 視点の初期化
	g_camera.posCameraR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 注視点の初期化
	g_camera.vecCameraU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		// 上方向ベクトルの初期化

	g_camera.rotCamera = D3DXVECTOR3(0.0f, -1.57f, 0.0f);			// カメラの角度
	g_camera.fLengthIntervalCamera = INTERVAL_BATTLECAMERA;
	g_camera.fLengthHeightCamera = HEIGHT_BATTLECAMERA;

	g_camera.posCameraP_Old = g_camera.posCameraP;				// 視点のリセット位置の保存
	g_camera.posCameraR_Old = g_camera.posCameraR;				// 注視点ｎリセット位置の保存

	bRotate = false;
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
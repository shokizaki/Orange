//===============================================
//
//  内容   ： カメラ処理
//  File   ： camera.h
//  制作者 ： 木崎　翔
//
//===============================================

//===============================================
// 
// 二重インクルード防止
// 
//===============================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

//===============================================
//
//  インクルードファイル
//
//===============================================
#include "main.h"

//===============================================
//
// マクロ定義
//
//===============================================

//===============================================
//
// 構造体定義
//
//===============================================
typedef struct
{
	D3DXVECTOR3 posCameraP;		// カメラ視点
	D3DXVECTOR3 posCameraR;		// カメラ注視点
	D3DXVECTOR3 vecCameraU;		// カメラ上方向ベクトル
	D3DXVECTOR3 rotCamera;		// カメラの角度
	D3DXVECTOR3 rotCameraGoal;		// カメラの角度

	float fLengthIntervalCamera;	// 視点から注視点までの距離
	float fLengthHeightCamera;		// 視点の高さ

	D3DXMATRIX mtxView;			// ビューマトリックス

	D3DXVECTOR3 posCameraP_Old;
	D3DXVECTOR3 posCameraR_Old;

	D3DXVECTOR3 posCameraRGoal;		// カメラ注視点目標値
	D3DXVECTOR3 posCameraRMove;		// カメラ注視点移動量

	D3DVIEWPORT9 viewport;			// ビューポート
}CAMERA;

//===============================================
//
//  関数プロトタイプ宣言
//
//===============================================

//===============================================
//
// カメラに関する初期化
//
//===============================================
void InitCamera(void);

//===============================================
//
// カメラに関する終了処理
//
//===============================================
void UninitCamera(void);	

//===============================================
//
// カメラに関する更新処理
//
//===============================================
void UpdateCamera(void);

//===============================================
//
// カメラのバトル時に関する初期化
//
//===============================================
void InitBattleCamera( void );

//===============================================
//
// カメラのバトル時に関する更新処理
//
//===============================================
void UpdateBattleCamera( void );

//===============================================
//
// カメラの回り込み処理
//
//===============================================
void RotationCamera(void);

//===============================================
//
// カメラに関するセット処理
//
//===============================================
void SetCamera(void);

//===============================================
//
// カメラ情報取得
//
//===============================================
CAMERA *GetCamera();

//===============================================
//
// カメラ角度取得
//
//===============================================
D3DXVECTOR3 GetRotCamera();

#endif


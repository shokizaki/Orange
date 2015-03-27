//===================================================================
//
//  内容：入力処理に必要なもの
//  Fail:inputPad.h
//  制作者：木崎　翔
//
//===================================================================

//===================================================================
// 二重インクルード防止
//===================================================================
#ifndef _INPUTPAD_H_
#define _INPUTPAD_H_

//===================================================================
//
//  インクルードファイル
//
//===================================================================
#include "main.h"

//===================================================================
//
//　マクロ定義
//
//===================================================================
 
//------ 列挙型 ------
typedef enum
{
	GAMEPAD_A = 0,
	GAMEPAD_B,
	GAMEPAD_X,
	GAMEPAD_Y,
	GAMEPAD_L,
	GAMEPAD_R,
	GAMEPAD_SELECT,
	GAMEPAD_START,

	GAMEPAD_MAX
}GAMEPAD;

typedef enum
{
	PAD_1 = 0,
	PAD_2,
	PAD_3,
	PAD_4,
	PAD_5,
	PAD_6,
	PAD_7,
	PAD_8,
	PAD_9,
	PAD_10,
	PAD_11,
	PAD_KEY_UP = 0,
	PAD_KEY_RIGHT = 9000,
	PAD_KEY_LEFT = 18000,
	PAD_KEY_DOWN = 27000,
	PAD_MAX
}PAD_ELECOM;

//===================================================================
//
//  関数プロトタイプ宣言
//
//===================================================================
HRESULT InitGamePad(HINSTANCE hInstance, HWND hWnd);		// 入力処理の初期化処理
void UpdateGamePad(void);									// 入力処理の更新処理
void UninitGamePad(void);									// 入力処理の終了処理
bool GetGamePadPress( GAMEPAD nKey );						// プレス情報取得
bool GetGamePadTrigger( GAMEPAD nKey );						// トリガー情報の取得
bool GetGamePadRelease( GAMEPAD nKey );						// リリース情報の取得
bool GetGamePadRepeat( GAMEPAD nKey );						// リピート情報の取得
int GetGamePadCrossKeyY( void );							// リピート情報の取得
int GetGamePadCrossKeyX( void );							// リピート情報の取得
bool GetPadElecomPress( PAD_ELECOM nKey );					// プレス情報取得
bool GetPadElecomTrigger( PAD_ELECOM nKey );				// トリガー情報の取得
bool GetPadElecomRelease( PAD_ELECOM nKey );				// リリース情報の取得
bool GetPadElecomRepeat( PAD_ELECOM nKey );					// リピート情報の取得
bool GetPadElecomCrossKeyDownTrigger( void );				// 十字キー情報の取得
bool GetPadElecomCrossKeyUpTrigger( void );					// 十字キー情報の取得
bool GetPadElecomCrossKeyRightTrigger( void );					// 十字キー情報の取得
bool GetPadElecomCrossKeyLeftTrigger( void );					// 十字キー情報の取得
int GetPadElecomStickRZ( void );							// 十字キー情報の取得
int GetPadElecomStickRX( void );							// 十字キー情報の取得

bool GetGamePadUse( void );					// パッドの読み込み情報

#endif
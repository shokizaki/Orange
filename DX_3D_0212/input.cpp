//===================================================================
//
//  内容：入力処理に関する処理
//  Fail:input.cpp
//  制作者：木崎　翔
//
//===================================================================

//===================================================================
//
//  インクルードファイル
//
//===================================================================
#include "input.h"

//===================================================================
//
//　マクロ定義
//
//===================================================================

//===================================================================
//
//  関数プロトタイプ宣言
//
//===================================================================

//===================================================================
//
//  グローバル変数
//
//===================================================================
LPDIRECTINPUT8 g_pDInput = NULL;				// DirectInputインターフェースへのポインタ
LPDIRECTINPUTDEVICE8 g_pDIDevKeyboard = NULL;	// 入力デバイス（キーボード）へのポインタ
BYTE g_aKeyStatePress[256];						// キーボードの情報（プレス）
BYTE g_aKeyStateTrigger[256];					// キーボードの情報（トリガー）
BYTE g_aKeyStateRelease[256];					// キーボードの情報（リリース）
BYTE g_aKeyStateRepeat[256];					// キーボードの情報（リリース）
int g_aKeyStateRepeatCount[256];				// 
int g_nRepeatCount = 0;

//===================================================================
//
// 入力初期化処理
//
//===================================================================
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	// ローカル変数
	//------------------------------------
	HRESULT hr;

	// グローバル変数の初期化
	//------------------------------------
	g_nRepeatCount = 0;

	// DirectInputインターフェースの生成
	//------------------------------------
	if (g_pDInput == NULL)
	{
		hr = DirectInput8Create(hInstance,
								DIRECTINPUT_VERSION,
								IID_IDirectInput8,
								(void**)&g_pDInput,
								NULL);
		if (FAILED ( hr ))
		{
			MessageBox(NULL, "DirectInputインターフェースの初期化に失敗しました", "エラー", MB_OK);		// エラーメッセージ
			return hr;
		}
	}

	// キーボード用デバイスの作成
	//------------------------------------
	hr = g_pDInput->CreateDevice(GUID_SysKeyboard,
								 &g_pDIDevKeyboard,
								 NULL);
	if (FAILED ( hr ))
	{
		MessageBox(NULL, "キーボード用デバイスの作成に失敗しました", "エラー", MB_OK);		// エラーメッセージ
		return hr;
	}

	// データフォーマット設定
	//------------------------------------
	hr = g_pDIDevKeyboard->SetDataFormat( &c_dfDIKeyboard );
	if (FAILED ( hr ))
	{
		MessageBox(NULL, "データフォーマット設定に失敗しました", "エラー", MB_OK);		// エラーメッセージ
		return hr;
	}

	// 強調モードを設定
	//------------------------------------
	hr = g_pDIDevKeyboard->SetCooperativeLevel(hWnd,
											   (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if (FAILED ( hr ))
	{
		MessageBox(NULL, "強調モード設定に失敗しました", "エラー", MB_OK);		// エラーメッセージ
		return hr;
	}

	// キーボードへのアクセス権を獲得（入力制御開始）
	//----------------------------------------------
	g_pDIDevKeyboard->Acquire();

	return S_OK;
}

//===================================================================
//
// 更新処理
//
//===================================================================
void UpdateKeyboard(void)
{
	BYTE aKeyState[256];
	static int nCnt = 0;

	if (SUCCEEDED (g_pDIDevKeyboard ->GetDeviceState( sizeof(aKeyState), &aKeyState[0])) )
	{
		for (int nKey = 0; nKey < 256; nKey++)
		{
			// キートリガーの処理
			//-------------------------------------
			g_aKeyStateTrigger[nKey] = ((g_aKeyStatePress[nKey] ^ aKeyState[nKey]) & aKeyState[nKey]);

			// キーリリースの処理
			//-------------------------------------
			g_aKeyStateRelease[nKey] = (g_aKeyStatePress[nKey] & (g_aKeyStatePress[nKey] ^ aKeyState[nKey]) );

			// キーリピートの処理
			//-------------------------------------
			if (g_aKeyStateRelease[nKey] > 0)
			{// キーを離したら
			 //----------------------------------------------

				// カウンタの初期化とキーリピートのフラグを折る
				//----------------------------------------------
				g_aKeyStateRepeatCount[nKey] = 0;
				g_aKeyStateRepeat[nKey] = 0;
				g_nRepeatCount = 0;
			}

			if (aKeyState[nKey] > 0)
			{// キーが押されていたら
			 //----------------------------------------------
				g_aKeyStateRepeatCount[nKey]++;		// フレーム数をカウント
				g_nRepeatCount++;

				if (g_aKeyStateTrigger[nKey] > 0 || g_aKeyStateRepeatCount[nKey] > 30)
				{// トリガー状態または30フレーム経過したら
				 //----------------------------------------------

					// リピート情報に現在のキー状態を格納
					//----------------------------------------------
					g_aKeyStateRepeat[nKey] = aKeyState[nKey];
				}
				else
				{
					// リピートのフラグを折る
					//----------------------------------------------
					g_aKeyStateRepeat[nKey] = 0;
				}
			}

			// キープレスの処理
			//------------------------------------
			g_aKeyStatePress[nKey] = aKeyState[nKey];
		}
	}
	else
	{
		// キーボードへのアクセス権を獲得（入力制御開始）
		//----------------------------------------------
		g_pDIDevKeyboard -> Acquire();
	}
}

//===================================================================
//
// 終了処理
//
//===================================================================
void UninitKeyboard(void)
{
	// キーボード用デバイスに何か入っていたら
	//---------------------------------------
	if ( g_pDIDevKeyboard != NULL)
	{
		g_pDIDevKeyboard -> Unacquire();	// キーボードへのアクセス権を破棄（入力制御終了）
		g_pDIDevKeyboard -> Release();		// キーボード用デバイスの解放
		g_pDIDevKeyboard = NULL;			// 初期化
	}

	// DirectInputインターフェースに何か入っていたら
	//----------------------------------------------
	if (g_pDInput != NULL )
	{
		g_pDInput -> Release();		// DirectInputインターフェースの解放
		g_pDInput = NULL;			// 初期化
	}
}

//===================================================================
//
// キーボード情報取得（プレス）
//
//===================================================================
bool GetKeyboardPress(int nKey)
{
	return (g_aKeyStatePress[nKey] & 0x80) ? true : false;
}

//===================================================================
//
// キーボード情報取得（トリガー）
//
//===================================================================
bool GetKeyboardTrigger(int nKey)
{
	return (g_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//===================================================================
//
// キーボード情報取得（リリース）
//
//===================================================================
bool GetKeyboardRelease(int nKey)
{
	return (g_aKeyStateRelease[nKey] & 0x80) ? true : false;
}

//===================================================================
//
// キーボード情報取得（リピート）
//
//===================================================================
bool GetKeyboardRepeat(int nKey)
{
	return (g_aKeyStateRepeat[nKey] & 0x80) ? true : false;
}

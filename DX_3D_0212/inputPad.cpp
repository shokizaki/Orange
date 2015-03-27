//===================================================================
//
//  内容：入力処理に関する処理
//  Fail:inputPad.cpp
//  制作者：木崎　翔
//
//===================================================================

//===================================================================
//
//  インクルードファイル
//
//===================================================================
#include "inputPad.h"
#include "motion.h"

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
BOOL CALLBACK EnumJoysticksCallback( const DIDEVICEINSTANCE *pdidInstance , VOID *pContext );
BOOL CALLBACK EnumAxesCallback( const DIDEVICEOBJECTINSTANCE *pdidoi , VOID *pContext );

//===================================================================
//
//  グローバル変数
//
//===================================================================
LPDIRECTINPUT8       g_pDInputPad = NULL;
LPDIRECTINPUTDEVICE8 g_pDInputDevicePad = NULL;
DIDEVCAPS            g_diDevCapsPad;
DIJOYSTATE g_js;
DIJOYSTATE jsPress;
DIJOYSTATE jsTrigger;
DIJOYSTATE jsRelease;
DIJOYSTATE jsRepeat;
BYTE g_aKeyStatePressPad[256];						// キーボードの情報（プレス）
BYTE g_aKeyStateTriggerPad[256];					// キーボードの情報（トリガー）
BYTE g_aKeyStateReleasePad[256];					// キーボードの情報（リリース）
BYTE g_aKeyStateRepeatPad[256];					// キーボードの情報（リリース）
int jsRepeatCountPad[256];						// 

bool g_bCrossKeyTrigger = false;
bool g_bCrossKeyUpTriggerReturn = false;
bool g_bCrossKeyDownTriggerReturn = false;

// 入力処理の初期化処理
HRESULT InitGamePad(HINSTANCE hInstance, HWND hWnd)
{
	// ローカル変数
	//------------------------------------
	HRESULT hr;

	// DirectInputインターフェースの生成
	//------------------------------------
	if (g_pDInputPad == NULL)
	{
		hr = DirectInput8Create(hInstance,
								DIRECTINPUT_VERSION,
								IID_IDirectInput8,
								(void**)&g_pDInputPad,
								NULL);
		if (FAILED ( hr ))
		{
			//MessageBox(NULL, "DirectInputインターフェースの初期化に失敗しました", "エラー", MB_OK);		// エラーメッセージ
			return hr;
		}
	}

	hr = g_pDInputPad ->EnumDevices( DI8DEVCLASS_GAMECTRL , (LPDIENUMDEVICESCALLBACK)EnumJoysticksCallback , NULL , DIEDFL_ATTACHEDONLY );
	if (FAILED ( hr ))
	{
		//MessageBox(NULL, "データフォーマット設定に失敗しました", "エラー", MB_OK);		// エラーメッセージ
		return hr;
	}

	// パッドを読み込めてなかったら
	if ( g_pDInputDevicePad == NULL )
	{
		//MessageBox(NULL, "ゲームパッドの読み込みに失敗", "エラー", MB_OK);		// エラーメッセージ

		return E_FAIL;
	}

	// データフォーマットの設定
	hr = g_pDInputDevicePad ->SetDataFormat( &c_dfDIJoystick );
	if (FAILED ( hr ))
	{
		//MessageBox(NULL, "データフォーマット設定に失敗しました", "エラー", MB_OK);		// エラーメッセージ
		return hr;
	}

	// 協調レベルの設定
	hr = g_pDInputDevicePad ->SetCooperativeLevel( hWnd,  DISCL_EXCLUSIVE | DISCL_FOREGROUND );
	if (FAILED ( hr ))
	{
		//MessageBox(NULL, "協調レベル設定に失敗しました", "エラー", MB_OK);		// エラーメッセージ
		return hr;
	}

	g_diDevCapsPad.dwSize = sizeof( DIDEVCAPS );

	// デバイスの能力
	hr = g_pDInputDevicePad ->GetCapabilities( &g_diDevCapsPad );
	if (FAILED ( hr ))
	{
		//MessageBox(NULL, "デバイス能力の設定に失敗しました", "エラー", MB_OK);		// エラーメッセージ
		return hr;
	}

	// 十字キーの範囲などを指定
	hr = g_pDInputDevicePad ->EnumObjects( EnumAxesCallback , (VOID*)hWnd , DIDFT_AXIS );
	if (FAILED ( hr ))
	{
		//MessageBox(NULL, "十字キーの範囲などの設定に失敗しました", "エラー", MB_OK);		// エラーメッセージ
		return hr;
	}

	// 入力確認
	//HRESULT hr;

	hr = g_pDInputDevicePad->Poll();
	if ( FAILED( hr ) )
	{
		hr = g_pDInputDevicePad->Acquire();
		while( hr == DIERR_INPUTLOST )
		{
			hr = g_pDInputDevicePad->Acquire();
		}
	}

	g_bCrossKeyUpTriggerReturn = false;
	g_bCrossKeyDownTriggerReturn = false;

	return S_OK;
}

BOOL CALLBACK EnumJoysticksCallback( const DIDEVICEINSTANCE *pdidInstance , VOID *pContext )
{
	HRESULT hr;
	
	hr = g_pDInputPad->CreateDevice( pdidInstance->guidInstance , &g_pDInputDevicePad , NULL );
	
	if ( FAILED( hr ) ) 
	{
		return DIENUM_CONTINUE;
	}

	return DIENUM_STOP;
}

BOOL CALLBACK EnumAxesCallback( const DIDEVICEOBJECTINSTANCE *pdidoi , VOID *pContext )
{
	HRESULT     hr;
	DIPROPRANGE diprg;
	
	diprg.diph.dwSize       = sizeof( DIPROPRANGE );
	diprg.diph.dwHeaderSize = sizeof( DIPROPHEADER );
	diprg.diph.dwHow        = DIPH_BYID;
	diprg.diph.dwObj        = pdidoi->dwType;
	diprg.lMin              = 0 - 1;
	diprg.lMax              = 0 + 1;
	hr = g_pDInputDevicePad->SetProperty( DIPROP_RANGE , &diprg.diph );
	
	if ( FAILED( hr ) ) return DIENUM_STOP;
	
	return DIENUM_CONTINUE;
}

// 入力処理の更新処理
void UpdateGamePad( void )
{
	//BYTE js.rgbButtons[256];
	static int nCnt = 0;
	HRESULT    hr;
	DIJOYSTATE js;

	if ( NULL == g_pDInputDevicePad )
	{
		//MessageBox(NULL, "失敗しました1", "エラー", MB_OK);		// エラーメッセージ
		return ;
	}

	hr = g_pDInputDevicePad ->Poll();
	if ( FAILED( hr ) ) 
	{
		//MessageBox(NULL, "失敗しました2", "エラー", MB_OK);		// エラーメッセージ
		//return ;
		g_pDInputDevicePad ->Acquire();
	}

	if (SUCCEEDED (g_pDInputDevicePad->GetDeviceState( sizeof( DIJOYSTATE ) , &g_js ) ) )
	{
		if ( g_js.rgdwPOV[ 0 ] != -1 )
		{
			g_bCrossKeyTrigger = true;
		}
		else
		{
			g_bCrossKeyTrigger = false;
			g_bCrossKeyUpTriggerReturn = false;
			g_bCrossKeyDownTriggerReturn = false;
		}
	}
	else
	{
		// キーボードへのアクセス権を獲得（入力制御開始）
		//----------------------------------------------
		g_pDInputDevicePad -> Acquire();
	}

	//PrintDebugProc("\n\n\n\n%d %d\n", g_js.lX, g_js.lY);
	//PrintDebugProc("atan : %f\n", atan2f( g_js.lX, g_js.lY ));
	//PrintDebugProc("右スティック : %d\n", g_js.lZ);

	if (SUCCEEDED (g_pDInputDevicePad->GetDeviceState( sizeof( DIJOYSTATE ) , &js )) )
	{
		for (int nKey = 0; nKey < 32; nKey++)
		{
			// キートリガーの処理
			//-------------------------------------
			jsTrigger.rgbButtons[nKey] = ((jsPress.rgbButtons[nKey] ^ js.rgbButtons[nKey]) & js.rgbButtons[nKey]);

			//// キーリリースの処理
			////-------------------------------------
			//jsRelease.rgbButtons[nKey] = (jsPress.rgbButtons[nKey] & (jsPress.rgbButtons[nKey] ^ js.rgbButtons[nKey]) );

			//// キーリピートの処理
			////-------------------------------------
			//if (jsRelease.rgbButtons[nKey] > 0)
			//{// キーを離したら
			// //----------------------------------------------

			//	// カウンタの初期化とキーリピートのフラグを折る
			//	//----------------------------------------------
			//	js.rgbButtons[nKey] = 0;
			//	jsRepeat.rgbButtons[nKey] = 0;
			//	g_nRepeatCountPad = 0;
			//}

			//if (js.rgbButtons[nKey] > 0)
			//{// キーが押されていたら
			// //----------------------------------------------
			//	js.rgbButtons[nKey]++;		// フレーム数をカウント
			//	g_nRepeatCountPad++;

			//	if (jsTrigger.rgbButtons[nKey] > 0 || js.rgbButtons[nKey] > 30)
			//	{// トリガー状態または30フレーム経過したら
			//	 //----------------------------------------------

			//		// リピート情報に現在のキー状態を格納
			//		//----------------------------------------------
			//		jsRepeat.rgbButtons[nKey] = js.rgbButtons[nKey];
			//	}
			//	else
			//	{
			//		// リピートのフラグを折る
			//		//----------------------------------------------
			//		jsRepeat.rgbButtons[nKey] = 0;
			//	}
			//}

			// キープレスの処理
			//------------------------------------
			jsPress.rgbButtons[nKey] = js.rgbButtons[nKey];
		}
	}
	else
	{
		// キーボードへのアクセス権を獲得（入力制御開始）
		//----------------------------------------------
		g_pDInputDevicePad -> Acquire();
	}
}

// 入力処理の終了処理
void UninitGamePad(void)
{
	// キーボード用デバイスに何か入っていたら
	//---------------------------------------
	if ( g_pDInputDevicePad != NULL)
	{
		g_pDInputDevicePad -> Unacquire();	// キーボードへのアクセス権を破棄（入力制御終了）
		g_pDInputDevicePad -> Release();		// キーボード用デバイスの解放
		g_pDInputDevicePad = NULL;			// 初期化
	}

	// DirectInputインターフェースに何か入っていたら
	//----------------------------------------------
	if (g_pDInputPad != NULL )
	{
		g_pDInputPad -> Release();		// DirectInputインターフェースの解放
		g_pDInputPad = NULL;			// 初期化
	}
}

// プレス情報取得
bool GetGamePadPress(GAMEPAD nKey)
{
	return jsPress.rgbButtons[ nKey ] & 0x80 ? true : false;
}

// トリガー情報の取得
bool GetGamePadTrigger(GAMEPAD nKey)
{
	return jsTrigger.rgbButtons[ nKey ] & 0x80 ? true : false;
}

// リリース情報の取得
bool GetGamePadRelease(GAMEPAD nKey)
{
	return jsRelease.rgbButtons[ nKey ] & 0x80 ? true : false;
}

// リピート情報の取得
bool GetGamePadRepeat(GAMEPAD nKey)
{
	return jsRepeat.rgbButtons[ nKey ] & 0x80 ? true : false;
}

int GetGamePadCrossKeyY( void )
{
	return g_js.lY;
}

int GetGamePadCrossKeyX( void )
{
	return g_js.lX;
}

// プレス情報取得
bool GetPadElecomPress(PAD_ELECOM nKey)
{
	return jsPress.rgbButtons[ nKey ] & 0x80 ? true : false;
}

// トリガー情報の取得
bool GetPadElecomTrigger(PAD_ELECOM nKey)
{
	return jsTrigger.rgbButtons[ nKey ] & 0x80 ? true : false;
}

// リリース情報の取得
bool GetPadElecomRelease(PAD_ELECOM nKey)
{
	return jsRelease.rgbButtons[ nKey ] & 0x80 ? true : false;
}

// リピート情報の取得
bool GetPadElecomRepeat(PAD_ELECOM nKey)
{
	return jsRepeat.rgbButtons[ nKey ] & 0x80 ? true : false;
}

// 十字キー情報の取得
bool GetPadElecomCrossKeyDownTrigger( void )
{
	if ( g_bCrossKeyTrigger == true && g_bCrossKeyDownTriggerReturn == false )
	{
		g_bCrossKeyDownTriggerReturn = true;
		return ( g_js.rgdwPOV[ 0 ] == 18000 ) ? true: false;
	}

	return false;
}

// 十字キー情報の取得
bool GetPadElecomCrossKeyUpTrigger( void )
{
	if ( g_bCrossKeyTrigger == true && g_bCrossKeyUpTriggerReturn == false )
	{
		g_bCrossKeyUpTriggerReturn = true;
		return ( g_js.rgdwPOV[ 0 ] == 0 ) ? true: false;
	}

	return false;
}

// 十字キー情報の取得
bool GetPadElecomCrossKeyRightTrigger( void )
{
	if ( g_bCrossKeyTrigger == true && g_bCrossKeyDownTriggerReturn == false )
	{
		g_bCrossKeyDownTriggerReturn = true;
		return ( g_js.rgdwPOV[ 0 ] == 9000 ) ? true: false;
	}

	return false;
}

// 十字キー情報の取得
bool GetPadElecomCrossKeyLeftTrigger( void )
{
	if ( g_bCrossKeyTrigger == true && g_bCrossKeyUpTriggerReturn == false )
	{
		g_bCrossKeyUpTriggerReturn = true;
		return ( g_js.rgdwPOV[ 0 ] == 27000 ) ? true: false;
	}

	return false;
}

bool GetGamePadUse( void )
{
	if ( g_pDInputDevicePad == NULL )
	{
		return false;
	}

	return true;
}

int GetPadElecomStickRZ( void )
{
	return g_js.lRz;
}

int GetPadElecomStickRX( void )
{
	return g_js.lZ;
}
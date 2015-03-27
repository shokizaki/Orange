//===================================================================
//
//  ���e�F���͏����Ɋւ��鏈��
//  Fail:inputPad.cpp
//  ����ҁF�؍�@��
//
//===================================================================

//===================================================================
//
//  �C���N���[�h�t�@�C��
//
//===================================================================
#include "inputPad.h"
#include "motion.h"

//===================================================================
//
//�@�}�N����`
//
//===================================================================

//===================================================================
//
//  �֐��v���g�^�C�v�錾
//
//===================================================================
BOOL CALLBACK EnumJoysticksCallback( const DIDEVICEINSTANCE *pdidInstance , VOID *pContext );
BOOL CALLBACK EnumAxesCallback( const DIDEVICEOBJECTINSTANCE *pdidoi , VOID *pContext );

//===================================================================
//
//  �O���[�o���ϐ�
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
BYTE g_aKeyStatePressPad[256];						// �L�[�{�[�h�̏��i�v���X�j
BYTE g_aKeyStateTriggerPad[256];					// �L�[�{�[�h�̏��i�g���K�[�j
BYTE g_aKeyStateReleasePad[256];					// �L�[�{�[�h�̏��i�����[�X�j
BYTE g_aKeyStateRepeatPad[256];					// �L�[�{�[�h�̏��i�����[�X�j
int jsRepeatCountPad[256];						// 

bool g_bCrossKeyTrigger = false;
bool g_bCrossKeyUpTriggerReturn = false;
bool g_bCrossKeyDownTriggerReturn = false;

// ���͏����̏���������
HRESULT InitGamePad(HINSTANCE hInstance, HWND hWnd)
{
	// ���[�J���ϐ�
	//------------------------------------
	HRESULT hr;

	// DirectInput�C���^�[�t�F�[�X�̐���
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
			//MessageBox(NULL, "DirectInput�C���^�[�t�F�[�X�̏������Ɏ��s���܂���", "�G���[", MB_OK);		// �G���[���b�Z�[�W
			return hr;
		}
	}

	hr = g_pDInputPad ->EnumDevices( DI8DEVCLASS_GAMECTRL , (LPDIENUMDEVICESCALLBACK)EnumJoysticksCallback , NULL , DIEDFL_ATTACHEDONLY );
	if (FAILED ( hr ))
	{
		//MessageBox(NULL, "�f�[�^�t�H�[�}�b�g�ݒ�Ɏ��s���܂���", "�G���[", MB_OK);		// �G���[���b�Z�[�W
		return hr;
	}

	// �p�b�h��ǂݍ��߂ĂȂ�������
	if ( g_pDInputDevicePad == NULL )
	{
		//MessageBox(NULL, "�Q�[���p�b�h�̓ǂݍ��݂Ɏ��s", "�G���[", MB_OK);		// �G���[���b�Z�[�W

		return E_FAIL;
	}

	// �f�[�^�t�H�[�}�b�g�̐ݒ�
	hr = g_pDInputDevicePad ->SetDataFormat( &c_dfDIJoystick );
	if (FAILED ( hr ))
	{
		//MessageBox(NULL, "�f�[�^�t�H�[�}�b�g�ݒ�Ɏ��s���܂���", "�G���[", MB_OK);		// �G���[���b�Z�[�W
		return hr;
	}

	// �������x���̐ݒ�
	hr = g_pDInputDevicePad ->SetCooperativeLevel( hWnd,  DISCL_EXCLUSIVE | DISCL_FOREGROUND );
	if (FAILED ( hr ))
	{
		//MessageBox(NULL, "�������x���ݒ�Ɏ��s���܂���", "�G���[", MB_OK);		// �G���[���b�Z�[�W
		return hr;
	}

	g_diDevCapsPad.dwSize = sizeof( DIDEVCAPS );

	// �f�o�C�X�̔\��
	hr = g_pDInputDevicePad ->GetCapabilities( &g_diDevCapsPad );
	if (FAILED ( hr ))
	{
		//MessageBox(NULL, "�f�o�C�X�\�͂̐ݒ�Ɏ��s���܂���", "�G���[", MB_OK);		// �G���[���b�Z�[�W
		return hr;
	}

	// �\���L�[�͈̔͂Ȃǂ��w��
	hr = g_pDInputDevicePad ->EnumObjects( EnumAxesCallback , (VOID*)hWnd , DIDFT_AXIS );
	if (FAILED ( hr ))
	{
		//MessageBox(NULL, "�\���L�[�͈̔͂Ȃǂ̐ݒ�Ɏ��s���܂���", "�G���[", MB_OK);		// �G���[���b�Z�[�W
		return hr;
	}

	// ���͊m�F
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

// ���͏����̍X�V����
void UpdateGamePad( void )
{
	//BYTE js.rgbButtons[256];
	static int nCnt = 0;
	HRESULT    hr;
	DIJOYSTATE js;

	if ( NULL == g_pDInputDevicePad )
	{
		//MessageBox(NULL, "���s���܂���1", "�G���[", MB_OK);		// �G���[���b�Z�[�W
		return ;
	}

	hr = g_pDInputDevicePad ->Poll();
	if ( FAILED( hr ) ) 
	{
		//MessageBox(NULL, "���s���܂���2", "�G���[", MB_OK);		// �G���[���b�Z�[�W
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
		// �L�[�{�[�h�ւ̃A�N�Z�X�����l���i���͐���J�n�j
		//----------------------------------------------
		g_pDInputDevicePad -> Acquire();
	}

	//PrintDebugProc("\n\n\n\n%d %d\n", g_js.lX, g_js.lY);
	//PrintDebugProc("atan : %f\n", atan2f( g_js.lX, g_js.lY ));
	//PrintDebugProc("�E�X�e�B�b�N : %d\n", g_js.lZ);

	if (SUCCEEDED (g_pDInputDevicePad->GetDeviceState( sizeof( DIJOYSTATE ) , &js )) )
	{
		for (int nKey = 0; nKey < 32; nKey++)
		{
			// �L�[�g���K�[�̏���
			//-------------------------------------
			jsTrigger.rgbButtons[nKey] = ((jsPress.rgbButtons[nKey] ^ js.rgbButtons[nKey]) & js.rgbButtons[nKey]);

			//// �L�[�����[�X�̏���
			////-------------------------------------
			//jsRelease.rgbButtons[nKey] = (jsPress.rgbButtons[nKey] & (jsPress.rgbButtons[nKey] ^ js.rgbButtons[nKey]) );

			//// �L�[���s�[�g�̏���
			////-------------------------------------
			//if (jsRelease.rgbButtons[nKey] > 0)
			//{// �L�[�𗣂�����
			// //----------------------------------------------

			//	// �J�E���^�̏������ƃL�[���s�[�g�̃t���O��܂�
			//	//----------------------------------------------
			//	js.rgbButtons[nKey] = 0;
			//	jsRepeat.rgbButtons[nKey] = 0;
			//	g_nRepeatCountPad = 0;
			//}

			//if (js.rgbButtons[nKey] > 0)
			//{// �L�[��������Ă�����
			// //----------------------------------------------
			//	js.rgbButtons[nKey]++;		// �t���[�������J�E���g
			//	g_nRepeatCountPad++;

			//	if (jsTrigger.rgbButtons[nKey] > 0 || js.rgbButtons[nKey] > 30)
			//	{// �g���K�[��Ԃ܂���30�t���[���o�߂�����
			//	 //----------------------------------------------

			//		// ���s�[�g���Ɍ��݂̃L�[��Ԃ��i�[
			//		//----------------------------------------------
			//		jsRepeat.rgbButtons[nKey] = js.rgbButtons[nKey];
			//	}
			//	else
			//	{
			//		// ���s�[�g�̃t���O��܂�
			//		//----------------------------------------------
			//		jsRepeat.rgbButtons[nKey] = 0;
			//	}
			//}

			// �L�[�v���X�̏���
			//------------------------------------
			jsPress.rgbButtons[nKey] = js.rgbButtons[nKey];
		}
	}
	else
	{
		// �L�[�{�[�h�ւ̃A�N�Z�X�����l���i���͐���J�n�j
		//----------------------------------------------
		g_pDInputDevicePad -> Acquire();
	}
}

// ���͏����̏I������
void UninitGamePad(void)
{
	// �L�[�{�[�h�p�f�o�C�X�ɉ��������Ă�����
	//---------------------------------------
	if ( g_pDInputDevicePad != NULL)
	{
		g_pDInputDevicePad -> Unacquire();	// �L�[�{�[�h�ւ̃A�N�Z�X����j���i���͐���I���j
		g_pDInputDevicePad -> Release();		// �L�[�{�[�h�p�f�o�C�X�̉��
		g_pDInputDevicePad = NULL;			// ������
	}

	// DirectInput�C���^�[�t�F�[�X�ɉ��������Ă�����
	//----------------------------------------------
	if (g_pDInputPad != NULL )
	{
		g_pDInputPad -> Release();		// DirectInput�C���^�[�t�F�[�X�̉��
		g_pDInputPad = NULL;			// ������
	}
}

// �v���X���擾
bool GetGamePadPress(GAMEPAD nKey)
{
	return jsPress.rgbButtons[ nKey ] & 0x80 ? true : false;
}

// �g���K�[���̎擾
bool GetGamePadTrigger(GAMEPAD nKey)
{
	return jsTrigger.rgbButtons[ nKey ] & 0x80 ? true : false;
}

// �����[�X���̎擾
bool GetGamePadRelease(GAMEPAD nKey)
{
	return jsRelease.rgbButtons[ nKey ] & 0x80 ? true : false;
}

// ���s�[�g���̎擾
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

// �v���X���擾
bool GetPadElecomPress(PAD_ELECOM nKey)
{
	return jsPress.rgbButtons[ nKey ] & 0x80 ? true : false;
}

// �g���K�[���̎擾
bool GetPadElecomTrigger(PAD_ELECOM nKey)
{
	return jsTrigger.rgbButtons[ nKey ] & 0x80 ? true : false;
}

// �����[�X���̎擾
bool GetPadElecomRelease(PAD_ELECOM nKey)
{
	return jsRelease.rgbButtons[ nKey ] & 0x80 ? true : false;
}

// ���s�[�g���̎擾
bool GetPadElecomRepeat(PAD_ELECOM nKey)
{
	return jsRepeat.rgbButtons[ nKey ] & 0x80 ? true : false;
}

// �\���L�[���̎擾
bool GetPadElecomCrossKeyDownTrigger( void )
{
	if ( g_bCrossKeyTrigger == true && g_bCrossKeyDownTriggerReturn == false )
	{
		g_bCrossKeyDownTriggerReturn = true;
		return ( g_js.rgdwPOV[ 0 ] == 18000 ) ? true: false;
	}

	return false;
}

// �\���L�[���̎擾
bool GetPadElecomCrossKeyUpTrigger( void )
{
	if ( g_bCrossKeyTrigger == true && g_bCrossKeyUpTriggerReturn == false )
	{
		g_bCrossKeyUpTriggerReturn = true;
		return ( g_js.rgdwPOV[ 0 ] == 0 ) ? true: false;
	}

	return false;
}

// �\���L�[���̎擾
bool GetPadElecomCrossKeyRightTrigger( void )
{
	if ( g_bCrossKeyTrigger == true && g_bCrossKeyDownTriggerReturn == false )
	{
		g_bCrossKeyDownTriggerReturn = true;
		return ( g_js.rgdwPOV[ 0 ] == 9000 ) ? true: false;
	}

	return false;
}

// �\���L�[���̎擾
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
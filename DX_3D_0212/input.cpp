//===================================================================
//
//  ���e�F���͏����Ɋւ��鏈��
//  Fail:input.cpp
//  ����ҁF�؍�@��
//
//===================================================================

//===================================================================
//
//  �C���N���[�h�t�@�C��
//
//===================================================================
#include "input.h"

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

//===================================================================
//
//  �O���[�o���ϐ�
//
//===================================================================
LPDIRECTINPUT8 g_pDInput = NULL;				// DirectInput�C���^�[�t�F�[�X�ւ̃|�C���^
LPDIRECTINPUTDEVICE8 g_pDIDevKeyboard = NULL;	// ���̓f�o�C�X�i�L�[�{�[�h�j�ւ̃|�C���^
BYTE g_aKeyStatePress[256];						// �L�[�{�[�h�̏��i�v���X�j
BYTE g_aKeyStateTrigger[256];					// �L�[�{�[�h�̏��i�g���K�[�j
BYTE g_aKeyStateRelease[256];					// �L�[�{�[�h�̏��i�����[�X�j
BYTE g_aKeyStateRepeat[256];					// �L�[�{�[�h�̏��i�����[�X�j
int g_aKeyStateRepeatCount[256];				// 
int g_nRepeatCount = 0;

//===================================================================
//
// ���͏���������
//
//===================================================================
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	// ���[�J���ϐ�
	//------------------------------------
	HRESULT hr;

	// �O���[�o���ϐ��̏�����
	//------------------------------------
	g_nRepeatCount = 0;

	// DirectInput�C���^�[�t�F�[�X�̐���
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
			MessageBox(NULL, "DirectInput�C���^�[�t�F�[�X�̏������Ɏ��s���܂���", "�G���[", MB_OK);		// �G���[���b�Z�[�W
			return hr;
		}
	}

	// �L�[�{�[�h�p�f�o�C�X�̍쐬
	//------------------------------------
	hr = g_pDInput->CreateDevice(GUID_SysKeyboard,
								 &g_pDIDevKeyboard,
								 NULL);
	if (FAILED ( hr ))
	{
		MessageBox(NULL, "�L�[�{�[�h�p�f�o�C�X�̍쐬�Ɏ��s���܂���", "�G���[", MB_OK);		// �G���[���b�Z�[�W
		return hr;
	}

	// �f�[�^�t�H�[�}�b�g�ݒ�
	//------------------------------------
	hr = g_pDIDevKeyboard->SetDataFormat( &c_dfDIKeyboard );
	if (FAILED ( hr ))
	{
		MessageBox(NULL, "�f�[�^�t�H�[�}�b�g�ݒ�Ɏ��s���܂���", "�G���[", MB_OK);		// �G���[���b�Z�[�W
		return hr;
	}

	// �������[�h��ݒ�
	//------------------------------------
	hr = g_pDIDevKeyboard->SetCooperativeLevel(hWnd,
											   (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if (FAILED ( hr ))
	{
		MessageBox(NULL, "�������[�h�ݒ�Ɏ��s���܂���", "�G���[", MB_OK);		// �G���[���b�Z�[�W
		return hr;
	}

	// �L�[�{�[�h�ւ̃A�N�Z�X�����l���i���͐���J�n�j
	//----------------------------------------------
	g_pDIDevKeyboard->Acquire();

	return S_OK;
}

//===================================================================
//
// �X�V����
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
			// �L�[�g���K�[�̏���
			//-------------------------------------
			g_aKeyStateTrigger[nKey] = ((g_aKeyStatePress[nKey] ^ aKeyState[nKey]) & aKeyState[nKey]);

			// �L�[�����[�X�̏���
			//-------------------------------------
			g_aKeyStateRelease[nKey] = (g_aKeyStatePress[nKey] & (g_aKeyStatePress[nKey] ^ aKeyState[nKey]) );

			// �L�[���s�[�g�̏���
			//-------------------------------------
			if (g_aKeyStateRelease[nKey] > 0)
			{// �L�[�𗣂�����
			 //----------------------------------------------

				// �J�E���^�̏������ƃL�[���s�[�g�̃t���O��܂�
				//----------------------------------------------
				g_aKeyStateRepeatCount[nKey] = 0;
				g_aKeyStateRepeat[nKey] = 0;
				g_nRepeatCount = 0;
			}

			if (aKeyState[nKey] > 0)
			{// �L�[��������Ă�����
			 //----------------------------------------------
				g_aKeyStateRepeatCount[nKey]++;		// �t���[�������J�E���g
				g_nRepeatCount++;

				if (g_aKeyStateTrigger[nKey] > 0 || g_aKeyStateRepeatCount[nKey] > 30)
				{// �g���K�[��Ԃ܂���30�t���[���o�߂�����
				 //----------------------------------------------

					// ���s�[�g���Ɍ��݂̃L�[��Ԃ��i�[
					//----------------------------------------------
					g_aKeyStateRepeat[nKey] = aKeyState[nKey];
				}
				else
				{
					// ���s�[�g�̃t���O��܂�
					//----------------------------------------------
					g_aKeyStateRepeat[nKey] = 0;
				}
			}

			// �L�[�v���X�̏���
			//------------------------------------
			g_aKeyStatePress[nKey] = aKeyState[nKey];
		}
	}
	else
	{
		// �L�[�{�[�h�ւ̃A�N�Z�X�����l���i���͐���J�n�j
		//----------------------------------------------
		g_pDIDevKeyboard -> Acquire();
	}
}

//===================================================================
//
// �I������
//
//===================================================================
void UninitKeyboard(void)
{
	// �L�[�{�[�h�p�f�o�C�X�ɉ��������Ă�����
	//---------------------------------------
	if ( g_pDIDevKeyboard != NULL)
	{
		g_pDIDevKeyboard -> Unacquire();	// �L�[�{�[�h�ւ̃A�N�Z�X����j���i���͐���I���j
		g_pDIDevKeyboard -> Release();		// �L�[�{�[�h�p�f�o�C�X�̉��
		g_pDIDevKeyboard = NULL;			// ������
	}

	// DirectInput�C���^�[�t�F�[�X�ɉ��������Ă�����
	//----------------------------------------------
	if (g_pDInput != NULL )
	{
		g_pDInput -> Release();		// DirectInput�C���^�[�t�F�[�X�̉��
		g_pDInput = NULL;			// ������
	}
}

//===================================================================
//
// �L�[�{�[�h���擾�i�v���X�j
//
//===================================================================
bool GetKeyboardPress(int nKey)
{
	return (g_aKeyStatePress[nKey] & 0x80) ? true : false;
}

//===================================================================
//
// �L�[�{�[�h���擾�i�g���K�[�j
//
//===================================================================
bool GetKeyboardTrigger(int nKey)
{
	return (g_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//===================================================================
//
// �L�[�{�[�h���擾�i�����[�X�j
//
//===================================================================
bool GetKeyboardRelease(int nKey)
{
	return (g_aKeyStateRelease[nKey] & 0x80) ? true : false;
}

//===================================================================
//
// �L�[�{�[�h���擾�i���s�[�g�j
//
//===================================================================
bool GetKeyboardRepeat(int nKey)
{
	return (g_aKeyStateRepeat[nKey] & 0x80) ? true : false;
}

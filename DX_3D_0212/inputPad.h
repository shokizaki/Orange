//===================================================================
//
//  ���e�F���͏����ɕK�v�Ȃ���
//  Fail:inputPad.h
//  ����ҁF�؍�@��
//
//===================================================================

//===================================================================
// ��d�C���N���[�h�h�~
//===================================================================
#ifndef _INPUTPAD_H_
#define _INPUTPAD_H_

//===================================================================
//
//  �C���N���[�h�t�@�C��
//
//===================================================================
#include "main.h"

//===================================================================
//
//�@�}�N����`
//
//===================================================================
 
//------ �񋓌^ ------
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
//  �֐��v���g�^�C�v�錾
//
//===================================================================
HRESULT InitGamePad(HINSTANCE hInstance, HWND hWnd);		// ���͏����̏���������
void UpdateGamePad(void);									// ���͏����̍X�V����
void UninitGamePad(void);									// ���͏����̏I������
bool GetGamePadPress( GAMEPAD nKey );						// �v���X���擾
bool GetGamePadTrigger( GAMEPAD nKey );						// �g���K�[���̎擾
bool GetGamePadRelease( GAMEPAD nKey );						// �����[�X���̎擾
bool GetGamePadRepeat( GAMEPAD nKey );						// ���s�[�g���̎擾
int GetGamePadCrossKeyY( void );							// ���s�[�g���̎擾
int GetGamePadCrossKeyX( void );							// ���s�[�g���̎擾
bool GetPadElecomPress( PAD_ELECOM nKey );					// �v���X���擾
bool GetPadElecomTrigger( PAD_ELECOM nKey );				// �g���K�[���̎擾
bool GetPadElecomRelease( PAD_ELECOM nKey );				// �����[�X���̎擾
bool GetPadElecomRepeat( PAD_ELECOM nKey );					// ���s�[�g���̎擾
bool GetPadElecomCrossKeyDownTrigger( void );				// �\���L�[���̎擾
bool GetPadElecomCrossKeyUpTrigger( void );					// �\���L�[���̎擾
bool GetPadElecomCrossKeyRightTrigger( void );					// �\���L�[���̎擾
bool GetPadElecomCrossKeyLeftTrigger( void );					// �\���L�[���̎擾
int GetPadElecomStickRZ( void );							// �\���L�[���̎擾
int GetPadElecomStickRX( void );							// �\���L�[���̎擾

bool GetGamePadUse( void );					// �p�b�h�̓ǂݍ��ݏ��

#endif
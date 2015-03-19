//=============================================================================
//
// ���e�F�T�E���h���� 
// file�Fsound.h
// Author : �؍�@��
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// �T�E���h�t�@�C��
//*****************************************************************************
typedef enum
{
	TITLE_BGM = 0,		// �^�C�g��
	RESULT_BGM,			// ���U���g
	GAME_BGM,			// �Q�[��
	TUTORIAL_BGM,		// �`���[�g���A��
	ABSORB_SE,			// �z��SE
	ONPCOL_SE,			// �����Փ�SE
	SUZU000_SE,			// ��PSE
	SUZU001_SE,			// ��QSE
	CANCEL_SE,			// �L�����Z��SE
	DESIDE_SE,			// ����SE
	DESIDE4_SE,			// ����SSE
	MENUON_SE,			// ���j���[ONSE
	CURSOR_SE,			// �J�[�\��SE

	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif

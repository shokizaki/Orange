//===============================================
//
//  ���e   �F ���[�h�J�ڏ���
//  File   �F mode.cpp
//  ����� �F �؍�@��
//
//===============================================

//------  �C���N���[�h�t�@�C��  ------
#include "mode.h"
#include "title.h"
#include "game.h"
#include "result.h"
#include "tutorial.h"
#include "ranking.h"

//------  �O���[�o���ϐ�  ------
MODE g_mode = MODE_TUTORIAL;
MODE g_nextMode = MODE_TITLE;

//===============================================
//
//  ���[�h�J�ڏ���
//
//===============================================
void ChangeScene( void )
{
	// ���݂̃��[�h�̏I���������s��
	switch ( g_mode )
	{
	case MODE_TITLE:
		UninitTitle();

		break;

	case MODE_TUTORIAL:
		UninitTutorial();

		break;

	case MODE_GAME:
		UninitGame();

		break;

	case MODE_RANKING:
		UninitRanking();

		break;

	case MODE_RESULT:
		UninitResult();

		break;
	}

	// ���̃��[�h�ɐ؂�ւ���
	g_mode = g_nextMode;

	// ����̃��[�h�̏������������s��
	switch ( g_mode )
	{
	case MODE_TITLE:
		InitTitle();

		break;

	case MODE_TUTORIAL:
		InitTutorial();

		break;

	case MODE_GAME:
		InitGame();

		break;

	case MODE_RANKING:
		InitRanking();

		break;

	case MODE_RESULT:
		InitResult();

		break;
	}

	// �t�F�[�h�C��
	SetFade( FADE_IN );
}

//===============================================
//
//  ���݂̃��[�h���擾
//
//===============================================
MODE GetMode( void )
{
	return g_mode;
}

//===============================================
//
//  ���[�h�Z�b�g����
//
//===============================================
void SetMode( MODE modeSet )
{
	g_nextMode = modeSet;
}
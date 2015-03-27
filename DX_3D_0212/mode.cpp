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

//------  �O���[�o���ϐ�  ------
MODE g_mode = MODE_TITLE;
MODE g_nextMode = MODE_GAME;

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
		//UninitTutorial();

		break;

	case MODE_GAME:
		UninitGame();

		break;

	case MODE_BATTLE:
		//UninitBattle();

		break;

	case MODE_RANKING:
		//UninitRanking();

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
		//InitTutorial();

		break;

	case MODE_GAME:
		InitGame();

		break;

	case MODE_BATTLE:
		//InitBattle();

		break;

	case MODE_RANKING:
		//InitRanking();

		break;

	case MODE_RESULT:
		InitResult();

		break;
	}

	// �t�F�[�h�C��
	SetFade( FADE_IN, 60 );
}

//===============================================
//
//  ���[�h�I������
//
//===============================================
void UninitMode( void )
{
	// ���݂̃��[�h�̏I���������s��
	switch ( g_mode )
	{
	case MODE_TITLE:
		UninitTitle();

		break;

	case MODE_TUTORIAL:
		//UninitTutorial();

		break;

	case MODE_GAME:
		UninitGame();

		break;

	case MODE_BATTLE:
		//UninitBattle();

		break;

	case MODE_RANKING:
		//UninitRanking();

		break;

	case MODE_RESULT:
		UninitResult();

		break;
	}
}

//===============================================
//
//  ���[�h�X�V����
//
//===============================================
void UpdateMode( void )
{
	switch ( g_mode )
	{
	case MODE_TITLE:
		UpdateTitle();

		break;

	case MODE_TUTORIAL:
		//UpdateTutorial();

		break;

	case MODE_GAME:
		UpdateGame();

		break;

	case MODE_BATTLE:
		//UpdateBattle();

		break;

	case MODE_RANKING:
		//UpdateRanking();

		break;

	case MODE_RESULT:
		UpdateResult();

		break;
	}
}

//===============================================
//
//  ���[�h�`�揈��
//
//===============================================
void DrawMode( void )
{
	switch ( GetMode() )
	{
	case MODE_TITLE:
		PrintDebugProc("MODE_TITLE\n");
		DrawTitle();

		break;

	case MODE_TUTORIAL:
		PrintDebugProc("MODE_TUTORIAL\n");
		//DrawTutorial();

		break;

	case MODE_GAME:
		PrintDebugProc("MODE_GAME\n");
		DrawGame();

		break;

	case MODE_BATTLE:
		//DrawBattle();

		break;

	case MODE_RANKING:
		PrintDebugProc("MODE_RANKING\n");
		//DrawRanking();

		break;

	case MODE_RESULT:
		PrintDebugProc("MODE_RESULT\n");
		DrawResult();

		break;
	}
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
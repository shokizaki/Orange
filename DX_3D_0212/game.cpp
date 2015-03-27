//-----------------------------------------------
//
//  ���e�F�Q�[������
//  Fail:game.cpp
//  ����ҁF�؍�@��
//
//-----------------------------------------------

//-----------------------------------------------
//  �C���N���[�h�t�@�C��
//-----------------------------------------------
#include "game.h"
#include "player.h"
#include "meshField.h"
#include "camera.h"
#include "light.h"
#include "cube.h"
#include "moveCube.h"
#include "gear.h"
#include "Edit.h"
#include "pause.h"
#include "billboard.h"
#include "sheet.h"
#include "background.h"
#include "timer.h"
#include "goal.h"

//-----------------------------------------------
//�@�}�N����`
//-----------------------------------------------

//-----------------------------------------------
//  �֐��v���g�^�C�v�錾
//-----------------------------------------------

//-----------------------------------------------
//  �O���[�o���ϐ�
//-----------------------------------------------
bool g_bMenu = false;
bool g_bUnintMenu = false;

// �Q�[��������
void InitGame()
{
	// �O���[�o���ϐ�������
	//------------------------------------
	g_bMenu = false;
	g_bUnintMenu = false;

	// 
	//InitEdit();

	// �r���{�[�h������
	InitBillboard();

	// �w�i������
	InitBackGround();

	// ���L���[�u������
	InitCube();

	// �ԃL���[�u������
	InitMoveCube();

	// ���ԏ�����
	InitGear();

	InitGoal();

	// �J����������
	InitCamera();

	// �v���C���[������
	InitPlayer();

	// ���b�V���t�B�[���h������
	InitMeshField( 20, 10, 200.0f, 200.0f );

	// ���C�g������
	InitLight();

	// �|�[�Y������
	InitPause();

	// �V�[�g������
	InitSheet();

	// �^�C�}�[������
	InitTimer();

	// ���j���[�̏�����
	//------------------------------------
	//InitMenu();

	if ( GetMode() == MODE_GAME )
	{
		PlaySound( GAME_BGM );
	}
}

// �Q�[���I������
void UninitGame()
{
	// ���b�V���t�B�[���h�I������
	//------------------------------------
	UninitMeshField();

	// �J�����̏I��
	//------------------------------------
	UninitCamera();

	// ���C�g�̏I��
	//------------------------------------
	UninitLight();

	// ���L���[�u�I��
	UninitCube();

	// �ԃL���[�u�I��
	UninitMoveCube();

	// ���ԏ��I��
	UninitGear();

	UninitGoal();

	// �|�[�Y�I��
	UninitPause();

	// �r���{�[�h������
	UninitBillboard();

	// �V�[�g�I��
	UninitSheet();

	// �w�i������
	UninitBackGround();

	// �^�C�}�[������
	UninitTimer();
	
	// ���j���[�̏I��
	//------------------------------------
	//UninitMenu();

	StopSound();
}

// �Q�[���X�V����
void UpdateGame()
{
	if ( GetKeyboardTrigger( DIK_P ) == true && g_bMenu == false )
	{
		g_bMenu = true;
	}
	else if ( GetKeyboardTrigger( DIK_P ) == true && g_bMenu == true )
	{
		g_bMenu = false;
	}

	if ( g_bMenu == true )
	{
		// �|�[�Y�X�V
		UpdatePause();

		return;
	}

	// �G�f�B�b�g�X�V
	UpdateEdit();

	// �J�����̍X�V
	//------------------------------------
	UpdateCamera();

	// ���b�V���t�B�[���h�X�V����
	//------------------------------------
	UpdateMeshField();

	// ���L���[�u�X�V
	UpdateCube();

	// �ԃL���[�u�X�V
	UpdateMoveCube();

	// ���ԍX�V
	UpdateGear();

	// �v���C���[�̍X�V
	//------------------------------------
	UpdatePlayer();

	// �r���{�[�h������
	UpdateBillboard();

	// ���C�g�̍X�V
	//------------------------------------
	UpdateLight();

	// �V�[�g�X�V
	UpdateSheet();

	UpdateGoal();

	// �w�i������
	UpdateBackGround();

	// �^�C�}�[������
	UpdateTimer();

	// �J�ڔ���
	if ( ( GetKeyboardTrigger( DIK_RSHIFT ) == true || GetPadElecomTrigger( PAD_10 ) ) && GetFade() == FADE_NONE )
	{
		SetFade( FADE_OUT, 60 );
		SetMode( MODE_RESULT );
	}
}

// �Q�[���`�揈��
void DrawGame()
{
	// �w�i������
	DrawBackGround();

	// �G�f�B�b�g�`��
	DrawEdit();

	// �v���C���[�̕`��
	//------------------------------------
	DrawPlayer();

	// ���L���[�u�`��
	DrawCube();

	// �ԃL���[�u�`��
	DrawMoveCube();

	// ���ԕ`��
	DrawGear();

	DrawGoal();

	// ���b�V���t�B�[���h�`�揈��
	//------------------------------------
	//DrawMeshField();

	// ���C�g�̕`��
	//------------------------------------
	DrawLight();

	// �r���{�[�h������
	DrawBillboard();

	// �V�[�g�`��
	DrawSheet();

	// �^�C�}�[������
	//DrawTimer();

	// UI�̏�����
	//------------------------------------
	//DrawUI();

	if ( g_bMenu == true )
	{
		// �|�[�Y�X�V
		DrawPause();
	}
}

//===============================================
// �Q�[���I�[�o�[�̏��������֐�
//===============================================
void GameOver()
{
	
}

//===============================================
// ���j���[��Ԏ擾
//===============================================
bool GetMenu( void )
{
	return g_bMenu;
}	

//===============================================
// ���j���[��ԃZ�b�g
//===============================================
void SetMenu( void )
{
	g_bMenu = false;
}

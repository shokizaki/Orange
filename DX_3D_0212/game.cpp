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
#include "background.h"

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

	//�@�w�i������
	InitBackGround();

	// 
	//InitEdit();

	// ���L���[�u������
	InitCube();

	// �ԃL���[�u������
	InitMoveCube();

	// ���ԏ�����
	InitGear();

	// �J����������
	InitCamera();

	// �v���C���[������
	InitPlayer();

	// ���b�V���t�B�[���h������
	InitMeshField( 5, 5, 200.0f, 200.0f );

	// ���C�g������
	InitLight();

	// ���j���[�̏�����
	//------------------------------------
	//InitMenu();

	if ( GetMode() == MODE_GAME )
	{
		//PlaySound( GAME_BGM );
	}
}

// �Q�[���I������
void UninitGame()
{
	//�@�w�i�I��
	UninitBackGround();

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
	
	// ���j���[�̏I��
	//------------------------------------
	//UninitMenu();

	//StopSound();
}

// �Q�[���X�V����
void UpdateGame()
{
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

	// ���C�g�̍X�V
	//------------------------------------
	UpdateLight();

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

	// ���b�V���t�B�[���h�`�揈��
	//------------------------------------
	DrawMeshField();

	// ���C�g�̕`��
	//------------------------------------
	DrawLight();

	// UI�̏�����
	//------------------------------------
	//DrawUI();
	//�@�w�i�`��
	DrawBackGround();

	//// ���j���[��ON��������
	//if ( g_bMenu == true )
	//{
	//	// ���j���[�̕`��
	//	//------------------------------------
	//	DrawMenu();
	//}
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
	g_bUnintMenu = false;
}

//-----------------------------------------------
//  ���e�F�^�C�g������
//  Fail:title.cpp
//  ����ҁF�؍�@��
//-----------------------------------------------

//-----------------------------------------------
//  �C���N���[�h�t�@�C��
//-----------------------------------------------
#include "title.h"
#include "main.h" 
#include "input.h"
#include "fade.h"
#include "camera.h"
#include "billboard.h"
#include "titleLogo.h"
#include "background.h"
#include "model.h"
#include "enemy.h"

//-----------------------------------------------
//�@�}�N����`
//-----------------------------------------------

//-----------------------------------------------
//  �֐��v���g�^�C�v�錾
//-----------------------------------------------

//-----------------------------------------------
//�@�}�N����`
//-----------------------------------------------
#define TITLE_MAX  ( 2 )			 // ���_�̐�
#define TEXTURE_MAX ( 2 )			 // �g�p����e�N�X�`���̐�

//-----------------------------------------------
//  �O���[�o���ϐ�
//-----------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureTitle[TEXTURE_MAX];		// �e�N�X�`���ւ̃|�C���^
bool g_bTitleFade = false;

//-----------------------------------------------
// �w�i����������
//-----------------------------------------------
void InitTitle()
{
	g_bTitleFade = false;

	// �J�����̏�����
	//------------------------------------
	InitCamera();

	InitBillboard();

	InitModel();

	InitEnemy();

	InitTitleLogo();

	InitBackground();

	PlaySound( TITLE_BGM );
}

//-----------------------------------------------
// �w�i�I������
//-----------------------------------------------
void UninitTitle()
{
	// �^�C�g���w�i�I������
	//------------------------------------
	UninitTitleLogo();

	UninitBackground();

	UninitModel();

	UninitEnemy();

	// �J�����̏I��
	//------------------------------------
	UninitCamera();

	UninitBillboard();

	StopSound();
}

//-----------------------------------------------
// �w�i�X�V����
//-----------------------------------------------
void UpdateTitle()
{
	// �^�C�g���w�i�X�V����
	//------------------------------------
	UpdateTitleLogo();

	UpdateModelTitle();

	UpdateEnemyTitle();

	//UpdateBackground();

	if ( ( GetKeyboardTrigger(DIK_RETURN) /*|| GetGamePadTrigger( GAMEPAD_START )*/ || GetPadElecomTrigger( PAD_10 ) ) && GetFade() == FADE_NONE)
	{
		PlaySound( DESIDE4_SE );
		SetFade( FADE_OUT );
		SetMode( MODE_TUTORIAL );

		g_bTitleFade = true;
	}
}

//-----------------------------------------------
// �w�i�`�揈��
//-----------------------------------------------
void DrawTitle()
{
	//DrawBackground();

	DrawBillboard();

	DrawModel();

	DrawEnemy();

	DrawTitleLogo();
}

//-----------------------------------------------
// �t�F�[�h�����ǂ���
//-----------------------------------------------
bool GetTitleFade( void )
{
	return g_bTitleFade;
}


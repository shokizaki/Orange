//===============================================
//
//  ���e   �F �`���[�g���A������
//  File   �F tutorial.cpp
//  ����� �F �؍�@��
//
//===============================================

//------ �C���N���[�h�t�@�C�� ------
#include "tutorial.h"
#include "stringDraw.h"
#include "model.h"
#include "meshField.h"
#include "shadow.h"
#include "wall.h"
#include "billboard.h"
#include "camera.h"
#include "light.h"
#include "enemy.h"
#include "object.h"
#include "bullet.h"
#include "UI.h"
#include "sky.h"
#include "game.h"

//------ �O���[�o���ϐ� ------

//===============================================
//
// �I�u�W�F�N�g�Ɋւ��鏉����
//
//===============================================
void InitTutorial(void)
{
	InitStringDraw();

	InitGame();

	PlaySound( TUTORIAL_BGM );
}

//===============================================
//
// �I�u�W�F�N�g�Ɋւ���I������
//
//===============================================
void UninitTutorial(void)
{
	UninitStringDraw();

	UninitGame();

	StopSound();
}

//===============================================
//
// �I�u�W�F�N�g�Ɋւ���X�V����
//
//===============================================
void UpdateTutorial(void)
{
	UpdateStringDraw();

	UpdateGame();
}

//===============================================
//
// �I�u�W�F�N�g�Ɋւ���`�揈��
//
//===============================================
void DrawTutorial(void)
{
	DrawGame();

	DrawStringDraw();
}
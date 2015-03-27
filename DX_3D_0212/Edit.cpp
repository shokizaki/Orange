//-----------------------------------------------
//  ���e�F�G�f�B�b�g����
//  File : Edit.cpp
//  ����ҁF�؍�@��
//-----------------------------------------------

//------ �C���N���[�h�t�@�C�� ------
#include "Edit.h"
#include "cube.h"
#include "gear.h"
#include "moveCube.h"
#include "goal.h"

//------ �}�N����` ------

//------ �O���[�o���ϐ� ------
bool g_bEdit = false;
int g_nEditNumber = 1;

//------ �����֐��錾 ------

//-----------------------------------------------
// �G�f�B�b�g�X�V����
//-----------------------------------------------
void UpdateEdit( void )
{
	// �G�f�B�b�g��Ԃɂ���
	if ( GetKeyboardTrigger( DIK_F3 ) == true && g_bEdit == false )
	{
		// �G�f�B�b�g�J�n
		g_bEdit = true;
	}
	else if ( GetKeyboardTrigger( DIK_F3 ) == true && g_bEdit == true )
	{	
		// �G�f�B�b�g�I��
		g_bEdit = false;

		ZeroCubeRotation();
		ZeroGearRotation();
		ZeroMoveCubeRotation();
		ZeroGoalRotation();
	}

	if ( g_bEdit == true )
	{
		PrintDebugProc("Now Edit\n");
		PrintDebugProc("[ F3�L�[�ŃG�f�B�b�g�I�� ]\n\n");
		PrintDebugProc("[ F2�L�[�ŃL���[�u�̈ʒu��[ CubePosition.txt ]�ɏ������� ]\n");
		PrintDebugProc("[ F4�L�[�ŃL���[�u�̈ʒu��[ GearPosition.txt ]�ɏ������� ]\n");
		PrintDebugProc("[ F5�L�[�ŃL���[�u�̈ʒu��[ MoveCubePosition.txt ]�ɏ������� ]\n");
		PrintDebugProc("[ F6�L�[�ŃS�[���̈ʒu��[ GoalPosition.txt ]�ɏ������� ]\n");
		PrintDebugProc("1 *** �L���[�u�G�f�B�b�g\n");
		PrintDebugProc("2 *** ���ԃG�f�B�b�g\n");
		PrintDebugProc("3 *** �����L���[�u�G�f�B�b�g\n");
		PrintDebugProc("3 *** �S�[���G�f�B�b�g\n\n");
		PrintDebugProc("W *** ��ړ�\n");
		PrintDebugProc("A *** ���ړ�\n");
		PrintDebugProc("S *** ���ړ�\n");
		PrintDebugProc("D *** �E�ړ�\n\n");

		// �G�f�B�b�g������̂�؂�ւ���
		if ( GetKeyboardTrigger( DIK_1 ) == true )
		{
			g_nEditNumber = 1;
			
			ZeroCubeRotation();
			ZeroMoveCubeRotation();
			ZeroGearRotation();
			ZeroGoalRotation();
		}
		if ( GetKeyboardTrigger( DIK_2 ) == true )
		{
			g_nEditNumber = 2;

			ZeroCubeRotation();
			ZeroMoveCubeRotation();
			ZeroGearRotation();
			ZeroGoalRotation();
		}
		if ( GetKeyboardTrigger( DIK_3 ) == true )
		{
			g_nEditNumber = 3;

			ZeroCubeRotation();
			ZeroMoveCubeRotation();
			ZeroGearRotation();
			ZeroGoalRotation();
		}
		if ( GetKeyboardTrigger( DIK_4 ) == true )
		{
			g_nEditNumber = 4;

			ZeroCubeRotation();
			ZeroMoveCubeRotation();
			ZeroGearRotation();
			ZeroGoalRotation();
		}
		
		if ( g_nEditNumber == 1 )
		{
			// �L���[�u�G�f�B�b�g
			EditCube();
		}
		if ( g_nEditNumber == 2 )
		{
			// ���ԃG�f�B�b�g
			EditGear();
		}
		if ( g_nEditNumber == 3 )
		{
			// �����L���[�u�G�f�B�b�g
			EditMoveCube();
		}
		if ( g_nEditNumber == 4 )
		{
			// �S�[���G�f�B�b�g
			EditGoal();
		}
	}
	else
	{
		PrintDebugProc("No Edit\n");
		PrintDebugProc("[ F3�L�[�ŃG�f�B�b�g�J�n ]\n");
	}
}

//-----------------------------------------------
// �G�f�B�b�g�`�揈��
//-----------------------------------------------
void DrawEdit( void )
{

}

//-----------------------------------------------
// �G�f�B�b�g��Ԏ擾
//-----------------------------------------------
bool GetEdit( void )
{
	return g_bEdit;
}
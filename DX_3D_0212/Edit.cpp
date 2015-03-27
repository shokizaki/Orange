//-----------------------------------------------
//  内容：エディット処理
//  File : Edit.cpp
//  制作者：木崎　翔
//-----------------------------------------------

//------ インクルードファイル ------
#include "Edit.h"
#include "cube.h"
#include "gear.h"
#include "moveCube.h"
#include "goal.h"

//------ マクロ定義 ------

//------ グローバル変数 ------
bool g_bEdit = false;
int g_nEditNumber = 1;

//------ 内部関数宣言 ------

//-----------------------------------------------
// エディット更新処理
//-----------------------------------------------
void UpdateEdit( void )
{
	// エディット状態にする
	if ( GetKeyboardTrigger( DIK_F3 ) == true && g_bEdit == false )
	{
		// エディット開始
		g_bEdit = true;
	}
	else if ( GetKeyboardTrigger( DIK_F3 ) == true && g_bEdit == true )
	{	
		// エディット終了
		g_bEdit = false;

		ZeroCubeRotation();
		ZeroGearRotation();
		ZeroMoveCubeRotation();
		ZeroGoalRotation();
	}

	if ( g_bEdit == true )
	{
		PrintDebugProc("Now Edit\n");
		PrintDebugProc("[ F3キーでエディット終了 ]\n\n");
		PrintDebugProc("[ F2キーでキューブの位置を[ CubePosition.txt ]に書き込む ]\n");
		PrintDebugProc("[ F4キーでキューブの位置を[ GearPosition.txt ]に書き込む ]\n");
		PrintDebugProc("[ F5キーでキューブの位置を[ MoveCubePosition.txt ]に書き込む ]\n");
		PrintDebugProc("[ F6キーでゴールの位置を[ GoalPosition.txt ]に書き込む ]\n");
		PrintDebugProc("1 *** キューブエディット\n");
		PrintDebugProc("2 *** 歯車エディット\n");
		PrintDebugProc("3 *** 動くキューブエディット\n");
		PrintDebugProc("3 *** ゴールエディット\n\n");
		PrintDebugProc("W *** 上移動\n");
		PrintDebugProc("A *** 左移動\n");
		PrintDebugProc("S *** 下移動\n");
		PrintDebugProc("D *** 右移動\n\n");

		// エディットするものを切り替える
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
			// キューブエディット
			EditCube();
		}
		if ( g_nEditNumber == 2 )
		{
			// 歯車エディット
			EditGear();
		}
		if ( g_nEditNumber == 3 )
		{
			// 動くキューブエディット
			EditMoveCube();
		}
		if ( g_nEditNumber == 4 )
		{
			// ゴールエディット
			EditGoal();
		}
	}
	else
	{
		PrintDebugProc("No Edit\n");
		PrintDebugProc("[ F3キーでエディット開始 ]\n");
	}
}

//-----------------------------------------------
// エディット描画処理
//-----------------------------------------------
void DrawEdit( void )
{

}

//-----------------------------------------------
// エディット状態取得
//-----------------------------------------------
bool GetEdit( void )
{
	return g_bEdit;
}
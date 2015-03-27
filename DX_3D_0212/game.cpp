//-----------------------------------------------
//
//  内容：ゲーム処理
//  Fail:game.cpp
//  制作者：木崎　翔
//
//-----------------------------------------------

//-----------------------------------------------
//  インクルードファイル
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
//　マクロ定義
//-----------------------------------------------

//-----------------------------------------------
//  関数プロトタイプ宣言
//-----------------------------------------------

//-----------------------------------------------
//  グローバル変数
//-----------------------------------------------
bool g_bMenu = false;
bool g_bUnintMenu = false;

// ゲーム初期化
void InitGame()
{
	// グローバル変数初期化
	//------------------------------------
	g_bMenu = false;
	g_bUnintMenu = false;

	// 
	//InitEdit();

	// ビルボード初期化
	InitBillboard();

	// 背景初期化
	InitBackGround();

	// 白キューブ初期化
	InitCube();

	// 赤キューブ初期化
	InitMoveCube();

	// 歯車初期化
	InitGear();

	InitGoal();

	// カメラ初期化
	InitCamera();

	// プレイヤー初期化
	InitPlayer();

	// メッシュフィールド初期化
	InitMeshField( 20, 10, 200.0f, 200.0f );

	// ライト初期化
	InitLight();

	// ポーズ初期化
	InitPause();

	// シート初期化
	InitSheet();

	// タイマー初期化
	InitTimer();

	// メニューの初期化
	//------------------------------------
	//InitMenu();

	if ( GetMode() == MODE_GAME )
	{
		PlaySound( GAME_BGM );
	}
}

// ゲーム終了処理
void UninitGame()
{
	// メッシュフィールド終了処理
	//------------------------------------
	UninitMeshField();

	// カメラの終了
	//------------------------------------
	UninitCamera();

	// ライトの終了
	//------------------------------------
	UninitLight();

	// 白キューブ終了
	UninitCube();

	// 赤キューブ終了
	UninitMoveCube();

	// 歯車初終了
	UninitGear();

	UninitGoal();

	// ポーズ終了
	UninitPause();

	// ビルボード初期化
	UninitBillboard();

	// シート終了
	UninitSheet();

	// 背景初期化
	UninitBackGround();

	// タイマー初期化
	UninitTimer();
	
	// メニューの終了
	//------------------------------------
	//UninitMenu();

	StopSound();
}

// ゲーム更新処理
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
		// ポーズ更新
		UpdatePause();

		return;
	}

	// エディット更新
	UpdateEdit();

	// カメラの更新
	//------------------------------------
	UpdateCamera();

	// メッシュフィールド更新処理
	//------------------------------------
	UpdateMeshField();

	// 白キューブ更新
	UpdateCube();

	// 赤キューブ更新
	UpdateMoveCube();

	// 歯車更新
	UpdateGear();

	// プレイヤーの更新
	//------------------------------------
	UpdatePlayer();

	// ビルボード初期化
	UpdateBillboard();

	// ライトの更新
	//------------------------------------
	UpdateLight();

	// シート更新
	UpdateSheet();

	UpdateGoal();

	// 背景初期化
	UpdateBackGround();

	// タイマー初期化
	UpdateTimer();

	// 遷移判定
	if ( ( GetKeyboardTrigger( DIK_RSHIFT ) == true || GetPadElecomTrigger( PAD_10 ) ) && GetFade() == FADE_NONE )
	{
		SetFade( FADE_OUT, 60 );
		SetMode( MODE_RESULT );
	}
}

// ゲーム描画処理
void DrawGame()
{
	// 背景初期化
	DrawBackGround();

	// エディット描画
	DrawEdit();

	// プレイヤーの描画
	//------------------------------------
	DrawPlayer();

	// 白キューブ描画
	DrawCube();

	// 赤キューブ描画
	DrawMoveCube();

	// 歯車描画
	DrawGear();

	DrawGoal();

	// メッシュフィールド描画処理
	//------------------------------------
	//DrawMeshField();

	// ライトの描画
	//------------------------------------
	DrawLight();

	// ビルボード初期化
	DrawBillboard();

	// シート描画
	DrawSheet();

	// タイマー初期化
	//DrawTimer();

	// UIの初期化
	//------------------------------------
	//DrawUI();

	if ( g_bMenu == true )
	{
		// ポーズ更新
		DrawPause();
	}
}

//===============================================
// ゲームオーバーの条件処理関数
//===============================================
void GameOver()
{
	
}

//===============================================
// メニュー状態取得
//===============================================
bool GetMenu( void )
{
	return g_bMenu;
}	

//===============================================
// メニュー状態セット
//===============================================
void SetMenu( void )
{
	g_bMenu = false;
}

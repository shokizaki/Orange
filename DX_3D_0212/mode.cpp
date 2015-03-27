//===============================================
//
//  内容   ： モード遷移処理
//  File   ： mode.cpp
//  制作者 ： 木崎　翔
//
//===============================================

//------  インクルードファイル  ------
#include "mode.h"
#include "title.h"
#include "game.h"
#include "result.h"

//------  グローバル変数  ------
MODE g_mode = MODE_TITLE;
MODE g_nextMode = MODE_GAME;

//===============================================
//
//  モード遷移処理
//
//===============================================
void ChangeScene( void )
{
	// 現在のモードの終了処理を行う
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

	// 次のモードに切り替える
	g_mode = g_nextMode;

	// 次回のモードの初期化処理を行う
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

	// フェードイン
	SetFade( FADE_IN, 60 );
}

//===============================================
//
//  モード終了処理
//
//===============================================
void UninitMode( void )
{
	// 現在のモードの終了処理を行う
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
//  モード更新処理
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
//  モード描画処理
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
//  現在のモードを取得
//
//===============================================
MODE GetMode( void )
{
	return g_mode;
}

//===============================================
//
//  モードセット処理
//
//===============================================
void SetMode( MODE modeSet )
{
	g_nextMode = modeSet;
}
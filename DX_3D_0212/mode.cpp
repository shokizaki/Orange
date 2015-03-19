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
#include "tutorial.h"
#include "ranking.h"

//------  グローバル変数  ------
MODE g_mode = MODE_TUTORIAL;
MODE g_nextMode = MODE_TITLE;

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

	// 次のモードに切り替える
	g_mode = g_nextMode;

	// 次回のモードの初期化処理を行う
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

	// フェードイン
	SetFade( FADE_IN );
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
//===============================================
//
//  内容   ： チュートリアル処理
//  File   ： tutorial.cpp
//  制作者 ： 木崎　翔
//
//===============================================

//------ インクルードファイル ------
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

//------ グローバル変数 ------

//===============================================
//
// オブジェクトに関する初期化
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
// オブジェクトに関する終了処理
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
// オブジェクトに関する更新処理
//
//===============================================
void UpdateTutorial(void)
{
	UpdateStringDraw();

	UpdateGame();
}

//===============================================
//
// オブジェクトに関する描画処理
//
//===============================================
void DrawTutorial(void)
{
	DrawGame();

	DrawStringDraw();
}
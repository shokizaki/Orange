//-----------------------------------------------
//  内容：タイトル処理
//  Fail:title.cpp
//  制作者：木崎　翔
//-----------------------------------------------

//-----------------------------------------------
//  インクルードファイル
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
//　マクロ定義
//-----------------------------------------------

//-----------------------------------------------
//  関数プロトタイプ宣言
//-----------------------------------------------

//-----------------------------------------------
//　マクロ定義
//-----------------------------------------------
#define TITLE_MAX  ( 2 )			 // 頂点の数
#define TEXTURE_MAX ( 2 )			 // 使用するテクスチャの数

//-----------------------------------------------
//  グローバル変数
//-----------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle = NULL;		// 頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureTitle[TEXTURE_MAX];		// テクスチャへのポインタ
bool g_bTitleFade = false;

//-----------------------------------------------
// 背景初期化処理
//-----------------------------------------------
void InitTitle()
{
	g_bTitleFade = false;

	// カメラの初期化
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
// 背景終了処理
//-----------------------------------------------
void UninitTitle()
{
	// タイトル背景終了処理
	//------------------------------------
	UninitTitleLogo();

	UninitBackground();

	UninitModel();

	UninitEnemy();

	// カメラの終了
	//------------------------------------
	UninitCamera();

	UninitBillboard();

	StopSound();
}

//-----------------------------------------------
// 背景更新処理
//-----------------------------------------------
void UpdateTitle()
{
	// タイトル背景更新処理
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
// 背景描画処理
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
// フェード中かどうか
//-----------------------------------------------
bool GetTitleFade( void )
{
	return g_bTitleFade;
}


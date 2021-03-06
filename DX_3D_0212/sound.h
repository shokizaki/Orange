//=============================================================================
//
// 内容：サウンド処理 
// file：sound.h
// Author : 木崎　翔
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// サウンドファイル
//*****************************************************************************
typedef enum
{
	ABSORB_SE = 0,			// 吸収SE
	ONPCOL_SE,			// 音符衝突SE
	SUZU000_SE,			// 鈴１SE
	SUZU001_SE,			// 鈴２SE
	CANCEL_SE,			// キャンセルSE
	DESIDE_SE,			// 決定SE
	DESIDE4_SE,			// 決定４SE
	MENUON_SE,			// メニューONSE
	CURSOR_SE,			// カーソルSE

    TITLE_BGM,
    GAME_BGM,
    RESULT_BGM,
    RANKING_BGM,

	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif

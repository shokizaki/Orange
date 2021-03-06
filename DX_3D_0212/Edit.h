//-----------------------------------------------
//  内容：エディット処理
//  File : Edit.h
//  制作者：木崎　翔
//-----------------------------------------------
//------------------------------------
// 二重インクルード防止
//------------------------------------
#ifndef _EDIT_H_
#define _EDIT_H_

//-----------------------------------------------
//  インクルードファイル
//-----------------------------------------------
#include "main.h"

//-----------------------------------------------
//  関数プロトタイプ宣言
//-----------------------------------------------

//-----------------------------------------------
// エディット更新処理
//-----------------------------------------------
void UpdateEdit( void );

//-----------------------------------------------
// エディット描画処理
//-----------------------------------------------
void DrawEdit( void );

//-----------------------------------------------
// エディット状態取得
//-----------------------------------------------
bool GetEdit( void );

#endif
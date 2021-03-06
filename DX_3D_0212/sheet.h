//===============================================
//
//  内容   ： シート処理
//  File   ： sheet.h
//  制作者 ： 木崎　翔
//
//===============================================

//===============================================
// 
// 二重インクルード防止
// 
//===============================================
#ifndef _SHEET_H_
#define _SHEET_H_

//===============================================
//
//  インクルードファイル
//
//===============================================
#include "main.h"

//===============================================
//
// マクロ定義
//
//===============================================
#define SHEET_VERTEX ( 4 )

//===============================================
//
// 構造体定義
//
//===============================================

//===============================================
//
//  関数プロトタイプ宣言
//
//===============================================

//===============================================
//
// ライトに関する初期化
//
//===============================================
void InitSheet(void);		

//===============================================
//
// ライトに関する終了処理
//
//===============================================
void UninitSheet(void);	

//===============================================
//
// ライトに関する更新処理
//
//===============================================
void UpdateSheet(void);

//===============================================
//
// ライトに関する描画処理
//
//===============================================
void DrawSheet(void);

//===============================================
//
// ライト情報取得
//
//===============================================
bool GetSheet(void);
#endif


//===============================================
//
//  内容   ： デバッグ表示処理
//  File   ： debugproc.h
//  制作者 ： 木崎　翔
//
//===============================================

//===============================================
// 
// 二重インクルード防止
// 
//===============================================
#ifndef _DEBUGPROC_H_
#define _DEBUGPROC_H_

//===============================================
//
//  インクルードファイル
//
//===============================================
#include "main.h"

void InitDebugProc(void);
void UninitDebugProc(void);
void UpdateDebugProc(void);
void DrawDebugProc(void);
void PrintDebugProc(char *fmt, ...);

#endif
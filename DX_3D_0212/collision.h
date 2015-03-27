//-----------------------------------------------
//  内容：当たり判定
//  Fail:collision.h
//  制作者：木崎　翔
//-----------------------------------------------

//------------------------------------
// 二重インクルード防止
//------------------------------------
#ifndef _COLLISION_H_
#define _COLLISION_H_

//-----------------------------------------------
//  インクルードファイル
//-----------------------------------------------
#include "main.h"

// 当たり判定用の構造体
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 harfSize;
}COL_RECT;

//-----------------------------------------------
//  関数プロトタイプ宣言
//-----------------------------------------------

//-----------------------------------------------
// オブジェクト同士の当たり判定
// 引数：
//  COL_RECT型の変数 * 2
//-----------------------------------------------
bool ColRectXY( COL_RECT *a, COL_RECT *b );

#endif
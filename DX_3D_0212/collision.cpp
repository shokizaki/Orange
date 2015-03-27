//-----------------------------------------------
//  内容：当たり判定
//  Fail:collision.cpp
//  制作者：木崎　翔
//-----------------------------------------------

//-----------------------------------------------
//  インクルードファイル
//-----------------------------------------------
#include "collision.h"

//-----------------------------------------------
// オブジェクト同士の当たり判定
// 引数：
//  COL_RECT型の変数 * 2
//-----------------------------------------------
bool ColRectXY( COL_RECT *a, COL_RECT *b )
{
	if ( a ->pos.x + a ->harfSize.x >= b ->pos.x - b ->harfSize.x
	&&	 a ->pos.x - a ->harfSize.x <= b ->pos.x + b ->harfSize.x
	&&	 a ->pos.y + a ->harfSize.y >= b ->pos.y - b ->harfSize.y
	&&	 a ->pos.y - a ->harfSize.y <= b ->pos.y + b ->harfSize.y )
	{
		return true;
	}

	return false;
}
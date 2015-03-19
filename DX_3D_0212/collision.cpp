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
// 円と円の当たり判定
//-----------------------------------------------
bool ColCircle( COL_RECT *a, COL_RECT *b )
{
	// 二つの座標（x,y）の距離（絶対値）を計算
	//--------------------------------------------------
	float fLengthX = (a ->pos.x - b ->pos.x);	// X座標の距離
	float fLengthY = (a ->pos.z - b ->pos.z);	// Y座標の距離

	// 円と円の距離を計算
	//------------------------------------
	float fLength  = sqrtf( ( fLengthX * fLengthX ) + ( fLengthY * fLengthY ) );

	float aRad = sqrtf( ( a ->harfSize.x * a ->harfSize.x ) + ( a ->harfSize.z * a ->harfSize.z ) );
	float bRad = sqrtf( ( b ->harfSize.x * b ->harfSize.x ) + ( b ->harfSize.z * b ->harfSize.z ) );

	// 当たり判定
	//------------------------------------
	if ( fLength <= aRad + bRad )
	{
		// 円と円の距離が、二つの円の半径を足したものより小さかったら当たり
		//------------------------------------------------------------------
		return true;
	}
	else
	{
		// 当たっていない
		//-----------------------------------
		return false;	
	}
}

//-----------------------------------------------
// 矩形と矩形の当たり判定
// 引数：
// 中心座標x,y * 2、幅１、高さ１、幅２、高さ２
//-----------------------------------------------
bool ColRect( COL_RECT *a, COL_RECT *b )
{
	// 当たり判定
	if (   (a ->pos.x - a->harfSize.x) <= (b ->pos.x + b ->harfSize.x) && (a ->pos.z - a ->harfSize.z) <= (b ->pos.z + b ->harfSize.z)
		&& (a ->pos.x + a->harfSize.x) >= (b ->pos.x - b ->harfSize.x) && (a ->pos.z - a ->harfSize.z) <= (b ->pos.z + b ->harfSize.z)  
		&& (a ->pos.x - a->harfSize.x) <= (b ->pos.x + b ->harfSize.x) && (a ->pos.z + a ->harfSize.z) >= (b ->pos.z - b ->harfSize.z)  
		&& (a ->pos.x + a->harfSize.x) >= (b ->pos.x - b ->harfSize.x) && (a ->pos.z + a ->harfSize.z) >= (b ->pos.z - b ->harfSize.z) )
	{
		return true;	//当たってる
	}
	//else
	{
		return false;	// 当たってない
	}
}

//-----------------------------------------------
// 矩形と矩形の当たり判定( X座標 )
// 引数：
// 中心座標x,y * 2、幅１、高さ１、幅２、高さ２
//-----------------------------------------------
bool ColRectX_Right( COL_RECT *a, COL_RECT *b )
{
	if ( ( a ->pos.x + a ->harfSize.x ) >= ( b ->pos.x - b ->harfSize.x ) )
	{
		return true;
	}

	return false;
}

//-----------------------------------------------
// 矩形と矩形の当たり判定
// 引数：
// 中心座標x,y * 2、幅１、高さ１、幅２、高さ２
//-----------------------------------------------
bool ColRectX_Left( COL_RECT *a, COL_RECT *b )
{
	if ( ( a ->pos.x - a ->harfSize.x ) <= ( b ->pos.x + b ->harfSize.x ) )
	{
		return true;
	}

	return false;
}

//-----------------------------------------------
// 矩形と矩形の当たり判定
// 引数：
// 中心座標x,y * 2、幅１、高さ１、幅２、高さ２
//-----------------------------------------------
bool ColRectZ_Depth( COL_RECT *a, COL_RECT *b )
{
	if ( ( a ->pos.z + a ->harfSize.z ) >= ( b ->pos.z - b ->harfSize.z ) )
	{
		return true;
	}

	return false;
}

//-----------------------------------------------
// 矩形と矩形の当たり判定
// 引数：
// 中心座標x,y * 2、幅１、高さ１、幅２、高さ２
//-----------------------------------------------
bool ColRectZ_Front( COL_RECT *a, COL_RECT *b )
{
	if ( ( a ->pos.z - a ->harfSize.z ) <= ( b ->pos.z + b ->harfSize.z ) )
	{
		return true;
	}

	return false;
}

//-----------------------------------------------
// 矩形と矩形の当たり判定( Z座標 )
// 引数：
// 中心座標x,y * 2、幅１、高さ１、幅２、高さ２
//-----------------------------------------------
bool ColRectZ( COL_RECT *a, COL_RECT *b )
{
	if ( (a ->pos.z - a->harfSize.z) <= (b ->pos.z + b ->harfSize.z) && (a ->pos.z + a->harfSize.z) >= (b ->pos.z - b ->harfSize.z)
	&&	 ( ( a ->pos.z > b ->pos.z ) || ( a ->pos.z < b ->pos.z ) ) )
	{
		return true;
	}

	return false;
}
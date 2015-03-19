//-----------------------------------------------
//  内容：数字を描画する処理
//  File：number.h
//  制作者：木崎　翔
//-----------------------------------------------
//------------------------------------
// 二重インクルード防止
//------------------------------------
#ifndef _NUMBER_H_
#define _NUMBER_H_

//-----------------------------------------------
//  インクルードファイル
//-----------------------------------------------
#include "main.h"

//------ 構造体定義 ------
typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuffer;

	D3DXVECTOR3 pos;	// 一番大きい桁の位置

	int nDigit;			// 数字を描画する数
	int nNumber;		// 描画する数字

	float fWidth;		// 一つの数字の幅
	float fHeight;		// 一つの数字の高さ
}NUMBER;

//-----------------------------------------------
//  関数プロトタイプ宣言
//-----------------------------------------------

// 初期化
void InitNumber( void );

// 終了
void UninitNumber( NUMBER *pNumber );

// テクスチャの解放処理
void UninitNumberTexture( void );

// 描画
void DrawNumber( NUMBER *pNumber, int nNumber );

// セット
void SetNumber( NUMBER *pNumber, D3DXVECTOR3 pos, int nDig, float fWidth, float fHeight );


#endif


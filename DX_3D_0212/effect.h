//-----------------------------------------------
//  内容：バレット処理
//  Fail:bullet.h
//  制作者：木崎　翔
//-----------------------------------------------
//------------------------------------
// 二重インクルード防止
//------------------------------------
#ifndef _EFFECT_H_
#define _EFFECT_H_

//-----------------------------------------------
//  インクルードファイル
//-----------------------------------------------
#include "main.h"

//------ 列挙型定義 ------
typedef enum
{
	EFFECT_000 = 0,
	EFFECT_001,
	EFFECT_002,
	EFFECT_MAX,
}EFFECT_TEX;

//-----------------------------------------------
// 構造体定義
//-----------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;
	int nLife;
	float fWidth;
	float fHeight;
	float r;
	float g;
	float b;
	float a;
	bool bUse;
	bool bCol;

	EFFECT_TEX effectTex;
}EFFECT;

// マクロ
#define EFFECTEX_MAX (2000)		// 弾の最大数


//-----------------------------------------------
//  関数プロトタイプ宣言
//-----------------------------------------------
void InitEffectEx(void);		// バレットに関する初期化
void UninitEffectEx(void);	// バレットに関する終了処理
void UpdateEffectEx(void);	// バレットに関する更新処理
void DrawEffectEx(void);		// バレットに関する描画処理
void SetEffectEx( D3DXVECTOR3 pos, int nLife, float fWidth, float fHeight, EFFECT_TEX tex );	// バレットのセット
void SetupEffectEx(void);	// バレットの頂点情報セット
EFFECT *GetEffectEx();

#endif


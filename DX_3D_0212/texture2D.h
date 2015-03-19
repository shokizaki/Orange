//===============================================
//
//  内容   ： ユーザーインターフェース処理
//  File   ： Texture2D.h
//  制作者 ： 木崎　翔
//
//===============================================

//===============================================
// 
// 二重インクルード防止
// 
//===============================================
#ifndef _TEXTURE2D_H_
#define _TEXTURE2D_H_

//------ インクルードファイル ------
#include "main.h"

//------ マクロ定義 ------
#define TEXTURE2D_MAX ( 10 )

//------ 列挙型定義 ------
typedef enum
{
	TEXTURE2DTYPE_000 = 0,
	TEXTURE2DTYPE_001,
	TEXTURE2DTYPE_HASTY2,
	TEXTURE2DTYPE_BOX,
	TEXTURE2DTYPE_BOXREVERS,
	TEXTURE2DTYPE_MAX
}TEXTURE2DTYPE;

//------ 構造体定義 ------
typedef struct
{
	D3DXVECTOR3 pos;
	D3DCOLOR color;

	float fWidth;
	float fHeight;
	int nFrame;							// テクスチャの切り替わるスピード設定
	int nFrameCount;					// 切り替わるフレームを数える
	int nTexDevideX;					// テクスチャの分割数X
	int nTexDevideY;					// テクスチャの分割数Y
	int nTexNumX;						// テクスチャの現在の位置
	int nTexNumY;						// テクスチャの現在の位置

	TEXTURE2DTYPE type;

	bool bUse;
	bool bLoop;			// ループ判定
}TEXTURE2D;

//------ 関数プロトタイプ宣言 ------

//===============================================
//
// Texture2Dに関する初期化
//
//===============================================
void InitTexture2D( void );		

//===============================================
//
// Texture2Dに関する終了処理
//
//===============================================
void UninitTexture2D(void);	

//===============================================
//
// Texture2Dに関する更新処理
//
//===============================================
void UpdateTexture2D(void);

//===============================================
//
// Texture2Dに関する描画処理
//
//===============================================
void DrawTexture2D(void);

//===============================================
//
// Texture2Dの頂点情報セット
//
//===============================================
void SetupTexture2D(void);

//===============================================
//
// Texture2Dのセット
//
//===============================================
int SetTexture2D( D3DXVECTOR3 pos, float fWdith, float fHeight, TEXTURE2DTYPE type );

//===============================================
//
// Texture2Dの使用OFF
//
//===============================================
void SetTexture2DUse( int num );


#endif
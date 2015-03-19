//===============================================
//
//  内容   ： ユーザーインターフェース処理
//  File   ： stringDraw.h
//  制作者 ： 木崎　翔
//
//===============================================

//===============================================
// 
// 二重インクルード防止
// 
//===============================================
#ifndef _STRINGDRAW_H_
#define _STRINGDRAW_H_

//------ インクルードファイル ------
#include "main.h"

//------ マクロ定義 ------
#define STRINGDRAW_MAX ( 6 )

//------ 構造体定義 ------
typedef enum
{
	FRAME = 0,
	HELLO,
	MOVEEXE,
	MOVEBUTTON,
	MOVE,
	NEXTEXE,
	GOTOUCH,
	GO,
	ABSORB,
	ATTENTION,
	STEPS,
	ONPEXE,
	TRY,
	BOX,
	RELEASETRY,
	ATTENTIONSPEED,
	LAST,
	MESSAGE_MAX
}MESSAGE;

typedef struct
{
	D3DXVECTOR3 pos;
	D3DCOLOR color;

	float fWidth;
	float fHeight;
}STRINGDRAW;

//------ 関数プロトタイプ宣言 ------

//===============================================
//
// StringDrawに関する初期化
//
//===============================================
void InitStringDraw( void );		

//===============================================
//
// StringDrawに関する終了処理
//
//===============================================
void UninitStringDraw( void );	

//===============================================
//
// StringDrawに関する更新処理
//
//===============================================
void UpdateStringDraw( void );

//===============================================
//
// StringDrawに関する描画処理
//
//===============================================
void DrawStringDraw( void );

//===============================================
//
// StringDrawの頂点情報セット
//
//===============================================
void SetupStringDraw( void );

//===============================================
//
// [ Enterで次へ ]がでているかどうか
//
//===============================================
bool GetEnterFlag( void );

//===============================================
//
// [ Enterで次へ ]の設定
//
//===============================================
void SetEnterFlag( bool bFlag );

//===============================================
//
// 現在のメッセージの番号を取得
//
//===============================================
MESSAGE GetStringMessage( void );

//===============================================
//
// メッセージの番号を設定
//
//===============================================
void SetStringMessage( MESSAGE type );





#endif
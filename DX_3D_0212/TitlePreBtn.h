/*
|   @Brief  : タイトル画面のボタンを押せ表示
|
|   @File   : TitlePreBtn.h
|
|   @Auhtor : Ayumi Yasui
|
|   @Note   : Press 4 or Enter
|
*------------------------------------------------------------------------*/
#ifndef _TITLEPREBTN_H_
#define _TITLEPREBTN_H_

/*--- インクルード ------------------------------------------------------*/

#include "title.h"
#include "main.h" 

/*--- 定数定義 ----------------------------------------------------------*/

/*--- 変数型定義 --------------------------------------------------------*/

/*************************************************************************
|
|   @Brief  : 初期化
|
|   @Param  : なし
|
|   @Return : なし
|
*------------------------------------------------------------------------*/
VOID InitTitlePreBtn( VOID );

/*************************************************************************
|
|   @Brief  : 終了
|
|   @Param  : なし
|
|   @Return : なし
|
*------------------------------------------------------------------------*/
VOID UninitTitlePreBtn( VOID );

/*************************************************************************
|
|   @Brief  : 更新
|
|   @Param  : なし
|
|   @Return : なし
|
*------------------------------------------------------------------------*/
VOID UpdateTitlePreBtn( VOID );

/*************************************************************************
|
|   @Brief  : 描画
|
|   @Param  : なし
|
|   @Return : なし
|
*------------------------------------------------------------------------*/
VOID DrawTitlePreBtn( VOID );


#endif // _TITLEPREBTN_H_
/*************************************************************************
|   End of File
**************************************************************************/
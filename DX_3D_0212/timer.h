#ifndef _TIMER_H_
#define _TIMER_H_

#define TIMER_DIGITS ( 3 )
#define TIMERPOLYGON_VERTEX ( TIMER_DIGITS * 4 )		//頂点数
#define TIMERPOLYGON_WIDTH ( 50 )						//ポリゴンの幅
#define TIMERPOLYGON_HEIGHT ( 30 )						//ポリゴンの高さ
#define TIMERPOLYGON_TEXTURE ( 1 )						//テクスチャ数
#define TIMERTEXTURE_WIDTH ( 1 )						//テクスチャの行枚数
#define TIMERTEXTURE_HEIGHT ( 10 )						//テクスチャの列枚数
#define LIMITTIME ( 500 )									//制限時間

int InitTimer(void);									//ポリゴンの初期化処理
void UninitTimer(void);									//ポリゴンの終了処理
int UpdateTimer(void);									//ポリゴンの更新処理
void DrawTimer(void);									//ポリゴン描画処理
int GetTimer(void);
#endif
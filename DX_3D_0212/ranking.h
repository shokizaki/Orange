#ifndef _RANKING_H_
#define _RANKING_H_

#define MAX_RANKING ( 5 )								//順位数
#define RANKING_POLYGON ( 4 )							//スコア桁数
#define MAX_RANK ( 4 )									//順位ポリゴン用
#define RANKINGPOLYGON_VERTEX ( (RANKING_POLYGON * 4) * MAX_RANKING  + (MAX_RANK * 4))		//頂点数
#define RANKINGPOLYGON_WIDTH ( 80 )						//ポリゴンの幅
#define RANKINGPOLYGON_HEIGHT ( 50 )					//ポリゴンの高さ
#define RANKPOLYGON_WIDTH ( 150 )						//ポリゴンの幅
#define RANKPOLYGON_HEIGHT ( 60 )						//ポリゴンの高さ
#define RANKINGPOLYGON_TEXTURE ( 5 )					//テクスチャ数
#define RANKINGTEXTURE_WIDTH ( 1 )						//テクスチャの行枚数
#define RANKINGTEXTURE_HEIGHT ( 10 )					//テクスチャの列枚数
#define NEXTRANKSPACE ( 130 )							//次位までの空間
#define STARTPOINTX ( 100 )								//ランキングX描画開始位置
#define STARTPOINTY ( 70 )								//ランキングY描画開始位置

int InitRanking(void);									//ポリゴンの初期化処理
void UninitRanking(void);								//ポリゴンの終了処理
void UpdateRanking(void);								//ポリゴンの更新処理
void DrawRanking(void);									//ポリゴン描画処理

#endif
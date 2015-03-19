//===============================================
//
//  内容   ： モーション処理
//  File   ： motion.h
//  制作者 ： 木崎　翔
//
//===============================================

//===============================================
// 
// 二重インクルード防止
// 
//===============================================
#ifndef _MOTION_H_
#define _MOTION_H_

//===============================================
//
//  インクルードファイル
//
//===============================================
#include "main.h"

//===============================================
//
// マクロ定義
//
//===============================================
#define PARTS_NUM	( 20 )
#define MOTION_NUM	( 5 )
#define KEY_NUM		( 16 )

//===============================================
//
// 構造体定義
//
//===============================================
typedef enum
{
	NEUTRAL = 0,
	WALK,
	ACTION,
	JUMP,
	RANDING,
	MAX
}MOTION_STATUS;

typedef enum
{
	DIRECTION_DEPTH = 0,
	DIRECTION_FRONT,
	DIRECTION_RIGHT,
	DIRECTION_LEFT,
	DIRECTION_MAX
}DIRECTION_MODEL;

// モーション情報
typedef struct
{	
	int nFrame[ KEY_NUM ];	// 切り替えるフレーム
	int nNumKey;			// キー数（最大数）
	bool bLoop;				// ループするかどうか
}MOTIONINFO;

// キー情報
typedef struct
{
	D3DXVECTOR3 pos[ PARTS_NUM ][ KEY_NUM ];		// 目標となる数値
	D3DXVECTOR3 rot[ PARTS_NUM ][ KEY_NUM ];		// 目標となる数値
}KEY;

// モデルの情報（１キャラクターにつき一つでok）
// マクロ定義で最大のパーツ数とかは変えられるよ。
// 使う人によってはいらないものも入っちゃってるけどそこはお任せで。
typedef struct
{
	// キャラクター / パーツ用
	char modelFileName[ PARTS_NUM ][ 256 ];	// ファイル名
	float fMove;							// 移動量
	float fJumpVal;							// ジャンプ量
	float fGravity;							// 重力量
	int nNumParts;							// パーツ数
	D3DXVECTOR3 posParts[ PARTS_NUM ];		// 位置（基本となる位置（動かすもの））
	D3DXVECTOR3 rotParts[ PARTS_NUM ];		// 角度（基本となる位置（動かすもの））
	D3DXVECTOR3 scl[ PARTS_NUM ];			// 大きさ
	D3DXVECTOR3 targetRotParts;				// 目標角度
	int nIndex[ PARTS_NUM ];				// インデックス
	int nParent[ PARTS_NUM ];				// 親のインデックス
	int nKey;								// 現在のキー数（ループ用変数）
	D3DXVECTOR3 posOld;						// 位置保存用
	D3DXVECTOR3 targetRot[ PARTS_NUM ];		// 向きの目標値（モーション用）
	D3DXVECTOR3 targetPos[ PARTS_NUM ];		// 向きの目標値（モーション用）
	D3DXVECTOR3 move;						// 移動量

	D3DXMATRIX mtxWorld[ PARTS_NUM ];			// ワールドマトリックス
	LPD3DXMESH pMeshModel[ PARTS_NUM ];			// メッシュ情報へのポインタ
	LPD3DXBUFFER pBuffMatModel[ PARTS_NUM ];	// マテリアル情報へのポインタ
	DWORD numMatModel[ PARTS_NUM ];				// マテリアルの数

	MOTIONINFO aMotion[ MOTION_NUM ];			// モーションの情報
	KEY aKeyInfo[ MOTION_NUM ];					// キーの情報

	int nFrameCount;						// フレームを数える変数
	int nBillboardNum;						// ビルボードのインデックス
	int nShadowNum;							// 影のインデックス
	int nMoveCnt;							// 移動するフレームカウンタ（敵用）

	D3DXVECTOR3 rotDevide[ PARTS_NUM ];		// １フレームに増加する角度量（モーション用）
	D3DXVECTOR3 posDevide[ PARTS_NUM ];		// １フレームに増加する位置量（モーション用）

	MOTION_STATUS status;				// 現在のモーションの番号
	MOTION_STATUS statusOld;			// 前回のモーションの番号

	DIRECTION_MODEL direction;			// 向いてる向き
	DIRECTION_MODEL directionSecond;	// 向いてる向き

	bool bMove;
	bool bAutoMove;
	bool bMotion;

}MODEL_INFO;

//===============================================
//
//  関数プロトタイプ宣言
//
//===============================================

//===============================================
//
// モデルのモーションに関する初期化
//
//===============================================
void InitModelMotionEx( MODEL_INFO *pModel, char *pFileName );	

//===============================================
//
// モデルのモーションに関する終了
//
//===============================================
void UninitModelMotion( MODEL_INFO *pModel );

//===============================================
//
// モデルのモーションに関する更新
//
//===============================================
void UpdateModelMotion( MODEL_INFO *pModel );

//===============================================
//
// モデルの描画
//
//===============================================
void DrawModelMotion( MODEL_INFO *pModel );



#endif


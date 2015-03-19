//===================================================================
//  内容：インクルードファイルやマクロ定義やライブラリのリンク
//  Fail:main.h
//  制作者：木崎　翔
//===================================================================
#ifndef _MAIN_H_
#define _MAIN_H_

//===================================================================
//
//  インクルードファイル
//
//===================================================================
#include <windows.h>
#include "d3dx9.h"		// 描画処理に必要
#define DIRECTINPUT_VERSION (0x0800)
#include "dinput.h"		// 入力処理に必要
#include "xaudio2.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "debugproc.h"
#include "fade.h"
#include "input.h"
#include "mode.h"
#include "collision.h"
#include "sound.h"
#include "inputPad.h"

//===================================================================
//
// ライブラリのリンク
//
//===================================================================
#pragma comment (lib, "d3d9.lib")		// 描画処理に必要
#pragma comment (lib, "d3dx9.lib")		// 上記の拡張ライブラリ
#pragma comment (lib, "dxguid.lib")		// DirectXコンポーネント使用に必要
#pragma comment (lib, "winmm.lib")		// 
#pragma comment (lib, "dinput8.lib")	// 入力処理に必要

//===================================================================
//
//  マクロ定義
//
//==================================================================
#define SCREEN_WIDTH  (800)			// 画面の幅
#define SCREEN_HEIGHT (600)			// 画面の高さ
#define DEGTOR(degree) (degree * (3.1415 / 180))		// 角度をラジアンに変える計算

// 頂点フォーマット
#define FVF_VERTEX_3D (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)
// 上記フォーマットに合わせた構造体
typedef struct
{
	D3DXVECTOR3 pos;		// 頂点座標
	D3DXVECTOR3 nor;		// 法線
	D3DCOLOR col;			// 頂点カラー
	D3DXVECTOR2 tex;		// テクスチャ座標
}VERTEX_3D;

// 頂点フォーマット（2D）
//-------------------------------------
#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
 // 上記フォーマットに合わせた構造体
typedef struct
{
	D3DXVECTOR3 pos;		// 頂点座標
	float rhw;				// テクスチャのパースペクティブコレクト用
	D3DCOLOR col;			// 頂点カラー
	D3DXVECTOR2 tex;		// テクスチャ座標
}VERTEX_2D;


//===================================================================
//
// プロトタイプ宣言
//
//===================================================================
LPDIRECT3DDEVICE9 GetDevice(void);		// デバイスの取得


#endif
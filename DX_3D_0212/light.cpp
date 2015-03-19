//===============================================
//
//  内容   ： ライトに関する処理
//  File   ： camera.cpp
//  制作者 ： 木崎　翔
//
//===============================================

//===============================================
// 
//  インクルードファイル
//
//===============================================
#include "light.h"
#include "input.h"

//===============================================
//
//　マクロ定義
//
//===============================================
//#define MODEL_TEXTURE_MAX 	(1)			// 使用するテクスチャの数
//#define TEX_PASS		"data/TEXTURE/airsrock.jpg"

//-----------------------------------------------
//  グローバル変数
//-----------------------------------------------
D3DLIGHT9 g_aLight[3];			// ライト情報

//===============================================
//
// ライトに関する初期化
//
//===============================================
void InitLight(void)
{
	// ローカル変数
	//------------------------------------
	LPDIRECT3DDEVICE9 pDevice;		// デバイスオブジェクト
	D3DXVECTOR3 vecDir;				// 計算用（ベクトルの向き）

	// デバイスの取得
	//------------------------------------
	pDevice = GetDevice();

	ZeroMemory(&g_aLight[0], sizeof(D3DLIGHT9));						// 中身を初期化
	g_aLight[0].Type = D3DLIGHT_DIRECTIONAL;							// ライトの種類（平行光源）
	g_aLight[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);			// ライトの色
	vecDir = D3DXVECTOR3(1.0f, 0.0f, 1.0f);								// ライトの向き
	D3DXVec3Normalize( (D3DXVECTOR3*)&g_aLight[0].Direction, &vecDir );	// ライトの向きをノーマライズ
	pDevice ->SetLight(0, &g_aLight[0]);								// ライトの設定
	pDevice ->LightEnable(0, TRUE);										// ライトの有効化

	ZeroMemory(&g_aLight[1], sizeof(D3DLIGHT9));						// 中身を初期化
	g_aLight[1].Type = D3DLIGHT_DIRECTIONAL;							// ライトの種類（平行光源）
	g_aLight[1].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);			// ライトの色
	vecDir = D3DXVECTOR3(0.0f, -1.0f, 0.0f);							// ライトの向き
	D3DXVec3Normalize( (D3DXVECTOR3*)&g_aLight[1].Direction, &vecDir );	// ライトの向きをノーマライズ
	pDevice ->SetLight(1, &g_aLight[1]);								// ライトの設定
	pDevice ->LightEnable(1, TRUE);										// ライトの有効化

	ZeroMemory(&g_aLight[2], sizeof(D3DLIGHT9));						// 中身を初期化
	g_aLight[2].Type = D3DLIGHT_DIRECTIONAL;							// ライトの種類（平行光源）
	g_aLight[2].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);			// ライトの色
	vecDir = D3DXVECTOR3(-1.0f, 0.0f, -1.0f);							// ライトの向き
	D3DXVec3Normalize( (D3DXVECTOR3*)&g_aLight[2].Direction, &vecDir );	// ライトの向きをノーマライズ
	pDevice ->SetLight(2, &g_aLight[2]);								// ライトの設定
	pDevice ->LightEnable(2, TRUE);
}

//===============================================
//
// ライトに関する終了処理
//
//===============================================
void UninitLight(void)
{

}

//===============================================
//
// ライトに関する更新処理
//
//===============================================
void UpdateLight(void)
{
	
}

//===============================================
//
// ライトに関する描画処理
//
//===============================================
void DrawLight(void)
{

}

//===============================================
//
// ライト情報取得
//
//===============================================
//CAMERA *GetLight()
//{
//	return &g_camera;
//}
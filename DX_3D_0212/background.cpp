/*******************************************************************************
*
*	File	:背景表示	[ background.cpp ]
*	Author	:AT12A271 26 keita bannno
*
*******************************************************************************/

/*******************************************************************************
インクルードファイル
*******************************************************************************/
#include "main.h"
#include "background.h"
#include "camera.h"
#include "input.h"

/*******************************************************************************
マクロ宣言
*******************************************************************************/
#define TEXTURE_BACKGROUND		"data/TEXTURE/field001.jpg"		// 背景テクスチャ
#define BG_WIDTH ( 300.0f )
#define BG_HEIGHT ( 300.0f )

/*******************************************************************************
グローバル変数宣言
*******************************************************************************/
BACKGROUND BackGround;

/*******************************************************************************
フィールド初期化
*******************************************************************************/
int InitBackGround(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();

	BackGround.m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	BackGround.m_scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	BackGround.m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//頂点情報の設定
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * BACKGROUND_VERTEX,
		D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &BackGround.m_pVtxBuff, NULL))){
		return E_FAIL;

		VERTEX_3D *pVtx;
		BackGround.m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);	//頂点データをロック

		//頂点座標初期化
		pVtx[0].pos = D3DXVECTOR3( -BG_WIDTH, -BG_HEIGHT, 0.f );
		pVtx[1].pos = D3DXVECTOR3( BG_WIDTH, -BG_HEIGHT, 0.f );
		pVtx[2].pos = D3DXVECTOR3( -BG_WIDTH, BG_HEIGHT, 0.f );
		pVtx[3].pos = D3DXVECTOR3( BG_WIDTH, BG_HEIGHT, 0.f );
		//pVtx[0].pos = D3DXVECTOR3(-15.f, 30.f, 0.f);
		//pVtx[1].pos = D3DXVECTOR3(15.f, 30.f, 0.f);
		//pVtx[2].pos = D3DXVECTOR3(-15.f, 0.f, 0.f);
		//pVtx[3].pos = D3DXVECTOR3(15.f, 0.f, 0.f);

		//頂点ベクトル初期化（いじらない）
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		//頂点カラー初期化
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//頂点テクスチャ初期化
		pVtx[0].tex = D3DXVECTOR2(0, 0);
		pVtx[1].tex = D3DXVECTOR2(1, 0);
		pVtx[2].tex = D3DXVECTOR2(0, 1);
		pVtx[3].tex = D3DXVECTOR2(1, 1);

		//テクスチャの読み込み
		if (FAILED(D3DXCreateTextureFromFile(pDevice, TEXTURE_BACKGROUND, &BackGround.m_pTexture))){
			return E_FAIL;
		}

		BackGround.m_pVtxBuff->Unlock();		//頂点データをアンロック
	}


	return 0;
}

/*******************************************************************************
フィールド終了処理
*******************************************************************************/
void UninitBackGround(void)
{
	//頂点バッファへのポインタの廃棄
	if (BackGround.m_pVtxBuff != NULL)
	{
		BackGround.m_pVtxBuff->Release();
		BackGround.m_pVtxBuff = NULL;
	}
}

/*******************************************************************************
フィールド更新
*******************************************************************************/
void UpdateBackGround(void)
{
}

/*******************************************************************************
フィールド描画
*******************************************************************************/
void DrawBackGround(void)
{
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	CAMERA *pCamera				= GetCamera();					//　カメラ情報を取得
	LPDIRECT3DDEVICE9 pDevice	= GetDevice();		//　デバイスポインタを取得		

	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();

		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&BackGround.m_mtxworld);	//初期化
		D3DXMatrixScaling(&mtxScl, BackGround.m_scl.x, BackGround.m_scl.y, BackGround.m_scl.z);	//スケールを反映
		D3DXMatrixMultiply(&BackGround.m_mtxworld, &BackGround.m_mtxworld, &mtxScl);
	
		D3DXMatrixRotationYawPitchRoll(&mtxRot, BackGround.m_rot.y, BackGround.m_rot.x, Shadow[cnt].m_rotShadow.z);	//向きを反映
		D3DXMatrixMultiply(&Shadow[cnt].m_mtxworld, &Shadow[cnt].m_mtxworld, &mtxRot);
		
		D3DXMatrixTranslation(&mtxTranslate, Shadow[cnt].m_posShadow.x, Shadow[cnt].m_posShadow.y, Shadow[cnt].m_posShadow.z);		//位置を反映
		D3DXMatrixMultiply(&Shadow[cnt].m_mtxworld, &Shadow[cnt].m_mtxworld, &mtxTranslate);
		
		pDevice->SetTransform(D3DTS_WORLD, &Shadow[cnt].m_mtxworld);	//設定

		//ポリゴンの描画設定開始
		pDevice->SetStreamSource(0, Shadow[cnt].m_pVtxBuffShadow, 0, sizeof(VERTEX_3D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		//テクスチャの設定
		pDevice->SetTexture(0, Shadow[0].m_pTextureShadow);

		//描画設定
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);		//αブレンドの設定
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

		//通常の設定に戻す
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);		//αブレンドの設定
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}
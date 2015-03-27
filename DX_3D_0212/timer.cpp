/*******************************************************************************
*
*	File	:タイマーポリゴン表示	[timer.cpp]
*	Author	:AT12A271 26 keita bannno
*
*******************************************************************************/

/*******************************************************************************
インクルードファイル
*******************************************************************************/
#include "main.h"
#include "timer.h"
#include "fade.h"

/*******************************************************************************
マクロ宣言
*******************************************************************************/

/*******************************************************************************
グローバル変数宣言
*******************************************************************************/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTimerPolygon = NULL;				//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9		g_pTimerTexture[TIMERPOLYGON_TEXTURE];		//テクスチャのポインタ
D3DXVECTOR3				g_posTimerPolygon;							//ポリゴンの中心座標
D3DXCOLOR				g_TimerCol;									//　色情報
static float			g_nTimer;									//制限時間
static int				g_nCnt;
static float			r, g, b, a;									//　各色
static int				g_Width;									//　幅
static int				g_Height;									//　高さ
static int				g_ndigits;									//　桁数

/*******************************************************************************
タイマーの初期化処理
*******************************************************************************/
int InitTimer(void){
	MODE pMode;							//モードポインタ
	FADE pFade;							//フェードポインタ
	LPDIRECT3DDEVICE9 pDevice;			//デバイスポインタ
	int nTimer = 0;			//各桁のスコア
	g_ndigits = 3;
	double CntNum = pow(10.0, g_ndigits);		//上記に必要
	float TexNumber = 0;				//UV座標
	g_nTimer = 0;
	g_nCnt = 0;

	pDevice = GetDevice();
	pMode = GetMode();					//現在のモードを取得
	pFade = GetFade();					//現在のフェード状態を取得

	//頂点バッファの作成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*TIMERPOLYGON_VERTEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffTimerPolygon, NULL))){
		return E_FAIL;
	}

	//ポリゴンの中心座標( 1160.f, 20.f, 0.f )
	g_posTimerPolygon.x = 450.f;
	g_posTimerPolygon.y = TIMERPOLYGON_HEIGHT * 2;
	g_posTimerPolygon.z = 0.f;
	g_Width = TIMERPOLYGON_WIDTH;
	g_Height = TIMERPOLYGON_HEIGHT;

	//　頂点カラー設定
	r = 1.0f;
	g = 1.0f;
	b = 1.0f;
	a = 1.0f;
	g_TimerCol = D3DXCOLOR( r, g, b, a );

	VERTEX_2D *pVtx;
	g_pVtxBuffTimerPolygon->Lock(0, 0, (void**)&pVtx, 0);

	//頂点データ設定
	for (int i = 0; i < g_ndigits; i++){
		//頂点座標初期化
		pVtx[0].pos.x = (i * g_Width) + g_posTimerPolygon.x;
		pVtx[0].pos.y = g_posTimerPolygon.y - g_Height;
		pVtx[0].pos.z = 0.f;

		pVtx[1].pos.x = (i * g_Width) + (g_posTimerPolygon.x + g_Width);
		pVtx[1].pos.y = g_posTimerPolygon.y - g_Height;
		pVtx[1].pos.z = 0.f;

		pVtx[2].pos.x = (i * g_Width) + g_posTimerPolygon.x;
		pVtx[2].pos.y = g_posTimerPolygon.y + g_Height;
		pVtx[2].pos.z = 0.f;

		pVtx[3].pos.x = (i * g_Width) + (g_posTimerPolygon.x + g_Width);
		pVtx[3].pos.y = g_posTimerPolygon.y + g_Height;
		pVtx[3].pos.z = 0.f;

		//テクスチャのパースペクティブ初期化
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラー初期化
		pVtx[0].col = g_TimerCol;
		pVtx[1].col = g_TimerCol;
		pVtx[2].col = g_TimerCol;
		pVtx[3].col = g_TimerCol;

		//頂点テクスチャ初期化	

		nTimer = ((int)g_nTimer % (int)CntNum) / ((int)CntNum / 10);	//各桁の数字を計算

		//各桁の数値が0以外の時
		if (0 < nTimer && nTimer <= 9){
			TexNumber = (float)nTimer / 10;

			pVtx[0].tex = D3DXVECTOR2(TexNumber, 0);
			pVtx[1].tex = D3DXVECTOR2(TexNumber + 0.1f, 0);
			pVtx[2].tex = D3DXVECTOR2(TexNumber, 1);
			pVtx[3].tex = D3DXVECTOR2(TexNumber + 0.1f, 1);
		}
		else{//各桁が0の時
			pVtx[0].tex = D3DXVECTOR2(0, 0);
			pVtx[1].tex = D3DXVECTOR2(0.1f, 0);
			pVtx[2].tex = D3DXVECTOR2(0, 1);
			pVtx[3].tex = D3DXVECTOR2(0.1f, 1);
		}
		pVtx += 4;								//1ポリゴン分のアドレスをずらす
		CntNum /= 10;
	}

	//テクスチャのポインタ初期化
	for (int i = 0; i < TIMERPOLYGON_TEXTURE; i++){
		g_pTimerTexture[i] = NULL;
	}

	//テクスチャファイルの読み込み
	if (FAILED(D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/number000.png", &g_pTimerTexture[0]))){
		return E_FAIL;
	}

	g_pVtxBuffTimerPolygon->Unlock();

	return 0;
}

/*******************************************************************************
タイマーの終了処理
*******************************************************************************/
void UninitTimer(void){
	//テクスチャポインタの廃棄
	for (int i = 0; i < TIMERPOLYGON_TEXTURE; i++){
		if (g_pTimerTexture[i] != NULL){
			g_pTimerTexture[i]->Release();
			g_pTimerTexture[i] = NULL;
		}
	}

	//頂点バッファへのポインタの廃棄
	if (g_pVtxBuffTimerPolygon != NULL){
		g_pVtxBuffTimerPolygon->Release();
		g_pVtxBuffTimerPolygon = NULL;
	}
}

/*******************************************************************************
タイマーの更新処理
*******************************************************************************/
int UpdateTimer(void){
	int nTimer = 0;							//各桁のスコア
	int nDigits = 0;
	double CntNum;			//上記に必要
	float TexNumber = 0;					//UV座標

	VERTEX_2D *pVtx;
	g_pVtxBuffTimerPolygon->Lock(0, 0, (void**)&pVtx, 0);

	g_nCnt++;
	if (g_nCnt % 60 == 0)
	{		//　60フレームにつき1へらす
		g_nTimer++;

	}

		//　現在の桁数を調べる
	if( g_nTimer >= 100  )
	{
		nDigits = 3;
	}
	else if(g_nTimer >= 10 )
	{
		nDigits = 2;
	}
	else if( g_nTimer >= 0 )
	{
		nDigits =1;
	}

	CntNum = pow(10.0, nDigits);


	//頂点データ設定
	for (int i = 0; i < nDigits; i++)
	{
		//頂点座標初期化
		pVtx[0].pos.x = (i * g_Width) + g_posTimerPolygon.x;
		pVtx[0].pos.y = g_posTimerPolygon.y - g_Height;
		pVtx[0].pos.z = 0.f;

		pVtx[1].pos.x = (i * g_Width) + (g_posTimerPolygon.x + g_Width);
		pVtx[1].pos.y = g_posTimerPolygon.y - g_Height;
		pVtx[1].pos.z = 0.f;

		pVtx[2].pos.x = (i * g_Width) + g_posTimerPolygon.x;
		pVtx[2].pos.y = g_posTimerPolygon.y + g_Height;
		pVtx[2].pos.z = 0.f;

		pVtx[3].pos.x = (i * g_Width) + (g_posTimerPolygon.x + g_Width);
		pVtx[3].pos.y = g_posTimerPolygon.y + g_Height;
		pVtx[3].pos.z = 0.f;

		nTimer = ((int)g_nTimer % (int)CntNum) / ((int)CntNum / 10);

		//各桁の数値が0以外の時
		if (0 < nTimer && nTimer <= 9)
		{
			TexNumber = (float)nTimer / 10;

			pVtx[0].tex = D3DXVECTOR2(TexNumber, 0);
			pVtx[1].tex = D3DXVECTOR2(TexNumber + 0.1f, 0);
			pVtx[2].tex = D3DXVECTOR2(TexNumber, 1);
			pVtx[3].tex = D3DXVECTOR2(TexNumber + 0.1f, 1);
		}
		else{//各桁が0の時
			pVtx[0].tex = D3DXVECTOR2(0, 0);
			pVtx[1].tex = D3DXVECTOR2(0.1f, 0);
			pVtx[2].tex = D3DXVECTOR2(0, 1);
			pVtx[3].tex = D3DXVECTOR2(0.1f, 1);
		}
	
		//　色更新
		pVtx[0].col = g_TimerCol;
		pVtx[1].col = g_TimerCol;
		pVtx[2].col = g_TimerCol;
		pVtx[3].col = g_TimerCol;

		pVtx += 4;								//1ポリゴン分のアドレスをずらす
		CntNum /= 10;							//1桁減少
	}

	g_pVtxBuffTimerPolygon->Unlock();
	return (int)g_nTimer;
}

/*******************************************************************************
タイマーの描画処理
*******************************************************************************/
void DrawTimer()
{
	int nDigits = 0;

	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();

	pDevice->SetStreamSource(0, g_pVtxBuffTimerPolygon, 0, sizeof(VERTEX_2D));
	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTimerTexture[0]);

	int PrimitiveVertex = 0;

	if( g_nTimer >= 100  )
	{
		nDigits = 3;
	}
	else if(g_nTimer >= 10 )
	{
		nDigits = 2;
	}
	else if( g_nTimer >= 0 )
	{
		nDigits =1;
	}

	for (int i = 0; i < nDigits; i++){
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, PrimitiveVertex, 2);
		PrimitiveVertex += 4;
	}
}

int GetTimer(void)
{
	return (int)g_nTimer;
}
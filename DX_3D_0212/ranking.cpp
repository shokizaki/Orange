/*******************************************************************************
*
*	File	:スコアポリゴン表示	[ranking.cpp]
*	Author	:AT12A271 26 keita bannno
*
*******************************************************************************/

/*******************************************************************************
インクルードファイル
*******************************************************************************/
#include <stdio.h>
#include "main.h"
#include "ranking.h"
#include "timer.h"
#include "RankingBG.h"

/*******************************************************************************
グローバル変数宣言
*******************************************************************************/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRankingPolygon = NULL;				//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9		g_pRankingTexture[RANKINGPOLYGON_TEXTURE];		//テクスチャのポインタ
D3DXVECTOR3 g_posRankingPolygon;										//ポリゴンの中心座標
int g_nRanking[MAX_RANKING];											//スコア履歴
int g_nAnime;
int A;
bool g_bSound = false;													//再生したかどうか
unsigned int g_nRankingFrameCount;

/*******************************************************************************
スコアの初期化処理
*******************************************************************************/
int InitRanking(void){

    g_nRankingFrameCount = 0;

    // BGM
    PlaySound(RANKING_BGM);

	LPDIRECT3DDEVICE9 pDevice;		//デバイスポインタ
	FILE *fp;						//ファイルポインタ
	errno_t error;					//エラーポインタ

	//変数初期化
	pDevice = GetDevice();			//デバイスポインタを取得
	g_bSound = false;

	int pTimer = GetTimer();

	//ランキングをロード
	if (error = fopen_s(&fp, "./data/savedate/hiscore.txt", "r") != 0){
		//エラー処理
	}
	for (int i = 0; i < MAX_RANKING; i++){
		fscanf_s(fp, "%d", &g_nRanking[i]);
	}
	fclose(fp);

	//最新のスコアを取得
	g_nRanking[MAX_RANKING - 1 ] = pTimer;

	//頂点バッファの作成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*RANKINGPOLYGON_VERTEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffRankingPolygon, NULL))){
		return E_FAIL;
	}

	//ポリゴンの中心座標( 300.f, 70.f, 0.f )
	g_posRankingPolygon.x = 300.f;
	g_posRankingPolygon.y = STARTPOINTY;
	g_posRankingPolygon.z = 0.f;

	VERTEX_2D *pVtx;
	g_pVtxBuffRankingPolygon->Lock(0, 0, (void**)&pVtx, 0);

	//頂点データ設定
	for (int i = 0; i < MAX_RANKING - 1; i++){
		for (int i = 0; i < RANKING_POLYGON; i++, pVtx += 4){
			//頂点座標初期化
			pVtx[0].pos.x = (i * RANKINGPOLYGON_WIDTH) + g_posRankingPolygon.x;
			pVtx[0].pos.y = g_posRankingPolygon.y - RANKINGPOLYGON_HEIGHT;
			pVtx[0].pos.z = 0.f;

			pVtx[1].pos.x = (i * RANKINGPOLYGON_WIDTH) + (g_posRankingPolygon.x + RANKINGPOLYGON_WIDTH);
			pVtx[1].pos.y = g_posRankingPolygon.y - RANKINGPOLYGON_HEIGHT;
			pVtx[1].pos.z = 0.f;

			pVtx[2].pos.x = (i * RANKINGPOLYGON_WIDTH) + g_posRankingPolygon.x;
			pVtx[2].pos.y = g_posRankingPolygon.y + RANKINGPOLYGON_HEIGHT;
			pVtx[2].pos.z = 0.f;

			pVtx[3].pos.x = (i * RANKINGPOLYGON_WIDTH) + (g_posRankingPolygon.x + RANKINGPOLYGON_WIDTH);
			pVtx[3].pos.y = g_posRankingPolygon.y + RANKINGPOLYGON_HEIGHT;
			pVtx[3].pos.z = 0.f;

			//テクスチャのパースペクティブ初期化
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			//頂点カラー初期化
			pVtx[0].col = D3DCOLOR_RGBA(255, 128, 0, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 128, 0, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 128, 0, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 128, 0, 255);

			//頂点テクスチャ初期化	
			pVtx[0].tex = D3DXVECTOR2(0, 0);
			pVtx[1].tex = D3DXVECTOR2(0.1f, 0);
			pVtx[2].tex = D3DXVECTOR2(0, 1);
			pVtx[3].tex = D3DXVECTOR2(0.1f, 1);
		}
		g_posRankingPolygon.y += NEXTRANKSPACE;		//次のポリゴンとの間
	}

	g_posRankingPolygon.x = STARTPOINTX;			//X座標の初期位置
	g_posRankingPolygon.y = STARTPOINTY;			//Y座標の初期位置

	for (int i = 0; i < MAX_RANK; i++, pVtx += 4){
		//頂点座標初期化
		pVtx[0].pos.x = g_posRankingPolygon.x;
		pVtx[0].pos.y = g_posRankingPolygon.y - RANKPOLYGON_HEIGHT;
		pVtx[0].pos.z = 0.f;

		pVtx[1].pos.x = g_posRankingPolygon.x + RANKPOLYGON_WIDTH;
		pVtx[1].pos.y = g_posRankingPolygon.y - RANKPOLYGON_HEIGHT;
		pVtx[1].pos.z = 0.f;

		pVtx[2].pos.x = g_posRankingPolygon.x;
		pVtx[2].pos.y = g_posRankingPolygon.y + RANKPOLYGON_HEIGHT;
		pVtx[2].pos.z = 0.f;

		pVtx[3].pos.x = g_posRankingPolygon.x + RANKPOLYGON_WIDTH;
		pVtx[3].pos.y = g_posRankingPolygon.y + RANKPOLYGON_HEIGHT;
		pVtx[3].pos.z = 0.f;

		//テクスチャのパースペクティブ初期化
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラー初期化
		pVtx[0].col = D3DCOLOR_RGBA(255, 128, 0, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 128, 0, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 128, 0, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 128, 0, 255);

		//頂点テクスチャ初期化	
		pVtx[0].tex = D3DXVECTOR2(0, 0);
		pVtx[1].tex = D3DXVECTOR2(1, 0);
		pVtx[2].tex = D3DXVECTOR2(0, 1);
		pVtx[3].tex = D3DXVECTOR2(1, 1);

		g_posRankingPolygon.y += NEXTRANKSPACE;
	}

	//テクスチャのポインタ初期化
	for (int i = 0; i < RANKINGPOLYGON_TEXTURE; i++){
		g_pRankingTexture[i] = NULL;
	}

	//テクスチャファイルの読み込み
	if (FAILED(D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/number000.png", &g_pRankingTexture[0]))){
		return E_FAIL;
	}

	if (FAILED(D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/rank1.png", &g_pRankingTexture[1]))){
		return E_FAIL;
	}

	if (FAILED(D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/rank2.png", &g_pRankingTexture[2]))){
		return E_FAIL;
	}

	if (FAILED(D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/rank3.png", &g_pRankingTexture[3]))){
		return E_FAIL;
	}

	if (FAILED(D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/rank4.png", &g_pRankingTexture[4]))){
		return E_FAIL;
	}

	g_pVtxBuffRankingPolygon->Unlock();

//    	FILE *fp;						//ファイルポインタ
//	errno_t error;					//エラーポインタ
	int nRanking = 0;				//各桁のスコア
	double CntNum = pow(10.0, RANKING_POLYGON);	//上記に必要
	float TexNumber = 0;			//UV座標
	int rank = 0;
//	int pScore;						//スコアポインタ

//	int pTimer = GetTimer();

	for (int i = 0; i < (MAX_RANKING - 1); i++)
	{
		for (int NextNum = 0; NextNum < (MAX_RANKING - 1 - i); NextNum++)
		{
			if (g_nRanking[NextNum] <= g_nRanking[(NextNum + 1)])
			{// 入れ替え
				int OldScore;
				OldScore = g_nRanking[NextNum];
				g_nRanking[NextNum] = g_nRanking[(NextNum + 1)];
				g_nRanking[(NextNum + 1)] = OldScore;
			}
		}
	}

	//ランキングの保存
	if (error = fopen_s(&fp, "./data/savedate/hiscore.txt", "w") != 0){		//ファイルの開放
		//エラー処理
	}

	for (int i = 0; i < MAX_RANKING; i++){
		fprintf_s(fp, "%d\n", g_nRanking[i]);
	}
	fclose(fp);				//ファイルを閉じる

//	VERTEX_2D *pVtx;
	g_pVtxBuffRankingPolygon->Lock(0, 0, (void**)&pVtx, 0);

	for (int y = 0; y < (MAX_RANKING - 1); y++){
		for (int i = 0; i < RANKING_POLYGON; i++){
			nRanking = (g_nRanking[y] % (int)CntNum) / ((int)CntNum / 10);

			//各桁の数値が0以外の時
			if (0 < nRanking && nRanking <= 9){
				TexNumber = (float)nRanking / 10;

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
			CntNum /= 10;							//1桁少なくする
		}
		CntNum = pow(10.0, (RANKING_POLYGON));
	}
	g_pVtxBuffRankingPolygon->Unlock();

	//プレイヤーのスコアランキングを検索
	for (int i = 0; i < (MAX_RANKING); i++){
		if (g_nRanking[i] == pTimer){
			rank = i;
			break;
		}
	}

	g_nAnime++;
	A = (g_nAnime / 30) % 2;		//α値変更

	if (rank < MAX_RANKING - 1){
		if (rank == 0 && g_bSound == false){
			g_bSound = true;
		}
		g_pVtxBuffRankingPolygon->Lock(0, 0, (void**)&pVtx, 0);

		for (int cnt = 0; cnt < RANKING_POLYGON; cnt++){
			pVtx[(rank * 16) + (cnt * RANKING_POLYGON) + 0].col = D3DCOLOR_RGBA(255, 128, 0, 255 * A);
			pVtx[(rank * 16) + (cnt * RANKING_POLYGON) + 1].col = D3DCOLOR_RGBA(255, 128, 0, 255 * A);
			pVtx[(rank * 16) + (cnt * RANKING_POLYGON) + 2].col = D3DCOLOR_RGBA(255, 128, 0, 255 * A);
			pVtx[(rank * 16) + (cnt * RANKING_POLYGON) + 3].col = D3DCOLOR_RGBA(255, 128, 0, 255 * A);
		}
		g_pVtxBuffRankingPolygon->Unlock();
	}

    InitRankingBG();

    return S_OK;
}

/*******************************************************************************
ランキングポリゴンの終了処理
*******************************************************************************/
void UninitRanking(void){

    // BGM
    StopSound();

    UninitRankingBG();

	//テクスチャポインタの廃棄
	for (int i = 0; i < RANKINGPOLYGON_TEXTURE; i++){
		if (g_pRankingTexture[i] != NULL){
			g_pRankingTexture[i]->Release();
			g_pRankingTexture[i] = NULL;
		}
	}

	//頂点バッファへのポインタの廃棄
	if (g_pVtxBuffRankingPolygon != NULL){
		g_pVtxBuffRankingPolygon->Release();
		g_pVtxBuffRankingPolygon = NULL;
	}
}

/*******************************************************************************
ランキングの更新処理
*******************************************************************************/
void UpdateRanking(void){

    UpdateRankingBG();

    g_nRankingFrameCount++;
    // 遷移判定
	if ( ( GetKeyboardTrigger(DIK_RETURN) || g_nRankingFrameCount > 60 * 8 || GetPadElecomTrigger( PAD_4 ) || GetPadElecomTrigger( PAD_10 ) ) && GetFade() == FADE_NONE )
	{
		PlaySound( DESIDE4_SE );
		SetFade( FADE_OUT, 60 );
		SetMode( MODE_TITLE );

	}

}

/*******************************************************************************
ランキングポリゴンの描画処理
*******************************************************************************/
void DrawRanking(void)
{
    DrawRankingBG();

	LPDIRECT3DDEVICE9 pDevice;		//デバイスポインタ
	int PrimitiveVertex = 0;

	pDevice = GetDevice();			//現状のデバイスを取得

	pDevice->SetStreamSource(0, g_pVtxBuffRankingPolygon, 0, sizeof(VERTEX_2D));
	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pRankingTexture[0]);

	for (int i = 0; i < MAX_RANKING - 1; i++){
		for (int i = 0; i < RANKING_POLYGON; i++){
			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, PrimitiveVertex, 2);
			PrimitiveVertex += 4;
		}
	}

	for (int i = 0; i < MAX_RANK; i++){
		//テクスチャの設定
		pDevice->SetTexture(0, g_pRankingTexture[i + 1]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, PrimitiveVertex, 2);
		PrimitiveVertex += 4;
	}
}
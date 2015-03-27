/*******************************************************************************
*
*	File	:�X�R�A�|���S���\��	[ranking.cpp]
*	Author	:AT12A271 26 keita bannno
*
*******************************************************************************/

/*******************************************************************************
�C���N���[�h�t�@�C��
*******************************************************************************/
#include <stdio.h>
#include "main.h"
#include "ranking.h"
#include "timer.h"
#include "RankingBG.h"

/*******************************************************************************
�O���[�o���ϐ��錾
*******************************************************************************/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRankingPolygon = NULL;				//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9		g_pRankingTexture[RANKINGPOLYGON_TEXTURE];		//�e�N�X�`���̃|�C���^
D3DXVECTOR3 g_posRankingPolygon;										//�|���S���̒��S���W
int g_nRanking[MAX_RANKING];											//�X�R�A����
int g_nAnime;
int A;
bool g_bSound = false;													//�Đ��������ǂ���
unsigned int g_nRankingFrameCount;

/*******************************************************************************
�X�R�A�̏���������
*******************************************************************************/
int InitRanking(void){

    g_nRankingFrameCount = 0;

    // BGM
    PlaySound(RANKING_BGM);

	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�|�C���^
	FILE *fp;						//�t�@�C���|�C���^
	errno_t error;					//�G���[�|�C���^

	//�ϐ�������
	pDevice = GetDevice();			//�f�o�C�X�|�C���^���擾
	g_bSound = false;

	int pTimer = GetTimer();

	//�����L���O�����[�h
	if (error = fopen_s(&fp, "./data/savedate/hiscore.txt", "r") != 0){
		//�G���[����
	}
	for (int i = 0; i < MAX_RANKING; i++){
		fscanf_s(fp, "%d", &g_nRanking[i]);
	}
	fclose(fp);

	//�ŐV�̃X�R�A���擾
	g_nRanking[MAX_RANKING - 1 ] = pTimer;

	//���_�o�b�t�@�̍쐬
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*RANKINGPOLYGON_VERTEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffRankingPolygon, NULL))){
		return E_FAIL;
	}

	//�|���S���̒��S���W( 300.f, 70.f, 0.f )
	g_posRankingPolygon.x = 300.f;
	g_posRankingPolygon.y = STARTPOINTY;
	g_posRankingPolygon.z = 0.f;

	VERTEX_2D *pVtx;
	g_pVtxBuffRankingPolygon->Lock(0, 0, (void**)&pVtx, 0);

	//���_�f�[�^�ݒ�
	for (int i = 0; i < MAX_RANKING - 1; i++){
		for (int i = 0; i < RANKING_POLYGON; i++, pVtx += 4){
			//���_���W������
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

			//�e�N�X�`���̃p�[�X�y�N�e�B�u������
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			//���_�J���[������
			pVtx[0].col = D3DCOLOR_RGBA(255, 128, 0, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 128, 0, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 128, 0, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 128, 0, 255);

			//���_�e�N�X�`��������	
			pVtx[0].tex = D3DXVECTOR2(0, 0);
			pVtx[1].tex = D3DXVECTOR2(0.1f, 0);
			pVtx[2].tex = D3DXVECTOR2(0, 1);
			pVtx[3].tex = D3DXVECTOR2(0.1f, 1);
		}
		g_posRankingPolygon.y += NEXTRANKSPACE;		//���̃|���S���Ƃ̊�
	}

	g_posRankingPolygon.x = STARTPOINTX;			//X���W�̏����ʒu
	g_posRankingPolygon.y = STARTPOINTY;			//Y���W�̏����ʒu

	for (int i = 0; i < MAX_RANK; i++, pVtx += 4){
		//���_���W������
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

		//�e�N�X�`���̃p�[�X�y�N�e�B�u������
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[������
		pVtx[0].col = D3DCOLOR_RGBA(255, 128, 0, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 128, 0, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 128, 0, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 128, 0, 255);

		//���_�e�N�X�`��������	
		pVtx[0].tex = D3DXVECTOR2(0, 0);
		pVtx[1].tex = D3DXVECTOR2(1, 0);
		pVtx[2].tex = D3DXVECTOR2(0, 1);
		pVtx[3].tex = D3DXVECTOR2(1, 1);

		g_posRankingPolygon.y += NEXTRANKSPACE;
	}

	//�e�N�X�`���̃|�C���^������
	for (int i = 0; i < RANKINGPOLYGON_TEXTURE; i++){
		g_pRankingTexture[i] = NULL;
	}

	//�e�N�X�`���t�@�C���̓ǂݍ���
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

//    	FILE *fp;						//�t�@�C���|�C���^
//	errno_t error;					//�G���[�|�C���^
	int nRanking = 0;				//�e���̃X�R�A
	double CntNum = pow(10.0, RANKING_POLYGON);	//��L�ɕK�v
	float TexNumber = 0;			//UV���W
	int rank = 0;
//	int pScore;						//�X�R�A�|�C���^

//	int pTimer = GetTimer();

	for (int i = 0; i < (MAX_RANKING - 1); i++)
	{
		for (int NextNum = 0; NextNum < (MAX_RANKING - 1 - i); NextNum++)
		{
			if (g_nRanking[NextNum] <= g_nRanking[(NextNum + 1)])
			{// ����ւ�
				int OldScore;
				OldScore = g_nRanking[NextNum];
				g_nRanking[NextNum] = g_nRanking[(NextNum + 1)];
				g_nRanking[(NextNum + 1)] = OldScore;
			}
		}
	}

	//�����L���O�̕ۑ�
	if (error = fopen_s(&fp, "./data/savedate/hiscore.txt", "w") != 0){		//�t�@�C���̊J��
		//�G���[����
	}

	for (int i = 0; i < MAX_RANKING; i++){
		fprintf_s(fp, "%d\n", g_nRanking[i]);
	}
	fclose(fp);				//�t�@�C�������

//	VERTEX_2D *pVtx;
	g_pVtxBuffRankingPolygon->Lock(0, 0, (void**)&pVtx, 0);

	for (int y = 0; y < (MAX_RANKING - 1); y++){
		for (int i = 0; i < RANKING_POLYGON; i++){
			nRanking = (g_nRanking[y] % (int)CntNum) / ((int)CntNum / 10);

			//�e���̐��l��0�ȊO�̎�
			if (0 < nRanking && nRanking <= 9){
				TexNumber = (float)nRanking / 10;

				pVtx[0].tex = D3DXVECTOR2(TexNumber, 0);
				pVtx[1].tex = D3DXVECTOR2(TexNumber + 0.1f, 0);
				pVtx[2].tex = D3DXVECTOR2(TexNumber, 1);
				pVtx[3].tex = D3DXVECTOR2(TexNumber + 0.1f, 1);
			}
			else{//�e����0�̎�
				pVtx[0].tex = D3DXVECTOR2(0, 0);
				pVtx[1].tex = D3DXVECTOR2(0.1f, 0);
				pVtx[2].tex = D3DXVECTOR2(0, 1);
				pVtx[3].tex = D3DXVECTOR2(0.1f, 1);
			}
			pVtx += 4;								//1�|���S�����̃A�h���X�����炷
			CntNum /= 10;							//1�����Ȃ�����
		}
		CntNum = pow(10.0, (RANKING_POLYGON));
	}
	g_pVtxBuffRankingPolygon->Unlock();

	//�v���C���[�̃X�R�A�����L���O������
	for (int i = 0; i < (MAX_RANKING); i++){
		if (g_nRanking[i] == pTimer){
			rank = i;
			break;
		}
	}

	g_nAnime++;
	A = (g_nAnime / 30) % 2;		//���l�ύX

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
�����L���O�|���S���̏I������
*******************************************************************************/
void UninitRanking(void){

    // BGM
    StopSound();

    UninitRankingBG();

	//�e�N�X�`���|�C���^�̔p��
	for (int i = 0; i < RANKINGPOLYGON_TEXTURE; i++){
		if (g_pRankingTexture[i] != NULL){
			g_pRankingTexture[i]->Release();
			g_pRankingTexture[i] = NULL;
		}
	}

	//���_�o�b�t�@�ւ̃|�C���^�̔p��
	if (g_pVtxBuffRankingPolygon != NULL){
		g_pVtxBuffRankingPolygon->Release();
		g_pVtxBuffRankingPolygon = NULL;
	}
}

/*******************************************************************************
�����L���O�̍X�V����
*******************************************************************************/
void UpdateRanking(void){

    UpdateRankingBG();

    g_nRankingFrameCount++;
    // �J�ڔ���
	if ( ( GetKeyboardTrigger(DIK_RETURN) || g_nRankingFrameCount > 60 * 8 || GetPadElecomTrigger( PAD_4 ) || GetPadElecomTrigger( PAD_10 ) ) && GetFade() == FADE_NONE )
	{
		PlaySound( DESIDE4_SE );
		SetFade( FADE_OUT, 60 );
		SetMode( MODE_TITLE );

	}

}

/*******************************************************************************
�����L���O�|���S���̕`�揈��
*******************************************************************************/
void DrawRanking(void)
{
    DrawRankingBG();

	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�|�C���^
	int PrimitiveVertex = 0;

	pDevice = GetDevice();			//����̃f�o�C�X���擾

	pDevice->SetStreamSource(0, g_pVtxBuffRankingPolygon, 0, sizeof(VERTEX_2D));
	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pRankingTexture[0]);

	for (int i = 0; i < MAX_RANKING - 1; i++){
		for (int i = 0; i < RANKING_POLYGON; i++){
			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, PrimitiveVertex, 2);
			PrimitiveVertex += 4;
		}
	}

	for (int i = 0; i < MAX_RANK; i++){
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pRankingTexture[i + 1]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, PrimitiveVertex, 2);
		PrimitiveVertex += 4;
	}
}
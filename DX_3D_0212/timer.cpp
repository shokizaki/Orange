/*******************************************************************************
*
*	File	:�^�C�}�[�|���S���\��	[timer.cpp]
*	Author	:AT12A271 26 keita bannno
*
*******************************************************************************/

/*******************************************************************************
�C���N���[�h�t�@�C��
*******************************************************************************/
#include "main.h"
#include "timer.h"
#include "fade.h"

/*******************************************************************************
�}�N���錾
*******************************************************************************/

/*******************************************************************************
�O���[�o���ϐ��錾
*******************************************************************************/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTimerPolygon = NULL;				//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9		g_pTimerTexture[TIMERPOLYGON_TEXTURE];		//�e�N�X�`���̃|�C���^
D3DXVECTOR3				g_posTimerPolygon;							//�|���S���̒��S���W
D3DXCOLOR				g_TimerCol;									//�@�F���
static float			g_nTimer;									//��������
static int				g_nCnt;
static float			r, g, b, a;									//�@�e�F
static int				g_Width;									//�@��
static int				g_Height;									//�@����
static int				g_ndigits;									//�@����

/*******************************************************************************
�^�C�}�[�̏���������
*******************************************************************************/
int InitTimer(void){
	MODE pMode;							//���[�h�|�C���^
	FADE pFade;							//�t�F�[�h�|�C���^
	LPDIRECT3DDEVICE9 pDevice;			//�f�o�C�X�|�C���^
	int nTimer = 0;			//�e���̃X�R�A
	g_ndigits = 3;
	double CntNum = pow(10.0, g_ndigits);		//��L�ɕK�v
	float TexNumber = 0;				//UV���W
	g_nTimer = 0;
	g_nCnt = 0;

	pDevice = GetDevice();
	pMode = GetMode();					//���݂̃��[�h���擾
	pFade = GetFade();					//���݂̃t�F�[�h��Ԃ��擾

	//���_�o�b�t�@�̍쐬
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*TIMERPOLYGON_VERTEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffTimerPolygon, NULL))){
		return E_FAIL;
	}

	//�|���S���̒��S���W( 1160.f, 20.f, 0.f )
	g_posTimerPolygon.x = 450.f;
	g_posTimerPolygon.y = TIMERPOLYGON_HEIGHT * 2;
	g_posTimerPolygon.z = 0.f;
	g_Width = TIMERPOLYGON_WIDTH;
	g_Height = TIMERPOLYGON_HEIGHT;

	//�@���_�J���[�ݒ�
	r = 1.0f;
	g = 1.0f;
	b = 1.0f;
	a = 1.0f;
	g_TimerCol = D3DXCOLOR( r, g, b, a );

	VERTEX_2D *pVtx;
	g_pVtxBuffTimerPolygon->Lock(0, 0, (void**)&pVtx, 0);

	//���_�f�[�^�ݒ�
	for (int i = 0; i < g_ndigits; i++){
		//���_���W������
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

		//�e�N�X�`���̃p�[�X�y�N�e�B�u������
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[������
		pVtx[0].col = g_TimerCol;
		pVtx[1].col = g_TimerCol;
		pVtx[2].col = g_TimerCol;
		pVtx[3].col = g_TimerCol;

		//���_�e�N�X�`��������	

		nTimer = ((int)g_nTimer % (int)CntNum) / ((int)CntNum / 10);	//�e���̐������v�Z

		//�e���̐��l��0�ȊO�̎�
		if (0 < nTimer && nTimer <= 9){
			TexNumber = (float)nTimer / 10;

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
		CntNum /= 10;
	}

	//�e�N�X�`���̃|�C���^������
	for (int i = 0; i < TIMERPOLYGON_TEXTURE; i++){
		g_pTimerTexture[i] = NULL;
	}

	//�e�N�X�`���t�@�C���̓ǂݍ���
	if (FAILED(D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/number000.png", &g_pTimerTexture[0]))){
		return E_FAIL;
	}

	g_pVtxBuffTimerPolygon->Unlock();

	return 0;
}

/*******************************************************************************
�^�C�}�[�̏I������
*******************************************************************************/
void UninitTimer(void){
	//�e�N�X�`���|�C���^�̔p��
	for (int i = 0; i < TIMERPOLYGON_TEXTURE; i++){
		if (g_pTimerTexture[i] != NULL){
			g_pTimerTexture[i]->Release();
			g_pTimerTexture[i] = NULL;
		}
	}

	//���_�o�b�t�@�ւ̃|�C���^�̔p��
	if (g_pVtxBuffTimerPolygon != NULL){
		g_pVtxBuffTimerPolygon->Release();
		g_pVtxBuffTimerPolygon = NULL;
	}
}

/*******************************************************************************
�^�C�}�[�̍X�V����
*******************************************************************************/
int UpdateTimer(void){
	int nTimer = 0;							//�e���̃X�R�A
	int nDigits = 0;
	double CntNum;			//��L�ɕK�v
	float TexNumber = 0;					//UV���W

	VERTEX_2D *pVtx;
	g_pVtxBuffTimerPolygon->Lock(0, 0, (void**)&pVtx, 0);

	g_nCnt++;
	if (g_nCnt % 60 == 0)
	{		//�@60�t���[���ɂ�1�ւ炷
		g_nTimer++;

	}

		//�@���݂̌����𒲂ׂ�
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


	//���_�f�[�^�ݒ�
	for (int i = 0; i < nDigits; i++)
	{
		//���_���W������
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

		//�e���̐��l��0�ȊO�̎�
		if (0 < nTimer && nTimer <= 9)
		{
			TexNumber = (float)nTimer / 10;

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
	
		//�@�F�X�V
		pVtx[0].col = g_TimerCol;
		pVtx[1].col = g_TimerCol;
		pVtx[2].col = g_TimerCol;
		pVtx[3].col = g_TimerCol;

		pVtx += 4;								//1�|���S�����̃A�h���X�����炷
		CntNum /= 10;							//1������
	}

	g_pVtxBuffTimerPolygon->Unlock();
	return (int)g_nTimer;
}

/*******************************************************************************
�^�C�}�[�̕`�揈��
*******************************************************************************/
void DrawTimer()
{
	int nDigits = 0;

	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();

	pDevice->SetStreamSource(0, g_pVtxBuffTimerPolygon, 0, sizeof(VERTEX_2D));
	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
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
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, PrimitiveVertex, 2);
		PrimitiveVertex += 4;
	}
}

int GetTimer(void)
{
	return (int)g_nTimer;
}
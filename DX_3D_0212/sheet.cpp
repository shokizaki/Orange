//-----------------------------------------------
//  ���e�F�^�C�g������
//  Fail:title.cpp
//  ����ҁF�؍�@��
//-----------------------------------------------

//-----------------------------------------------
//  �C���N���[�h�t�@�C��
//-----------------------------------------------
#include "main.h" 
#include "input.h"
#include "sheet.h"

//-----------------------------------------------
//�@�}�N����`
//-----------------------------------------------

//-----------------------------------------------
//  �֐��v���g�^�C�v�錾
//-----------------------------------------------

//-----------------------------------------------
//�@�}�N����`
//-----------------------------------------------

//-----------------------------------------------
//  �O���[�o���ϐ�
//-----------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSheet = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
bool g_bSheetUse = false;
D3DXCOLOR g_nSheet;

//-----------------------------------------------
// �w�i����������
//-----------------------------------------------
void InitSheet()
{
		LPDIRECT3DDEVICE9 pDevice;					//�f�o�C�X�|�C���^
	pDevice = GetDevice();

	//���_�o�b�t�@�̍쐬
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*SHEET_VERTEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffSheet, NULL);

		VERTEX_2D *pVtx;
	g_pVtxBuffSheet->Lock(0, 0, (void**)&pVtx, 0);

			//���_���W������
	pVtx[ 0 ].pos = D3DXVECTOR3 ( 0.0f, 0.0f, 0.0f );
	pVtx[ 1 ].pos = D3DXVECTOR3 ( SCREEN_WIDTH, 0.0f, 0.0f );
	pVtx[ 2 ].pos = D3DXVECTOR3 ( 0.0f, SCREEN_HEIGHT, 0.0f );
	pVtx[ 3 ].pos = D3DXVECTOR3 ( SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f );

		//�e�N�X�`���̃p�[�X�y�N�e�B�u������
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[������ 

		g_nSheet = D3DCOLOR_RGBA( 255, 255, 255, 0 );
		pVtx[0].col = g_nSheet;
		pVtx[1].col = g_nSheet;
		pVtx[2].col = g_nSheet;
		pVtx[3].col = g_nSheet;

		//���_�e�N�X�`��������	
		pVtx[0].tex = D3DXVECTOR2(0, 0);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0);
		pVtx[2].tex = D3DXVECTOR2(0, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		g_pVtxBuffSheet->Unlock();		//���_�f�[�^���A�����b�N

}

//-----------------------------------------------
// �w�i�I������
//-----------------------------------------------
void UninitSheet()
{
		//���_�o�b�t�@�ւ̃|�C���^�̔p��
	if (g_pVtxBuffSheet != NULL){
		g_pVtxBuffSheet->Release();
		g_pVtxBuffSheet = NULL;
	}

}

//-----------------------------------------------
// �w�i�X�V����
//-----------------------------------------------
void UpdateSheet()
{
			LPDIRECT3DDEVICE9 pDevice;					//�f�o�C�X�|�C���^
	pDevice = GetDevice();

	if( GetKeyboardTrigger( DIK_L ) == true)
	{
		if( g_bSheetUse == false )
		{
			g_nSheet = D3DCOLOR_RGBA( 255, 0, 0, 100 );
			g_bSheetUse = true;
		}
		else if( g_bSheetUse == true )
		{
			g_nSheet = D3DCOLOR_RGBA( 255, 255, 255, 0 );
			g_bSheetUse = false;
		}

	VERTEX_2D *pVtx;
	g_pVtxBuffSheet->Lock(0, 0, (void**)&pVtx, 0);

		pVtx[0].col = g_nSheet;
		pVtx[1].col = g_nSheet;
		pVtx[2].col = g_nSheet;
		pVtx[3].col = g_nSheet;

		g_pVtxBuffSheet->Unlock();		//���_�f�[�^���A�����b�N

	}

}

//-----------------------------------------------
// �w�i�`�揈��
//-----------------------------------------------
void DrawSheet()
{
		LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();

	pDevice->SetStreamSource(0, g_pVtxBuffSheet, 0, sizeof(VERTEX_2D));
	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice ->SetTexture( 0, NULL );

	//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//-----------------------------------------------
// �t�F�[�h�����ǂ���
//-----------------------------------------------
bool GetSheet( void )
{
	return g_bSheetUse;
}


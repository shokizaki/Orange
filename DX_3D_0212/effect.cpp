//-----------------------------------------------
//  ���e�F�e�Ɋւ��鏈��
//  Fail:bullet.cpp
//  ����ҁF�؍�@��
//-----------------------------------------------

//-----------------------------------------------
//  �C���N���[�h�t�@�C��
//-----------------------------------------------
#include "main.h"
#include "bullet.h"
#include "effect.h"
#include "enemy.h"
#include "collision.h"

//-----------------------------------------------
//�@�}�N����`
//-----------------------------------------------
#define VERTEX_MAX  (8)			// ���_�̐�
#define TEXTURE_MAX ( 3 )			// �g�p����e�N�X�`���̐�
#define UP_LIMIT (0)			// ��̌��E�l
#define EFFECT_WIDTH (50)		// �e�̕�
#define EFFECT_HEIGHT (50)		// �e�̍���

//-----------------------------------------------
//  �O���[�o���ϐ�
//-----------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffectEx = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureEffectEx[TEXTURE_MAX];		// �e�N�X�`���ւ̃|�C���^
EFFECT g_aEffectEx[EFFECTEX_MAX];						// EFFECT�\���̌^�ϐ�

//-----------------------------------------------
// �o���b�g�Ɋւ��鏉����
//-----------------------------------------------
void InitEffectEx()
{
	// ���[�J���ϐ�
	//---------------------------------
	LPDIRECT3DDEVICE9 pDevice = NULL;		// �f�o�C�X�I�u�W�F�N�g
	VERTEX_2D *pVtx;

	// �f�o�C�X�擾
	//------------------------------------
	pDevice = GetDevice();
	
	pDevice ->CreateVertexBuffer((sizeof(VERTEX_2D) * (EFFECTEX_MAX * 4)),	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@
								 D3DUSAGE_WRITEONLY,				// ���_�o�b�t�@�̎g�p���@
								 FVF_VERTEX_2D,						// �g�p���钸�_�t�H�[�}�b�g
								 D3DPOOL_MANAGED,					// �o�b�t�@��ێ����郁�����N���X�̎w��
								 &g_pVtxBuffEffectEx,					// ���_�o�b�t�@�ւ̃|�C���^
								 NULL);								// NULL�Œ�

	// �f�[�^�̈ʒu���m�ۂ���i���b�N�j
	//------------------------------------
	g_pVtxBuffEffectEx ->Lock(0,
						0,
						(void**)&pVtx,
						0);

	// �e�N�X�`���ւ̃|�C���^��������
	//------------------------------------
	for (int nCnt = 0; nCnt < TEXTURE_MAX; nCnt++)				
	{
		g_pTextureEffectEx[nCnt] = NULL;
	}

	// �e�N�X�`���̓ǂݍ���
	//------------------------------------
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ef000.png", &g_pTextureEffectEx[ 0 ]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ef001.png", &g_pTextureEffectEx[ 1 ]);

	// ���
	//------------------------------------
	g_pVtxBuffEffectEx -> Unlock();		

	// �o���b�g�̒��_���Z�b�g
	//------------------------------------
	SetupEffectEx();
}

// �o���b�g�̒��_���Z�b�g
void SetupEffectEx(void)
{
	// ���[�J���ϐ�
	//---------------------------------
	LPDIRECT3DDEVICE9 pDevice;		// �f�o�C�X�I�u�W�F�N�g
	int nCnt;						// �J�E���^�ϐ�
	VERTEX_2D *pVtx;				// ���_�̍\���̃|�C���^

	// �f�o�C�X�擾
	//------------------------------------
	pDevice = GetDevice();

	// �f�[�^�̈ʒu���m�ۂ���i���b�N�j
	//------------------------------------
	g_pVtxBuffEffectEx ->Lock(0,
						0,
						(void**)&pVtx,
						0);

	for (nCnt = 0; nCnt < EFFECTEX_MAX; nCnt++)
	{
		if ( g_aEffectEx[ nCnt ].bUse == true )
		{
			// �e
			//------------------------------------
			pVtx[0 + (nCnt * 4)].pos.x = g_aEffectEx[nCnt].pos.x - g_aEffectEx[nCnt].fWidth;	// ���_�̈ʒu
			pVtx[0 + (nCnt * 4)].pos.y = g_aEffectEx[nCnt].pos.y - g_aEffectEx[nCnt].fHeight;
			pVtx[0 + (nCnt * 4)].pos.z = 0.0f;
			pVtx[1 + (nCnt * 4)].pos.x = g_aEffectEx[nCnt].pos.x + g_aEffectEx[nCnt].fWidth;	// ���_�̈ʒu
			pVtx[1 + (nCnt * 4)].pos.y = g_aEffectEx[nCnt].pos.y - g_aEffectEx[nCnt].fHeight;
			pVtx[1 + (nCnt * 4)].pos.z = 0.0f;
			pVtx[2 + (nCnt * 4)].pos.x = g_aEffectEx[nCnt].pos.x - g_aEffectEx[nCnt].fWidth;	// ���_�̈ʒu
			pVtx[2 + (nCnt * 4)].pos.y = g_aEffectEx[nCnt].pos.y + g_aEffectEx[nCnt].fHeight;
			pVtx[2 + (nCnt * 4)].pos.z = 0.0f;
			pVtx[3 + (nCnt * 4)].pos.x = g_aEffectEx[nCnt].pos.x + g_aEffectEx[nCnt].fWidth;	// ���_�̈ʒu
			pVtx[3 + (nCnt * 4)].pos.y = g_aEffectEx[nCnt].pos.y + g_aEffectEx[nCnt].fHeight;
			pVtx[3 + (nCnt * 4)].pos.z = 0.0f;

			pVtx[0 + nCnt * 4].rhw = 1.0f;			// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g��1.0f�ŌŒ�
			pVtx[1 + nCnt * 4].rhw = 1.0f;			// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g��1.0f�ŌŒ�
			pVtx[2 + nCnt * 4].rhw = 1.0f;			// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g��1.0f�ŌŒ�
			pVtx[3 + nCnt * 4].rhw = 1.0f;			// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g��1.0f�ŌŒ�

			// �e
			//------------------------------------
			pVtx[0 + (nCnt * 4)].col = D3DXCOLOR(g_aEffectEx[nCnt].r, g_aEffectEx[nCnt].g, g_aEffectEx[nCnt].b, g_aEffectEx[nCnt].a);			// ���_���Ƃ̐F�̐ݒ�
			pVtx[1 + (nCnt * 4)].col = D3DXCOLOR(g_aEffectEx[nCnt].r, g_aEffectEx[nCnt].g, g_aEffectEx[nCnt].b, g_aEffectEx[nCnt].a);			//			��
			pVtx[2 + (nCnt * 4)].col = D3DXCOLOR(g_aEffectEx[nCnt].r, g_aEffectEx[nCnt].g, g_aEffectEx[nCnt].b, g_aEffectEx[nCnt].a);			//			��
			pVtx[3 + (nCnt * 4)].col = D3DXCOLOR(g_aEffectEx[nCnt].r, g_aEffectEx[nCnt].g, g_aEffectEx[nCnt].b, g_aEffectEx[nCnt].a);			//			��
		
			//switch ( g_aEffectEx[ nCnt ].effectTex )
			//{
			//case EFFECT_000:
			//	// �e
			//	//------------------------------------
			//	pVtx[0 + (nCnt * 4)].tex = D3DXVECTOR2(0.0f,0.0f);			// ���_�̈ʒu�ɑ΂���e�N�X�`�����W�̈ʒu
			//	pVtx[1 + (nCnt * 4)].tex = D3DXVECTOR2(1.0f,0.0f);			//					��
			//	pVtx[2 + (nCnt * 4)].tex = D3DXVECTOR2(0.0f,1.0f);			//					��
			//	pVtx[3 + (nCnt * 4)].tex = D3DXVECTOR2(1.0f,1.0f);			//					��

			//	break;

			//case EFFECT_001:
			//	// �e
			//	//------------------------------------
			//	pVtx[0 + (nCnt * 4)].tex = D3DXVECTOR2(0.0f,0.0f);			// ���_�̈ʒu�ɑ΂���e�N�X�`�����W�̈ʒu
			//	pVtx[1 + (nCnt * 4)].tex = D3DXVECTOR2(1.0f,0.0f);			//					��
			//	pVtx[2 + (nCnt * 4)].tex = D3DXVECTOR2(0.0f,1.0f);			//					��
			//	pVtx[3 + (nCnt * 4)].tex = D3DXVECTOR2(1.0f,1.0f);			//					��

			//	break;

			//case EFFECT_002:
			//	// �e
			//	//------------------------------------
			//	pVtx[0 + (nCnt * 4)].tex = D3DXVECTOR2(0.0f,0.0f);			// ���_�̈ʒu�ɑ΂���e�N�X�`�����W�̈ʒu
			//	pVtx[1 + (nCnt * 4)].tex = D3DXVECTOR2(1.0f,0.0f);			//					��
			//	pVtx[2 + (nCnt * 4)].tex = D3DXVECTOR2(0.0f,1.0f);			//					��
			//	pVtx[3 + (nCnt * 4)].tex = D3DXVECTOR2(1.0f,1.0f);			//					��

			//	break;
			//}

			// �e
			//------------------------------------
			pVtx[0 + (nCnt * 4)].tex = D3DXVECTOR2(0.0f,0.0f);			// ���_�̈ʒu�ɑ΂���e�N�X�`�����W�̈ʒu
			pVtx[1 + (nCnt * 4)].tex = D3DXVECTOR2(1.0f,0.0f);			//					��
			pVtx[2 + (nCnt * 4)].tex = D3DXVECTOR2(0.0f,1.0f);			//					��
			pVtx[3 + (nCnt * 4)].tex = D3DXVECTOR2(1.0f,1.0f);			//					��

		}
	}

	// ���
	//------------------------------------
	g_pVtxBuffEffectEx -> Unlock();		
}

//-----------------------------------------------
// �o���b�g�Ɋւ���I������
//-----------------------------------------------
void UninitEffectEx()
{
	// ���[�J���ϐ�
	//------------------------------------
	int nCnt;

	for (nCnt = 0; nCnt < TEXTURE_MAX; nCnt++)
	{
		if (g_pTextureEffectEx[nCnt] != NULL)
		{ // �g�����e�N�X�`���̉���Ə�����
		  //------------------------------------
			g_pTextureEffectEx[nCnt] -> Release();
			g_pTextureEffectEx[nCnt] = NULL;
		}
	}

	if (g_pVtxBuffEffectEx != NULL)
	{ // ���_�o�b�t�@�̉���Ə�����
	  //------------------------------------
		g_pVtxBuffEffectEx->Release();
		g_pVtxBuffEffectEx = NULL;
	}

	// �G�t�F�N�g�̏�����
	//-------------------------------------------
	for (nCnt = 0; nCnt < EFFECTEX_MAX; nCnt++)
	{
		g_aEffectEx[nCnt].pos.x = 0;
		g_aEffectEx[nCnt].pos.y = 0;
		g_aEffectEx[nCnt].nLife = 0;
		g_aEffectEx[nCnt].bUse = false;
		g_aEffectEx[nCnt].bCol = false;
	}
}	

//-----------------------------------------------
// �o���b�g�Ɋւ���X�V����
//-----------------------------------------------
void UpdateEffectEx()
{
	// ���[�J���ϐ�
	//------------------------------------
	int nCnt = 0;

	for (nCnt = 0; nCnt < EFFECTEX_MAX; nCnt++)
	{	
		if (g_aEffectEx[nCnt].bUse == true)
		{
			//g_aEffectEx[nCnt].nLife -= 1;

			g_aEffectEx[ nCnt ].a -= 0.02f;

			if (g_aEffectEx[nCnt].a <= 0)
			{
				g_aEffectEx[nCnt].bUse = false;
			}
		}
	}

	// �o���b�g�̒��_���Z�b�g
	//------------------------------------
	SetupEffectEx();
}

//-----------------------------------------------
// �o���b�g�Ɋւ���`�揈��
//-----------------------------------------------
void DrawEffectEx()
{
	int nCnt = 0;
	LPDIRECT3DDEVICE9 pDevice;		// �f�o�C�X�I�u�W�F�N�g
	
	// �f�o�C�X�̎擾
	//------------------------------------
	pDevice = GetDevice();

	// ���Z����
	//------------------------------------
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// ���_�o�b�t�@���f�[�^�X�g���[���Ƀo�C���h
	//-----------------------------------------
	pDevice ->SetStreamSource(0,
								g_pVtxBuffEffectEx,
								0,
								sizeof(VERTEX_2D));	// ���_�f�[�^�̊Ԋu

	// ���_�t�H�[�}�b�g�̐ݒ�
	//------------------------------------
	pDevice -> SetFVF(FVF_VERTEX_2D);

	for (nCnt = 0; nCnt < EFFECTEX_MAX; nCnt++)
	{
		if (g_aEffectEx[nCnt].bUse == true)
		{		

			//switch ( g_aEffectEx[ nCnt ].effectTex )
			//{
			//case EFFECT_000:
			//	// �e�N�X�`���̐ݒ�i�w�i�j
			//	//------------------------------------
			//	pDevice -> SetTexture(0, g_pTextureEffectEx[ 0 ]);

			//	break;

			//case EFFECT_001:
			//	// �e�N�X�`���̐ݒ�i�w�i�j
			//	//------------------------------------
			//	pDevice -> SetTexture(0, g_pTextureEffectEx[ 0 ]);

			//	break;

			//case EFFECT_002:
			//	// �e�N�X�`���̐ݒ�i�w�i�j
			//	//------------------------------------
			//	pDevice -> SetTexture(0, g_pTextureEffectEx[ 0 ]);

			//	break;
			//}

			// �e�N�X�`���̐ݒ�i�w�i�j
			//------------------------------------
			pDevice -> SetTexture(0, g_pTextureEffectEx[ 0 ]);

			// �|���S���̕`��
			//------------------------------------
			pDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,	// �v���~�e�B�u�̎��
										  nCnt * 4,			// ���[�h����ŏ��̒��_�̃C���f�b�N�X
										  2);				// �`�悷��v���~�e�B�u��
		}
	}

	// �����_�[�X�e�[�g�̐ݒ�
	////------------------------------------
	//pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// �J�����O�̐ݒ�
	//pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// ���u�����h���s��
	//pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// �����x�̊���
	//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// �����x�̊���
	
	pDevice-> SetRenderState(D3DRS_SRCBLEND	, D3DBLEND_SRCALPHA);
	pDevice-> SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//=====================================================================
//
// �G�t�F�N�g�̐���
//
// �i�����j
//  �\���ʒu�A������܂ł̃t���[�����ԁA���A����
//
//=====================================================================
void SetEffectEx(D3DXVECTOR3 pos, int nLife, float fWidth, float fHeight )
{
	int nCnt = 0;

	for (nCnt = 0; nCnt < EFFECTEX_MAX; nCnt++)
	{
		if (g_aEffectEx[nCnt].bUse == false)
		{
			g_aEffectEx[nCnt].pos.x = pos.x;
			g_aEffectEx[nCnt].pos.y = pos.y;
			g_aEffectEx[nCnt].nLife = nLife;
			g_aEffectEx[nCnt].fWidth = fWidth;
			g_aEffectEx[nCnt].fHeight = fHeight;
			g_aEffectEx[nCnt].r = 1.0f;
			g_aEffectEx[nCnt].g = 1.0f;
			g_aEffectEx[nCnt].b = 1.0f;
			g_aEffectEx[nCnt].a = 1.0f;
			g_aEffectEx[nCnt].bUse = true;

			// �o���b�g�̒��_���Z�b�g
			//------------------------------------
			SetupEffectEx();

			break;	// ��������o��
		}
	}
}	

EFFECT *GetEffectEx()
{
	return (&g_aEffectEx[0]);
}
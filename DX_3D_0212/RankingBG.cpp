//-----------------------------------------------
//  ���e�F�����L���O�w�i
//  Fail:RankingBG.cpp
//  ����ҁF����@�����
//-----------------------------------------------

//-----------------------------------------------
//  �C���N���[�h�t�@�C��
//-----------------------------------------------
#include "RankingBG.h"

//-----------------------------------------------
//�@�}�N����`
//-----------------------------------------------

//-----------------------------------------------
//�@�\���̒�`
//-----------------------------------------------

typedef struct tagRESULT
{
    LPDIRECT3DVERTEXBUFFER9 vtx;    // ���_�o�b�t�@
    LPDIRECT3DTEXTURE9      tex;    // �e�N�X�`���o�b�t�@
}RESULT;

//-----------------------------------------------
//  �O���[�o���ϐ�
//-----------------------------------------------
RESULT g_RankingBG;

//-----------------------------------------------
// �w�i����������
//-----------------------------------------------
void InitRankingBG()
{
	LPDIRECT3DDEVICE9 pDevice;		// �f�o�C�X�I�u�W�F�N�g

	// �f�o�C�X�擾
	//------------------------------------
	pDevice = GetDevice();

	pDevice ->CreateVertexBuffer(( sizeof( VERTEX_2D ) * 4 ),	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@
								 D3DUSAGE_WRITEONLY,				// ���_�o�b�t�@�̎g�p���@
								 FVF_VERTEX_2D,						// �g�p���钸�_�t�H�[�}�b�g
								 D3DPOOL_MANAGED,					// �o�b�t�@��ێ����郁�����N���X�̎w��
								 &g_RankingBG.vtx,					// ���_�o�b�t�@�ւ̃|�C���^
								 NULL);								// NULL�Œ�
		// �f�[�^�̈ʒu���m�ۂ���i���b�N�j
	//------------------------------------
	VERTEX_2D *pVtx;
	g_RankingBG.vtx ->Lock( 0, 0, (void**)&pVtx, 0 );

    pVtx[ 0 ].pos.x = 0.0f;
	pVtx[ 0 ].pos.y = 0.0f;	// ���_�̈ʒu
	pVtx[ 0 ].pos.z = 1.0f;												// ���_�̈ʒu

	pVtx[ 1 ].pos.x = SCREEN_WIDTH;
	pVtx[ 1 ].pos.y = 0.0f;	// ���_�̈ʒu
	pVtx[ 1 ].pos.z = 1.0f;												// ���_�̈ʒu

	pVtx[ 2 ].pos.x = 0.0f;
	pVtx[ 2 ].pos.y = SCREEN_HEIGHT;	// ���_�̈ʒu
	pVtx[ 2 ].pos.z = 1.0f;												// ���_�̈ʒu

	pVtx[ 3 ].pos.x = SCREEN_WIDTH;
	pVtx[ 3 ].pos.y = SCREEN_HEIGHT;	// ���_�̈ʒu
	pVtx[ 3 ].pos.z = 1.0f;												// ���_�̈ʒu

	// �@��
	pVtx[ 0 ].rhw = 1.0f;
	pVtx[ 1 ].rhw = 1.0f;
	pVtx[ 2 ].rhw = 1.0f;
	pVtx[ 3 ].rhw = 1.0f;			

	// �t�F�[�h�̐F
	//------------------------------------
	pVtx[ 0 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );			// ���_���Ƃ̐F�̐ݒ�
	pVtx[ 1 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );			//			��
	pVtx[ 2 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );			//			��
	pVtx[ 3 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );			//			��

	pVtx[ 0 ].tex = D3DXVECTOR2( 0.0f, 0.0f );
	pVtx[ 1 ].tex = D3DXVECTOR2( 1.0f, 0.0f );
	pVtx[ 2 ].tex = D3DXVECTOR2( 0.0f, 1.0f );
	pVtx[ 3 ].tex = D3DXVECTOR2( 1.0f, 1.0f );


	// ���
	//------------------------------------
	g_RankingBG.vtx -> Unlock();

	// �e�N�X�`���̓ǂݍ���
	//------------------------------------
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Ranking.jpg", &g_RankingBG.tex);
}

//-----------------------------------------------
// �w�i�I������
//-----------------------------------------------
void UninitRankingBG()
{
	if (g_RankingBG.tex != NULL)
	{ // �g�����e�N�X�`���̉���Ə�����
		//------------------------------------
		g_RankingBG.tex -> Release();
		g_RankingBG.tex = NULL;
	}

	if (g_RankingBG.vtx != NULL)
	{ // ���_�o�b�t�@�̉���Ə�����
	  //------------------------------------
		g_RankingBG.vtx->Release();
		g_RankingBG.vtx = NULL;
	}

    // BGM
	StopSound();
}

//-----------------------------------------------
// ���U���g�X�V����
//-----------------------------------------------
void UpdateRankingBG()
{
}

//-----------------------------------------------
// �w�i�`�揈��
//-----------------------------------------------
void DrawRankingBG()
{
	// ���[�J���ϐ�
	//------------------------------------
	LPDIRECT3DDEVICE9 pDevice;						// �f�o�C�X�I�u�W�F�N�g
	
	// �f�o�C�X�̎擾
	//------------------------------------
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���Ƀo�C���h
	//-----------------------------------------
	pDevice ->SetStreamSource(0,
								g_RankingBG.vtx,
								0,
								sizeof(VERTEX_2D));	// ���_�f�[�^�̊Ԋu

	// ���_�t�H�[�}�b�g�̐ݒ�
	//------------------------------------
	pDevice -> SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	//------------------------------------
	pDevice -> SetTexture( 0, g_RankingBG.tex );

	// �|���S���̕`��
	//------------------------------------
	pDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,	// �v���~�e�B�u�̎��
									0,				// �ǂݍ��ލŏ��̔ԍ�
									2);				// �`�悷��v���~�e�B�u��
}


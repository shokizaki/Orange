//===============================================
//
//  ���e   �F �t�B�[���h�Ɋւ��鏈��
//  File   �F field.cpp
//  ����� �F �؍�@��
//
//===============================================

//===============================================
// 
//  �C���N���[�h�t�@�C��
//
//===============================================
#include "field.h"
#include "input.h"
#include "camera.h"

//===============================================
//
//�@�}�N����`
//
//===============================================
#define TEXTURE_MAX 	(1)			// �g�p����e�N�X�`���̐�
#define FIELD_MAX		(4)			// �t�B�[���h�̍ő吔
#define TEX_PASS		"data/TEXTURE/buis.jpg"

//-----------------------------------------------
//  �O���[�o���ϐ�
//-----------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffField = NULL;						// ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9		g_pTextureField[TEXTURE_MAX] = {NULL};		// �e�N�X�`���ւ̃|�C���^

FIELD g_field[FIELD_MAX];

D3DXVECTOR3 g_posField;			// �ʒu
D3DXVECTOR3 g_rotField;			// ����
D3DXVECTOR3 g_sclField;			// �傫��

static D3DXMATRIX g_mtxProjection;		// �v���W�F�N�V�����}�g���b�N�X
static D3DXMATRIX g_mtxView;			// �r���[�}�g���b�N�X
static D3DXMATRIX g_mtxWorld;			// ���[���h�}�g���b�N�X

//===============================================
//
// �t�B�[���h�Ɋւ��鏉����
//
//===============================================
void InitField(void)	
{
	// ���[�J���ϐ�
	//---------------------------------
	LPDIRECT3DDEVICE9 pDevice = NULL;		// �f�o�C�X�I�u�W�F�N�g
	//VERTEX_3D *pVtx;

	// �f�o�C�X�擾
	//------------------------------------
	pDevice = GetDevice();

	// �f�o�C�X�̐ݒ�
	//------------------------------------
	pDevice ->CreateVertexBuffer((sizeof(VERTEX_3D) * 4),				// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@
								 D3DUSAGE_WRITEONLY,					// ���_�o�b�t�@�̎g�p���@
								 FVF_VERTEX_3D,							// �g�p���钸�_�t�H�[�}�b�g
								 D3DPOOL_MANAGED,						// �o�b�t�@��ێ����郁�����N���X�̎w��
								 &g_pVtxBuffField,						// ���_�o�b�t�@�ւ̃|�C���^
								 NULL);									// NULL�Œ�	

	// �t�B�[���h������
	//------------------------------------
	g_posField = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_rotField = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_sclField = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	// �e�N�X�`���ւ̃|�C���^��������
	//------------------------------------
	for (int nCnt = 0; nCnt < TEXTURE_MAX; nCnt++)				
	{
		g_pTextureField[nCnt] = NULL;
	}

	// �e�N�X�`���̓ǂݍ���
	//------------------------------------
	D3DXCreateTextureFromFile(pDevice, TEX_PASS, &g_pTextureField[0]);

	// �t�B�[���h�̒��_���Z�b�g
	//------------------------------------
	SetupField();
}

//===============================================
//
// �t�B�[���h�̒��_���Z�b�g
//
//===============================================
void SetupField(void)
{
	// ���[�J���ϐ�
	//---------------------------------
	LPDIRECT3DDEVICE9 pDevice;		// �f�o�C�X�I�u�W�F�N�g
	VERTEX_3D *pVtx;				// ���_�̍\���̃|�C���^

	// �f�o�C�X�擾
	//------------------------------------
	pDevice = GetDevice();

	// �f�[�^�̈ʒu���m�ۂ���i���b�N�j
	//------------------------------------
	g_pVtxBuffField ->Lock(0,
						0,
						(void**)&pVtx,
						0);

	// �ʒu
	//------------------------------------
	//pVtx[0].pos.x = g_posField.x - 100.0f;
	//pVtx[0].pos.y = g_posField.y - 0.0f;
	//pVtx[0].pos.z = g_posField.z + 100.0f;

	pVtx[0].pos = D3DXVECTOR3(-100.0f, 0.0f, 100.0f);

	pVtx[1].pos.x = g_posField.x + 100.0f;
	pVtx[1].pos.y = g_posField.y - 0.0f;
	pVtx[1].pos.z = g_posField.z + 100.0f;

	pVtx[2].pos.x = g_posField.x - 100.0f;
	pVtx[2].pos.y = g_posField.y - 0.0f;
	pVtx[2].pos.z = g_posField.z - 100.0f;

	pVtx[3].pos.x = g_posField.x + 100.0f;
	pVtx[3].pos.y = g_posField.y - 0.0f;
	pVtx[3].pos.z = g_posField.z - 100.0f;

	// �@���x�N�g���̌���
	//------------------------------------
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// �F
	//------------------------------------
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// �e�N�X�`�����W
	//------------------------------------
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	
	// ���
	//------------------------------------
	g_pVtxBuffField -> Unlock();		
}

//===============================================
//
// �t�B�[���h�Ɋւ���I������
//
//===============================================
void UninitField()
{
	// ���[�J���ϐ�
	//------------------------------------
	int nCnt;

	for (nCnt = 0; nCnt < TEXTURE_MAX; nCnt++)
	{
		if (g_pTextureField[nCnt] != NULL)
		{ 
			// �g�����e�N�X�`���̉���Ə�����
			//------------------------------------
			g_pTextureField[nCnt] -> Release();
			g_pTextureField[nCnt] = NULL;
		}
	}

	if (g_pVtxBuffField != NULL)
	{ 
		// ���_�o�b�t�@�̉���Ə�����
		//------------------------------------
		g_pVtxBuffField->Release();
		g_pVtxBuffField = NULL;
	}
}	

//===============================================
//
// �t�B�[���h�Ɋւ���X�V����
//
//===============================================
void UpdateField()
{
	
}

//===============================================
//
// �t�B�[���h�Ɋւ���`�揈��
//
//===============================================
void DrawField()
{
	// ���[�J���ϐ�
	//------------------------------------
	LPDIRECT3DDEVICE9 pDevice;						// �f�o�C�X�I�u�W�F�N�g
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;		// �v�Z�p
	CAMERA *pCamera = GetCamera();
	
	// �f�o�C�X�̎擾
	//------------------------------------
	pDevice = GetDevice();

	// �J�����̐ݒ�
	//------------------------------------
	SetCamera();

	// ���[���h�}�g���b�N�X�̐ݒ�
	//------------------------------------
	D3DXMatrixIdentity(&g_mtxWorld);			// �t�H�[�}�b�g�̏�����

	//==============================================
	// 
	// ���f���鏇�Ԃ͌���I�I
	// 
	//==============================================

	// �X�P�[���𔽉f
	D3DXMatrixScaling(&mtxScl, g_sclField.x, g_sclField.y, g_sclField.z);
	D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorld, &mtxScl);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotField.y, g_rotField.x, g_rotField.z);
	D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTranslate, g_posField.x, g_posField.y, g_posField.z);
	D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorld, &mtxTranslate);

	// �ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���Ƀo�C���h
	//-----------------------------------------
	pDevice ->SetStreamSource(0,
								g_pVtxBuffField,
								0,
								sizeof(VERTEX_3D));	// ���_�f�[�^�̊Ԋu

	// ���_�t�H�[�}�b�g�̐ݒ�
	//------------------------------------
	pDevice -> SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	//------------------------------------
	pDevice -> SetTexture(0, g_pTextureField[0]);

	// �|���S���̕`��
	//------------------------------------
	pDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,	// �v���~�e�B�u�̎��
									0,				// ���[�h����ŏ��̒��_�̃C���f�b�N�X
									2);				// �`�悷��v���~�e�B�u��
}
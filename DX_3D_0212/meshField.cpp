//===============================================
//
//  ���e   �F �C���f�b�N�X������
//				���b�V���t�B�[���h�Ɋւ��鏈��
//  File   �F meshField.cpp
//  ����� �F �؍�@��
//
//===============================================

//===============================================
// 
//  �C���N���[�h�t�@�C��
//
//===============================================
#include "meshField.h"
#include "camera.h"

//===============================================
//
//�@�}�N����`
//
//===============================================
#define TEXTURE_MAX 	(1)			// �g�p����e�N�X�`���̐�
#define FIELD_MAX		(4)			// �t�B�[���h�̍ő吔
#define TEX_PASS		"data/TEXTURE/field001.jpg"

//-----------------------------------------------
//  �O���[�o���ϐ�
//-----------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshField = NULL;				// ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshField = NULL;
LPDIRECT3DTEXTURE9		g_pTextureMeshField[TEXTURE_MAX];		// �e�N�X�`���ւ̃|�C���^

MESHFIELD g_meshField;

D3DXVECTOR3 g_posMeshField;			// �ʒu
D3DXVECTOR3 g_rotMeshField;			// ����
D3DXVECTOR3 g_sclMeshField;			// �傫��

static D3DXMATRIX g_mtxProjection;		// �v���W�F�N�V�����}�g���b�N�X
static D3DXMATRIX g_mtxView;			// �r���[�}�g���b�N�X
static D3DXMATRIX g_mtxWorld;			// ���[���h�}�g���b�N�X

int g_nNumVtx = 0;
int g_nNumIdx = 0;
int g_nMaxVtx = 0;
int g_nDrawPrimitiveNum;

//===============================================
//
// �t�B�[���h�Ɋւ��鏉����
//
//===============================================
void InitMeshField(int numX, int numZ, float fWidth, float fHeight)	
{
	// ���[�J���ϐ�
	//---------------------------------
	LPDIRECT3DDEVICE9 pDevice = NULL;		// �f�o�C�X�I�u�W�F�N�g

	// �O���[�o���ϐ��̏�����
	//------------------------------------
	g_nNumVtx = 0;
	g_nNumIdx = 0;
	g_nMaxVtx = (numX + 1) * (numZ + 1);
	g_nDrawPrimitiveNum = (numX * numZ * 2) + ( (numZ - 1) * 4 );

	// �f�o�C�X�擾
	//------------------------------------
	pDevice = GetDevice();

	// ���_�o�b�t�@�̊m��
	//------------------------------------
	pDevice ->CreateVertexBuffer((sizeof(VERTEX_3D) * g_nMaxVtx),		// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@
								 D3DUSAGE_WRITEONLY,					// ���_�o�b�t�@�̎g�p���@
								 FVF_VERTEX_3D,							// �g�p���钸�_�t�H�[�}�b�g
								 D3DPOOL_MANAGED,						// �o�b�t�@��ێ����郁�����N���X�̎w��
								 &g_pVtxBuffMeshField,					// ���_�o�b�t�@�ւ̃|�C���^
								 NULL);									// NULL�Œ�	

	// �C���f�b�N�X�o�b�t�@�̊m��
	//------------------------------------
	pDevice->CreateIndexBuffer(( sizeof(WORD) * ( g_nDrawPrimitiveNum + 2 ) ),	// �m�ۂ���o�b�t�@�T�C�Y
							   D3DUSAGE_WRITEONLY,								// ���_�o�b�t�@�̎g�p���@
							   D3DFMT_INDEX16,									// �g�p���钸�_�t�H�[�}�b�g
							   D3DPOOL_MANAGED,									// �o�b�t�@��ێ����郁�����N���X�̎w��
							   &g_pIdxBuffMeshField,							// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
							   NULL);

	// �t�B�[���h������
	//------------------------------------
	g_posMeshField = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_rotMeshField = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_sclMeshField = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	g_meshField.nNumX = numX;
	g_meshField.nNumZ = numZ;
	g_meshField.fWidth = fWidth;
	g_meshField.fHeight = fHeight;

	// �e�N�X�`���ւ̃|�C���^��������
	//------------------------------------
	for (int nCnt = 0; nCnt < TEXTURE_MAX; nCnt++)				
	{
		g_pTextureMeshField[nCnt] = NULL;
	}

	// �e�N�X�`���̓ǂݍ���
	//------------------------------------
	D3DXCreateTextureFromFile(pDevice, TEX_PASS, &g_pTextureMeshField[0]);

	// �t�B�[���h�̒��_���ƃC���f�b�N�X�̃Z�b�g
	//-------------------------------------------
	SetupMeshField(numX, numZ, fWidth, fHeight);
}

//===============================================
//
// �t�B�[���h�̒��_���Z�b�g
//
//===============================================
void SetupMeshField(int numX, int numZ, float fWidth, float fHeight)
{
	VERTEX_3D *pVtx;

	// �f�[�^�̈ʒu���m�ۂ���i���b�N�j
	//------------------------------------
	g_pVtxBuffMeshField ->Lock(0, 0, (void**)&pVtx, 0);

	//==============================================
	// 
	// ���b�V���t�B�[���h���ݒ�
	// 
	//==============================================

	// ���[�v�p�ϐ�
	int nCntX = 0;
	int nCntZ = 0;
	float fRot = 0.0f;

	// ���_���Ɏg�p���郏�[�N
	float fStartX = 0 - ((float)numZ / 2) * fWidth;
	float fStartZ = 0 + ((float)numX / 2) * fHeight;

	// ���_���̐ݒ�
	//------------------------------------
	for (nCntZ = 0; nCntZ < (numZ + 1); nCntZ++)
	{
		for (nCntX = 0; nCntX < (numX + 1); nCntX++)
		{
			// ���_���W
			pVtx[g_nNumVtx].pos = D3DXVECTOR3( ( fStartX + (nCntX * fWidth) ), 0.0f, ( fStartZ - (nCntZ * fHeight)));

			// �@��
			pVtx[g_nNumVtx].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// �F
			pVtx[g_nNumVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// �e�N�X�`�����W
			pVtx[g_nNumVtx].tex = D3DXVECTOR2((float)nCntX, (float)nCntZ);

			// ���_���̌����C���N�������g
			g_nNumVtx++;

			//fRot += D3DX_PI / 4;
		}
	}

	// �C���f�b�N�X�Ɏg�p����ϐ�
	//------------------------------------
	WORD *pIdx = NULL;

	// �C���f�b�N�X�o�b�t�@�̃��b�N
	//------------------------------------
	g_pIdxBuffMeshField->Lock(0, 0, (void**)&pIdx, 0);

	// �C���f�b�N�X�̐ݒ�
	//------------------------------------
	for (nCntZ = 0; nCntZ < numZ; nCntZ++)
	{
		for (nCntX = 0; nCntX < (numX + 1); nCntX++)
		{
			pIdx[g_nNumIdx] = (numX + 1) + nCntX + nCntZ * (numX + 1);
			g_nNumIdx++;

			pIdx[g_nNumIdx] = nCntX + nCntZ * (numX + 1);
			g_nNumIdx++;
		}

		// �܂�Ԃ��K�v����������
		//-----------------------------------
		if (nCntZ != numZ - 1)
		{
			pIdx[g_nNumIdx] = pIdx[g_nNumIdx - 1];
			g_nNumIdx++;

			pIdx[g_nNumIdx] = nCntX + ((nCntZ + 1) * nCntX);
			g_nNumIdx++;
		}
	}

	// �C���f�b�N�X�o�b�t�@�̃A�����b�N
	//------------------------------------
	g_pIdxBuffMeshField->Unlock();

	// ���_���̃A�����b�N
	//------------------------------------
	g_pVtxBuffMeshField -> Unlock();
}

//===============================================
//
// �t�B�[���h�Ɋւ���I������
//
//===============================================
void UninitMeshField()
{
	// ���[�J���ϐ�
	//------------------------------------
	int nCnt;

	for (nCnt = 0; nCnt < TEXTURE_MAX; nCnt++)
	{
		if (g_pTextureMeshField[nCnt] != NULL)
		{ 
			// �g�����e�N�X�`���̉���Ə�����
			//------------------------------------
			g_pTextureMeshField[nCnt] -> Release();
			g_pTextureMeshField[nCnt] = NULL;
		}
	}

	if (g_pVtxBuffMeshField != NULL)
	{ 
		// ���_�o�b�t�@�̉���Ə�����
		//------------------------------------
		g_pVtxBuffMeshField -> Release();
		g_pVtxBuffMeshField = NULL;
	}

	if (g_pIdxBuffMeshField != NULL)
	{
		g_pIdxBuffMeshField -> Release();
		g_pIdxBuffMeshField = NULL;
	}
}	

//===============================================
//
// �t�B�[���h�Ɋւ���X�V����
//
//===============================================
void UpdateMeshField()
{
	
}

//===============================================
//
// �t�B�[���h�Ɋւ���`�揈��
//
//===============================================
void DrawMeshField()
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
	D3DXMatrixScaling(&mtxScl, g_sclMeshField.x, g_sclMeshField.y, g_sclMeshField.z);
	D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorld, &mtxScl);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotMeshField.y, g_rotMeshField.x, g_rotMeshField.z);
	D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTranslate, g_posMeshField.x, g_posMeshField.y, g_posMeshField.z);
	D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorld, &mtxTranslate);

	// �ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���Ƀo�C���h
	//-----------------------------------------
	pDevice ->SetStreamSource(0,
								g_pVtxBuffMeshField,
								0,
								sizeof(VERTEX_3D));	// ���_�f�[�^�̊Ԋu

	// �C���f�b�N�X�o�b�t�@�̃o�C���h
	//------------------------------------
	pDevice->SetIndices(g_pIdxBuffMeshField);

	// ���_�t�H�[�}�b�g�̐ݒ�
	//------------------------------------
	pDevice -> SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	//------------------------------------
	pDevice -> SetTexture(0, g_pTextureMeshField[ 0 ]);

	// �|���S���̕`��
	//------------------------------------
	pDevice -> DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,	// �v���~�e�B�u�̎��
									0,						// 
									0,						// 
									g_nMaxVtx,				// ���_��
									0,						// 
									g_nDrawPrimitiveNum);	// �`�悷��v���~�e�B�u��
}

//===============================================
//
// �t�B�[���h�̏��擾
//
//===============================================
MESHFIELD *GetMeshField(void)
{
	return &g_meshField;
}
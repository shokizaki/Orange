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
#include "input.h"
#include "camera.h"
#include "wall.h"

//===============================================
//
//�@�}�N����`
//
//===============================================
#define TEXTURE_MAX 	(1)			// �g�p����e�N�X�`���̐�
#define WALL_MAX		(4)			// �t�B�[���h�̍ő吔
#define TEX_PASS		"data/TEXTURE/wall000.jpg"

#define NUM_X (4)
#define NUM_Z (2)

#define WIDTH (750.0f)
#define HEIGHT (100.0f)

//-----------------------------------------------
//  �O���[�o���ϐ�
//-----------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWall = NULL;				// ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffWall = NULL;
LPDIRECT3DTEXTURE9		g_pTextureWall[TEXTURE_MAX];		// �e�N�X�`���ւ̃|�C���^

WALL g_wall[WALL_MAX];

static D3DXMATRIX g_mtxProjection;		// �v���W�F�N�V�����}�g���b�N�X
static D3DXMATRIX g_mtxView;			// �r���[�}�g���b�N�X

static int g_nNumVtx = 0;
static int g_nNumIdx = 0;
static int g_nMaxVtx = 0;
static int g_nDrawPrimitiveNum;

//===============================================
//
// �t�B�[���h�Ɋւ��鏉����
//
//===============================================
void InitWall()	
{
	// ���[�J���ϐ�
	//---------------------------------
	LPDIRECT3DDEVICE9 pDevice = NULL;		// �f�o�C�X�I�u�W�F�N�g
	g_nNumVtx = 0;
	g_nNumIdx = 0;
	g_nMaxVtx = (NUM_X + 1) * (NUM_Z + 1);
	g_nDrawPrimitiveNum = (NUM_X * NUM_Z * 2) + ( (NUM_Z - 1) * 4 );

	// �f�o�C�X�擾
	//------------------------------------
	pDevice = GetDevice();

	// ���_�o�b�t�@�̊m��
	//------------------------------------
	pDevice ->CreateVertexBuffer((sizeof(VERTEX_3D) * g_nMaxVtx),		// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@
								 D3DUSAGE_WRITEONLY,					// ���_�o�b�t�@�̎g�p���@
								 FVF_VERTEX_3D,							// �g�p���钸�_�t�H�[�}�b�g
								 D3DPOOL_MANAGED,						// �o�b�t�@��ێ����郁�����N���X�̎w��
								 &g_pVtxBuffWall,					// ���_�o�b�t�@�ւ̃|�C���^
								 NULL);									// NULL�Œ�	

	// �C���f�b�N�X�o�b�t�@�̊m��
	//------------------------------------
	pDevice->CreateIndexBuffer(( sizeof(WORD) * ( g_nDrawPrimitiveNum + 2 ) ),	// �m�ۂ���o�b�t�@�T�C�Y
							   D3DUSAGE_WRITEONLY,								// ���_�o�b�t�@�̎g�p���@
							   D3DFMT_INDEX16,									// �g�p���钸�_�t�H�[�}�b�g
							   D3DPOOL_MANAGED,									// �o�b�t�@��ێ����郁�����N���X�̎w��
							   &g_pIdxBuffWall,							// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
							   NULL);

	// �t�B�[���h������
	//------------------------------------
	g_wall[0].posWall = D3DXVECTOR3(-750.0f, 0.0f, 1500.0f);
	g_wall[0].rotWall = D3DXVECTOR3(-(D3DX_PI / 2), 0.0f, 0.0f);
	g_wall[0].sclWall = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	g_wall[1].posWall = D3DXVECTOR3(-1500.0f, 0.0f, -750.0f);
	g_wall[1].rotWall = D3DXVECTOR3(-(D3DX_PI / 2), -(D3DX_PI / 2), 0.0f);
	g_wall[1].sclWall = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	g_wall[2].posWall = D3DXVECTOR3(1500.0f, 0.0f, 750.0f);
	g_wall[2].rotWall = D3DXVECTOR3(-(D3DX_PI / 2),  (D3DX_PI / 2), 0.0f);
	g_wall[2].sclWall = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	g_wall[3].posWall = D3DXVECTOR3(750.0f, 0.0f, -1500.0f);
	g_wall[3].rotWall = D3DXVECTOR3(-(D3DX_PI / 2), (D3DX_PI), 0.0f);
	g_wall[3].sclWall = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	// �e�N�X�`���ւ̃|�C���^��������
	//------------------------------------
	for (int nCnt = 0; nCnt < TEXTURE_MAX; nCnt++)				
	{
		g_pTextureWall[nCnt] = NULL;
	}

	// �e�N�X�`���̓ǂݍ���
	//------------------------------------
	D3DXCreateTextureFromFile(pDevice, TEX_PASS, &g_pTextureWall[0]);

	// �t�B�[���h�̒��_���ƃC���f�b�N�X�̃Z�b�g
	//-------------------------------------------
	SetupWall();
}

//===============================================
//
// �t�B�[���h�̒��_���Z�b�g
//
//===============================================
void SetupWall()
{
	VERTEX_3D *pVtx;

	// �f�[�^�̈ʒu���m�ۂ���i���b�N�j
	//------------------------------------
	g_pVtxBuffWall ->Lock(0, 0, (void**)&pVtx, 0);

	//==============================================
	// 
	// ���b�V���t�B�[���h���ݒ�
	// 
	//==============================================

	// ���[�v�p�ϐ�
	int nCntX = 0;
	int nCntZ = 0;

	// ���_���Ɏg�p���郏�[�N
	float fStartX = 0 - (NUM_Z / 2) * WIDTH;
	float fStartZ = 0 + (NUM_X / 2) * HEIGHT;

	// ���_���̐ݒ�
	//------------------------------------
	for (nCntZ = 0; nCntZ < (NUM_Z + 1); nCntZ++)
	{
		for (nCntX = 0; nCntX < (NUM_X + 1); nCntX++)
		{
			// ���_���W
			pVtx[g_nNumVtx].pos = D3DXVECTOR3( fStartX + (nCntX * WIDTH), 0.0f, fStartZ - (nCntZ * HEIGHT));

			// �@��
			pVtx[g_nNumVtx].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

			// �F
			pVtx[g_nNumVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// �e�N�X�`�����W
			pVtx[g_nNumVtx].tex = D3DXVECTOR2((float)nCntX, (float)nCntZ);

			// ���_���̌����C���N�������g
			g_nNumVtx++;
		}
	}

	// �C���f�b�N�X�Ɏg�p����ϐ�
	//------------------------------------
	WORD *pIdx = NULL;

	// �C���f�b�N�X�o�b�t�@�̃��b�N
	//------------------------------------
	g_pIdxBuffWall->Lock(0, 0, (void**)&pIdx, 0);

	// �C���f�b�N�X�̐ݒ�
	//------------------------------------
	for (nCntZ = 0; nCntZ < NUM_Z; nCntZ++)
	{
		for (nCntX = 0; nCntX < (NUM_X + 1); nCntX++)
		{
			pIdx[g_nNumIdx] = (NUM_X + 1) + nCntX + nCntZ * (NUM_X + 1);
			g_nNumIdx++;

			pIdx[g_nNumIdx] = nCntX + nCntZ * (NUM_X + 1);
			g_nNumIdx++;
		}

		// �܂�Ԃ��K�v����������
		//-----------------------------------
		if (nCntZ != NUM_Z - 1)
		{
			pIdx[g_nNumIdx] = pIdx[g_nNumIdx - 1];
			g_nNumIdx++;

			pIdx[g_nNumIdx] = nCntX + ((nCntZ + 1) * nCntX);
			g_nNumIdx++;
		}
	}

	// �C���f�b�N�X�o�b�t�@�̃A�����b�N
	//------------------------------------
	g_pIdxBuffWall->Unlock();

	// ���_���̃A�����b�N
	//------------------------------------
	g_pVtxBuffWall -> Unlock();
}

//===============================================
//
// �t�B�[���h�Ɋւ���I������
//
//===============================================
void UninitWall()
{
	// ���[�J���ϐ�
	//------------------------------------
	int nCnt;

	for (nCnt = 0; nCnt < TEXTURE_MAX; nCnt++)
	{
		if (g_pTextureWall[nCnt] != NULL)
		{ 
			// �g�����e�N�X�`���̉���Ə�����
			//------------------------------------
			g_pTextureWall[nCnt] -> Release();
			g_pTextureWall[nCnt] = NULL;
		}
	}

	if (g_pVtxBuffWall != NULL)
	{ 
		// ���_�o�b�t�@�̉���Ə�����
		//------------------------------------
		g_pVtxBuffWall -> Release();
		g_pVtxBuffWall = NULL;
	}

	if (g_pIdxBuffWall != NULL)
	{
		g_pIdxBuffWall -> Release();
		g_pIdxBuffWall = NULL;
	}
}	

//===============================================
//
// �t�B�[���h�Ɋւ���X�V����
//
//===============================================
void UpdateWall()
{
	
}

//===============================================
//
// �t�B�[���h�Ɋւ���`�揈��
//
//===============================================
void DrawWall()
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

	for (int nCnt = 0; nCnt < WALL_MAX; nCnt++)
	{
		// ���[���h�}�g���b�N�X�̐ݒ�
		//------------------------------------
		D3DXMatrixIdentity(&g_wall[nCnt].mtxWorldWall);			// �t�H�[�}�b�g�̏�����

		//==============================================
		// 
		// ���f���鏇�Ԃ͌���I�I
		// 
		//==============================================

		// �X�P�[���𔽉f
		D3DXMatrixScaling(&mtxScl, g_wall[nCnt].sclWall.x, g_wall[nCnt].sclWall.y, g_wall[nCnt].sclWall.z);
		D3DXMatrixMultiply(&g_wall[nCnt].mtxWorldWall, &g_wall[nCnt].mtxWorldWall, &mtxScl);

		// �����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_wall[nCnt].rotWall.y, g_wall[nCnt].rotWall.x, g_wall[nCnt].rotWall.z);
		D3DXMatrixMultiply(&g_wall[nCnt].mtxWorldWall, &g_wall[nCnt].mtxWorldWall, &mtxRot);

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTranslate, g_wall[nCnt].posWall.x, g_wall[nCnt].posWall.y, g_wall[nCnt].posWall.z);
		D3DXMatrixMultiply(&g_wall[nCnt].mtxWorldWall, &g_wall[nCnt].mtxWorldWall, &mtxTranslate);

		// �ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_wall[nCnt].mtxWorldWall);

		// ���_�o�b�t�@���f�[�^�X�g���[���Ƀo�C���h
		//-----------------------------------------
		pDevice ->SetStreamSource(0,
									g_pVtxBuffWall,
									0,
									sizeof(VERTEX_3D));	// ���_�f�[�^�̊Ԋu

		// �C���f�b�N�X�o�b�t�@�̃o�C���h
		//------------------------------------
		pDevice->SetIndices(g_pIdxBuffWall);

		// ���_�t�H�[�}�b�g�̐ݒ�
		//------------------------------------
		pDevice -> SetFVF(FVF_VERTEX_3D);

		// �e�N�X�`���̐ݒ�
		//------------------------------------
		pDevice -> SetTexture(0, g_pTextureWall[0]);

		// �|���S���̕`��
		//------------------------------------
		pDevice -> DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,	// �v���~�e�B�u�̎��
										0,						// 
										0,						// 
										g_nMaxVtx,				// ���_��
										0,						// 
										g_nDrawPrimitiveNum);	// �`�悷��v���~�e�B�u��
	}
}

//===============================================
//
// �ǂ̏����擾
//
//===============================================
WALL *GetWall(void)
{
	return &g_wall[0];
}
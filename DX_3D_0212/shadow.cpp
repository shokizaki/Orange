//===============================================
//
//  ���e   �F �C���f�b�N�X������
//				���b�V���e�Ɋւ��鏈��
//  File   �F shadow.cpp
//  ����� �F �؍�@��
//
//===============================================

//===============================================
// 
//  �C���N���[�h�t�@�C��
//
//===============================================
#include "shadow.h"
#include "model.h"
#include "input.h"
#include "camera.h"
#include "billboard.h"
#include "enemy.h"

//===============================================
//
//�@�}�N����`
//
//===============================================
#define TEXTURE_MAX 	( 1 )			// �g�p����e�N�X�`���̐�
#define SHADOW_MAX		( 100 )			// �e�̍ő吔
#define TEX_PASS		"data/TEXTURE/shadow000.jpg"

//-----------------------------------------------
//  �O���[�o���ϐ�
//-----------------------------------------------
LPDIRECT3DTEXTURE9		g_pTextureShadow[TEXTURE_MAX];			// �e�N�X�`���ւ̃|�C���^

LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffShadow = NULL;				// ���_�o�b�t�@�ւ̃|�C���^
SHADOW g_shadow[SHADOW_MAX];

static D3DXMATRIX g_mtxProjection;								// �v���W�F�N�V�����}�g���b�N�X

//===============================================
//
// �e�Ɋւ��鏉����
//
//===============================================
void InitShadow()	
{
	// ���[�J���ϐ�
	//---------------------------------
	LPDIRECT3DDEVICE9 pDevice = NULL;		// �f�o�C�X�I�u�W�F�N�g
	
	// �f�o�C�X�擾
	//------------------------------------
	pDevice = GetDevice();

	// ���_�o�b�t�@�̊m��
	//------------------------------------
	pDevice ->CreateVertexBuffer((sizeof(VERTEX_3D) * SHADOW_MAX * 4),	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@
								 D3DUSAGE_WRITEONLY,					// ���_�o�b�t�@�̎g�p���@
								 FVF_VERTEX_3D,							// �g�p���钸�_�t�H�[�}�b�g
								 D3DPOOL_MANAGED,						// �o�b�t�@��ێ����郁�����N���X�̎w��
								 &g_pVtxBuffShadow,						// ���_�o�b�t�@�ւ̃|�C���^
								 NULL);									// NULL�Œ�

	// �e������
	//------------------------------------
	g_shadow[0].posShadow = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_shadow[0].rotShadow = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_shadow[0].sclShadow = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	/*for ( int i = 0; i < SHADOW_MAX; i++ )
	{
		g_shadow[ i ].bUse = false;
	}*/

	// �e�N�X�`���ւ̃|�C���^��������
	//------------------------------------
	for (int nCnt = 0; nCnt < TEXTURE_MAX; nCnt++)				
	{
		g_pTextureShadow[nCnt] = NULL;
	}

	// �e�N�X�`���̓ǂݍ���
	//------------------------------------
	D3DXCreateTextureFromFile(pDevice, TEX_PASS, &g_pTextureShadow[ 0 ]);
}

//===============================================
//
// �e�̒��_���Z�b�g(�C���f�b�N�X�w��)
//
//===============================================
void SetupShadowEx( int num )
{
	// ���[�J���ϐ�
	//------------------------------------
	VERTEX_3D *pVtx = NULL;

	// �f�[�^�̈ʒu���m�ۂ���i���b�N�j
	//------------------------------------
	g_pVtxBuffShadow ->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0 + num * 4].pos = D3DXVECTOR3(-g_shadow[num].fWidth, 0.1f, g_shadow[num].fHeight);
	pVtx[1 + num * 4].pos = D3DXVECTOR3(g_shadow[num].fWidth,  0.1f, g_shadow[num].fHeight);
	pVtx[2 + num * 4].pos = D3DXVECTOR3(-g_shadow[num].fWidth, 0.1f, -g_shadow[num].fHeight);
	pVtx[3 + num * 4].pos = D3DXVECTOR3(g_shadow[num].fWidth,  0.1f, -g_shadow[num].fHeight);
	
	pVtx[0 + num * 4].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1 + num * 4].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2 + num * 4].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3 + num * 4].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	pVtx[0 + num * 4].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, g_shadow[ num ].a );
	pVtx[1 + num * 4].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, g_shadow[ num ].a );
	pVtx[2 + num * 4].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, g_shadow[ num ].a );
	pVtx[3 + num * 4].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, g_shadow[ num ].a );

	pVtx[0 + num * 4].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1 + num * 4].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2 + num * 4].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3 + num * 4].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_���̃A�����b�N
	//------------------------------------
 	g_pVtxBuffShadow -> Unlock();
}

//===============================================
//
// �e�Ɋւ���I������
//
//===============================================
void UninitShadow()
{
	// ���[�J���ϐ�
	//------------------------------------
	int nCnt;

	for (nCnt = 0; nCnt < TEXTURE_MAX; nCnt++)
	{
		if (g_pTextureShadow[nCnt] != NULL)
		{ 
			// �g�����e�N�X�`���̉���Ə�����
			//------------------------------------
			g_pTextureShadow[nCnt] -> Release();
			g_pTextureShadow[nCnt] = NULL;
		}
	}

	if (g_pVtxBuffShadow != NULL)
	{ 
		// ���_�o�b�t�@�̉���Ə�����
		//------------------------------------
		g_pVtxBuffShadow -> Release();
		g_pVtxBuffShadow = NULL;
	}

	for (int nCnt = 0; nCnt < SHADOW_MAX; nCnt++)	
	{
		g_shadow[nCnt].bUse = false;
	}
}	

//===============================================
//
// �e�Ɋւ���X�V����
//
//===============================================
void UpdateShadowEx( int num, D3DXVECTOR3 pos )
{
	//// �|�W�V�����̍X�V
	//g_shadow[ num ].posShadow = D3DXVECTOR3( pos.x, 0.1f, pos.z );

	//// ���_���̃Z�b�g
	//SetupShadowEx( num );
	
}

//===============================================
//
// �e�Ɋւ���`�揈��
//
//===============================================
void DrawShadow( void )
{
	// ���[�J���ϐ�
	//------------------------------------
	LPDIRECT3DDEVICE9 pDevice;						// �f�o�C�X�I�u�W�F�N�g
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;		// �v�Z�p
	CAMERA *pCamera = GetCamera();					// �J�����擾
	
	// �f�o�C�X�̎擾
	//------------------------------------
	pDevice = GetDevice();

	// �J�����̐ݒ�
	//------------------------------------
	SetCamera();

	// �����_�[�X�e�[�g�̐ݒ�i���Z�����j
	//------------------------------------
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// �A���t�@�e�X�g�̗L����
	pDevice ->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice ->SetRenderState(D3DRS_ALPHAREF,		10);
	pDevice ->SetRenderState(D3DRS_ALPHAFUNC,		D3DCMP_GREATER);

	// Z�o�b�t�@����
	//pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_GREATER);
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	for (int nCnt = 0; nCnt < SHADOW_MAX; nCnt++)
	{
		if (g_shadow[nCnt].bUse == true)
		{
			// ���[���h�}�g���b�N�X�̐ݒ�
			//------------------------------------
			D3DXMatrixIdentity(&g_shadow[nCnt].mtxWorld);			// �t�H�[�}�b�g�̏�����

			//==============================================
			// 
			// ���f���鏇�Ԃ͌���I�I
			// 
			//==============================================

			// �X�P�[���𔽉f
			D3DXMatrixScaling(&mtxScl, g_shadow[nCnt].sclShadow.x, g_shadow[nCnt].sclShadow.y, g_shadow[nCnt].sclShadow.z);
			D3DXMatrixMultiply(&g_shadow[nCnt].mtxWorld, &g_shadow[nCnt].mtxWorld, &mtxScl);

			// �����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_shadow[nCnt].rotShadow.y, g_shadow[nCnt].rotShadow.x, g_shadow[nCnt].rotShadow.z);
			D3DXMatrixMultiply(&g_shadow[nCnt].mtxWorld, &g_shadow[nCnt].mtxWorld, &mtxRot);

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTranslate, g_shadow[nCnt].posShadow.x, g_shadow[nCnt].posShadow.y, g_shadow[nCnt].posShadow.z);
			D3DXMatrixMultiply(&g_shadow[nCnt].mtxWorld, &g_shadow[nCnt].mtxWorld, &mtxTranslate);

			// �ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_shadow[nCnt].mtxWorld);

			// ���_�o�b�t�@���f�[�^�X�g���[���Ƀo�C���h
			//-----------------------------------------
			pDevice ->SetStreamSource(0,
										g_pVtxBuffShadow,
										0,
										sizeof(VERTEX_3D));	// ���_�f�[�^�̊Ԋu

			// ���_�t�H�[�}�b�g�̐ݒ�
			//------------------------------------
			pDevice -> SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			//------------------------------------
			pDevice -> SetTexture(0, g_pTextureShadow[ 0 ]);

			// �|���S���̕`��
			//------------------------------------
			pDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,	// �v���~�e�B�u�̎��
											nCnt * 4,		// 
											2);				// �`�悷��v���~�e�B�u��
		}
	}

	// �ʏ�u�����h�ɖ߂�
	//------------------------------------
	pDevice-> SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice-> SetRenderState(D3DRS_SRCBLEND	, D3DBLEND_SRCALPHA);
	pDevice-> SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//===============================================
//
// �e�̃Z�b�g(�C���f�b�N�X��Ԃ�)
//
//===============================================
int SetShadowEx(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scl, float fWidth, float fHeight, float a)
{
	int nCnt;

	for (nCnt = 0; nCnt < SHADOW_MAX; nCnt++)	
	{
		if (g_shadow[nCnt].bUse == false)
		{
			g_shadow[nCnt].posShadow = pos;
			g_shadow[nCnt].rotShadow = rot;
			g_shadow[nCnt].sclShadow = scl;
			
			g_shadow[nCnt].a = a;

			g_shadow[nCnt].fWidth = fWidth;
			g_shadow[nCnt].fHeight = fHeight;

			g_shadow[nCnt].bUse = true;

			break;
		}
	}

	// �C���f�b�N�X��Ԃ�
	return nCnt;
}

//===============================================
//
// �e�̏����擾����
//
//===============================================
SHADOW *GetShadow()
{
	return &g_shadow[0];
}

//===============================================
//
// �e�̎g�pOff
//
//===============================================
void UseShadow( int num )
{
	g_shadow[ num ].bUse = false;
}


void UpdateShadow( void )
{
	for ( int i = 0; i < SHADOW_MAX; i++ )
	{
		if ( g_shadow[ i ].bUse == true )
		{
			g_shadow[ i ].bUse = false;
		}
	}
}

void SetShadow(D3DXVECTOR3 pos, float fWidth, float fHeight, float a)
{
	int nCnt = 0;
	for (nCnt = 0; nCnt < SHADOW_MAX; nCnt++)	
	{
		if (g_shadow[nCnt].bUse == false)
		{
			g_shadow[nCnt].posShadow = pos;
			g_shadow[nCnt].rotShadow = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
			g_shadow[nCnt].sclShadow = D3DXVECTOR3( 1.0f, 1.0f, 1.0f );
			
			g_shadow[nCnt].a = a;

			g_shadow[nCnt].fWidth = fWidth;
			g_shadow[nCnt].fHeight = fHeight;

			g_shadow[nCnt].bUse = true;

			break;
		}
	}

	SetupShadowEx( nCnt );
}
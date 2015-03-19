//===============================================
//
//  ���e   �F �r���{�[�h����
//  File   �F bullet.cpp
//  ����� �F �؍�@��
//
//===============================================

//===============================================
// 
//  �C���N���[�h�t�@�C��
//
//===============================================
#include "bullet.h"
#include "shadow.h"
#include "meshField.h"
#include "camera.h"

//===============================================
//
//�@�}�N����`
//
//===============================================
#define BULLET_TEX_MAX	( 1 )
#define BULLET_WIDTH	( 15.0f )
#define BULLET_HEIGHT	( 15.0f )

#define TEX_PASS_A		"data/TEXTURE/onp001_white.png"

//-----------------------------------------------
//  �O���[�o���ϐ�
//-----------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;				// ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9		g_pTextureBullet[ BULLET_TEX_MAX ];		// �e�N�X�`���ւ̃|�C���^

BULLET g_aBullet[ BULLET_MAX ];
COL_RECT rectBullet[ BULLET_MAX ];

//===============================================
//
// �o���b�g�Ɋւ��鏉����
//
//===============================================
void InitBullet( void )
{
	// ���[�J���ϐ�
	//---------------------------------
	LPDIRECT3DDEVICE9 pDevice = NULL;		// �f�o�C�X�I�u�W�F�N�g
	
	// �f�o�C�X�擾
	//------------------------------------
	pDevice = GetDevice();

	// ���_�o�b�t�@�̊m��
	//------------------------------------
	pDevice ->CreateVertexBuffer((sizeof(VERTEX_3D) * 4 * BULLET_MAX),		// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@
								 D3DUSAGE_WRITEONLY,					// ���_�o�b�t�@�̎g�p���@
								 FVF_VERTEX_3D,							// �g�p���钸�_�t�H�[�}�b�g
								 D3DPOOL_MANAGED,						// �o�b�t�@��ێ����郁�����N���X�̎w��
								 &g_pVtxBuffBullet,					// ���_�o�b�t�@�ւ̃|�C���^
								 NULL);									// NULL�Œ�	

	// �r���{�[�h������
	//------------------------------------
	for ( int i = 0; i < BULLET_MAX; i++ )
	{
		g_aBullet[ i ].bUse = false;
	}

	// �e�N�X�`���ւ̃|�C���^��������
	//------------------------------------
	for (int nCnt = 0; nCnt < BULLET_TEX_MAX; nCnt++)				
	{
		g_pTextureBullet[ nCnt ] = NULL;
	}

	// �e�N�X�`���̓ǂݍ���
	//------------------------------------
	D3DXCreateTextureFromFile(pDevice, TEX_PASS_A, &g_pTextureBullet[ 0 ]);
}

//===============================================
//
// �o���b�g�Ɋւ���I������
//
//===============================================
void UninitBullet( void )
{
	// �g�����e�N�X�`���̉���Ə�����
	//------------------------------------
	for (int nCnt = 0; nCnt < BULLET_TEX_MAX; nCnt++)
	{
		if (g_pTextureBullet[ nCnt ] != NULL)
		{ 
			g_pTextureBullet[ nCnt ] ->Release();
			g_pTextureBullet[ nCnt ] = NULL;
		}
	}

	// ���_�o�b�t�@�̉���Ə�����
	//------------------------------------
	if (g_pVtxBuffBullet != NULL)
	{ 
		g_pVtxBuffBullet ->Release();
		g_pVtxBuffBullet = NULL;
	}
}

//===============================================
//
// �o���b�g�Ɋւ���X�V����
//
//===============================================
void UpdateBullet( void )
{
	// ���[�J���ϐ�
	//------------------------------------
	MESHFIELD *pMeshField = GetMeshField();		// ���b�V���t�B�[���h�̏����擾

	for ( int i = 0; i < BULLET_MAX; i++ )
	{
		if ( g_aBullet[ i ].bUse == true )
		{
			// �ړ�����
			g_aBullet[ i ].posBullet.x -= g_aBullet[ i ].move.x * 10.0f;
			g_aBullet[ i ].posBullet.z += g_aBullet[ i ].move.z * 10.0f;	
		
			// �����蔻��p�ϐ����X�V
			rectBullet[ i ].pos = g_aBullet[ i ].posBullet;

			// �ǂɓ���������
			//------------------------------------
			if (g_aBullet[ i ].posBullet.x - g_aBullet[ i ].fWidth < -(pMeshField ->nNumX * pMeshField ->fWidth) / 2 
			||	g_aBullet[ i ].posBullet.x + g_aBullet[ i ].fWidth > (pMeshField ->nNumX * pMeshField ->fWidth) / 2)
			{
				g_aBullet[ i ].bUse = false;		// �g�pOFF
				UseShadow( g_aBullet[ i ].nShadowNum );
			}
			if (g_aBullet[ i ].posBullet.z - g_aBullet[ i ].fHeight < -(pMeshField ->nNumZ * pMeshField ->fHeight) / 2
			||	g_aBullet[ i ].posBullet.z + g_aBullet[ i ].fHeight > (pMeshField ->nNumZ * pMeshField ->fHeight) / 2)
			{
				g_aBullet[ i ].bUse = false;		// �g�pOFF
				UseShadow( g_aBullet[ i ].nShadowNum );
			}

			// �e�̍X�V�i�C���f�b�N�X�w��j
			//UpdateShadowEx( g_aBullet[ i ].nShadowNum, g_aBullet[ i ].posBullet );
			SetShadow( D3DXVECTOR3( g_aBullet[ i ].posBullet.x, 0.1f, g_aBullet[ i ].posBullet.z ), g_aBullet[ i ].fWidth, g_aBullet[ i ].fHeight, 0.7f );
		}
	}

	// �r���{�[�h�̒��_���ƃC���f�b�N�X�̃Z�b�g
	//-------------------------------------------
	SetupBullet();
}

//===============================================
//
// �o���b�g�Ɋւ���`�揈��
//
//===============================================
void DrawBullet( void )
{
	// ���[�J���ϐ�
	//------------------------------------
	LPDIRECT3DDEVICE9 pDevice;						// �f�o�C�X�I�u�W�F�N�g
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;		// �v�Z�p
	CAMERA *pCamera = GetCamera();					// �J�����擾
	
	// �f�o�C�X�̎擾
	//------------------------------------
	pDevice = GetDevice();

	// ���C�g�̖�����
	//------------------------------------
	pDevice ->LightEnable(0, FALSE);
	pDevice ->LightEnable(2, FALSE);

	// �J�����̐ݒ�
	//------------------------------------
	SetCamera();

	// �A���t�@�e�X�g�̗L����
	//------------------------------------
	pDevice ->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice ->SetRenderState(D3DRS_ALPHAREF,		255);
	pDevice ->SetRenderState(D3DRS_ALPHAFUNC,		D3DCMP_EQUAL);

	for (int nCnt = 0; nCnt < BULLET_MAX; nCnt++)
	{
		if (g_aBullet[nCnt].bUse == true)
		{
			// ���[���h�}�g���b�N�X�̐ݒ�
			//------------------------------------
			D3DXMatrixIdentity(&g_aBullet[nCnt].mtxWorldBullet);			// �t�H�[�}�b�g�̏�����

			// �t�s������߂�
			D3DXMatrixInverse(&g_aBullet[nCnt].mtxWorldBullet, NULL, &pCamera->mtxView);

			// �O�̂��߂̏�����
			g_aBullet[nCnt].mtxWorldBullet._41 = 0.0f;
			g_aBullet[nCnt].mtxWorldBullet._42 = 0.0f;
			g_aBullet[nCnt].mtxWorldBullet._43 = 0.0f;

			//==============================================
			// 
			// ���f���鏇�Ԃ͌���I�I
			// 
			//==============================================

			// �X�P�[���𔽉f
			D3DXMatrixScaling(&mtxScl, g_aBullet[nCnt].sclBullet.x, g_aBullet[nCnt].sclBullet.y, g_aBullet[nCnt].sclBullet.z);
			D3DXMatrixMultiply(&g_aBullet[nCnt].mtxWorldBullet, &g_aBullet[nCnt].mtxWorldBullet, &mtxScl);

			// �����𔽉f
			//D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aBullet[nCnt].rotBullet.y, g_aBullet[nCnt].rotBullet.x, g_aBullet[nCnt].rotBullet.z);
			//D3DXMatrixMultiply(&g_aBullet[nCnt].mtxWorldBullet, &g_aBullet[nCnt].mtxWorldBullet, &mtxRot);

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTranslate, g_aBullet[nCnt].posBullet.x, g_aBullet[nCnt].posBullet.y, g_aBullet[nCnt].posBullet.z);
			D3DXMatrixMultiply(&g_aBullet[nCnt].mtxWorldBullet, &g_aBullet[nCnt].mtxWorldBullet, &mtxTranslate);

			// �ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aBullet[nCnt].mtxWorldBullet);

			// ���_�o�b�t�@���f�[�^�X�g���[���Ƀo�C���h
			//-----------------------------------------
			pDevice ->SetStreamSource(0,
										g_pVtxBuffBullet,
										0,
										sizeof(VERTEX_3D));	// ���_�f�[�^�̊Ԋu

			// ���_�t�H�[�}�b�g�̐ݒ�
			//------------------------------------
			pDevice -> SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			//------------------------------------
			pDevice -> SetTexture( 0, g_pTextureBullet[ 0 ] );

			// �|���S���̕`��
			//------------------------------------
			pDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,	// �v���~�e�B�u�̎��
											nCnt * 4,		// �ǂݍ��ލŏ��̔ԍ�
											2);				// �`�悷��v���~�e�B�u��
		}
	}

	// �A���t�@�e�X�g�̖�����
	pDevice ->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	// ���C�g�̗L����
	pDevice ->LightEnable(0, TRUE);
	pDevice ->LightEnable(2, TRUE);
}

//===============================================
//
// �o���b�g�̒��_���Z�b�g
//
//===============================================
void SetupBullet( void )
{
	// ���[�J���ϐ�
	//------------------------------------
	VERTEX_3D *pVtx;

	// �f�[�^�̈ʒu���m�ۂ���i���b�N�j
	//------------------------------------
	g_pVtxBuffBullet ->Lock(0, 0, (void**)&pVtx, 0);

	for ( int nCnt = 0; nCnt < BULLET_MAX; nCnt++ )
	{
		if (g_aBullet[ nCnt ].bUse == true)
		{
			pVtx[ 0 + nCnt * 4 ].pos = D3DXVECTOR3(-g_aBullet[ nCnt ].fWidth, g_aBullet[ nCnt ].fHeight, 0.0f);
			pVtx[ 1 + nCnt * 4 ].pos = D3DXVECTOR3(g_aBullet[ nCnt ].fWidth, g_aBullet[ nCnt ].fHeight, 0.0f);
			pVtx[ 2 + nCnt * 4 ].pos = D3DXVECTOR3(-g_aBullet[ nCnt ].fWidth, -g_aBullet[ nCnt ].fHeight, 0.0f);
			pVtx[ 3 + nCnt * 4 ].pos = D3DXVECTOR3(g_aBullet[ nCnt ].fWidth, -g_aBullet[ nCnt ].fHeight, 0.0f);
	
			pVtx[ 0 + nCnt * 4 ].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[ 1 + nCnt * 4 ].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[ 2 + nCnt * 4 ].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[ 3 + nCnt * 4 ].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			pVtx[ 0 + nCnt * 4 ].col = g_aBullet[ nCnt ].color;
			pVtx[ 1 + nCnt * 4 ].col = g_aBullet[ nCnt ].color;
			pVtx[ 2 + nCnt * 4 ].col = g_aBullet[ nCnt ].color;
			pVtx[ 3 + nCnt * 4 ].col = g_aBullet[ nCnt ].color;

			pVtx[ 0 + nCnt * 4 ].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[ 1 + nCnt * 4 ].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[ 2 + nCnt * 4 ].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[ 3 + nCnt * 4 ].tex = D3DXVECTOR2(1.0f, 1.0f);
		}
	}

	// ���_���̃A�����b�N
	//------------------------------------
	g_pVtxBuffBullet -> Unlock();
}

//===============================================
//
// �o���b�g�̃Z�b�g
//
//===============================================
void SetBullet( D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scl, D3DXVECTOR3 move, BULLETTYPE type )
{
	for ( int i = 0; i < BULLET_MAX; i++ )
	{
		if (g_aBullet[ i ].bUse == false)
		{
			g_aBullet[ i ].posBullet.x = pos.x;
			g_aBullet[ i ].posBullet.y = pos.y + 20.0f;
			g_aBullet[ i ].posBullet.z = pos.z;
			g_aBullet[ i ].rotBullet = rot;
			g_aBullet[ i ].sclBullet = scl;

			g_aBullet[ i ].move = move;
			g_aBullet[ i ].fWidth = BULLET_WIDTH;
			g_aBullet[ i ].fHeight = BULLET_HEIGHT;

			g_aBullet[ i ].type = type;

			switch ( g_aBullet[ i ].type )
			{
			case BULLETTYPE_ONP_RED:
				g_aBullet[ i ].color = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

				break;

			case BULLETTYPE_ONP_YELLOW:
				g_aBullet[ i ].color = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);

				break;

			case BULLETTYPE_ONP_BLUE:
				g_aBullet[ i ].color = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);

				break;
			}

			g_aBullet[ i ].bUse = true;

			rectBullet[ i ].pos = g_aBullet[ i ].posBullet;
			rectBullet[ i ].harfSize.x = g_aBullet[ i ].fWidth;
			rectBullet[ i ].harfSize.z = g_aBullet[ i ].fHeight;

			// �e�̐����i�C���f�b�N�X�w��j
			g_aBullet[ i ].nShadowNum = SetShadowEx( pos, rot, scl, 5.0f, 5.0f, 1.0f );

			break;
		}
	}
}

//===============================================
//
// �o���b�g�̏��擾
//
//===============================================
BULLET *GetBullet( void )
{
	return &g_aBullet[ 0 ];
}

//===============================================
//
// �o���b�g�̓����蔻����擾
//
//===============================================
COL_RECT *GetRectBullet( void )
{
	return &rectBullet[ 0 ];
}
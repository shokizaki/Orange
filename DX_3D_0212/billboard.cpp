//===============================================
//
//  ���e   �F �r���{�[�h����
//  File   �F billboard.cpp
//  ����� �F �؍�@��
//
//===============================================

//===============================================
// 
//  �C���N���[�h�t�@�C��
//
//===============================================
#include "billboard.h"
#include "model.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "meshField.h"
#include "enemy.h"
#include "collision.h"

//===============================================
//
//�@�}�N����`
//
//===============================================
#define TEXTURE_MAX 	( 3 )			// �g�p����e�N�X�`���̐�
#define BILL_WIDTH		( 15.0f )
#define BILL_HEIGHT		( 15.0f )

#define TEX_EMOTION		"data/TEXTURE/emotion.png"
#define TEX_CHARGE		"data/TEXTURE/ef002.png"
#define TEX_TITLE		"data/TEXTURE/ringleDash_backTitle.jpg"

//-----------------------------------------------
//  �O���[�o���ϐ�
//-----------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBillboard = NULL;				// ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9		g_pTextureBillboard[TEXTURE_MAX];		// �e�N�X�`���ւ̃|�C���^

BILLBOARD g_aBillboard[ BILLBOARD_MAX ];		// �r���{�[�h�̍\���̕ϐ�
COL_RECT rectBillboard[ BILLBOARD_MAX ];
int nShadow[ 10 ];

D3DXMATRIX g_mtxProjectionBillboard;		// �v���W�F�N�V�����}�g���b�N�X

//===============================================
//
// �o���b�g�Ɋւ��鏉����
//
//===============================================
void InitBillboard( void )
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
								 &g_pVtxBuffBillboard,					// ���_�o�b�t�@�ւ̃|�C���^
								 NULL);									// NULL�Œ�	

	// �r���{�[�h������
	//------------------------------------
	for ( int i = 0; i < BULLET_MAX; i++ )
	{
		g_aBillboard[ i ].bUse = false;
	}

	// �e�N�X�`���ւ̃|�C���^��������
	//------------------------------------
	for (int nCnt = 0; nCnt < TEXTURE_MAX; nCnt++)				
	{
		g_pTextureBillboard[ nCnt ] = NULL;
	}

	// �e�N�X�`���̓ǂݍ���
	//------------------------------------
	D3DXCreateTextureFromFile(pDevice, TEX_EMOTION, &g_pTextureBillboard[ 0 ]);
	D3DXCreateTextureFromFile(pDevice, TEX_CHARGE, &g_pTextureBillboard[ 1 ]);
	D3DXCreateTextureFromFile(pDevice, TEX_TITLE, &g_pTextureBillboard[ 2 ]);
}

//===============================================
//
// �o���b�g�Ɋւ���I������
//
//===============================================
void UninitBillboard( void )
{
	// �g�����e�N�X�`���̉���Ə�����
	//------------------------------------
	for (int nCnt = 0; nCnt < TEXTURE_MAX; nCnt++)
	{
		if (g_pTextureBillboard[ nCnt ] != NULL)
		{ 
			g_pTextureBillboard[ nCnt ] ->Release();
			g_pTextureBillboard[ nCnt ] = NULL;
		}
	}

	// ���_�o�b�t�@�̉���Ə�����
	//------------------------------------
	if (g_pVtxBuffBillboard != NULL)
	{ 
		g_pVtxBuffBillboard ->Release();
		g_pVtxBuffBillboard = NULL;
	}
}

//===============================================
//
// �o���b�g�Ɋւ���X�V����
//
//===============================================
void UpdateBillboard( void )
{
	// ���[�J���ϐ�
	//------------------------------------
	VERTEX_3D *pVtx3D;

	for ( int i = 0; i < BILLBOARD_MAX; i++ )
	{
		if ( g_aBillboard[ i ].bUse == true )
		{
			if ( g_aBillboard[ i ].texAnim != TEXTUREANIM_TITLE )
			{
				// �e�N�X�`�����W�̐ؑ�
				if ( ( ++g_aBillboard[ i ].nFrameCount % g_aBillboard[ i ].nFrame ) == 0 )
				{
					g_aBillboard[ i ].nTexNumX++;
				
					if ( g_aBillboard[ i ].nTexNumX == g_aBillboard[ i ].nTexDevideX )
					{
						if ( g_aBillboard[ i ].bLoop == true )
						{
							g_aBillboard[ i ].nTexNumX = 0;
						}
						else
						{
							g_aBillboard[ i ].bUse = false;		// �g�p��OFF
							SetCollision( false );				// �G�ƃv���C���[�̓����蔻���OFF
						}
					}
				}
		
				// ���b�N
				g_pVtxBuffBillboard ->Lock( 0, 0, ( void** )&pVtx3D, 0 );

				// �e�N�X�`���̎�ނ̔���
				switch ( g_aBillboard[ i ].texAnim )
				{
				case TEXTUREANIM_HASTY:

					pVtx3D[ 0 + i * 4 ].tex = D3DXVECTOR2( ( 1.0f / 30 * 3 ) + ( ( float )g_aBillboard[ i ].nTexNumX / 30 ), ( 1.0f / 9 ) * 2 );
					pVtx3D[ 1 + i * 4 ].tex = D3DXVECTOR2( ( 1.0f / 30 * 4 ) + ( ( float )g_aBillboard[ i ].nTexNumX / 30 ), ( 1.0f / 9 ) * 2 );
					pVtx3D[ 2 + i * 4 ].tex = D3DXVECTOR2( ( 1.0f / 30 * 3 ) + ( ( float )g_aBillboard[ i ].nTexNumX / 30 ), ( 1.0f / 9 ) * 3 );
					pVtx3D[ 3 + i * 4 ].tex = D3DXVECTOR2( ( 1.0f / 30 * 4 ) + ( ( float )g_aBillboard[ i ].nTexNumX / 30 ), ( 1.0f / 9 ) * 3 );

					break;

				// [ �G�t�F�N�g000 ]
				case TEXTUREANIM_CHARGE:
			
					pVtx3D[ 0 + i * 4 ].tex = D3DXVECTOR2( 0.0f + ( ( float )g_aBillboard[ i ].nTexNumX / g_aBillboard[ i ].nTexDevideX ),									   0.0f );
					pVtx3D[ 1 + i * 4 ].tex = D3DXVECTOR2( ( 1.0f / g_aBillboard[ i ].nTexDevideX ) + ( ( float )g_aBillboard[ i ].nTexNumX / g_aBillboard[ i ].nTexDevideX ), 0.0f );
					pVtx3D[ 2 + i * 4 ].tex = D3DXVECTOR2( 0.0f + ( ( float )g_aBillboard[ i ].nTexNumX / g_aBillboard[ i ].nTexDevideX ),									   1.0f );
					pVtx3D[ 3 + i * 4 ].tex = D3DXVECTOR2( ( 1.0f / g_aBillboard[ i ].nTexDevideX ) + ( ( float )g_aBillboard[ i ].nTexNumX / g_aBillboard[ i ].nTexDevideX ), 1.0f );

					break;
				}

				// �A�����b�N
				g_pVtxBuffBillboard ->Unlock();
			}
		}
	}

	// �r���{�[�h�̒��_���ƃC���f�b�N�X�̃Z�b�g
	//-------------------------------------------
	SetupBillboard();
}

//===============================================
//
// �o���b�g�Ɋւ���`�揈��
//
//===============================================
void DrawBillboard( void )
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

	for (int nCnt = 0; nCnt < BILLBOARD_MAX; nCnt++)
	{
		if (g_aBillboard[nCnt].bUse == true)
		{
			// ���[���h�}�g���b�N�X�̐ݒ�
			//------------------------------------
			D3DXMatrixIdentity(&g_aBillboard[nCnt].mtxWorldBillboard);			// �t�H�[�}�b�g�̏�����

			// �t�s������߂�
			D3DXMatrixInverse(&g_aBillboard[nCnt].mtxWorldBillboard, NULL, &pCamera->mtxView);

			// �O�̂��߂̏�����
			g_aBillboard[nCnt].mtxWorldBillboard._41 = 0.0f;
			g_aBillboard[nCnt].mtxWorldBillboard._42 = 0.0f;
			g_aBillboard[nCnt].mtxWorldBillboard._43 = 0.0f;

			//==============================================
			// 
			// ���f���鏇�Ԃ͌���I�I
			// 
			//==============================================

			// �X�P�[���𔽉f
			D3DXMatrixScaling(&mtxScl, g_aBillboard[nCnt].sclBillboard.x, g_aBillboard[nCnt].sclBillboard.y, g_aBillboard[nCnt].sclBillboard.z);
			D3DXMatrixMultiply(&g_aBillboard[nCnt].mtxWorldBillboard, &g_aBillboard[nCnt].mtxWorldBillboard, &mtxScl);

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTranslate, g_aBillboard[nCnt].posBillboard.x, g_aBillboard[nCnt].posBillboard.y, g_aBillboard[nCnt].posBillboard.z);
			D3DXMatrixMultiply(&g_aBillboard[nCnt].mtxWorldBillboard, &g_aBillboard[nCnt].mtxWorldBillboard, &mtxTranslate);

			// �ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aBillboard[nCnt].mtxWorldBillboard);

			// ���_�o�b�t�@���f�[�^�X�g���[���Ƀo�C���h
			//-----------------------------------------
			pDevice ->SetStreamSource(0,
										g_pVtxBuffBillboard,
										0,
										sizeof(VERTEX_3D));	// ���_�f�[�^�̊Ԋu

			// ���_�t�H�[�}�b�g�̐ݒ�
			//------------------------------------
			pDevice -> SetFVF(FVF_VERTEX_3D);


			switch ( g_aBillboard[ nCnt ].texAnim )
			{
			// [ �G�t�F�N�g000 ]
			case TEXTUREANIM_CHARGE:
			
				// ���Z����
				//------------------------------------
				pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
				pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

				break;

			default:

				break;
			}

			// �e�N�X�`���̐ݒ�
			//------------------------------------
  			pDevice -> SetTexture( 0, g_pTextureBillboard[ ( int )g_aBillboard[ nCnt ].texAnim ] );

			// �|���S���̕`��
			//------------------------------------
			pDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,	// �v���~�e�B�u�̎��
											nCnt * 4,		// �ǂݍ��ލŏ��̔ԍ�
											2);				// �`�悷��v���~�e�B�u��

			// ���ɖ߂�
			pDevice-> SetRenderState(D3DRS_SRCBLEND	, D3DBLEND_SRCALPHA);
			pDevice-> SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
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
void SetupBillboard( void )
{
	// ���[�J���ϐ�
	//------------------------------------
	VERTEX_3D *pVtx;

	// �f�[�^�̈ʒu���m�ۂ���i���b�N�j
	//------------------------------------
	g_pVtxBuffBillboard ->Lock(0, 0, (void**)&pVtx, 0);

	for ( int num = 0; num < BILLBOARD_MAX; num++ )
	{
		if (g_aBillboard[ num ].bUse == true)
		{
			pVtx[ 0 + num * 4 ].pos = D3DXVECTOR3(-g_aBillboard[ num ].fWidth, g_aBillboard[ num ].fHeight, 0.0f);
			pVtx[ 1 + num * 4 ].pos = D3DXVECTOR3(g_aBillboard[ num ].fWidth, g_aBillboard[ num ].fHeight, 0.0f);
			pVtx[ 2 + num * 4 ].pos = D3DXVECTOR3(-g_aBillboard[ num ].fWidth, -g_aBillboard[ num ].fHeight, 0.0f);
			pVtx[ 3 + num * 4 ].pos = D3DXVECTOR3(g_aBillboard[ num ].fWidth, -g_aBillboard[ num ].fHeight, 0.0f);
	
			pVtx[ 0 + num * 4 ].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[ 1 + num * 4 ].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[ 2 + num * 4 ].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[ 3 + num * 4 ].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			pVtx[ 0 + num * 4 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );
			pVtx[ 1 + num * 4 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );
			pVtx[ 2 + num * 4 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );
			pVtx[ 3 + num * 4 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );

			//pVtx[ 0 + num * 4 ].tex = D3DXVECTOR2(0.0f, 0.0f);
			//pVtx[ 1 + num * 4 ].tex = D3DXVECTOR2(1.0f, 0.0f);
			//pVtx[ 2 + num * 4 ].tex = D3DXVECTOR2(0.0f, 1.0f);
			//pVtx[ 3 + num * 4 ].tex = D3DXVECTOR2(1.0f, 1.0f);
		}
	}

	// ���_���̃A�����b�N
	//------------------------------------
	g_pVtxBuffBillboard -> Unlock();
}

//===============================================
//
// �o���b�g�̃Z�b�g
//
//===============================================
int SetBillboard( D3DXVECTOR3 pos, TEXTURE_ANIM animType, float fParentFromHeight, float fHeight, float fWidth )
{
	VERTEX_3D *pVtx3D;

	for ( int i = 0; i < BILLBOARD_MAX; i++ )
	{
		if ( g_aBillboard[ i ].bUse == false )
		{
			g_aBillboard[ i ].posBillboard = pos;								// �ʒu
			g_aBillboard[ i ].rotBillboard = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );	// �p�x
			g_aBillboard[ i ].sclBillboard = D3DXVECTOR3( 1.0f, 1.0f, 1.0f );	// �X�P�[��

			g_aBillboard[ i ].fWidth = fWidth;			// ��
			g_aBillboard[ i ].fHeight = fHeight;		// ����
			g_aBillboard[ i ].fHeightFromParent = fParentFromHeight;	// �e����̍���

			g_aBillboard[ i ].texAnim = animType;			// �e�N�X�`���̎��
			g_aBillboard[ i ].nFrameCount = 0;

			// �g�������e�N�X�`���̎��
			switch ( animType )
			{
				// [ �ł� ]
			case TEXTUREANIM_HASTY:
				g_aBillboard[ i ].nFrame = 30;		// �A�j���[�V�����̐؂�ւ��^�C�~���O�i�t���[���j
				g_aBillboard[ i ].nTexDevideX = 2;	// �e�N�X�`���ɉ�������
				g_aBillboard[ i ].nTexDevideY = 1;	// �e�N�X�`���̏c������

				g_aBillboard[ i ].nTexNumX = 0;		// �Y������
				g_aBillboard[ i ].nTexNumY = 0;		// �Y������

				g_aBillboard[ i ].bLoop = true;	// ���[�v

				break;

			// [ �G�t�F�N�g ]
			case TEXTUREANIM_CHARGE:
				g_aBillboard[ i ].nFrame = 5;		// �A�j���[�V�����̐؂�ւ��^�C�~���O�i�t���[���j
				g_aBillboard[ i ].nTexDevideX = 10;	// �e�N�X�`���ɉ�������
				g_aBillboard[ i ].nTexDevideY = 1;	// �e�N�X�`���̏c������

				g_aBillboard[ i ].nTexNumX = 0;		// �Y������
				g_aBillboard[ i ].nTexNumY = 0;		// �Y������

				g_aBillboard[ i ].bLoop = false;	// ���[�v

				break;

			case TEXTUREANIM_TITLE:

				// ���b�N
				g_pVtxBuffBillboard ->Lock( 0, 0, ( void** )&pVtx3D, 0 );

				pVtx3D[ 0 + i * 4 ].pos = D3DXVECTOR3(-g_aBillboard[ i ].fWidth, g_aBillboard[ i ].fHeight, 0.0f);
				pVtx3D[ 1 + i * 4 ].pos = D3DXVECTOR3(g_aBillboard[ i ].fWidth, g_aBillboard[ i ].fHeight, 0.0f);
				pVtx3D[ 2 + i * 4 ].pos = D3DXVECTOR3(-g_aBillboard[ i ].fWidth, -g_aBillboard[ i ].fHeight, 0.0f);
				pVtx3D[ 3 + i * 4 ].pos = D3DXVECTOR3(g_aBillboard[ i ].fWidth, -g_aBillboard[ i ].fHeight, 0.0f);
	
				pVtx3D[ 0 + i * 4 ].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
				pVtx3D[ 1 + i * 4 ].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
				pVtx3D[ 2 + i * 4 ].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
				pVtx3D[ 3 + i * 4 ].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

				pVtx3D[ 0 + i * 4 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );
				pVtx3D[ 1 + i * 4 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );
				pVtx3D[ 2 + i * 4 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );
				pVtx3D[ 3 + i * 4 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );

				pVtx3D[ 0 + i * 4 ].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx3D[ 1 + i * 4 ].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx3D[ 2 + i * 4 ].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx3D[ 3 + i * 4 ].tex = D3DXVECTOR2(1.0f, 1.0f);

				g_pVtxBuffBillboard ->Unlock();

				break;
			}

			g_aBillboard[ i ].bUse = true;	// �g�p���ɂ���

			return i;	// �C���f�b�N�X��Ԃ�
		}
	}

	return NULL;
}

//===============================================
//
// �o���b�g�̏��擾
//
//===============================================
BILLBOARD *GetBillboard( void )
{
	return &g_aBillboard[ 0 ];
}

//===============================================
//
// �o���b�g�̓����蔻����擾
//
//===============================================
COL_RECT *GetRectBillboard( void )
{
	return &rectBillboard[ 0 ];
}

//===============================================
//
// �r���{�[�h�̈ʒu��ݒ�
//
//===============================================
void SetPosBillboard( int num, D3DXVECTOR3 pos )
{
	g_aBillboard[ num ].posBillboard.x = pos.x;
	g_aBillboard[ num ].posBillboard.y = pos.y + g_aBillboard[ num ].fHeightFromParent;
	g_aBillboard[ num ].posBillboard.z = pos.z;
}

//===============================================
//
// �r���{�[�h��OFF
//
//===============================================
void SetUseBillboard( int num )
{
	g_aBillboard[ num ].bUse = false;
}
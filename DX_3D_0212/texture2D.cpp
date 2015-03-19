//===============================================
//
//  ���e   �F ���[�U�[�C���^�[�t�F�[�X����
//  File   �F Texture2D.h
//  ����� �F �؍�@��
//
//===============================================

//------ �C���N���[�h�t�@�C�� ------
#include "texture2D.h"

//------ �}�N����` ------
#define TEXTURE2D_TEX_MAX	( 5 )

#define TEXTURE2D_EF000		"data/TEXTURE/ef000.png"
#define TEXTURE2D_EF001		"data/TEXTURE/ef001.png"
#define TEXTURE2D_EMOTION	"data/TEXTURE/emotion.png"
#define TEXTURE2D_HINTBOX	"data/TEXTURE/hintBox.png"


//------ �O���[�o���ϐ� ------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTexture2D = NULL;			// ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9		g_pTextureTexture2D[ TEXTURE2D_TEX_MAX ];		// �e�N�X�`���ւ̃|�C���^

TEXTURE2D g_aTexture2D[ TEXTURE2D_MAX ];

D3DXMATRIX g_mtxProjectionTexture2D;		// �v���W�F�N�V�����}�g���b�N�X

//===============================================
//
// Texture2D�Ɋւ��鏉����
//
//===============================================
void InitTexture2D( void )
{
	// ���[�J���ϐ�
	//---------------------------------
	LPDIRECT3DDEVICE9 pDevice = NULL;		// �f�o�C�X�I�u�W�F�N�g

	// �O���[�o���ϐ��̏�����
	//---------------------------------


	// �f�o�C�X�擾
	//------------------------------------
	pDevice = GetDevice();

	// ���_�o�b�t�@�̊m��
	//------------------------------------
	pDevice ->CreateVertexBuffer((sizeof(VERTEX_2D) * 4 * TEXTURE2D_MAX),		// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@
								 D3DUSAGE_WRITEONLY,					// ���_�o�b�t�@�̎g�p���@
								 FVF_VERTEX_2D,							// �g�p���钸�_�t�H�[�}�b�g
								 D3DPOOL_MANAGED,						// �o�b�t�@��ێ����郁�����N���X�̎w��
								 &g_pVtxBuffTexture2D,							// ���_�o�b�t�@�ւ̃|�C���^
								 NULL);									// NULL�Œ�	

	// �e�N�X�`���ւ̃|�C���^��������
	//------------------------------------
	for (int nCnt = 0; nCnt < TEXTURE2D_TEX_MAX; nCnt++)				
	{
		g_pTextureTexture2D[ nCnt ] = NULL;
	}

	// �e�N�X�`���̓ǂݍ���
	//------------------------------------
	D3DXCreateTextureFromFile(pDevice, TEXTURE2D_EF000, &g_pTextureTexture2D[ 0 ]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE2D_EF001, &g_pTextureTexture2D[ 1 ]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE2D_EMOTION, &g_pTextureTexture2D[ 2 ]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE2D_HINTBOX, &g_pTextureTexture2D[ 3 ]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE2D_HINTBOX, &g_pTextureTexture2D[ 4 ]);

	// ���_���̃Z�b�g
	//------------------------------------
	SetupTexture2D();
}

//===============================================
//
// Texture2D�̒��_���Z�b�g
//
//===============================================
void SetupTexture2D( void )
{
	// ���[�J���ϐ�
	//---------------------------------
	LPDIRECT3DDEVICE9 pDevice = NULL;		// �f�o�C�X�I�u�W�F�N�g
	VERTEX_2D *pVtx2D;

	// �f�o�C�X�擾
	//------------------------------------
	pDevice = GetDevice();

	g_pVtxBuffTexture2D ->Lock( 0, 0, (void**)&pVtx2D, 0 );

	for ( int i = 0; i < TEXTURE2D_MAX; i++ )
	{
		// �ʒu
		pVtx2D[ 0 + i * 4 ].pos = D3DXVECTOR3( g_aTexture2D[ i ].pos.x - g_aTexture2D[ i ].fWidth, g_aTexture2D[ i ].pos.y - g_aTexture2D[ i ].fHeight, 0.0f );
		pVtx2D[ 1 + i * 4 ].pos = D3DXVECTOR3( g_aTexture2D[ i ].pos.x + g_aTexture2D[ i ].fWidth, g_aTexture2D[ i ].pos.y - g_aTexture2D[ i ].fHeight, 0.0f );
		pVtx2D[ 2 + i * 4 ].pos = D3DXVECTOR3( g_aTexture2D[ i ].pos.x - g_aTexture2D[ i ].fWidth, g_aTexture2D[ i ].pos.y + g_aTexture2D[ i ].fHeight, 0.0f );
		pVtx2D[ 3 + i * 4 ].pos = D3DXVECTOR3( g_aTexture2D[ i ].pos.x + g_aTexture2D[ i ].fWidth, g_aTexture2D[ i ].pos.y + g_aTexture2D[ i ].fHeight, 0.0f );

		// �p�[�X�y�N�e�B�u�R���N�g���F
		for ( int j = 0; j < 4; j++ )
		{
			pVtx2D[ j + i * 4 ].rhw = 1.0f;
			pVtx2D[ j + i * 4 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );
		}

		// �e�N�X�`�����W
		//pVtx2D[ 0 + i * 4  ].tex = D3DXVECTOR2( 0.0f, 0.0f );
		//pVtx2D[ 1 + i * 4  ].tex = D3DXVECTOR2( 1.0f, 0.0f );
		//pVtx2D[ 2 + i * 4  ].tex = D3DXVECTOR2( 0.0f, 1.0f );
		//pVtx2D[ 3 + i * 4  ].tex = D3DXVECTOR2( 1.0f, 1.0f );
	}

	g_pVtxBuffTexture2D ->Unlock();
}

//===============================================
//
// Texture2D�Ɋւ���I������
//
//===============================================
void UninitTexture2D( void )
{
	// �g�����e�N�X�`���̉���Ə�����
	//------------------------------------
	for (int nCnt = 0; nCnt < TEXTURE2D_TEX_MAX; nCnt++)
	{
		if (g_pTextureTexture2D[ nCnt ] != NULL)
		{ 
			g_pTextureTexture2D[ nCnt ] ->Release();
			g_pTextureTexture2D[ nCnt ] = NULL;
		}
	}

	// ���_�o�b�t�@�̉���Ə�����
	//------------------------------------
	if (g_pVtxBuffTexture2D != NULL)
	{ 
		g_pVtxBuffTexture2D ->Release();
		g_pVtxBuffTexture2D = NULL;
	}
}

//===============================================
//
// Texture2D�Ɋւ���X�V����
//
//===============================================
void UpdateTexture2D( void )
{
	VERTEX_2D *pVtx2D;

	g_pVtxBuffTexture2D ->Lock( 0, 0, (void**)&pVtx2D, 0 );

	for ( int i = 0; i < TEXTURE2D_MAX; i++ )
	{
		if ( g_aTexture2D[ i ].bUse == true )
		{
			// �e�N�X�`�����W�̍X�V
			g_aTexture2D[ i ].nFrameCount++;

			if ( ( ++g_aTexture2D[ i ].nFrameCount % g_aTexture2D[ i ].nFrame ) == 0 )
			{
				g_aTexture2D[ i ].nTexNumX++;

				if ( g_aTexture2D[ i ].nTexNumX == g_aTexture2D[ i ].nTexDevideX )
				{
					if ( g_aTexture2D[ i ].bLoop == true )
					{
						g_aTexture2D[ i ].nTexNumX = 0;
					}
					else
					{
						g_aTexture2D[ i ].bUse = false;
					}
				}
			} 

			switch ( g_aTexture2D[ i ].type )
			{
				// [ �G�t�F�N�g000 ]
			case TEXTURE2DTYPE_000:
			
				pVtx2D[ 0 + i * 4 ].tex = D3DXVECTOR2( 0.0f + ( ( float )g_aTexture2D[ i ].nTexNumX / g_aTexture2D[ i ].nTexDevideX ),									   0.0f );
				pVtx2D[ 1 + i * 4 ].tex = D3DXVECTOR2( ( 1.0f / g_aTexture2D[ i ].nTexDevideX ) + ( ( float )g_aTexture2D[ i ].nTexNumX / g_aTexture2D[ i ].nTexDevideX ), 0.0f );
				pVtx2D[ 2 + i * 4 ].tex = D3DXVECTOR2( 0.0f + ( ( float )g_aTexture2D[ i ].nTexNumX / g_aTexture2D[ i ].nTexDevideX ),									   1.0f );
				pVtx2D[ 3 + i * 4 ].tex = D3DXVECTOR2( ( 1.0f / g_aTexture2D[ i ].nTexDevideX ) + ( ( float )g_aTexture2D[ i ].nTexNumX / g_aTexture2D[ i ].nTexDevideX ), 1.0f );

				break;

				// [ �G�t�F�N�g001 ]
			case TEXTURE2DTYPE_001:

				pVtx2D[ 0 + i * 4 ].tex = D3DXVECTOR2( 0.0f + ( ( float )g_aTexture2D[ i ].nTexNumX / g_aTexture2D[ i ].nTexDevideX ),									   0.0f );
				pVtx2D[ 1 + i * 4 ].tex = D3DXVECTOR2( ( 1.0f / g_aTexture2D[ i ].nTexDevideX ) + ( ( float )g_aTexture2D[ i ].nTexNumX / g_aTexture2D[ i ].nTexDevideX ), 0.0f );
				pVtx2D[ 2 + i * 4 ].tex = D3DXVECTOR2( 0.0f + ( ( float )g_aTexture2D[ i ].nTexNumX / g_aTexture2D[ i ].nTexDevideX ),									   1.0f );
				pVtx2D[ 3 + i * 4 ].tex = D3DXVECTOR2( ( 1.0f / g_aTexture2D[ i ].nTexDevideX ) + ( ( float )g_aTexture2D[ i ].nTexNumX / g_aTexture2D[ i ].nTexDevideX ), 1.0f );

				break;

			case TEXTURE2DTYPE_HASTY2:

				pVtx2D[ 0 + i * 4 ].tex = D3DXVECTOR2( ( 1.0f / 30 * 6 ) + ( ( float )g_aTexture2D[ i ].nTexNumX / 30 ), ( 1.0f / 9 ) * 2 );
				pVtx2D[ 1 + i * 4 ].tex = D3DXVECTOR2( ( 1.0f / 30 * 7 ) + ( ( float )g_aTexture2D[ i ].nTexNumX / 30 ), ( 1.0f / 9 ) * 2 );
				pVtx2D[ 2 + i * 4 ].tex = D3DXVECTOR2( ( 1.0f / 30 * 6 ) + ( ( float )g_aTexture2D[ i ].nTexNumX / 30 ), ( 1.0f / 9 ) * 3 );
				pVtx2D[ 3 + i * 4 ].tex = D3DXVECTOR2( ( 1.0f / 30 * 7 ) + ( ( float )g_aTexture2D[ i ].nTexNumX / 30 ), ( 1.0f / 9 ) * 3 );

				break;

			case TEXTURE2DTYPE_BOX:

				pVtx2D[ 0 + i * 4 ].tex = D3DXVECTOR2( 0.0f, 0.0f );
				pVtx2D[ 1 + i * 4 ].tex = D3DXVECTOR2( 1.0f, 0.0f );
				pVtx2D[ 2 + i * 4 ].tex = D3DXVECTOR2( 0.0f, 1.0f );
				pVtx2D[ 3 + i * 4 ].tex = D3DXVECTOR2( 1.0f, 1.0f );

				break;

			case TEXTURE2DTYPE_BOXREVERS:

				pVtx2D[ 0 + i * 4 ].tex = D3DXVECTOR2( 1.0f, 0.0f );
				pVtx2D[ 1 + i * 4 ].tex = D3DXVECTOR2( 0.0f, 0.0f );
				pVtx2D[ 2 + i * 4 ].tex = D3DXVECTOR2( 1.0f, 1.0f );
				pVtx2D[ 3 + i * 4 ].tex = D3DXVECTOR2( 0.0f, 1.0f );

				break;

			default:

				break;
			}
		}
	}

	g_pVtxBuffTexture2D ->Unlock();

	// ���_���Z�b�g
	SetupTexture2D();
}

//===============================================
//
// Texture2D�Ɋւ���`�揈��
//
//===============================================
void DrawTexture2D( void )
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
								g_pVtxBuffTexture2D,
								0,
								sizeof(VERTEX_2D));	// ���_�f�[�^�̊Ԋu

	
	// ���_�t�H�[�}�b�g�̐ݒ�
	//------------------------------------
	pDevice ->SetFVF( FVF_VERTEX_2D );

	for ( int i = 0; i < TEXTURE2D_MAX; i++ )
	{
		if ( g_aTexture2D[ i ].bUse == true )
		{
			switch ( g_aTexture2D[ i ].type )
			{
				// [ �G�t�F�N�g000 ]
			case TEXTURE2DTYPE_000:
			
				// ���Z����
				//------------------------------------
				pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
				pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

				break;

				// [ �G�t�F�N�g001 ]
			case TEXTURE2DTYPE_001:
			
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
			pDevice -> SetTexture(0, g_pTextureTexture2D[ ( int )g_aTexture2D[ i ].type ]);
			
			// �|���S���̕`��
			//------------------------------------
			pDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,	// �v���~�e�B�u�̎��
											i * 4,				// �ǂݍ��ލŏ��̔ԍ�
											2);				// �`�悷��v���~�e�B�u��

			// ���ɖ߂�
			pDevice-> SetRenderState(D3DRS_SRCBLEND	, D3DBLEND_SRCALPHA);
			pDevice-> SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		}
	}
}


//===============================================
//
// Texture2D�̃Z�b�g
//
//===============================================
int SetTexture2D( D3DXVECTOR3 pos, float fWidth, float fHeight, TEXTURE2DTYPE type )
{
	for ( int i = 0; i < TEXTURE2D_MAX; i++ )
	{
		if ( g_aTexture2D[ i ].bUse == false )
		{
			g_aTexture2D[ i ].pos = pos;				// �ʒu

			g_aTexture2D[ i ].fWidth = fWidth;			// ��
			g_aTexture2D[ i ].fHeight = fHeight;		// ����

			g_aTexture2D[ i ].type = type;				// �e�N�X�`���̎��
			g_aTexture2D[ i ].nFrameCount = 0;			// �t���[���J�E���^

			// �g�������e�N�X�`���̎��
			switch ( type )
			{
				// [ �G�t�F�N�g ]
			case TEXTURE2DTYPE_000:
				g_aTexture2D[ i ].nFrame = 10;		// �A�j���[�V�����̐؂�ւ��^�C�~���O�i�t���[���j
				g_aTexture2D[ i ].nTexDevideX = 10;	// �e�N�X�`���ɉ�������
				g_aTexture2D[ i ].nTexDevideY = 1;	// �e�N�X�`���̏c������

				g_aTexture2D[ i ].nTexNumX = 0;		// �Y������
				g_aTexture2D[ i ].nTexNumY = 0;		// �Y������

				g_aTexture2D[ i ].bLoop = false;	// ���[�v

				break;

				// [ �G�t�F�N�g ]
			case TEXTURE2DTYPE_001:
				g_aTexture2D[ i ].nFrame = 10;		// �A�j���[�V�����̐؂�ւ��^�C�~���O�i�t���[���j
				g_aTexture2D[ i ].nTexDevideX = 10;	// �e�N�X�`���ɉ�������
				g_aTexture2D[ i ].nTexDevideY = 1;	// �e�N�X�`���̏c������

				g_aTexture2D[ i ].nTexNumX = 0;		// �Y������
				g_aTexture2D[ i ].nTexNumY = 0;		// �Y������

				g_aTexture2D[ i ].bLoop = false;	// ���[�v

				break;

			case TEXTURE2DTYPE_HASTY2:
				g_aTexture2D[ i ].nFrame = 15;		// �A�j���[�V�����̐؂�ւ��^�C�~���O�i�t���[���j
				g_aTexture2D[ i ].nTexDevideX = 2;	// �e�N�X�`���ɉ�������
				g_aTexture2D[ i ].nTexDevideY = 1;	// �e�N�X�`���̏c������

				g_aTexture2D[ i ].nTexNumX = 0;		// �Y������
				g_aTexture2D[ i ].nTexNumY = 0;		// �Y������

				g_aTexture2D[ i ].bLoop = true;	// ���[�v

				break;

			// [ �G�t�F�N�g ]
			case TEXTURE2DTYPE_BOXREVERS:
				g_aTexture2D[ i ].nFrame = 10;		// �A�j���[�V�����̐؂�ւ��^�C�~���O�i�t���[���j
				g_aTexture2D[ i ].nTexDevideX = 1;	// �e�N�X�`���ɉ�������
				g_aTexture2D[ i ].nTexDevideY = 1;	// �e�N�X�`���̏c������

				g_aTexture2D[ i ].nTexNumX = 0;		// �Y������
				g_aTexture2D[ i ].nTexNumY = 0;		// �Y������

				g_aTexture2D[ i ].bLoop = true;	// ���[�v

				break;

			// [ �G�t�F�N�g ]
			case TEXTURE2DTYPE_BOX:
				g_aTexture2D[ i ].nFrame = 10;		// �A�j���[�V�����̐؂�ւ��^�C�~���O�i�t���[���j
				g_aTexture2D[ i ].nTexDevideX = 1;	// �e�N�X�`���ɉ�������
				g_aTexture2D[ i ].nTexDevideY = 1;	// �e�N�X�`���̏c������

				g_aTexture2D[ i ].nTexNumX = 0;		// �Y������
				g_aTexture2D[ i ].nTexNumY = 0;		// �Y������

				g_aTexture2D[ i ].bLoop = true;	// ���[�v

				break;
			}

			g_aTexture2D[ i ].bUse = true;	// �g�p���ɂ���

			// ���_���Z�b�g
			SetupTexture2D();

			return i;
		}
	}

	return 0;
}

//===============================================
//
// Texture2D�̎g�pOFF
//
//===============================================
void SetTexture2DUse( int num )
{
	if ( g_aTexture2D[ num ].bUse == true )
	{
		g_aTexture2D[ num ].bUse = false;
	}
}
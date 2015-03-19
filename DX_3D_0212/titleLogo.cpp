//===============================================
//
//  ���e   �F �^�C�g�����S����
//  File   �F titleLogo.cpp
//  ����� �F �؍�@��
//
//===============================================

//------ �C���N���[�h�t�@�C�� ------
#include "titleLogo.h"
#include "title.h"

//------ �}�N����` ------
#define TITLELOGO_TEX_MAX	( 5 )

#define TITLELOGO_TEX			"data/TEXTURE/titleLogo.png"
#define TITLELOGO_WIDTH			( 200.0f )
#define TITLELOGO_HEIGHT		( 70.0f )
#define TITLELOGO_TARGETPOS_X	( SCREEN_WIDTH / 2 )
#define TITLELOGO_TARGETPOS_Y	( 150.0f )

#define PRESS10_TEX			"data/TEXTURE/press10button.png"
#define PRESSENTER_TEX			"data/TEXTURE/pressEnter.png"
#define PRESSENTER_WIDTH		( 150.0f )
#define PRESSENTER_HEIGHT		( 35.0f )
#define PRESSENTER_POS_X		( SCREEN_WIDTH / 2 )
#define PRESSENTER_POS_Y		( SCREEN_HEIGHT - 150.0f )

//------ �O���[�o���ϐ� ------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitleLogo = NULL;						// ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9		g_pTextureTitleLogo[ TITLELOGO_TEX_MAX ];		// �e�N�X�`���ւ̃|�C���^

TITLELOGO g_aTitleLogo[ TITLELOGO_MAX ];
int g_nFrameCountPressEnter;
float g_TitleLogoMove = 0;
int g_nCntBound = 0;

D3DXMATRIX g_mtxProjectionTitleLogo;		// �v���W�F�N�V�����}�g���b�N�X

//------ �֐��v���g�^�C�v�錾 ------

//===============================================
//
// �^�C�g�����S�Ɋւ��鏉����
//
//===============================================
void InitTitleLogo( void )
{
	// ���[�J���ϐ�
	//---------------------------------
	LPDIRECT3DDEVICE9 pDevice = NULL;		// �f�o�C�X�I�u�W�F�N�g

	// �O���[�o���ϐ��̏�����
	//---------------------------------
	g_nFrameCountPressEnter = 60;
	g_TitleLogoMove = 1.0f;
	g_nCntBound = 0;

	// [ ���S ]
	g_aTitleLogo[ 0 ].pos = D3DXVECTOR3( SCREEN_WIDTH / 2, -300.0f, 0.0f );
	g_aTitleLogo[ 0 ].fWidth =TITLELOGO_WIDTH;
	g_aTitleLogo[ 0 ].fHeight = TITLELOGO_HEIGHT;
	g_aTitleLogo[ 0 ].color = D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f );

	g_aTitleLogo[ 1 ].pos = D3DXVECTOR3( PRESSENTER_POS_X, PRESSENTER_POS_Y, 0.0f );
	g_aTitleLogo[ 1 ].fWidth =PRESSENTER_WIDTH;
	g_aTitleLogo[ 1 ].fHeight = PRESSENTER_HEIGHT;
	g_aTitleLogo[ 1 ].color = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );

	// �f�o�C�X�擾
	//------------------------------------
	pDevice = GetDevice();

	// ���_�o�b�t�@�̊m��
	//------------------------------------
	pDevice ->CreateVertexBuffer((sizeof(VERTEX_2D) * 4 * TITLELOGO_MAX),		// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@
								 D3DUSAGE_WRITEONLY,					// ���_�o�b�t�@�̎g�p���@
								 FVF_VERTEX_2D,							// �g�p���钸�_�t�H�[�}�b�g
								 D3DPOOL_MANAGED,						// �o�b�t�@��ێ����郁�����N���X�̎w��
								 &g_pVtxBuffTitleLogo,							// ���_�o�b�t�@�ւ̃|�C���^
								 NULL);									// NULL�Œ�	

	// �e�N�X�`���ւ̃|�C���^��������
	//------------------------------------
	for (int nCnt = 0; nCnt < TITLELOGO_TEX_MAX; nCnt++)				
	{
		g_pTextureTitleLogo[ nCnt ] = NULL;
	}

	// �e�N�X�`���̓ǂݍ���
	//------------------------------------
	D3DXCreateTextureFromFile(pDevice, TITLELOGO_TEX, &g_pTextureTitleLogo[ 0 ]);
	if ( GetGamePadUse() == true )
	{
		D3DXCreateTextureFromFile(pDevice, PRESS10_TEX, &g_pTextureTitleLogo[ 1 ]);
	}
	else
	{
		D3DXCreateTextureFromFile(pDevice, PRESSENTER_TEX, &g_pTextureTitleLogo[ 1 ]);
	}

	// ���_���̃Z�b�g
	//------------------------------------
	SetupTitleLogo();
}

//===============================================
//
// �^�C�g�����S�Ɋւ���I������
//
//===============================================
void UninitTitleLogo( void )
{
	// �g�����e�N�X�`���̉���Ə�����
	//------------------------------------
	for (int nCnt = 0; nCnt < TITLELOGO_TEX_MAX; nCnt++)
	{
		if (g_pTextureTitleLogo[ nCnt ] != NULL)
		{ 
			g_pTextureTitleLogo[ nCnt ] ->Release();
			g_pTextureTitleLogo[ nCnt ] = NULL;
		}
	}

	// ���_�o�b�t�@�̉���Ə�����
	//------------------------------------
	if (g_pVtxBuffTitleLogo != NULL)
	{ 
		g_pVtxBuffTitleLogo ->Release();
		g_pVtxBuffTitleLogo = NULL;
	}

}

//===============================================
//
// �^�C�g�����S�Ɋւ���X�V����
//
//===============================================
void UpdateTitleLogo( void )
{
	// ���[�J���ϐ�
	//------------------------------------
	
	// �G���^�[�L�[�������ꂽ��
	if ( GetTitleFade() == true )
	{
		// �_�ő��x�𑬂߂�
		g_nFrameCountPressEnter = 10;
		g_nCntBound = 3;
	}

	if ( GetTitleFade() == false && g_nCntBound < 3 )
	{
		// �^�C�g�����S�ړ�����
		g_TitleLogoMove += 1.f;
		g_aTitleLogo[ 0 ].pos.y += g_TitleLogoMove;

		if ( g_aTitleLogo[ 0 ].pos.y >= TITLELOGO_TARGETPOS_Y )
		{
			g_aTitleLogo[ 0 ].pos.y = TITLELOGO_TARGETPOS_Y;
			g_TitleLogoMove = -g_TitleLogoMove + 10.0f;

			g_nCntBound++;
		}
	}
	else if ( g_nCntBound == 3 )
	{
		g_aTitleLogo[ 0 ].pos.y = TITLELOGO_TARGETPOS_Y;
	}

	// ���_���̃Z�b�g
	//------------------------------------
	SetupTitleLogo();
}

//===============================================
//
// �^�C�g�����S�Ɋւ���`�揈��
//
//===============================================
void DrawTitleLogo( void )
{
	// ���[�J���ϐ�
	//------------------------------------
	LPDIRECT3DDEVICE9 pDevice;						// �f�o�C�X�I�u�W�F�N�g
	static int nFrameCount = 0;
	
	// �f�o�C�X�̎擾
	//------------------------------------
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���Ƀo�C���h
	//-----------------------------------------
	pDevice ->SetStreamSource(0,
								g_pVtxBuffTitleLogo,
								0,
								sizeof(VERTEX_2D));	// ���_�f�[�^�̊Ԋu

	
	// ���_�t�H�[�}�b�g�̐ݒ�
	//------------------------------------
	pDevice ->SetFVF( FVF_VERTEX_2D );

	// [ titleLogo ]�`��ݒ�

	// �e�N�X�`���̐ݒ�
	//----------------------- -------------
	pDevice -> SetTexture(0, g_pTextureTitleLogo[ 0 ]);

	// �|���S���̕`��
	//------------------------------------
	pDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,	// �v���~�e�B�u�̎��
									0,				// �ǂݍ��ލŏ��̔ԍ�
									2);				// �`�悷��v���~�e�B�u��


	// [ Press Enter ]�_�ŕ`��
	if ( ( nFrameCount = ++nFrameCount % g_nFrameCountPressEnter ) <= g_nFrameCountPressEnter / 2 )
	{
		// �e�N�X�`���̐ݒ�
		//----------------------- -------------
		pDevice -> SetTexture(0, g_pTextureTitleLogo[ 1 ]);

		// �|���S���̕`��
		//------------------------------------
		pDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,	// �v���~�e�B�u�̎��
										4,				// �ǂݍ��ލŏ��̔ԍ�
										2);				// �`�悷��v���~�e�B�u��
	}
}

//===============================================
//
// �^�C�g�����S�̒��_���Z�b�g
//
//===============================================
void SetupTitleLogo( void )
{
		// ���[�J���ϐ�
	//---------------------------------
	LPDIRECT3DDEVICE9 pDevice = NULL;		// �f�o�C�X�I�u�W�F�N�g
	VERTEX_2D *pVtx2D;

	// �f�o�C�X�擾
	//------------------------------------
	pDevice = GetDevice();

	g_pVtxBuffTitleLogo ->Lock( 0, 0, (void**)&pVtx2D, 0 );

	for ( int i = 0; i < TITLELOGO_MAX; i++ )
	{
		// �ʒu
		pVtx2D[ 0 + i * 4 ].pos = D3DXVECTOR3( g_aTitleLogo[ i ].pos.x - g_aTitleLogo[ i ].fWidth, g_aTitleLogo[ i ].pos.y - g_aTitleLogo[ i ].fHeight, 0.0f );
		pVtx2D[ 1 + i * 4 ].pos = D3DXVECTOR3( g_aTitleLogo[ i ].pos.x + g_aTitleLogo[ i ].fWidth, g_aTitleLogo[ i ].pos.y - g_aTitleLogo[ i ].fHeight, 0.0f );
		pVtx2D[ 2 + i * 4 ].pos = D3DXVECTOR3( g_aTitleLogo[ i ].pos.x - g_aTitleLogo[ i ].fWidth, g_aTitleLogo[ i ].pos.y + g_aTitleLogo[ i ].fHeight, 0.0f );
		pVtx2D[ 3 + i * 4 ].pos = D3DXVECTOR3( g_aTitleLogo[ i ].pos.x + g_aTitleLogo[ i ].fWidth, g_aTitleLogo[ i ].pos.y + g_aTitleLogo[ i ].fHeight, 0.0f );

		// �p�[�X�y�N�e�B�u�R���N�g���F
		for ( int j = 0; j < 4; j++ )
		{
			pVtx2D[ j + i * 4 ].rhw = 1.0f;
			pVtx2D[ j + i * 4 ].col = g_aTitleLogo[ i ].color;
		}

		// �e�N�X�`�����W
		pVtx2D[ 0 + i * 4  ].tex = D3DXVECTOR2( 0.0f, 0.0f );
		pVtx2D[ 1 + i * 4  ].tex = D3DXVECTOR2( 1.0f, 0.0f );
		pVtx2D[ 2 + i * 4  ].tex = D3DXVECTOR2( 0.0f, 1.0f );
		pVtx2D[ 3 + i * 4  ].tex = D3DXVECTOR2( 1.0f, 1.0f );
	}

	g_pVtxBuffTitleLogo ->Unlock();
}


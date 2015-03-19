//===============================================
//
//  ���e   �F �^�C�g�����S����
//  File   �F titleLogo.cpp
//  ����� �F �؍�@��
//
//===============================================

//------ �C���N���[�h�t�@�C�� ------
#include "background.h"
#include "title.h"
#include "billboard.h"
#include "camera.h"

//------ �}�N����` ------
#define TITLELOGO_TEX_MAX	( 5 )

//------ �O���[�o���ϐ� ------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBackground = NULL;						// ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9		g_pTextureBackground[ TITLELOGO_TEX_MAX ];		// �e�N�X�`���ւ̃|�C���^

//------ �֐��v���g�^�C�v�錾 ------

//===============================================
//
// �^�C�g�����S�Ɋւ��鏉����
//
//===============================================
void InitBackground( void )
{
	// ���[�J���ϐ�
	//---------------------------------
	LPDIRECT3DDEVICE9 pDevice = NULL;		// �f�o�C�X�I�u�W�F�N�g
	CAMERA *pCamera = GetCamera();

	// �f�o�C�X�擾
	//------------------------------------
	pDevice = GetDevice();

	// ���_�o�b�t�@�̊m��
	//------------------------------------
	pDevice ->CreateVertexBuffer((sizeof(VERTEX_2D) * 4),		// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@
								 D3DUSAGE_WRITEONLY,					// ���_�o�b�t�@�̎g�p���@
								 FVF_VERTEX_2D,							// �g�p���钸�_�t�H�[�}�b�g
								 D3DPOOL_MANAGED,						// �o�b�t�@��ێ����郁�����N���X�̎w��
								 &g_pVtxBuffBackground,							// ���_�o�b�t�@�ւ̃|�C���^
								 NULL);									// NULL�Œ�	

	// �e�N�X�`���ւ̃|�C���^��������
	//------------------------------------
	for (int nCnt = 0; nCnt < TITLELOGO_TEX_MAX; nCnt++)				
	{
		g_pTextureBackground[ nCnt ] = NULL;
	}

	// �e�N�X�`���̓ǂݍ���
	//------------------------------------
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ringleDash_backTitle.jpg", &g_pTextureBackground[ 0 ]);

	SetBillboard( D3DXVECTOR3( pCamera ->posCameraP.x, pCamera ->posCameraP.y - 100.0f, 40.0f ), TEXTUREANIM_TITLE, 0.0f, SCREEN_HEIGHT / 2.5f, SCREEN_WIDTH / 2.5f );

	// ���_���̃Z�b�g
	//------------------------------------
	SetupBackground();
}

//===============================================
//
// �^�C�g�����S�Ɋւ���I������
//
//===============================================
void UninitBackground( void )
{
	// �g�����e�N�X�`���̉���Ə�����
	//------------------------------------
	for (int nCnt = 0; nCnt < TITLELOGO_TEX_MAX; nCnt++)
	{
		if (g_pTextureBackground[ nCnt ] != NULL)
		{ 
			g_pTextureBackground[ nCnt ] ->Release();
			g_pTextureBackground[ nCnt ] = NULL;
		}
	}

	// ���_�o�b�t�@�̉���Ə�����
	//------------------------------------
	if (g_pVtxBuffBackground != NULL)
	{ 
		g_pVtxBuffBackground ->Release();
		g_pVtxBuffBackground = NULL;
	}

}

//===============================================
//
// �^�C�g�����S�Ɋւ���X�V����
//
//===============================================
void UpdateBackground( void )
{

}

//===============================================
//
// �^�C�g�����S�Ɋւ���`�揈��
//
//===============================================
void DrawBackground( void )
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
								g_pVtxBuffBackground,
								0,
								sizeof(VERTEX_2D));	// ���_�f�[�^�̊Ԋu

	
	// ���_�t�H�[�}�b�g�̐ݒ�
	//------------------------------------
	pDevice ->SetFVF( FVF_VERTEX_2D );

	// [ titleLogo ]�`��ݒ�

	// �e�N�X�`���̐ݒ�
	//----------------------- -------------
	pDevice -> SetTexture(0, g_pTextureBackground[ 0 ]);

	// �|���S���̕`��
	//------------------------------------
	pDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,	// �v���~�e�B�u�̎��
									0,				// �ǂݍ��ލŏ��̔ԍ�
									2);				// �`�悷��v���~�e�B�u��
}

//===============================================
//
// �^�C�g�����S�̒��_���Z�b�g
//
//===============================================
void SetupBackground( void )
{
		// ���[�J���ϐ�
	//---------------------------------
	LPDIRECT3DDEVICE9 pDevice = NULL;		// �f�o�C�X�I�u�W�F�N�g
	VERTEX_2D *pVtx2D;

	// �f�o�C�X�擾
	//------------------------------------
	pDevice = GetDevice();

	g_pVtxBuffBackground ->Lock( 0, 0, (void**)&pVtx2D, 0 );

	// �ʒu
	pVtx2D[ 0 ].pos = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	pVtx2D[ 1 ].pos = D3DXVECTOR3( SCREEN_WIDTH, 0.0f, 0.0f );
	pVtx2D[ 2 ].pos = D3DXVECTOR3( 0.0f, SCREEN_HEIGHT, 0.0f );
	pVtx2D[ 3 ].pos = D3DXVECTOR3( SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f );

	// �p�[�X�y�N�e�B�u�R���N�g���F
	for ( int j = 0; j < 4; j++ )
	{
		pVtx2D[ j ].rhw = 1.0f;
		pVtx2D[ j ].col = D3DXCOLOR( 1.0, 1.0f, 1.0f, 1.0f );
	}

	// �e�N�X�`�����W
	pVtx2D[ 0 ].tex = D3DXVECTOR2( 0.0f, 0.0f );
	pVtx2D[ 1 ].tex = D3DXVECTOR2( 1.0f, 0.0f );
	pVtx2D[ 2 ].tex = D3DXVECTOR2( 0.0f, 1.0f );
	pVtx2D[ 3 ].tex = D3DXVECTOR2( 1.0f, 1.0f );

	g_pVtxBuffBackground ->Unlock();
}


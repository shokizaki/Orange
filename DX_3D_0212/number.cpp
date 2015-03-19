//-----------------------------------------------
//  ���e�F�X�R�A�Ɋւ��鏈��
//  Fail:score.cpp
//  ����ҁF�؍�@��
//-----------------------------------------------

//-----------------------------------------------
//  �C���N���[�h�t�@�C��
//-----------------------------------------------
#include "number.h"

//-----------------------------------------------
//�@�}�N����`
//-----------------------------------------------
#define NUMBER_TEX		(2)				// �e�N�X�`���̎g�p��
#define NUMBER_MAX		(5)				// �v���~�e�B�u��
#define WAVE_NUM		(2)				// �c�G���̕`��J�n�v���~�e�B�u�ԍ�

//-----------------------------------------------
//  �O���[�o���ϐ�
//-----------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffNumber = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureNumber = NULL;		// �e�N�X�`���ւ̃|�C���^

void InitNumber( void )
{
	// ���[�J���ϐ�
	//---------------------------------
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// �f�o�C�X�I�u�W�F�N�g

	// �e�N�X�`���ւ̃|�C���^��������
	//------------------------------------
	g_pTextureNumber = NULL;

	// �e�N�X�`���̓ǂݍ���
	//------------------------------------
	D3DXCreateTextureFromFile( pDevice, "data/TEXTURE/number000.png", &g_pTextureNumber );
}

// �I��
void UninitNumber( NUMBER *pNumber )
{
	// ���_�o�b�t�@�̉���Ə�����
	//------------------------------------
	if ( pNumber ->pVtxBuffer != NULL)
	{ 
		pNumber ->pVtxBuffer ->Release();
		pNumber ->pVtxBuffer = NULL;
	}
}

// �e�N�X�`���̉������
void UninitNumberTexture( void )
{
	// �g�����e�N�X�`���̉���Ə�����
	//------------------------------------
	if ( g_pTextureNumber != NULL )
	{ 
		g_pTextureNumber ->Release();
		g_pTextureNumber = NULL;
	}
}

// �����̃Z�b�g
void SetNumber( NUMBER *pNumber, D3DXVECTOR3 pos, int nDig, float fWidth, float fHeight )
{
	// ���[�J���ϐ�
	//---------------------------------
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// �f�o�C�X�I�u�W�F�N�g
	VERTEX_2D *pVtx2D = NULL;

	// �\���̂Ɋi�[
	//---------------------------------
	pNumber ->pos = pos;		// �ʒu
	pNumber ->nDigit = nDig;	// ������

	// ���_�o�b�t�@�̊m��
	//------------------------------------
	pDevice ->CreateVertexBuffer((sizeof(VERTEX_2D) * 4 * nDig),		// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@
								 D3DUSAGE_WRITEONLY,					// ���_�o�b�t�@�̎g�p���@
								 FVF_VERTEX_2D,							// �g�p���钸�_�t�H�[�}�b�g
								 D3DPOOL_MANAGED,						// �o�b�t�@��ێ����郁�����N���X�̎w��
								 &pNumber ->pVtxBuffer,					// ���_�o�b�t�@�ւ̃|�C���^
								 NULL);									// NULL�Œ�	
	
	// ���b�N
	pNumber ->pVtxBuffer ->Lock( 0, 0, ( void** )&pVtx2D, 0 );

	// ���_�����Z�b�g
	for ( int i = 0; i < nDig; i++ )
	{
		pVtx2D[ 0 + i * 4 ].pos = D3DXVECTOR3( ( pos.x + i * fWidth * 2 ) - fWidth, pos.y - fHeight, 0.0f );
		pVtx2D[ 1 + i * 4 ].pos = D3DXVECTOR3( ( pos.x + i * fWidth * 2 ) + fWidth, pos.y - fHeight, 0.0f );
		pVtx2D[ 2 + i * 4 ].pos = D3DXVECTOR3( ( pos.x + i * fWidth * 2 ) - fWidth, pos.y + fHeight, 0.0f );
		pVtx2D[ 3 + i * 4 ].pos = D3DXVECTOR3( ( pos.x + i * fWidth * 2 ) + fWidth, pos.y + fHeight, 0.0f );

		pVtx2D[ 0 + i * 4 ].rhw = 1.0f;
		pVtx2D[ 1 + i * 4 ].rhw = 1.0f;
		pVtx2D[ 2 + i * 4 ].rhw = 1.0f;
		pVtx2D[ 3 + i * 4 ].rhw = 1.0f;

		pVtx2D[ 0 + i * 4 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );
		pVtx2D[ 1 + i * 4 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );
		pVtx2D[ 2 + i * 4 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );
		pVtx2D[ 3 + i * 4 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );
	}

	// �A�����b�N
	pNumber ->pVtxBuffer ->Unlock();
}

// �����̕`��
void DrawNumber( NUMBER *pNumber, int nNumber )
{
	// ���[�J���ϐ�
	//------------------------------------
	LPDIRECT3DDEVICE9 pDevice = GetDevice();						// �f�o�C�X�I�u�W�F�N�g
	VERTEX_2D *pVtx2D = NULL;
	int nDevide = 1;

	// �`�悷�錅�����v�Z
	for ( int i = 0; i < pNumber ->nDigit - 1; i++ )
	{
		nDevide *= 10;
	}

	// ���b�N
	pNumber ->pVtxBuffer ->Lock( 0, 0, ( void** )&pVtx2D, 0 );

	// �e�N�X�`�����W�̌v�Z
	for ( int i = 0; i < pNumber ->nDigit; i++ )
	{
		float fNumber = ( nNumber / nDevide ) * 0.1f;

		pVtx2D[ 0 + i * 4 ].tex = D3DXVECTOR2( 0.0f + fNumber, 0.0f );
		pVtx2D[ 1 + i * 4 ].tex = D3DXVECTOR2( 0.1f + fNumber, 0.0f );
		pVtx2D[ 2 + i * 4 ].tex = D3DXVECTOR2( 0.0f + fNumber, 1.0f );
		pVtx2D[ 3 + i * 4 ].tex = D3DXVECTOR2( 0.1f + fNumber, 1.0f );

		nDevide /= 10;
	}

	// �A�����b�N
	pNumber ->pVtxBuffer ->Unlock();

	// ���_�o�b�t�@���f�[�^�X�g���[���Ƀo�C���h
	//-----------------------------------------
	pDevice ->SetStreamSource( 0, pNumber ->pVtxBuffer, 0, sizeof( VERTEX_2D ) );

	// ���_�t�H�[�}�b�g�̐ݒ�
	//------------------------------------
	pDevice ->SetFVF( FVF_VERTEX_2D );

	// �e�N�X�`���̐ݒ�[ �t���[�� ]
	//------------------------------------
	pDevice -> SetTexture( 0, g_pTextureNumber );

	for ( int i = 0; i < pNumber ->nDigit; i++ )
	{
		// �|���S���̕`��
		//------------------------------------
		pDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,	// �v���~�e�B�u�̎��
										i * 4,			// �ǂݍ��ލŏ��̔ԍ�
										2);				// �`�悷��v���~�e�B�u��
	}
}
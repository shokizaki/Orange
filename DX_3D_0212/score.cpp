//-----------------------------------------------
//  ���e�F�X�R�A����
//  File : score.cpp
//  ����ҁF�؍�@��
//-----------------------------------------------

//-----------------------------------------------
//  �C���N���[�h�t�@�C��
//-----------------------------------------------
#include "score.h"

//-----------------------------------------------
//�@�}�N����`
//-----------------------------------------------

//-----------------------------------------------
//  �֐��v���g�^�C�v�錾
//-----------------------------------------------

//-----------------------------------------------
//�@�}�N����`
//-----------------------------------------------
#define SCORE_WIDTH		( 20.0f )
#define SCORE_HEIGHT	( 20.0f )
#define SCORE_DIGIT		( 6 )
#define SCORE_POS_X		( SCREEN_WIDTH - ( ( SCORE_DIGIT * 2 - 1 ) * SCORE_WIDTH ) )
#define SCORE_POS_Y		( 80.0f )

#define SCOREPOLI_WIDTH		( 80.0f )
#define SCOREPOLI_HEIGHT	( 20.0f )
#define SCOREPOLI_POS_X		( SCREEN_WIDTH - 120.0f )
#define SCOREPOLI_POS_Y		( 40.0f )

#define SCORE_PASS			"data/TEXTURE/score.png"

//-----------------------------------------------
//  �O���[�o���ϐ�
//-----------------------------------------------
NUMBER g_score;
int g_nScore = 0;

LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL;			// ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9		g_pTextureScore = NULL;			// �e�N�X�`���ւ̃|�C���^

//-----------------------------------------------
// �w�i����������
//-----------------------------------------------
void InitScore()
{
	// ���[�J���ϐ�
	//---------------------------------
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// �f�o�C�X�I�u�W�F�N�g
	VERTEX_2D *pVtx2D;

	// �O���[�o���ϐ�������
	g_nScore = 0;
	g_pTextureScore = NULL;

	// ���_�o�b�t�@�̊m��
	//------------------------------------
	pDevice ->CreateVertexBuffer(( sizeof(VERTEX_2D) * 4 ),		// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@
								 D3DUSAGE_WRITEONLY,					// ���_�o�b�t�@�̎g�p���@
								 FVF_VERTEX_2D,							// �g�p���钸�_�t�H�[�}�b�g
								 D3DPOOL_MANAGED,						// �o�b�t�@��ێ����郁�����N���X�̎w��
								 &g_pVtxBuffScore,							// ���_�o�b�t�@�ւ̃|�C���^
								 NULL);									// NULL�Œ�	

	g_pVtxBuffScore ->Lock( 0, 0, ( void** )&pVtx2D, 0 );

	pVtx2D[ 0 ].pos = D3DXVECTOR3( SCOREPOLI_POS_X - SCOREPOLI_WIDTH, SCOREPOLI_POS_Y - SCOREPOLI_HEIGHT, 0.0f );
	pVtx2D[ 1 ].pos = D3DXVECTOR3( SCOREPOLI_POS_X + SCOREPOLI_WIDTH, SCOREPOLI_POS_Y - SCOREPOLI_HEIGHT, 0.0f );
	pVtx2D[ 2 ].pos = D3DXVECTOR3( SCOREPOLI_POS_X - SCOREPOLI_WIDTH, SCOREPOLI_POS_Y + SCOREPOLI_HEIGHT, 0.0f );
	pVtx2D[ 3 ].pos = D3DXVECTOR3( SCOREPOLI_POS_X + SCOREPOLI_WIDTH, SCOREPOLI_POS_Y + SCOREPOLI_HEIGHT, 0.0f );

	pVtx2D[ 0 ].rhw = 1.0f;
	pVtx2D[ 1 ].rhw = 1.0f;
	pVtx2D[ 2 ].rhw = 1.0f;
	pVtx2D[ 3 ].rhw = 1.0f;

	pVtx2D[ 0 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );
	pVtx2D[ 1 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );
	pVtx2D[ 2 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );
	pVtx2D[ 3 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );

	pVtx2D[ 0 ].tex = D3DXVECTOR2( 0.0f, 0.0f );
	pVtx2D[ 1 ].tex = D3DXVECTOR2( 1.0f, 0.0f );
	pVtx2D[ 2 ].tex = D3DXVECTOR2( 0.0f, 1.0f );
	pVtx2D[ 3 ].tex = D3DXVECTOR2( 1.0f, 1.0f );

	g_pVtxBuffScore ->Unlock();

	// �e�N�X�`���̓ǂݍ���
	//------------------------------------
	D3DXCreateTextureFromFile(pDevice, SCORE_PASS, &g_pTextureScore);

	// �X�R�A�̏�����
	SetNumber( &g_score, D3DXVECTOR3( SCORE_POS_X, SCORE_POS_Y, 0.0f ), SCORE_DIGIT, SCORE_WIDTH, SCORE_HEIGHT );
}

//-----------------------------------------------
// �w�i�I������
//-----------------------------------------------
void UninitScore()
{
	// �I������
	UninitNumber( &g_score );

	// �g�����e�N�X�`���̉���Ə�����
	//------------------------------------
	if (g_pTextureScore != NULL)
	{ 
		g_pTextureScore ->Release();
		g_pTextureScore = NULL;
	}


	// ���_�o�b�t�@�̉���Ə�����
	//------------------------------------
	if (g_pVtxBuffScore != NULL)
	{ 
		g_pVtxBuffScore ->Release();
		g_pVtxBuffScore = NULL;
	}
}

//-----------------------------------------------
// �w�i�X�V����
//-----------------------------------------------
void UpdateScore()
{
	
}

//-----------------------------------------------
// �w�i�`�揈��
//-----------------------------------------------
void DrawScore()
{
	DrawNumber( &g_score, g_nScore );

	DrawScorePoligon();
}

//-----------------------------------------------
// �X�R�A�ݒ�
//-----------------------------------------------
void AddScore( int nScore )
{
	g_nScore += nScore;
}

//-----------------------------------------------
// �X�R�A�擾
//-----------------------------------------------
int GetScore( void )
{
	return g_nScore;
}

//-----------------------------------------------
// [ �X�R�A ]�|���S���̕`��
//-----------------------------------------------
void DrawScorePoligon( void )
{
	// ���[�J���ϐ�
	//------------------------------------
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�I�u�W�F�N�g
	
	// ���_�o�b�t�@���f�[�^�X�g���[���Ƀo�C���h
	//-----------------------------------------
	pDevice ->SetStreamSource(0,
								g_pVtxBuffScore,
								0,
								sizeof(VERTEX_2D));	// ���_�f�[�^�̊Ԋu

	
	// ���_�t�H�[�}�b�g�̐ݒ�
	//------------------------------------
	pDevice ->SetFVF( FVF_VERTEX_2D );

	// �e�N�X�`���̐ݒ�
	//------------------------------------
	pDevice -> SetTexture(0, g_pTextureScore);
			
	// �|���S���̕`��
	//------------------------------------
	pDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,	// �v���~�e�B�u�̎��
									0,				// �ǂݍ��ލŏ��̔ԍ�
									2);				// �`�悷��v���~�e�B�u��

}

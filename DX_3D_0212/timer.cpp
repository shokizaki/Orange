//===============================================
//
//  ���e   �F �^�C�}�[����
//  File   �F timer.h
//  ����� �F �؍�@��
//
//===============================================

//------ �C���N���[�h�t�@�C�� ------
#include "timer.h"
#include "number.h"

//------ �}�N����` ------
#define TIMER			( 45 )
#define TIMER_WIDTH		( 15.0f )
#define TIMER_HEIGHT	( 30.0f )
#define TIMER_DIG		( 3 )
#define TIMER_POS_X		( SCREEN_WIDTH / 2 - 25.0f )
#define TIMER_POS_Y		( 90.0f )

#define TIMERPOLI_WIDTH		( 50.0f )
#define TIMERPOLI_HEIGHT	( 30.0f )
#define TIMERPOLI_POS_X		( TIMER_POS_X + 35.0f )
#define TIMERPOLI_POS_Y		( TIMER_POS_Y - 60.0f )

//------ �O���[�o���ϐ� ------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTimer = NULL;						// ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9		g_pTextureTimer;		// �e�N�X�`���ւ̃|�C���^
int g_nTimer = TIMER;
NUMBER g_timer;

//===============================================
//
// �^�C�}�[�Ɋւ��鏉����
//
//===============================================
void InitTimer( void )
{
	g_nTimer = TIMER;

	SetNumber( &g_timer, D3DXVECTOR3( TIMER_POS_X, TIMER_POS_Y, 0.0f ), TIMER_DIG, TIMER_WIDTH, TIMER_HEIGHT );

	// ���[�J���ϐ�
	//---------------------------------
	LPDIRECT3DDEVICE9 pDevice = NULL;		// �f�o�C�X�I�u�W�F�N�g
	VERTEX_2D *pVtx2D;

	// �O���[�o���ϐ��̏�����
	//---------------------------------
	
	// �f�o�C�X�擾
	//------------------------------------
	pDevice = GetDevice();

	// ���_�o�b�t�@�̊m��
	//------------------------------------
	pDevice ->CreateVertexBuffer((sizeof(VERTEX_2D) * 4 ),		// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@
								 D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p���@
								 FVF_VERTEX_2D,					// �g�p���钸�_�t�H�[�}�b�g
								 D3DPOOL_MANAGED,				// �o�b�t�@��ێ����郁�����N���X�̎w��
								 &g_pVtxBuffTimer,				// ���_�o�b�t�@�ւ̃|�C���^
								 NULL);							// NULL�Œ�	

	g_pVtxBuffTimer ->Lock( 0, 0, ( void** )&pVtx2D, 0 );

	pVtx2D[ 0 ].pos = D3DXVECTOR3( TIMERPOLI_POS_X - TIMERPOLI_WIDTH, TIMERPOLI_POS_Y - TIMERPOLI_HEIGHT, 0.0f );
	pVtx2D[ 1 ].pos = D3DXVECTOR3( TIMERPOLI_POS_X + TIMERPOLI_WIDTH, TIMERPOLI_POS_Y - TIMERPOLI_HEIGHT, 0.0f );
	pVtx2D[ 2 ].pos = D3DXVECTOR3( TIMERPOLI_POS_X - TIMERPOLI_WIDTH, TIMERPOLI_POS_Y + TIMERPOLI_HEIGHT, 0.0f );
	pVtx2D[ 3 ].pos = D3DXVECTOR3( TIMERPOLI_POS_X + TIMERPOLI_WIDTH, TIMERPOLI_POS_Y + TIMERPOLI_HEIGHT, 0.0f );

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

	g_pVtxBuffTimer ->Unlock();

	// �e�N�X�`���̓ǂݍ���
	//------------------------------------
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/timer.png", &g_pTextureTimer);
}

//===============================================
//
// �^�C�}�[�Ɋւ���I������
//
//===============================================
void UninitTimer(void)
{
	UninitNumber( &g_timer );
}

//===============================================
//
// �^�C�}�[�Ɋւ���X�V����
//
//===============================================
void UpdateTimer(void)
{
	// ���[�J���ϐ�
	//------------------------------------
	static int nFrameCount = 0;

	// �^�C�}�[�X�V
	if ( ++nFrameCount % 60 == 0 )
	{
		g_nTimer -= 1;
		if ( g_nTimer <= 0 && GetFade() == FADE_NONE )
		{
			SetFade( FADE_OUT );
			SetMode( MODE_RANKING );
		}
	}
}

//===============================================
//
// �^�C�}�[�Ɋւ���`�揈��
//
//===============================================
void DrawTimer(void)
{
	// �f�o�C�X�̎擾
	//------------------------------------
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���Ƀo�C���h
	//-----------------------------------------
	pDevice ->SetStreamSource(0,
								g_pVtxBuffTimer,
								0,
								sizeof(VERTEX_2D));	// ���_�f�[�^�̊Ԋu

	
	// ���_�t�H�[�}�b�g�̐ݒ�
	//------------------------------------
	pDevice ->SetFVF( FVF_VERTEX_2D );

	// �e�N�X�`���̐ݒ�[ Timer ]
	//------------------------------------
	pDevice -> SetTexture(0, g_pTextureTimer);

	// �|���S���̕`��[ Timer ]
	//------------------------------------
	pDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,	// �v���~�e�B�u�̎��
									0,				// �ǂݍ��ލŏ��̔ԍ�
									2);				// �`�悷��v���~�e�B�u

	// �^�C�}�[�`��
	DrawNumber( &g_timer, g_nTimer );

}

//===============================================
//
// �^�C�}�[�̒��_���Z�b�g
//
//===============================================
void SetupTimer(void)
{
	// ���[�J���ϐ�
	//------------------------------------
	VERTEX_2D *pVtx2D;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	float fTimerDevide = 0.0f;
	int nDevide = 100;

	g_pVtxBuffTimer ->Lock( 0, 0, (void**)&pVtx2D, 0 );

	for ( int i = 0; i < TIMER_MAX; i++ )
	{
		fTimerDevide = ( g_nTimer / nDevide ) * 0.1f;

		pVtx2D[ 0 + i * 4 ].pos = D3DXVECTOR3( TIMER_POS_X + ( TIMER_WIDTH * 2 * (i + 1) ) - TIMER_WIDTH, TIMER_POS_Y - TIMER_HEIGHT, 0.0f );
		pVtx2D[ 1 + i * 4 ].pos = D3DXVECTOR3( TIMER_POS_X + ( TIMER_WIDTH * 2 * (i + 1) ) + TIMER_WIDTH, TIMER_POS_Y - TIMER_HEIGHT, 0.0f );
		pVtx2D[ 2 + i * 4 ].pos = D3DXVECTOR3( TIMER_POS_X + ( TIMER_WIDTH * 2 * (i + 1) ) - TIMER_WIDTH, TIMER_POS_Y + TIMER_HEIGHT, 0.0f );
		pVtx2D[ 3 + i * 4 ].pos = D3DXVECTOR3( TIMER_POS_X + ( TIMER_WIDTH * 2 * (i + 1) ) + TIMER_WIDTH, TIMER_POS_Y + TIMER_HEIGHT, 0.0f );

		pVtx2D[ 0 + i * 4 ].tex = D3DXVECTOR2( 0.0f + fTimerDevide, 0.0f );
		pVtx2D[ 1 + i * 4 ].tex = D3DXVECTOR2( 0.1f + fTimerDevide, 0.0f );
		pVtx2D[ 2 + i * 4 ].tex = D3DXVECTOR2( 0.0f + fTimerDevide, 1.0f );
		pVtx2D[ 3 + i * 4 ].tex = D3DXVECTOR2( 0.1f + fTimerDevide, 1.0f );

		nDevide /= 10;
	}

	g_pVtxBuffTimer ->Unlock();
}

//===============================================
//
// �^�C�}�[�̒��_���Z�b�g
//
//===============================================
int GetTimer( void )
{
	return g_nTimer;
}

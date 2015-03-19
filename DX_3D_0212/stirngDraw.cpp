//===============================================
//
//  ���e   �F ���[�U�[�C���^�[�t�F�[�X����
//  File   �F StringDraw.h
//  ����� �F �؍�@��
//
//===============================================

//------ �C���N���[�h�t�@�C�� ------
#include "stringDraw.h"
#include "bullet.h"
#include "model.h"

//------ �}�N����` ------
#define STRINGDRAW_TEX_MAX	( 25 )

#define TEX000			"data/TEXTURE/0_frame.png"
#define TEX001			"data/TEXTURE/1_helloRingle.png"
#define TEX002			"data/TEXTURE/2_moveExe.png"
#define TEX003			"data/TEXTURE/3_moveButton.png"
#define TEX004			"data/TEXTURE/4_move.png"
#define TEX005			"data/TEXTURE/5_nextExe.png"
#define TEX006			"data/TEXTURE/6_goTouch.png"
#define TEX007			"data/TEXTURE/7_go.png"
#define TEX008			"data/TEXTURE/8_absorbExe.png"
#define TEX009			"data/TEXTURE/9_attention.png"
#define TEX010			"data/TEXTURE/10_steps.png"
#define TEX011			"data/TEXTURE/11_onpExe.png"
#define TEX012			"data/TEXTURE/12_try.png"
#define TEX013			"data/TEXTURE/13_box.png"
#define TEX014			"data/TEXTURE/14_releaseTry.png"
#define TEX015			"data/TEXTURE/15_attentionSpeed.png"
#define TEX016			"data/TEXTURE/16_last.png"
#define TEX099			"data/TEXTURE/99_6button.png"
#define TEX100			"data/TEXTURE/10ButtonToSkipTutorial.png"

#define TEX003KEY		"data/TEXTURE/3_moveButtonKey.png"
#define TEX099KEY		"data/TEXTURE/99_enterToNext.png"
#define TEX011KEY		"data/TEXTURE/11_onpExeKey.png"
#define TEX013KEY		"data/TEXTURE/13_boxKey.png"
#define TEX100KEY		"data/TEXTURE/p_ToSkip.png"

#define TEX101SIX		"data/TEXTURE/6button.png"
#define TEX101ENTER		"data/TEXTURE/enterKey.png"

#define TEX102TUTO		"data/TEXTURE/tutorial.png"

#define STRING_WIDTH	( 300.0f )
#define STRING_HEIGHT	( 100.0f )
#define STRING_POS_X	( SCREEN_WIDTH / 2 )
#define STRING_POS_Y	( SCREEN_HEIGHT - 120.0f )

#define PRESS_P_WIDTH	( 160.0f )
#define PRESS_P_HEIGHT	( 10.0f )
#define PRESS_P_POS_X	( SCREEN_WIDTH - 240.0f )
#define PRESS_P_POS_Y	( SCREEN_HEIGHT - 230.0f )

#define PRESS_6_WIDTH	( 30.0f )
#define PRESS_6_HEIGHT	( 20.0f )
#define PRESS_6_POS_X	( SCREEN_WIDTH - 170.0f )
#define PRESS_6_POS_Y	( SCREEN_HEIGHT - 60.0f )

#define TUTORIAL_WIDTH	( 120.0f )
#define TUTORIAL_HEIGHT	( 30.0f )
#define TUTORIAL_POS_X	( SCREEN_WIDTH / 2 )
#define TUTORIAL_POS_Y	( 60.0f )


//------ �O���[�o���ϐ� ------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffStringDraw = NULL;					// ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9		g_pTextureStringDraw[ STRINGDRAW_TEX_MAX ];		// �e�N�X�`���ւ̃|�C���^

STRINGDRAW g_aStringDraw[ STRINGDRAW_MAX ];

D3DXMATRIX g_mtxProjectionStringDraw;		// �v���W�F�N�V�����}�g���b�N�X

float g_fAlpha = 0.8f;
bool g_bMessageSkip = false;
bool g_bFlagEnter = false;
MESSAGE g_message = HELLO;

//===============================================
//
// StringDraw�Ɋւ��鏉����
//
//===============================================
void InitStringDraw( void )
{
	// ���[�J���ϐ�
	//---------------------------------
	LPDIRECT3DDEVICE9 pDevice = NULL;		// �f�o�C�X�I�u�W�F�N�g

	// �O���[�o���ϐ��̏�����
	//---------------------------------
	g_fAlpha = 0.8f;
	g_bMessageSkip = false;
	g_bFlagEnter = false;
	g_message = HELLO;

	// �f�o�C�X�擾
	//------------------------------------
	pDevice = GetDevice();

	// ���_�o�b�t�@�̊m��
	//------------------------------------
	pDevice ->CreateVertexBuffer((sizeof(VERTEX_2D) * 4 * STRINGDRAW_MAX),		// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@
								 D3DUSAGE_WRITEONLY,					// ���_�o�b�t�@�̎g�p���@
								 FVF_VERTEX_2D,							// �g�p���钸�_�t�H�[�}�b�g
								 D3DPOOL_MANAGED,						// �o�b�t�@��ێ����郁�����N���X�̎w��
								 &g_pVtxBuffStringDraw,							// ���_�o�b�t�@�ւ̃|�C���^
								 NULL);									// NULL�Œ�	

	// �e�N�X�`���ւ̃|�C���^��������
	//------------------------------------
	for (int nCnt = 0; nCnt < STRINGDRAW_TEX_MAX; nCnt++)				
	{
		g_pTextureStringDraw[ nCnt ] = NULL;
	}

	// �e�N�X�`���̓ǂݍ���
	//------------------------------------
	if ( GetGamePadUse() == true )
	{
		D3DXCreateTextureFromFile(pDevice, TEX000, &g_pTextureStringDraw[ 0 ]);
		D3DXCreateTextureFromFile(pDevice, TEX001, &g_pTextureStringDraw[ 1 ]);
		D3DXCreateTextureFromFile(pDevice, TEX002, &g_pTextureStringDraw[ 2 ]);
		D3DXCreateTextureFromFile(pDevice, TEX003, &g_pTextureStringDraw[ 3 ]);
		D3DXCreateTextureFromFile(pDevice, TEX004, &g_pTextureStringDraw[ 4 ]);
		D3DXCreateTextureFromFile(pDevice, TEX005, &g_pTextureStringDraw[ 5 ]);
		D3DXCreateTextureFromFile(pDevice, TEX006, &g_pTextureStringDraw[ 6 ]);
		D3DXCreateTextureFromFile(pDevice, TEX007, &g_pTextureStringDraw[ 7 ]);
		D3DXCreateTextureFromFile(pDevice, TEX008, &g_pTextureStringDraw[ 8 ]);
		D3DXCreateTextureFromFile(pDevice, TEX009, &g_pTextureStringDraw[ 9 ]);
		D3DXCreateTextureFromFile(pDevice, TEX010, &g_pTextureStringDraw[ 10 ]);
		D3DXCreateTextureFromFile(pDevice, TEX011, &g_pTextureStringDraw[ 11 ]);
		D3DXCreateTextureFromFile(pDevice, TEX012, &g_pTextureStringDraw[ 12 ]);
		D3DXCreateTextureFromFile(pDevice, TEX013, &g_pTextureStringDraw[ 13 ]);
		D3DXCreateTextureFromFile(pDevice, TEX014, &g_pTextureStringDraw[ 14 ]);
		D3DXCreateTextureFromFile(pDevice, TEX015, &g_pTextureStringDraw[ 15 ]);
		D3DXCreateTextureFromFile(pDevice, TEX016, &g_pTextureStringDraw[ 16 ]);
		D3DXCreateTextureFromFile(pDevice, TEX100, &g_pTextureStringDraw[ 18 ]);
		D3DXCreateTextureFromFile(pDevice, TEX099, &g_pTextureStringDraw[ 19 ]);
		D3DXCreateTextureFromFile(pDevice, TEX101SIX, &g_pTextureStringDraw[ 20 ]);
		D3DXCreateTextureFromFile(pDevice, TEX102TUTO, &g_pTextureStringDraw[ 21 ]);
	}
	else
	{
		D3DXCreateTextureFromFile(pDevice, TEX000, &g_pTextureStringDraw[ 0 ]);
		D3DXCreateTextureFromFile(pDevice, TEX001, &g_pTextureStringDraw[ 1 ]);
		D3DXCreateTextureFromFile(pDevice, TEX002, &g_pTextureStringDraw[ 2 ]);
		D3DXCreateTextureFromFile(pDevice, TEX003KEY, &g_pTextureStringDraw[ 3 ]);
		D3DXCreateTextureFromFile(pDevice, TEX004, &g_pTextureStringDraw[ 4 ]);
		D3DXCreateTextureFromFile(pDevice, TEX005, &g_pTextureStringDraw[ 5 ]);
		D3DXCreateTextureFromFile(pDevice, TEX006, &g_pTextureStringDraw[ 6 ]);
		D3DXCreateTextureFromFile(pDevice, TEX007, &g_pTextureStringDraw[ 7 ]);
		D3DXCreateTextureFromFile(pDevice, TEX008, &g_pTextureStringDraw[ 8 ]);
		D3DXCreateTextureFromFile(pDevice, TEX009, &g_pTextureStringDraw[ 9 ]);
		D3DXCreateTextureFromFile(pDevice, TEX010, &g_pTextureStringDraw[ 10 ]);
		D3DXCreateTextureFromFile(pDevice, TEX011KEY, &g_pTextureStringDraw[ 11 ]);
		D3DXCreateTextureFromFile(pDevice, TEX012, &g_pTextureStringDraw[ 12 ]);
		D3DXCreateTextureFromFile(pDevice, TEX013KEY, &g_pTextureStringDraw[ 13 ]);
		D3DXCreateTextureFromFile(pDevice, TEX014, &g_pTextureStringDraw[ 14 ]);
		D3DXCreateTextureFromFile(pDevice, TEX015, &g_pTextureStringDraw[ 15 ]);
		D3DXCreateTextureFromFile(pDevice, TEX016, &g_pTextureStringDraw[ 16 ]);
		D3DXCreateTextureFromFile(pDevice, TEX100KEY, &g_pTextureStringDraw[ 18 ]);
		D3DXCreateTextureFromFile(pDevice, TEX099KEY, &g_pTextureStringDraw[ 19 ]);
		D3DXCreateTextureFromFile(pDevice, TEX101ENTER, &g_pTextureStringDraw[ 20 ]);
		D3DXCreateTextureFromFile(pDevice, TEX102TUTO, &g_pTextureStringDraw[ 21 ]);
	}

	// ���_���̃Z�b�g
	//------------------------------------
	SetupStringDraw();
}

//===============================================
//
// StringDraw�̒��_���Z�b�g
//
//===============================================
void SetupStringDraw( void )
{
	// ���[�J���ϐ�
	//---------------------------------
	LPDIRECT3DDEVICE9 pDevice = NULL;		// �f�o�C�X�I�u�W�F�N�g
	VERTEX_2D *pVtx2D;

	// �f�o�C�X�擾
	//------------------------------------
	pDevice = GetDevice();

	g_pVtxBuffStringDraw ->Lock( 0, 0, (void**)&pVtx2D, 0 );

	for ( int i = 0; i < STRINGDRAW_MAX - 1; i++ )
	{
		// �ʒu
		pVtx2D[ 0 + i * 4 ].pos = D3DXVECTOR3( STRING_POS_X - STRING_WIDTH, STRING_POS_Y - STRING_HEIGHT, 0.0f );
		pVtx2D[ 1 + i * 4 ].pos = D3DXVECTOR3( STRING_POS_X + STRING_WIDTH, STRING_POS_Y - STRING_HEIGHT, 0.0f );
		pVtx2D[ 2 + i * 4 ].pos = D3DXVECTOR3( STRING_POS_X - STRING_WIDTH, STRING_POS_Y + STRING_HEIGHT, 0.0f );
		pVtx2D[ 3 + i * 4 ].pos = D3DXVECTOR3( STRING_POS_X + STRING_WIDTH, STRING_POS_Y + STRING_HEIGHT, 0.0f );

		// �p�[�X�y�N�e�B�u�R���N�g���F
		for ( int j = 0; j < 4; j++ )
		{
			pVtx2D[ j + i * 4 ].rhw = 1.0f;
			pVtx2D[ j + i * 4 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, g_fAlpha );
		}

		// �e�N�X�`�����W
		pVtx2D[ 0 + i * 4 ].tex = D3DXVECTOR2( 0.0f, 0.0f );
		pVtx2D[ 1 + i * 4 ].tex = D3DXVECTOR2( 1.0f, 0.0f );
		pVtx2D[ 2 + i * 4 ].tex = D3DXVECTOR2( 0.0f, 1.0f );
		pVtx2D[ 3 + i * 4 ].tex = D3DXVECTOR2( 1.0f, 1.0f );
	}

	// �ʒu
	pVtx2D[ 12 ].pos = D3DXVECTOR3( PRESS_P_POS_X - PRESS_P_WIDTH, PRESS_P_POS_Y - PRESS_P_HEIGHT, 0.0f );
	pVtx2D[ 13 ].pos = D3DXVECTOR3( PRESS_P_POS_X + PRESS_P_WIDTH, PRESS_P_POS_Y - PRESS_P_HEIGHT, 0.0f );
	pVtx2D[ 14 ].pos = D3DXVECTOR3( PRESS_P_POS_X - PRESS_P_WIDTH, PRESS_P_POS_Y + PRESS_P_HEIGHT, 0.0f );
	pVtx2D[ 15 ].pos = D3DXVECTOR3( PRESS_P_POS_X + PRESS_P_WIDTH, PRESS_P_POS_Y + PRESS_P_HEIGHT, 0.0f );

	// �p�[�X�y�N�e�B�u�R���N�g���F
	for ( int j = 0; j < 4; j++ )
	{
		pVtx2D[ j + 3 * 4 ].rhw = 1.0f;
		pVtx2D[ j + 3 * 4 ].col = D3DXCOLOR( 1.0f, 1.0f, 0.0f, g_fAlpha );
	}

	// �e�N�X�`�����W
	pVtx2D[ 12 ].tex = D3DXVECTOR2( 0.0f, 0.0f );
	pVtx2D[ 13 ].tex = D3DXVECTOR2( 1.0f, 0.0f );
	pVtx2D[ 14 ].tex = D3DXVECTOR2( 0.0f, 1.0f );
	pVtx2D[ 15 ].tex = D3DXVECTOR2( 1.0f, 1.0f );

	// �ʒu
	pVtx2D[ 16 ].pos = D3DXVECTOR3( PRESS_6_POS_X - PRESS_6_WIDTH, PRESS_6_POS_Y - PRESS_6_HEIGHT, 0.0f );
	pVtx2D[ 17 ].pos = D3DXVECTOR3( PRESS_6_POS_X + PRESS_6_WIDTH, PRESS_6_POS_Y - PRESS_6_HEIGHT, 0.0f );
	pVtx2D[ 18 ].pos = D3DXVECTOR3( PRESS_6_POS_X - PRESS_6_WIDTH, PRESS_6_POS_Y + PRESS_6_HEIGHT, 0.0f );
	pVtx2D[ 19 ].pos = D3DXVECTOR3( PRESS_6_POS_X + PRESS_6_WIDTH, PRESS_6_POS_Y + PRESS_6_HEIGHT, 0.0f );

	// �p�[�X�y�N�e�B�u�R���N�g���F
	for ( int j = 0; j < 4; j++ )
	{
		pVtx2D[ j + 4 * 4 ].rhw = 1.0f;
		pVtx2D[ j + 4 * 4 ].col = D3DXCOLOR( 0.0f, 0.0f, 0.0f, g_fAlpha );
	}

	// �e�N�X�`�����W
	pVtx2D[ 20 ].tex = D3DXVECTOR2( 0.0f, 0.0f );
	pVtx2D[ 21 ].tex = D3DXVECTOR2( 1.0f, 0.0f );
	pVtx2D[ 22 ].tex = D3DXVECTOR2( 0.0f, 1.0f );
	pVtx2D[ 23 ].tex = D3DXVECTOR2( 1.0f, 1.0f );

	// �ʒu
	pVtx2D[ 20 ].pos = D3DXVECTOR3( TUTORIAL_POS_X - TUTORIAL_WIDTH, TUTORIAL_POS_Y - TUTORIAL_HEIGHT, 0.0f );
	pVtx2D[ 21 ].pos = D3DXVECTOR3( TUTORIAL_POS_X + TUTORIAL_WIDTH, TUTORIAL_POS_Y - TUTORIAL_HEIGHT, 0.0f );
	pVtx2D[ 22 ].pos = D3DXVECTOR3( TUTORIAL_POS_X - TUTORIAL_WIDTH, TUTORIAL_POS_Y + TUTORIAL_HEIGHT, 0.0f );
	pVtx2D[ 23 ].pos = D3DXVECTOR3( TUTORIAL_POS_X + TUTORIAL_WIDTH, TUTORIAL_POS_Y + TUTORIAL_HEIGHT, 0.0f );

	// �p�[�X�y�N�e�B�u�R���N�g���F
	for ( int j = 0; j < 4; j++ )
	{
		pVtx2D[ j + 5 * 4 ].rhw = 1.0f;
		pVtx2D[ j + 5 * 4 ].col = D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f );
	}

	// �e�N�X�`�����W
	pVtx2D[ 20 ].tex = D3DXVECTOR2( 0.0f, 0.0f );
	pVtx2D[ 21 ].tex = D3DXVECTOR2( 1.0f, 0.0f );
	pVtx2D[ 22 ].tex = D3DXVECTOR2( 0.0f, 1.0f );
	pVtx2D[ 23 ].tex = D3DXVECTOR2( 1.0f, 1.0f );

	g_pVtxBuffStringDraw ->Unlock();
}

//===============================================
//
// StringDraw�Ɋւ���I������
//
//===============================================
void UninitStringDraw( void )
{
	// �g�����e�N�X�`���̉���Ə�����
	//------------------------------------
	for (int nCnt = 0; nCnt < STRINGDRAW_TEX_MAX; nCnt++)
	{
		if (g_pTextureStringDraw[ nCnt ] != NULL)
		{ 
			g_pTextureStringDraw[ nCnt ] ->Release();
			g_pTextureStringDraw[ nCnt ] = NULL;
		}
	}

	// ���_�o�b�t�@�̉���Ə�����
	//------------------------------------
	if (g_pVtxBuffStringDraw != NULL)
	{ 
		g_pVtxBuffStringDraw ->Release();
		g_pVtxBuffStringDraw = NULL;
	}

	g_message = MESSAGE_MAX;
}

//===============================================
//
// StringDraw�Ɋւ���X�V����
//
//===============================================
void UpdateStringDraw( void )
{
	// �ǂݐi�߂鏈��
	if ( ( GetKeyboardTrigger( DIK_RETURN )/* || GetGamePadTrigger( GAMEPAD_R )*/ || GetPadElecomTrigger( PAD_6 ) ) && GetFade() == FADE_NONE )
	{
		PlaySound( DESIDE_SE );
		g_message = ( MESSAGE )( ( int )g_message + 1 );
		if ( g_message == MESSAGE_MAX )
		{
			g_message = ( MESSAGE )( ( int )g_message - 1 );
			SetFade( FADE_OUT );
			SetMode( MODE_GAME );
		}
		if ( g_message == MOVE || g_message == GO || g_message == TRY || g_message == RELEASETRY )
		{
			g_bFlagEnter = true;
		}
		else
		{
			g_bFlagEnter = false;
		}
	}

	// �X�L�b�v����
	if ( ( GetKeyboardTrigger( DIK_P ) /*|| GetGamePadTrigger( GAMEPAD_START )*/ || GetPadElecomTrigger( PAD_10 ) ) && GetFade() == FADE_NONE )
	{
		PlaySound( CANCEL_SE );
		SetFade( FADE_OUT );
		SetMode( MODE_GAME );
	}

	// ���_���̃Z�b�g
	//------------------------------------
	SetupStringDraw();
}

//===============================================
//
// StringDraw�Ɋւ���`�揈��
//
//===============================================
void DrawStringDraw( void )
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
								g_pVtxBuffStringDraw,
								0,
								sizeof(VERTEX_2D));	// ���_�f�[�^�̊Ԋu

	
	// ���_�t�H�[�}�b�g�̐ݒ�
	//------------------------------------
	pDevice ->SetFVF( FVF_VERTEX_2D );

	// �e�N�X�`���̐ݒ�[ �t���[�� ]
	//------------------------------------
	pDevice -> SetTexture(0, g_pTextureStringDraw[ 0 ]);

	// �|���S���̕`��
	//------------------------------------
	pDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,	// �v���~�e�B�u�̎��
									0,				// �ǂݍ��ލŏ��̔ԍ�
									2);				// �`�悷��v���~�e�B�u��

	// �e�N�X�`���̐ݒ�[ �`���[�g���A���� ]
	//------------------------------------
	pDevice -> SetTexture(0, g_pTextureStringDraw[ 21 ]);

	// �|���S���̕`��
	//------------------------------------
	pDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,	// �v���~�e�B�u�̎��
									20,				// �ǂݍ��ލŏ��̔ԍ�
									2);				// �`�悷��v���~�e�B�u��

	// �e�N�X�`���̐ݒ�[ ���� ]
	//------------------------------------
	pDevice -> SetTexture( 0, g_pTextureStringDraw[ ( int )g_message ] );

	// �|���S���̕`��
	//------------------------------------
	pDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,	// �v���~�e�B�u�̎��
									4,				// �ǂݍ��ލŏ��̔ԍ�
									2);				// �`�悷��v���~�e�B�u��

	// �e�N�X�`���̐ݒ�[ Skip ]
	//------------------------------------
	pDevice -> SetTexture(0, g_pTextureStringDraw[ 18 ]);

	// �|���S���̕`��
	//------------------------------------
	pDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,	// �v���~�e�B�u�̎��
									12,				// �ǂݍ��ލŏ��̔ԍ�
									2);				// �`�悷��v���~�e�B�u��

	if ( g_bFlagEnter == true )
	{
		// �e�N�X�`���̐ݒ�[ Enter�Ŏ��� ]
		//------------------------------------
		pDevice -> SetTexture(0, g_pTextureStringDraw[ 19 ]);

		// �|���S���̕`��
		//------------------------------------
		pDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,	// �v���~�e�B�u�̎��
										8,				// �ǂݍ��ލŏ��̔ԍ�
										2);				// �`�悷��v���~�e�B�u��
	}
	else
	{
		// �e�N�X�`���̐ݒ�[ �U ]
		//------------------------------------
		pDevice -> SetTexture(0, g_pTextureStringDraw[ 20 ]);

		// �|���S���̕`��
		//------------------------------------
		pDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,	// �v���~�e�B�u�̎��
										16,				// �ǂݍ��ލŏ��̔ԍ�
										2);				// �`�悷��v���~�e�B�u��
	}
}

//===============================================
//
// [ Enter�Ŏ��� ]���łĂ��邩�ǂ���
//
//===============================================
bool GetEnterFlag( void )
{
	return g_bFlagEnter;
}

//===============================================
//
// [ Enter�Ŏ��� ]�̐ݒ�
//
//===============================================
void SetEnterFlag( bool bFlag )
{
	g_bFlagEnter = bFlag;
}

//===============================================
//
// ���݂̃��b�Z�[�W�̔ԍ����擾
//
//===============================================
MESSAGE GetStringMessage( void )
{
	return g_message;
}

//===============================================
//
// ���b�Z�[�W�̔ԍ���ݒ�
//
//===============================================
void SetStringMessage( MESSAGE type )
{
	g_message = type;
}
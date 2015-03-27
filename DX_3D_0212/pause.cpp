//=======================================================================================
//	�N���X�FAT11C272 30623 13
//	����ҁF�؍�@��
//	���e�F�|�[�Y����
//
//=======================================================================================

//-----------------------------------------------
//
//  �C���N���[�h�t�@�C��
//
//-----------------------------------------------
#include "pause.h"
#include "camera.h"

// �}�N����`
#define PAUSE_OFFSET_Y ( 120.0f )

// �O���[�o���ϐ�
PAUSE g_pause[ 5 ];
PAUSE_SELECT g_select;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTexturePause[ 5 ];				// �e�N�X�`���ւ̃|�C���^
bool g_bPauseDraw = true;								// �|�[�Y��ʂ̕`�攻��

//-----------------------------------------------
//
//�@�|�[�Y������
//�@
//-----------------------------------------------
void InitPause(void)
{
	// ���[�J���ϐ�
	//---------------------------------
	LPDIRECT3DDEVICE9 pDevice;		// �f�o�C�X�I�u�W�F�N�g

	// �f�o�C�X�擾
	//------------------------------------
	pDevice = GetDevice();

	pDevice ->CreateVertexBuffer( (sizeof( VERTEX_2D ) * 4 * 5 ),	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@
								 D3DUSAGE_WRITEONLY,				// ���_�o�b�t�@�̎g�p���@
								 FVF_VERTEX_2D,						// �g�p���钸�_�t�H�[�}�b�g
								 D3DPOOL_MANAGED,					// �o�b�t�@��ێ����郁�����N���X�̎w��
								 &g_pVtxBuffPause,					// ���_�o�b�t�@�ւ̃|�C���^
								 NULL);								// NULL�Œ�

	// ���ꂼ��̈ʒu�̏�����
	g_pause[ 0 ].posPause = D3DXVECTOR3( SCREEN_WIDTH / 2, PAUSE_OFFSET_Y, 0.0f );	
	g_pause[ 0 ].rotPause = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	g_pause[ 0 ].sclPause = D3DXVECTOR3( 1.0f, 1.0f, 1.0f );
	g_pause[ 0 ].fWidth = 100.0f;
	g_pause[ 0 ].fheight = 20.0f;
	g_pause[ 0 ].a = 1.0f;

	g_pause[ 1 ].posPause = D3DXVECTOR3( SCREEN_WIDTH / 2, PAUSE_OFFSET_Y * 2, 0.0f );	
	g_pause[ 1 ].rotPause = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	g_pause[ 1 ].sclPause = D3DXVECTOR3( 1.0f, 1.0f, 1.0f );
	g_pause[ 1 ].fWidth = 100.0f;
	g_pause[ 1 ].fheight = 20.0f;
	g_pause[ 1 ].a = 1.0f;

	g_pause[ 2 ].posPause = D3DXVECTOR3( SCREEN_WIDTH / 2, PAUSE_OFFSET_Y * 3, 0.0f );	
	g_pause[ 2 ].rotPause = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	g_pause[ 2 ].sclPause = D3DXVECTOR3( 1.0f, 1.0f, 1.0f );
	g_pause[ 2 ].fWidth = 100.0f;
	g_pause[ 2 ].fheight = 20.0f;
	g_pause[ 2 ].a = 1.0f;

	g_pause[ 3 ].posPause = D3DXVECTOR3( SCREEN_WIDTH / 2, PAUSE_OFFSET_Y * 4, 0.0f );	
	g_pause[ 3 ].rotPause = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	g_pause[ 3 ].sclPause = D3DXVECTOR3( 1.0f, 1.0f, 1.0f );
	g_pause[ 3 ].fWidth = 100.0f;
	g_pause[ 3 ].fheight = 20.0f;
	g_pause[ 3 ].a = 1.0f;

	g_pause[ 4 ].posPause = D3DXVECTOR3( SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f );	
	g_pause[ 4 ].rotPause = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	g_pause[ 4 ].sclPause = D3DXVECTOR3( 1.0f, 1.0f, 1.0f );
	g_pause[ 4 ].fWidth = 200.0f;
	g_pause[ 4 ].fheight = 220.0f;
	g_pause[ 4 ].a = 0.5f;

	// �I�����Ă��郂�[�h�̏�����
	g_select = CONTINUE;
	
	g_bPauseDraw = true;

	// �e�N�X�`���̓ǂݍ���
	//------------------------------------
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/pause.png",			&g_pTexturePause[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/return_game.png",		&g_pTexturePause[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/retry.png",			&g_pTexturePause[2]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/return_title.png",		&g_pTexturePause[3]);
}


//-----------------------------------------------
//
//�@�|�[�Y�Z�b�g�A�b�v
//�@
//-----------------------------------------------
void SetupPause(void)
{
	// ���[�J���ϐ�
	//------------------------------------
	int nCnt;						// �J�E���^�ϐ�
	VERTEX_2D *pVtx;				// ���_�̍\���̃|�C���^
	LPDIRECT3DDEVICE9 pDevice;		// �f�o�C�X�I�u�W�F�N�g

	// �f�o�C�X�擾
	//------------------------------------
	pDevice = GetDevice();

	// �f�[�^�̈ʒu���m�ۂ���i���b�N�j
	//------------------------------------
	g_pVtxBuffPause ->Lock(0,
						0,
						(void**)&pVtx,
						0);

	for ( nCnt = 0; nCnt < 4; nCnt++ )
	{
		// �t�F�[�h�|���S���̑傫��
		//------------------------------------
		pVtx[ 0 + nCnt * 4 ].pos.x = g_pause[ nCnt ].posPause.x - g_pause[ nCnt ].fWidth;
		pVtx[ 0 + nCnt * 4 ].pos.y = g_pause[ nCnt ].posPause.y - g_pause[ nCnt ].fheight;	// ���_�̈ʒu
		pVtx[ 0 + nCnt * 4 ].pos.z = 0.0f;												// ���_�̈ʒu

		pVtx[ 1 + nCnt * 4 ].pos.x = g_pause[ nCnt ].posPause.x + g_pause[ nCnt ].fWidth;
		pVtx[ 1 + nCnt * 4 ].pos.y = g_pause[ nCnt ].posPause.y - g_pause[ nCnt ].fheight;	// ���_�̈ʒu
		pVtx[ 1 + nCnt * 4 ].pos.z = 0.0f;												// ���_�̈ʒu

		pVtx[ 2 + nCnt * 4 ].pos.x = g_pause[ nCnt ].posPause.x - g_pause[ nCnt ].fWidth;
		pVtx[ 2 + nCnt * 4 ].pos.y = g_pause[ nCnt ].posPause.y + g_pause[ nCnt ].fheight;	// ���_�̈ʒu
		pVtx[ 2 + nCnt * 4 ].pos.z = 0.0f;												// ���_�̈ʒu

		pVtx[ 3 + nCnt * 4 ].pos.x = g_pause[ nCnt ].posPause.x + g_pause[ nCnt ].fWidth;
		pVtx[ 3 + nCnt * 4 ].pos.y = g_pause[ nCnt ].posPause.y + g_pause[ nCnt ].fheight;	// ���_�̈ʒu
		pVtx[ 3 + nCnt * 4 ].pos.z = 0.0f;												// ���_�̈ʒu

		// �@��
		pVtx[ 0 + nCnt * 4 ].rhw = 1.0f;
		pVtx[ 1 + nCnt * 4 ].rhw = 1.0f;
		pVtx[ 2 + nCnt * 4 ].rhw = 1.0f;
		pVtx[ 3 + nCnt * 4 ].rhw = 1.0f;			

		// �t�F�[�h�̐F
		//------------------------------------
		pVtx[ 0 + nCnt * 4 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, g_pause[ nCnt ].a );			// ���_���Ƃ̐F�̐ݒ�
		pVtx[ 1 + nCnt * 4 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, g_pause[ nCnt ].a );			//			��
		pVtx[ 2 + nCnt * 4 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, g_pause[ nCnt ].a );			//			��
		pVtx[ 3 + nCnt * 4 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, g_pause[ nCnt ].a );			//			��

		pVtx[ 0 + nCnt * 4 ].tex = D3DXVECTOR2( 0.0f, 0.0f );
		pVtx[ 1 + nCnt * 4 ].tex = D3DXVECTOR2( 1.0f, 0.0f );
		pVtx[ 2 + nCnt * 4 ].tex = D3DXVECTOR2( 0.0f, 1.0f );
		pVtx[ 3 + nCnt * 4 ].tex = D3DXVECTOR2( 1.0f, 1.0f );
	}

	// �t�F�[�h�|���S���̑傫��
	//------------------------------------
	pVtx[ 0 + 4 * 4 ].pos.x = g_pause[ 4 ].posPause.x - g_pause[ 4 ].fWidth;
	pVtx[ 0 + 4 * 4 ].pos.y = g_pause[ 4 ].posPause.y - g_pause[ 4 ].fheight;	// ���_�̈ʒu
	pVtx[ 0 + 4 * 4 ].pos.z = 0.0f;												// ���_�̈ʒu

	pVtx[ 1 + 4 * 4 ].pos.x = g_pause[ 4 ].posPause.x + g_pause[ 4 ].fWidth;
	pVtx[ 1 + 4 * 4 ].pos.y = g_pause[ 4 ].posPause.y - g_pause[ 4 ].fheight;	// ���_�̈ʒu
	pVtx[ 1 + 4 * 4 ].pos.z = 0.0f;												// ���_�̈ʒu

	pVtx[ 2 + 4 * 4 ].pos.x = g_pause[ 4 ].posPause.x - g_pause[ 4 ].fWidth;
	pVtx[ 2 + 4 * 4 ].pos.y = g_pause[ 4 ].posPause.y + g_pause[ 4 ].fheight;	// ���_�̈ʒu
	pVtx[ 2 + 4 * 4 ].pos.z = 0.0f;												// ���_�̈ʒu

	pVtx[ 3 + 4 * 4 ].pos.x = g_pause[ 4 ].posPause.x + g_pause[ 4 ].fWidth;
	pVtx[ 3 + 4 * 4 ].pos.y = g_pause[ 4 ].posPause.y + g_pause[ 4 ].fheight;	// ���_�̈ʒu
	pVtx[ 3 + 4 * 4 ].pos.z = 0.0f;												// ���_�̈ʒu

	// �p�[�X�y�N�e�B�u�R���N�g
	pVtx[ 0 + 4 * 4 ].rhw = 1.0f;
	pVtx[ 1 + 4 * 4 ].rhw = 1.0f;
	pVtx[ 2 + 4 * 4 ].rhw = 1.0f;
	pVtx[ 3 + 4 * 4 ].rhw = 1.0f;			

	// �t�F�[�h�̐F
	//------------------------------------
	pVtx[ 0 + 4 * 4 ].col = D3DXCOLOR( 0.0f, 0.0f, 0.0f, g_pause[ 4 ].a );			// ���_���Ƃ̐F�̐ݒ�
	pVtx[ 1 + 4 * 4 ].col = D3DXCOLOR( 0.0f, 0.0f, 0.0f, g_pause[ 4 ].a );			//			��
	pVtx[ 2 + 4 * 4 ].col = D3DXCOLOR( 0.0f, 0.0f, 0.0f, g_pause[ 4 ].a );			//			��
	pVtx[ 3 + 4 * 4 ].col = D3DXCOLOR( 0.0f, 0.0f, 0.0f, g_pause[ 4 ].a );			//			��

	pVtx[ 0 + 4 * 4 ].tex = D3DXVECTOR2( 0.0f, 0.0f );
	pVtx[ 1 + 4 * 4 ].tex = D3DXVECTOR2( 1.0f, 0.0f );
	pVtx[ 2 + 4 * 4 ].tex = D3DXVECTOR2( 0.0f, 1.0f );
	pVtx[ 3 + 4 * 4 ].tex = D3DXVECTOR2( 1.0f, 1.0f );

	// ���
	//------------------------------------
	g_pVtxBuffPause -> Unlock();
}

//-----------------------------------------------
//
//�@�|�[�Y�I������
//�@
//-----------------------------------------------
void UninitPause(void)
{
	for ( int nCnt = 0; nCnt < 4; nCnt++ )
	{
		if (g_pTexturePause[ nCnt ] != NULL)
		{ // �g�����e�N�X�`���̉���Ə�����
		  //------------------------------------
			g_pTexturePause[ nCnt ] -> Release();
			g_pTexturePause[ nCnt ] = NULL;
		}
	}

	if (g_pVtxBuffPause != NULL)
	{ // ���_�o�b�t�@�̉���Ə�����
	  //------------------------------------
		g_pVtxBuffPause->Release();
		g_pVtxBuffPause = NULL;
	}
}

//-----------------------------------------------
//
//�@�|�[�Y�X�V����
//�@
//-----------------------------------------------
void UpdatePause(void)
{
	/*if ( GetFade() != FADE_NONE )
	{
		return;
	}*/

	// �|�[�Y�̕`���ON / OFF
	if ( GetKeyboardTrigger( DIK_F4 ) == true && g_bPauseDraw == true )
	{
		g_bPauseDraw = false;
	}
	else if ( GetKeyboardTrigger( DIK_F4 ) == true && g_bPauseDraw == false )
	{
		g_bPauseDraw = true;
	}

	// ���j���[�I������
	if ( GetKeyboardTrigger( DIK_S ) == true /*|| GetGamePadCrossKeyY() == -1000*/ || GetPadElecomCrossKeyDownTrigger() )
	{
		PlaySound( CURSOR_SE );
		g_select = (PAUSE_SELECT)( (int)g_select + 1);
		if ( g_select >= SELECT_MAX )
		{
			g_select = CONTINUE;
		}
	}
	else if ( GetKeyboardTrigger( DIK_W ) == true /*|| GetGamePadCrossKeyY() == -1000*/ || GetPadElecomCrossKeyUpTrigger() )
	{
		PlaySound( CURSOR_SE );
		g_select = (PAUSE_SELECT)( (int)g_select - 1);
		if ( (int)g_select < (int)CONTINUE )
		{
			g_select = RETURN_TITLE;
		}
	}

	// ���j���[�I����ԏ����i ���� / ������ �j
	switch ( g_select )
	{
	case CONTINUE:
		g_pause[ 1 ].a = 1.0f;
		g_pause[ 2 ].a = 0.5f;
		g_pause[ 3 ].a = 0.5f;

		if ( GetKeyboardTrigger( DIK_RETURN ) == true /*|| GetGamePadTrigger( GAMEPAD_A )*/ || GetPadElecomTrigger( PAD_4 ) )
		{
			PlaySound( CANCEL_SE );
			SetMenu();
			g_select = CONTINUE;
			g_bPauseDraw = true;
		}
		break;

	case RETRY:
		g_pause[ 1 ].a = 0.5f;
		g_pause[ 2 ].a = 1.0f;
		g_pause[ 3 ].a = 0.5f;

		if ( GetKeyboardTrigger( DIK_RETURN ) == true /*|| GetGamePadTrigger( GAMEPAD_A )*/ || GetPadElecomTrigger( PAD_4 ) )
		{
			PlaySound( DESIDE4_SE );
			SetFade( FADE_OUT, 60 );
			SetMode( MODE_GAME );
		}
		break;

	case RETURN_TITLE:
		g_pause[ 1 ].a = 0.5f;
		g_pause[ 2 ].a = 0.5f;
		g_pause[ 3 ].a = 1.0f;

		if ( GetKeyboardTrigger( DIK_RETURN ) == true /*|| GetGamePadTrigger( GAMEPAD_A )*/ || GetPadElecomTrigger( PAD_4 ) )
		{
			PlaySound( DESIDE4_SE );
			SetFade( FADE_OUT, 60 );
			SetMode( MODE_TITLE );
		}
		break;
	}

	// ���_���Z�b�g
	//------------------------------------
	SetupPause();
}

//-----------------------------------------------
//
//�@�|�[�Y�`�揈��
//�@
//-----------------------------------------------
void DrawPause(void)
{
	// ���[�J���ϐ�
	//------------------------------------
	LPDIRECT3DDEVICE9 pDevice;						// �f�o�C�X�I�u�W�F�N�g
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;		// �v�Z�p
	
	// �f�o�C�X�̎擾
	//------------------------------------
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���Ƀo�C���h
	//-----------------------------------------
	pDevice ->SetStreamSource(0,
								g_pVtxBuffPause,
								0,
								sizeof(VERTEX_2D));	// ���_�f�[�^�̊Ԋu

	// ���_�t�H�[�}�b�g�̐ݒ�
	//------------------------------------
	pDevice -> SetFVF(FVF_VERTEX_2D);

	if ( g_bPauseDraw == true )
	{
		// �e�N�X�`���̐ݒ�
		//------------------------------------
		pDevice -> SetTexture(0, NULL);

		// �|���S���̕`��
		//------------------------------------
		pDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,	// �v���~�e�B�u�̎��
										16,				// �ǂݍ��ލŏ��̔ԍ�
										2);				// �`�悷��v���~�e�B�u��

	
		for ( int i = 0; i < 4; i++ )
		{
			// �e�N�X�`���̐ݒ�
			//------------------------------------
			pDevice -> SetTexture(0, g_pTexturePause[ i ]);

			// �|���S���̕`��
			//------------------------------------
			pDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,	// �v���~�e�B�u�̎��
											i * 4,				// �ǂݍ��ލŏ��̔ԍ�
											2);				// �`�悷��v���~�e�B�u��
		}
	}
}

//-----------------------------------------------
//
//�@�|�[�Y�`�揈��
//�@
//-----------------------------------------------
bool GetPauseDraw( void )
{
	return g_bPauseDraw;
}

//-----------------------------------------------
//
//�@�|�[�Y�`���ԃZ�b�g
//�@
//-----------------------------------------------
void SetPauseDraw( bool pauseDraw )
{
	g_bPauseDraw = pauseDraw;
}

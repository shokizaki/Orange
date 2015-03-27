//-----------------------------------------------
//  ���e�F���U���g����
//  Fail:result.cpp
//  ����ҁF�؍�@��
//-----------------------------------------------

//-----------------------------------------------
//  �C���N���[�h�t�@�C��
//-----------------------------------------------
#include "result.h"
#include "main.h"
#include "input.h"
#include "fade.h"
//#include "resultbg.h"

//-----------------------------------------------
//�@�}�N����`
//-----------------------------------------------
#define RESULT_MAX  ( 2 )			 // ���_�̐�
#define TEXTURE_MAX ( 2 )			 // �g�p����e�N�X�`���̐�

#define THANKS_POS_X	( SCREEN_WIDTH / 2 )
#define THANKS_POS_Y	( SCREEN_HEIGHT - 100.0f )
#define THANKS_WIDTH	( 300.0f )
#define THANKS_HEIGHT	( 40.0f )

//-----------------------------------------------
//  �O���[�o���ϐ�
//-----------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResult = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureResult[TEXTURE_MAX];		// �e�N�X�`���ւ̃|�C���^
int g_nFrameCount = 0;
bool g_bResultFade = false;

//-----------------------------------------------
// �w�i����������
//-----------------------------------------------
void InitResult()
{
	// ������
	//------------------------------------
	g_nFrameCount = 0;
	g_bResultFade = false;

	// ���[�J���ϐ�
	//---------------------------------
	LPDIRECT3DDEVICE9 pDevice;		// �f�o�C�X�I�u�W�F�N�g
	VERTEX_2D *pVtx;

	// �f�o�C�X�擾
	//------------------------------------
	pDevice = GetDevice();

	pDevice ->CreateVertexBuffer((sizeof( VERTEX_2D ) * 4 * RESULT_MAX),	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@
								 D3DUSAGE_WRITEONLY,				// ���_�o�b�t�@�̎g�p���@
								 FVF_VERTEX_2D,						// �g�p���钸�_�t�H�[�}�b�g
								 D3DPOOL_MANAGED,					// �o�b�t�@��ێ����郁�����N���X�̎w��
								 &g_pVtxBuffResult,					// ���_�o�b�t�@�ւ̃|�C���^
								 NULL);								// NULL�Œ�
	// �^�C�g���w�i����������
	//------------------------------------
	//InitResultBG();

		// �f�[�^�̈ʒu���m�ۂ���i���b�N�j
	//------------------------------------
	g_pVtxBuffResult ->Lock(0,
						0,
						(void**)&pVtx,
						0);

	// 
	//------------------------------------
	pVtx[ 0 ].pos.x = 0.0f;
	pVtx[ 0 ].pos.y = 0.0f;	// ���_�̈ʒu
	pVtx[ 0 ].pos.z = 0.0f;												// ���_�̈ʒu

	pVtx[ 1 ].pos.x = SCREEN_WIDTH;
	pVtx[ 1 ].pos.y = 0.0f;	// ���_�̈ʒu
	pVtx[ 1 ].pos.z = 0.0f;												// ���_�̈ʒu

	pVtx[ 2 ].pos.x = 0.0f;
	pVtx[ 2 ].pos.y = SCREEN_HEIGHT;	// ���_�̈ʒu
	pVtx[ 2 ].pos.z = 0.0f;												// ���_�̈ʒu

	pVtx[ 3 ].pos.x = SCREEN_WIDTH;
	pVtx[ 3 ].pos.y = SCREEN_HEIGHT;	// ���_�̈ʒu
	pVtx[ 3 ].pos.z = 0.0f;												// ���_�̈ʒu

	// �@��
	pVtx[ 0 ].rhw = 1.0f;
	pVtx[ 1 ].rhw = 1.0f;
	pVtx[ 2 ].rhw = 1.0f;
	pVtx[ 3 ].rhw = 1.0f;			

	// �t�F�[�h�̐F
	//------------------------------------
	pVtx[ 0 ].col = D3DXCOLOR( 1.0f, 0.0f, 0.0f, 1.0f );			// ���_���Ƃ̐F�̐ݒ�
	pVtx[ 1 ].col = D3DXCOLOR( 1.0f, 0.0f, 0.0f, 1.0f );			//			��
	pVtx[ 2 ].col = D3DXCOLOR( 1.0f, 0.0f, 0.0f, 1.0f );			//			��
	pVtx[ 3 ].col = D3DXCOLOR( 1.0f, 0.0f, 0.0f, 1.0f );			//			��

	pVtx[ 0 ].tex = D3DXVECTOR2( 0.0f, 0.0f );
	pVtx[ 1 ].tex = D3DXVECTOR2( 1.0f, 0.0f );
	pVtx[ 2 ].tex = D3DXVECTOR2( 0.0f, 1.0f );
	pVtx[ 3 ].tex = D3DXVECTOR2( 1.0f, 1.0f );

	// ���_���Z�b�g
	//------------------------------------
	pVtx[ 4 ].pos.x = SCREEN_WIDTH / 2 - 200.0f;
	pVtx[ 4 ].pos.y = 500.0f - 40.0f;	// ���_�̈ʒu
	pVtx[ 4 ].pos.z = 0.0f;												// ���_�̈ʒu

	pVtx[ 5 ].pos.x = SCREEN_WIDTH / 2 + 200.0f;
	pVtx[ 5 ].pos.y = 500.0f - 40.0f;	// ���_�̈ʒu
	pVtx[ 5 ].pos.z = 0.0f;												// ���_�̈ʒu

	pVtx[ 6 ].pos.x = SCREEN_WIDTH / 2 - 200.0f;
	pVtx[ 6 ].pos.y = 500.0f + 40.0f;	// ���_�̈ʒu
	pVtx[ 6 ].pos.z = 0.0f;												// ���_�̈ʒu

	pVtx[ 7 ].pos.x = SCREEN_WIDTH / 2 + 200.0f;
	pVtx[ 7 ].pos.y = 500.0f + 40.0f;	// ���_�̈ʒu
	pVtx[ 7 ].pos.z = 0.0f;												// ���_�̈ʒu

	// �p�[�X�y�N�e�B�u�R���N�g
	pVtx[ 4 ].rhw = 1.0f;
	pVtx[ 5 ].rhw = 1.0f;
	pVtx[ 6 ].rhw = 1.0f;
	pVtx[ 7 ].rhw = 1.0f;	

	// �F
	//------------------------------------
	pVtx[ 4 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );			// ���_���Ƃ̐F�̐ݒ�
	pVtx[ 5 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );			//			��
	pVtx[ 6 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );			//			��
	pVtx[ 7 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );			//			��

	pVtx[ 4 ].tex = D3DXVECTOR2( 0.0f, 0.0f );
	pVtx[ 5 ].tex = D3DXVECTOR2( 1.0f, 0.0f );
	pVtx[ 6 ].tex = D3DXVECTOR2( 0.0f, 1.0f );
	pVtx[ 7 ].tex = D3DXVECTOR2( 1.0f, 1.0f );

	// ���
	//------------------------------------
	g_pVtxBuffResult -> Unlock();

	// �e�N�X�`���̓ǂݍ���
	//------------------------------------
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/result.jpg",	&g_pTextureResult[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/thanks.png", &g_pTextureResult[ 1 ]);
}

//-----------------------------------------------
// �w�i�I������
//-----------------------------------------------
void UninitResult()
{
	for ( int nCnt = 0; nCnt < 4; nCnt++ )
	{
		if (g_pTextureResult[ nCnt ] != NULL)
		{ // �g�����e�N�X�`���̉���Ə�����
		  //------------------------------------
			g_pTextureResult[ nCnt ] -> Release();
			g_pTextureResult[ nCnt ] = NULL;
		}
	}

	if (g_pVtxBuffResult != NULL)
	{ // ���_�o�b�t�@�̉���Ə�����
	  //------------------------------------
		g_pVtxBuffResult->Release();
		g_pVtxBuffResult = NULL;
	}

	//StopSound();
}

//-----------------------------------------------
// ���U���g�X�V����
//-----------------------------------------------
void UpdateResult()
{
	// �G���^�[����������
	//------------------------------------
	// �J�ڔ���
	if ( ( GetKeyboardTrigger(DIK_RETURN) /*|| GetGamePadTrigger( GAMEPAD_START ) || GetGamePadTrigger( GAMEPAD_A )*/ || GetPadElecomTrigger( PAD_4 ) || GetPadElecomTrigger( PAD_10 ) ) && GetFade() == FADE_NONE )
	{
		PlaySound( DESIDE4_SE );
		SetFade( FADE_OUT, 60 );
		SetMode( MODE_TITLE );

		g_bResultFade = true;
	}
}

//-----------------------------------------------
// �w�i�`�揈��
//-----------------------------------------------
void DrawResult()
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
								g_pVtxBuffResult,
								0,
								sizeof(VERTEX_2D));	// ���_�f�[�^�̊Ԋu

	// ���_�t�H�[�}�b�g�̐ݒ�
	//------------------------------------
	pDevice -> SetFVF(FVF_VERTEX_2D);

	for ( int i = 0; i < RESULT_MAX; i++ )
	{
		//// �e�N�X�`���̐ݒ�
		////------------------------------------
		//pDevice -> SetTexture(0, g_pTextureResult[ i ]);

		// �e�N�X�`���̐ݒ�
		//------------------------------------
		pDevice -> SetTexture( 0, NULL );

		// �|���S���̕`��
		//------------------------------------
		pDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,	// �v���~�e�B�u�̎��
										i * 4,				// �ǂݍ��ލŏ��̔ԍ�
										2);				// �`�悷��v���~�e�B�u��
	}
}

//-----------------------------------------------
// ���_���Z�b�g
//-----------------------------------------------
void SetupResult()
{
}
//===============================================
//
//  ���e   �F ���[�U�[�C���^�[�t�F�[�X����
//  File   �F UI.h
//  ����� �F �؍�@��
//
//===============================================

//------ �C���N���[�h�t�@�C�� ------
#include "UI.h"
#include "bullet.h"
#include "model.h"
#include "texture2D.h"
#include "stringDraw.h"
#include "number.h"

//------ �}�N����` ------
#define UI_TEX_MAX	( 5 )

#define BELL_TEX		"data/TEXTURE/suzu000.png"
#define CATCHENEMY_TEX	"data/TEXTURE/absorbNum.png"

// �����̐F������UI
#define BELL_WIDTH	( 70.0f )
#define BELL_HEIGHT ( 70.0f )
#define BELL_POS_X	( 80.0f )
#define BELL_POS_Y	( SCREEN_HEIGHT - 80.0f )

// �z����������UI
#define BELLSTATUS_WIDTH	( 120.0f )
#define BELLSTATUS_HEIGHT	( 120.0f )
#define BELLSTATUS_POS_X	( SCREEN_WIDTH - 100.0f )
#define BELLSTATUS_POS_Y	( SCREEN_HEIGHT - 100.0f )

// �z��������������UI
#define CATCHENEMY_WIDTH	( 90.0f )
#define CATCHENEMY_HEIGHT	( 40.0f )
#define CATCHENEMY_POS_X	( 100.0f )
#define CATCHENEMY_POS_Y	( 50.0f )
#define CATCHNUM_WIDTH		( 15.0f )
#define CATCHNUM_HEIGHT		( 30.0f )
#define CATCHNUM_POS_X		( CATCHENEMY_POS_X + 40.0f )
#define CATCHNUM_POS_Y		( CATCHENEMY_POS_Y + 35.0f )

//------ �O���[�o���ϐ� ------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUI = NULL;			// ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9		g_pTextureUI[ UI_TEX_MAX ];		// �e�N�X�`���ւ̃|�C���^

UI g_aUI[ UI_MAX ];		// UI�̏��
bool g_bHasty = false;

NUMBER g_absorbNum;		// �z����������`�悷�邽�߂̕ϐ�

//D3DXMATRIX g_mtxProjectionUI;		// �v���W�F�N�V�����}�g���b�N�X

//===============================================
//
// UI�Ɋւ��鏉����
//
//===============================================
void InitUI( void )
{
	// ���[�J���ϐ�
	//---------------------------------
	LPDIRECT3DDEVICE9 pDevice = NULL;		// �f�o�C�X�I�u�W�F�N�g

	// �O���[�o���ϐ��̏�����
	//---------------------------------

	// [ �� ]
	g_aUI[ 0 ].pos = D3DXVECTOR3( BELL_POS_X, BELL_POS_Y, 0.0f );
	g_aUI[ 0 ].fWidth = BELL_WIDTH;
	g_aUI[ 0 ].fHeight = BELL_HEIGHT;
	g_aUI[ 0 ].type = UITYPE_BELL;

	// [ ��X�e�[�^�X ]
	g_aUI[ 1 ].pos = D3DXVECTOR3( BELLSTATUS_POS_X, BELLSTATUS_POS_Y, 0.0f );
	g_aUI[ 1 ].fWidth = BELLSTATUS_WIDTH;
	g_aUI[ 1 ].fHeight = BELLSTATUS_HEIGHT;
	g_aUI[ 1 ].type = UITYPE_BELL;

	// [ �G���z�������� ]
	g_aUI[ 2 ].pos = D3DXVECTOR3( CATCHENEMY_POS_X, CATCHENEMY_POS_Y, 0.0f );
	g_aUI[ 2 ].fWidth = CATCHENEMY_WIDTH;
	g_aUI[ 2 ].fHeight = CATCHENEMY_HEIGHT;
	g_aUI[ 2 ].type = UITYPE_CATCHENEMY;
	SetNumber( &g_absorbNum, D3DXVECTOR3( CATCHNUM_POS_X, CATCHNUM_POS_Y, 0.0f ), 2, CATCHNUM_WIDTH, CATCHNUM_HEIGHT );

	g_bHasty = false;

	// �f�o�C�X�擾
	//------------------------------------
	pDevice = GetDevice();

	// ���_�o�b�t�@�̊m��
	//------------------------------------
	pDevice ->CreateVertexBuffer((sizeof(VERTEX_2D) * 4 * UI_MAX),		// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@
								 D3DUSAGE_WRITEONLY,					// ���_�o�b�t�@�̎g�p���@
								 FVF_VERTEX_2D,							// �g�p���钸�_�t�H�[�}�b�g
								 D3DPOOL_MANAGED,						// �o�b�t�@��ێ����郁�����N���X�̎w��
								 &g_pVtxBuffUI,							// ���_�o�b�t�@�ւ̃|�C���^
								 NULL);									// NULL�Œ�	

	// �e�N�X�`���ւ̃|�C���^��������
	//------------------------------------
	for (int nCnt = 0; nCnt < UI_TEX_MAX; nCnt++)				
	{
		g_pTextureUI[ nCnt ] = NULL;
	}

	// �e�N�X�`���̓ǂݍ���
	//------------------------------------
	D3DXCreateTextureFromFile(pDevice, BELL_TEX, &g_pTextureUI[ 0 ]);
	D3DXCreateTextureFromFile(pDevice, CATCHENEMY_TEX, &g_pTextureUI[ 1 ]);

	// ���_���̃Z�b�g
	//------------------------------------
	SetupUI();
}

//===============================================
//
// UI�̒��_���Z�b�g
//
//===============================================
void SetupUI( void )
{
	// ���[�J���ϐ�
	//---------------------------------
	LPDIRECT3DDEVICE9 pDevice = NULL;		// �f�o�C�X�I�u�W�F�N�g
	VERTEX_2D *pVtx2D;

	// �f�o�C�X�擾
	//------------------------------------
	pDevice = GetDevice();

	g_pVtxBuffUI ->Lock( 0, 0, (void**)&pVtx2D, 0 );

	for ( int i = 0; i < UI_MAX; i++ )
	{
		// �ʒu
		pVtx2D[ 0 + i * 4 ].pos = D3DXVECTOR3( g_aUI[ i ].pos.x - g_aUI[ i ].fWidth, g_aUI[ i ].pos.y - g_aUI[ i ].fHeight, 0.0f );
		pVtx2D[ 1 + i * 4 ].pos = D3DXVECTOR3( g_aUI[ i ].pos.x + g_aUI[ i ].fWidth, g_aUI[ i ].pos.y - g_aUI[ i ].fHeight, 0.0f );
		pVtx2D[ 2 + i * 4 ].pos = D3DXVECTOR3( g_aUI[ i ].pos.x - g_aUI[ i ].fWidth, g_aUI[ i ].pos.y + g_aUI[ i ].fHeight, 0.0f );
		pVtx2D[ 3 + i * 4 ].pos = D3DXVECTOR3( g_aUI[ i ].pos.x + g_aUI[ i ].fWidth, g_aUI[ i ].pos.y + g_aUI[ i ].fHeight, 0.0f );

		// �p�[�X�y�N�e�B�u�R���N�g���F
		for ( int j = 0; j < 4; j++ )
		{
			pVtx2D[ j + i * 4 ].rhw = 1.0f;
			pVtx2D[ j + i * 4 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );
		}

		// �e�N�X�`�����W
		pVtx2D[ 0 + i * 4  ].tex = D3DXVECTOR2( 0.0f, 0.0f );
		pVtx2D[ 1 + i * 4  ].tex = D3DXVECTOR2( 1.0f, 0.0f );
		pVtx2D[ 2 + i * 4  ].tex = D3DXVECTOR2( 0.0f, 1.0f );
		pVtx2D[ 3 + i * 4  ].tex = D3DXVECTOR2( 1.0f, 1.0f );
	}

	g_pVtxBuffUI ->Unlock();
}

//===============================================
//
// UI�Ɋւ���I������
//
//===============================================
void UninitUI( void )
{
	// �g�����e�N�X�`���̉���Ə�����
	//------------------------------------
	for (int nCnt = 0; nCnt < UI_TEX_MAX; nCnt++)
	{
		if (g_pTextureUI[ nCnt ] != NULL)
		{ 
			g_pTextureUI[ nCnt ] ->Release();
			g_pTextureUI[ nCnt ] = NULL;
		}
	}

	// ���_�o�b�t�@�̉���Ə�����
	//------------------------------------
	if (g_pVtxBuffUI != NULL)
	{ 
		g_pVtxBuffUI ->Release();
		g_pVtxBuffUI = NULL;
	}

	UninitNumber( &g_absorbNum );
}

//===============================================
//
// UI�Ɋւ���X�V����
//
//===============================================
void UpdateUI( void )
{
	// �o���b�gUI�̍X�V
	UpdateUIBullet();

	// �z����UI�̍X�V
	UpdateUIAbsorb();

	// �z��������UI�̍X�V
	UpdateUICatchEnemy();
}

//===============================================
//
// UI�Ɋւ���`�揈��
//
//===============================================
void DrawUI( void )
{
	// ���[�J���ϐ�
	//------------------------------------
	LPDIRECT3DDEVICE9 pDevice;						// �f�o�C�X�I�u�W�F�N�g
	
	// �f�o�C�X�̎擾
	//------------------------------------
	pDevice = GetDevice();

	// �����̕`��
	DrawNumber( &g_absorbNum, GetCatchEnemy() );

	// ���_�o�b�t�@���f�[�^�X�g���[���Ƀo�C���h
	//-----------------------------------------
	pDevice ->SetStreamSource(0,
								g_pVtxBuffUI,
								0,
								sizeof(VERTEX_2D));	// ���_�f�[�^�̊Ԋu

	
	// ���_�t�H�[�}�b�g�̐ݒ�
	//------------------------------------
	pDevice ->SetFVF( FVF_VERTEX_2D );

	{
		for ( int i = 0; i < UI_MAX; i++ )
		{
			switch ( g_aUI[ i ].type )
			{
			case UITYPE_BELL:
			
				// �e�N�X�`���̐ݒ�
				//------------------------------------
				pDevice -> SetTexture(0, g_pTextureUI[ ( int )g_aUI[ i ].type ]);
				break;

			case UITYPE_CATCHENEMY:
			
				// �e�N�X�`���̐ݒ�
				//------------------------------------
				pDevice -> SetTexture(0, g_pTextureUI[ ( int )g_aUI[ i ].type ]);

				break;

			case UITYPE_MAX:

				// �e�N�X�`���̐ݒ�
				//------------------------------------
				pDevice -> SetTexture(0, NULL);
				break;
			}

			// �|���S���̕`��
			//------------------------------------
			pDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,	// �v���~�e�B�u�̎��
											i * 4,				// �ǂݍ��ލŏ��̔ԍ�
											2);				// �`�悷��v���~�e�B�u��
		}
	}
}

//===============================================
//
// �o���b�g�֘A��UI�X�V
//
//===============================================
void UpdateUIBullet( void )
{
	VERTEX_2D *pVtx2D;

	g_pVtxBuffUI ->Lock( 0, 0, (void**)&pVtx2D, 0 );

	if ( ( GetKeyboardTrigger( DIK_UP ) || GetKeyboardTrigger( DIK_DOWN ) || GetGamePadTrigger( GAMEPAD_X ) || GetGamePadTrigger( GAMEPAD_B ) )
	&&	 ( ( GetMode() == MODE_TUTORIAL && GetEnterFlag() == true ) || GetMode() == MODE_GAME ) )
	{
		PlaySound( SUZU001_SE );
		
		// �e�N�X�`���̃Z�b�g
		SetTexture2D( g_aUI[ 0 ].pos, 100.0f, 100.0f, TEXTURE2DTYPE_000 );
	}

	// �����̔���
	switch ( GetBulletType() )
	{
	case BULLETTYPE_ONP_RED:

		// �ԐF
		pVtx2D[ 0 ].col = D3DXCOLOR( 1.0f, 0.0f, 0.0f, 1.0f );
		pVtx2D[ 1 ].col = D3DXCOLOR( 1.0f, 0.0f, 0.0f, 1.0f );
		pVtx2D[ 2 ].col = D3DXCOLOR( 1.0f, 0.0f, 0.0f, 1.0f );
		pVtx2D[ 3 ].col = D3DXCOLOR( 1.0f, 0.0f, 0.0f, 1.0f );

		break;

	case BULLETTYPE_ONP_YELLOW:

		// ���F
		pVtx2D[ 0 ].col = D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f );
		pVtx2D[ 1 ].col = D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f );
		pVtx2D[ 2 ].col = D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f );
		pVtx2D[ 3 ].col = D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f );

		break;

	case BULLETTYPE_ONP_BLUE:

		// �F
		pVtx2D[ 0 ].col = D3DXCOLOR( 0.0f, 0.0f, 1.0f, 1.0f );
		pVtx2D[ 1 ].col = D3DXCOLOR( 0.0f, 0.0f, 1.0f, 1.0f );
		pVtx2D[ 2 ].col = D3DXCOLOR( 0.0f, 0.0f, 1.0f, 1.0f );
		pVtx2D[ 3 ].col = D3DXCOLOR( 0.0f, 0.0f, 1.0f, 1.0f );

		break;
	}

	g_pVtxBuffUI ->Unlock();
}

//===============================================
//
// �z�����֘A��UI�X�V
//
//===============================================
void UpdateUIAbsorb( void )
{
	// ���[�J���ϐ�
	//------------------------------------
	VERTEX_2D *pVtx2D;

	g_pVtxBuffUI ->Lock( 0, 0, (void**)&pVtx2D, 0 );

	if ( GetCatchEnemy() >= 0 &&  GetCatchEnemy() < 5 )
	{
		// �ΐF
		pVtx2D[ 4 ].col = D3DXCOLOR( 0.0f, 1.0f, 0.0f, 1.0f );
		pVtx2D[ 5 ].col = D3DXCOLOR( 0.0f, 1.0f, 0.0f, 1.0f );
		pVtx2D[ 6 ].col = D3DXCOLOR( 0.0f, 1.0f, 0.0f, 1.0f );
		pVtx2D[ 7 ].col = D3DXCOLOR( 0.0f, 1.0f, 0.0f, 1.0f );

		if ( g_bHasty == true )
		{
			g_bHasty = false;
			SetTexture2DUse( g_aUI[ 1 ].nNumTexture2D );
		}
	}
	else if ( GetCatchEnemy() >= 5 &&  GetCatchEnemy() < 8 )
	{
		// ���F
		pVtx2D[ 4 ].col = D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f );
		pVtx2D[ 5 ].col = D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f );
		pVtx2D[ 6 ].col = D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f );
		pVtx2D[ 7 ].col = D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f );

		if ( g_bHasty == true )
		{
			g_bHasty = false;
			SetTexture2DUse( g_aUI[ 1 ].nNumTexture2D );
		}
	}
	else if ( GetCatchEnemy() >= 10 )
	{
		// �ԐF
		pVtx2D[ 4 ].col = D3DXCOLOR( 1.0f, 0.0f, 0.0f, 1.0f );
		pVtx2D[ 5 ].col = D3DXCOLOR( 1.0f, 0.0f, 0.0f, 1.0f );
		pVtx2D[ 6 ].col = D3DXCOLOR( 1.0f, 0.0f, 0.0f, 1.0f );
		pVtx2D[ 7 ].col = D3DXCOLOR( 1.0f, 0.0f, 0.0f, 1.0f );

		if ( g_bHasty == false )
		{
			g_bHasty = true;
			g_aUI[ 1 ].nNumTexture2D = SetTexture2D( D3DXVECTOR3( g_aUI[ 1 ].pos.x, g_aUI[ 1 ].pos.y - g_aUI[ 1 ].fHeight - 20.0f, 0.0f ), 30.0f, 30.0f, TEXTURE2DTYPE_HASTY2 );
		}
	}

	g_pVtxBuffUI ->Unlock();
}

//===============================================
//
// �G���z���������֘A��UI�X�V
//
//===============================================
void UpdateUICatchEnemy(void)
{

}
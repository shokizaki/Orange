//=======================================================================================
//
//	�N���X�FAT11C272 30623 13
//	����ҁF�؍�@��
//	���e�F�t�F�[�h�C���E�A�E�g
//
//=======================================================================================

//-----------------------------------------------
//
//  �C���N���[�h�t�@�C��
//
//-----------------------------------------------
#include "fade.h"
#include "mode.h"
#include "camera.h"

//-----------------------------------------------
//�@�}�N����`
//-----------------------------------------------
#define FADE_VTX_MAX	( 4 )		 // ���_�̐�
#define TEXTURE_MAX		( 1 )		 // �g�p����e�N�X�`���̐�
#define FADE_FRAME		( 60 )

//-----------------------------------------------
//
//  �O���[�o���ϐ� 
//
//-----------------------------------------------
static float fadeR = 1.0f;
static float fadeG = 1.0f;
static float fadeB = 1.0f;
static float fadeA = 1.0f;

FADE g_fade = FADE_NONE;
int fadeFrame = 0;
int fadeCount = 0;
float g_fAlphaPlus = 0;

bool fadeEnable = false;
bool fadeExecute = false;
bool fadeIn = false;

LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFade = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureFade[TEXTURE_MAX];		// �e�N�X�`���ւ̃|�C���^

D3DXVECTOR3 posFade;			// �ʒu
D3DXVECTOR3 rotFade;			// ����
D3DXVECTOR3 sclFade;			// �傫��

D3DXMATRIX mtxViewFade;			// ���[���h�r���[
D3DXMATRIX mtxWorldFade;		// ���[���h�}�g���b�N�X
D3DXMATRIX g_mtxProjectionFade;	// �v���W�F�N�V�����}�g���b�N�X

//-----------------------------------------------
//
//   �֐��v���g�^�C�v�錾
//
//-----------------------------------------------

//-----------------------------------------------
// �t�F�[�h�J���[�Z�b�g
//-----------------------------------------------
static void SetFadeColor( float r, float g, float b);	// �����֐�

//-----------------------------------------------
// �t�F�[�h�J���[�Z�b�g
//-----------------------------------------------
static void SetFadeColor(float r, float g, float b)
{
	fadeR = r;
	fadeG = g;
	fadeB = b;
}

//-----------------------------------------------
//
//�@�t�F�[�h������
//�@
//-----------------------------------------------
void InitFade()
{
	// ���[�J���ϐ�
	//---------------------------------
	LPDIRECT3DDEVICE9 pDevice;		// �f�o�C�X�I�u�W�F�N�g

	// ������
	//------------------------------------
	fadeFrame = 0;
	fadeCount = 0;
	g_fAlphaPlus = 0;
	fadeIn = true;
	fadeR = 1.0f;
	fadeG = 1.0f;
	fadeB = 1.0f;
	fadeA = 1.0f;

	g_fade = FADE_NONE;

	// �f�o�C�X�擾
	//------------------------------------
	pDevice = GetDevice();

	pDevice ->CreateVertexBuffer((sizeof( VERTEX_2D ) * FADE_VTX_MAX),	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@
								 D3DUSAGE_WRITEONLY,				// ���_�o�b�t�@�̎g�p���@
								 FVF_VERTEX_2D,						// �g�p���钸�_�t�H�[�}�b�g
								 D3DPOOL_MANAGED,					// �o�b�t�@��ێ����郁�����N���X�̎w��
								 &g_pVtxBuffFade,					// ���_�o�b�t�@�ւ̃|�C���^
								 NULL);								// NULL�Œ�

	posFade = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	rotFade = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	sclFade = D3DXVECTOR3( 1.0f, 1.0f, 1.0f );

	// �w�i�̒��_���Z�b�g
	//---------------------------------
	SetupFade();
}

//-----------------------------------------------
//
//�@�t�F�[�h�Z�b�g�A�b�v
//�@
//-----------------------------------------------
void SetupFade()
{
	// ���[�J���ϐ�
	//------------------------------------
	VERTEX_2D *pVtx;				// ���_�̍\���̃|�C���^
	LPDIRECT3DDEVICE9 pDevice;		// �f�o�C�X�I�u�W�F�N�g

	// �f�o�C�X�擾
	//------------------------------------
	pDevice = GetDevice();

	// �f�[�^�̈ʒu���m�ۂ���i���b�N�j
	//------------------------------------
	g_pVtxBuffFade ->Lock(0,
						0,
						(void**)&pVtx,
						0);

	// �t�F�[�h�|���S���̑傫��
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
	pVtx[ 0 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, fadeA );			// ���_���Ƃ̐F�̐ݒ�
	pVtx[ 1 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, fadeA );			//			��
	pVtx[ 2 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, fadeA );			//			��
	pVtx[ 3 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, fadeA );			//			��

	pVtx[ 0 ].tex = D3DXVECTOR2( 0.0f, 0.0f );
	pVtx[ 1 ].tex = D3DXVECTOR2( 1.0f, 0.0f );
	pVtx[ 2 ].tex = D3DXVECTOR2( 0.0f, 1.0f );
	pVtx[ 3 ].tex = D3DXVECTOR2( 1.0f, 1.0f );

	// ���
	//------------------------------------
	g_pVtxBuffFade -> Unlock();
}

//-----------------------------------------------
//
//�@�t�F�[�h�I������
//�@
//-----------------------------------------------
void UninitFade()
{
	// ���[�J���ϐ�
	//------------------------------------
	int nCnt;

	for (nCnt = 0; nCnt < TEXTURE_MAX; nCnt++)
	{
		if (g_pTextureFade[nCnt] != NULL)
		{ // �g�����e�N�X�`���̉���Ə�����
		  //------------------------------------
			g_pTextureFade[nCnt] -> Release();
			g_pTextureFade[nCnt] = NULL;
		}
	}

	if (g_pVtxBuffFade != NULL)
	{ // ���_�o�b�t�@�̉���Ə�����
	  //------------------------------------
		g_pVtxBuffFade->Release();
		g_pVtxBuffFade = NULL;
	}
}

//-----------------------------------------------
//
//�@�t�F�[�h�C���֐�
//�@�t�F�[�h�C�����w��F�ŃX�^�[�g����
//  r,g,b�@�E�E�E�@�t�F�[�h�F�@
//�@frame�@�E�E�E�@�t�F�[�h���ԁi�t���[�����j
//�@
//-----------------------------------------------
void FadeIn( float r, float g, float b, int frame )
{
	SetFadeColor( r, g, b );		// �t�F�[�h�F���w��
	fadeFrame = frame;				// �t�F�[�h����t���[����������
	fadeCount = 0;					// �J�E���g������
	fadeExecute = true;				// �t�F�[�h��L��
	fadeEnable = true;				// �t�F�[�h���s��
	fadeIn = true;					// �t�F�[�h�C��
	g_fAlphaPlus = 1.0f / fadeFrame;	// ���Z�����A���t�@�̗�
}

//-----------------------------------------------
//
//�@�t�F�[�h�A�E�g�֐�
//�@�t�F�[�h�A�E�g���w��F�ŃX�^�[�g����
//  r,g,b�@�E�E�E�@�t�F�[�h�F�@
//�@frame�@�E�E�E�@�t�F�[�h���ԁi�t���[�����j
//�@
//-----------------------------------------------
void FadeOut( float r, float g, float b, int frame )
{
	SetFadeColor( r, g, b );		// �t�F�[�h�F���w��
	fadeFrame = frame;				// �t�F�[�h����t���[����������
	fadeCount = 0;					// �J�E���g������
	fadeExecute = true;				// �t�F�[�h���s��
	fadeEnable = true;				// �t�F�[�h��L��
	fadeIn = false;					// �t�F�[�h�A�E�g
	g_fAlphaPlus = 1.0f / fadeFrame;	// ���Z�����A���t�@�̗�
}

//-----------------------------------------------
//
//�@�t�F�[�h�X�V�����֐�
//�@
//-----------------------------------------------
void UpdateFade(void)
{
	CAMERA *pCamera = GetCamera();

	// �t�F�[�h������������������
	//----------------------------------------------
	if ( g_fade == FADE_NONE )
	{
		return;
	}

	// �t�F�[�h�v�Z
	//------------------------------------
	//fadeA = (int)( 255 * (fadeIn ? 1 - (float)fadeCount / fadeFrame : (float)fadeCount / fadeFrame) );

	// �t�F�[�h�C������
	//----------------------------------------------
	if ( g_fade == FADE_IN )
	{
		g_fAlphaPlus = 1.0f / FADE_FRAME;

		fadeA -= g_fAlphaPlus;
		if (fadeCount == FADE_FRAME)
		{
			fadeA = 0.0f;

			fadeCount = 0;

			g_fade = FADE_NONE;			// �t�F�[�h�̏I��
		}
	}

	// �t�F�[�h�A�E�g����
	//----------------------------------------------
	if ( g_fade == FADE_OUT )
	{
		g_fAlphaPlus = 1.0f / FADE_FRAME;

		fadeA += g_fAlphaPlus;
		if (fadeCount == FADE_FRAME)
		{
			fadeA = 1.0f;

			fadeCount = 0;

			g_fade = FADE_NONE;			// �t�F�[�h�̏I��
			
			// ���[�h�J��
			//-----------------------------------
			ChangeScene();
		}
	}

	// �t�F�[�h�J�E���g�A�b�v
	//----------------------------------------------
	fadeCount += 1;

	// �t�F�[�h�|���S���̈ʒu���J�����̖ڂ̑O�ɂ���
	//posFade = D3DXVECTOR3( pCamera->posCameraP.x, pCamera->posCameraP.y, pCamera->posCameraP.z + ( 22.0f * cosf( pCamera->rotCamera.y ) ) );

	SetupFade();

	// �t�F�[�h�J�E���g���t�F�[�h�t���[����葽���Ȃ�����
	//---------------------------------------------------
	//if (FADE_FRAME < fadeCount)
	//{
	//	fadeCount = fadeFrame;		// �t�F�[�h�t���[��������
	//	fadeExecute = false;		// �t�F�[�h�I��
	//	return;
	//}
}

//-----------------------------------------------
//
//�@�t�F�[�h�`�揈���֐�
//�@
//-----------------------------------------------
void DrawFade(void)
{
	// ���[�J���ϐ�
	//------------------------------------
	LPDIRECT3DDEVICE9 pDevice;						// �f�o�C�X�I�u�W�F�N�g
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;		// �v�Z�p
	CAMERA *pCamera = GetCamera();					// �J�����擾
	
	// �f�o�C�X�̎擾
	//------------------------------------
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���Ƀo�C���h
	//-----------------------------------------
	pDevice ->SetStreamSource(0,
								g_pVtxBuffFade,
								0,
								sizeof(VERTEX_2D));	// ���_�f�[�^�̊Ԋu

	// ���_�t�H�[�}�b�g�̐ݒ�
	//------------------------------------
	pDevice -> SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	//------------------------------------
	pDevice -> SetTexture(0, NULL);

	// �|���S���̕`��
	//------------------------------------
	pDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,	// �v���~�e�B�u�̎��
									0,				// �ǂݍ��ލŏ��̔ԍ�
									2);				// �`�悷��v���~�e�B�u��

	switch ( g_fade )
	{
	case FADE_NONE:
		PrintDebugProc("FADE_NONE\n");
		break;

	case FADE_IN:
		PrintDebugProc("FADE_IN\n");
		break;

	case FADE_OUT:
		PrintDebugProc("FADE_OUT\n");
		break;

	}
	
}

//-----------------------------------------------
//
//�@���݂̃t�F�[�h�擾
//�@
//-----------------------------------------------
FADE GetFade( void )
{
	return g_fade;
}

//-----------------------------------------------
//
//�@�t�F�[�h�̃Z�b�g
//�@
//-----------------------------------------------
void SetFade( FADE fadeSet )
{
	g_fade = fadeSet;
}

//-----------------------------------------------
//
//�@�t�F�[�h�̈ʒu�Z�b�g
//�@
//-----------------------------------------------
void SetFadePos( D3DXVECTOR3 pos, D3DXVECTOR3 rot )
{
	posFade.x = pos.x;
	posFade.x = pos.y;
	posFade.x = pos.z + ( 21.0f * cosf( rot.y ) );
}
//-----------------------------------------------
//  ���e�F�^�C�g������
//  Fail:title.cpp
//  ����ҁF�؍�@��
//-----------------------------------------------

//-----------------------------------------------
//  �C���N���[�h�t�@�C��
//-----------------------------------------------
#include "title.h"
#include "main.h" 
#include "input.h"
#include "fade.h"
#include "camera.h"
#include "TitlePreBtn.h"

//-----------------------------------------------
//�@�}�N����`
//-----------------------------------------------

//-----------------------------------------------
//�@�\���̒�`
//-----------------------------------------------

typedef struct tagTITLE
{
    LPDIRECT3DVERTEXBUFFER9 vtx;		// ���_�o�b�t�@�ւ̃|�C���^
    LPDIRECT3DTEXTURE9      tex;		// �e�N�X�`���ւ̃|�C���^
    unsigned int            nCntFrame;  // �o�߃t���[��
}TITLE;

//-----------------------------------------------
//  �֐��v���g�^�C�v�錾
//-----------------------------------------------

//-----------------------------------------------
//�@�}�N����`
//-----------------------------------------------

//-----------------------------------------------
//  �O���[�o���ϐ�
//-----------------------------------------------
TITLE g_Title;  // �^�C�g���̃Z�b�g
bool g_bTitleFade = false;

//-----------------------------------------------
// �w�i����������
//-----------------------------------------------
void InitTitle()
{
    // �o�߃t���[���̃��Z�b�g
    g_Title.nCntFrame = 0;

    // BGM
	PlaySound( TITLE_BGM );

	// �f�o�C�X�擾
	//------------------------------------
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice ->CreateVertexBuffer((sizeof( VERTEX_2D ) * 4),	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@
								 D3DUSAGE_WRITEONLY,				// ���_�o�b�t�@�̎g�p���@
								 FVF_VERTEX_2D,						// �g�p���钸�_�t�H�[�}�b�g
								 D3DPOOL_MANAGED,					// �o�b�t�@��ێ����郁�����N���X�̎w��
								 &g_Title.vtx,					// ���_�o�b�t�@�ւ̃|�C���^
								 NULL);								// NULL�Œ�

	// �f�[�^�̈ʒu���m�ۂ���i���b�N�j
	//------------------------------------
    VERTEX_2D *pVtx;
	g_Title.vtx->Lock( 0, 0, (void**)&pVtx, 0 );

	// ���_�o�b�t�@�̕ҏW
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

	// �F
	pVtx[ 0 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );			// ���_���Ƃ̐F�̐ݒ�
	pVtx[ 1 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );			//			��
	pVtx[ 2 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );			//			��
	pVtx[ 3 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );			//			��

    // �e�N�X�`��
	pVtx[ 0 ].tex = D3DXVECTOR2( 0.0f, 0.0f );
	pVtx[ 1 ].tex = D3DXVECTOR2( 1.0f, 0.0f );
	pVtx[ 2 ].tex = D3DXVECTOR2( 0.0f, 1.0f );
	pVtx[ 3 ].tex = D3DXVECTOR2( 1.0f, 1.0f );

	// �ҏW�I��
	//------------------------------------
	g_Title.vtx -> Unlock();

	// �e�N�X�`���̓ǂݍ���
	//------------------------------------
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Title/Title.jpg",	&g_Title.tex );


    // PreBtn
    InitTitlePreBtn();
}

//-----------------------------------------------
// �w�i�I������
//-----------------------------------------------
void UninitTitle()
{
    // PreBtn
    UninitTitlePreBtn();

    // BGM
	StopSound();

    // �e�N�X�`���o�b�t�@�̉��
    if( g_Title.tex != NULL )
    {
        g_Title.tex->Release();
        g_Title.tex = NULL;
    }

    // ���_�o�b�t�@�̉��
    if( g_Title.vtx != NULL )
    {
        g_Title.vtx->Release();
        g_Title.vtx = NULL;
    }
}

//-----------------------------------------------
// �w�i�X�V����
//-----------------------------------------------
void UpdateTitle()
{
    UpdateTitlePreBtn();

    g_Title.nCntFrame++;
    if( g_Title.nCntFrame >= 60 * 60 )
    {
		SetFade( FADE_OUT, 60 );
		SetMode( MODE_RANKING );
    }

	// �J�ڔ���
	if ( ( GetKeyboardTrigger(DIK_RETURN) || GetPadElecomTrigger( PAD_4 ) ) && GetFade() == FADE_NONE)
	{
		PlaySound( DESIDE4_SE );
		SetFade( FADE_OUT, 60 );
		SetMode( MODE_GAME );
	}
}

//-----------------------------------------------
// �w�i�`�揈��
//-----------------------------------------------
void DrawTitle()
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();
    pDevice->SetStreamSource( 0, g_Title.vtx, 0, sizeof( VERTEX_2D ) );
    pDevice->SetFVF( FVF_VERTEX_2D );
    pDevice->SetTexture( 0, g_Title.tex );
    pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );

    DrawTitlePreBtn();
}

//-----------------------------------------------
// �t�F�[�h�����ǂ���
//-----------------------------------------------
bool GetTitleFade( void )
{
	return g_bTitleFade;
}


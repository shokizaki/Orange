/*
|   @Brief  : �^�C�g����ʂ̃{�^���������\��
|
|   @File   : TitlePreBtn.cpp
|
|   @Auhtor : Ayumi Yasui
|
|   @Note   : Press 4 or Enter
|
*------------------------------------------------------------------------*/

/*--- �C���N���[�h ------------------------------------------------------*/

#include "TitlePreBtn.h"

/*--- �萔��` ----------------------------------------------------------*/

// �ʒu
#define TX      ( SCREEN_WIDTH * 0.5f )
#define TY      ( 500.0f )

// �傫��
#define WIDTH   ( 534 )
#define HEIGHT  ( 162 )

/*--- �ϐ��^��` --------------------------------------------------------*/

typedef struct tagTITLEPREBTN
{
    LPDIRECT3DVERTEXBUFFER9 vtx;        // ���_�o�b�t�@
    LPDIRECT3DTEXTURE9      tex;        // �e�N�X�`��
    float                   fAlpha;     // �_�ŗp�F�A���t�@�����l
    D3DXCOLOR               col;        // �F
}TITLEPREBTN;

/*--- �v���g�^�C�v�錾 --------------------------------------------------*/

/*--- �O���[�o���ϐ� ----------------------------------------------------*/

TITLEPREBTN g_TitlePreBtn;

/*************************************************************************
|
|   @Brief  : ������
|
|   @Param  : �Ȃ�
|
|   @Return : �Ȃ�
|
*------------------------------------------------------------------------*/
VOID InitTitlePreBtn( VOID )
{
    // �A���t�@�����l
    g_TitlePreBtn.fAlpha = 0.0f;

    // ���݂̃A���t�@�l
    g_TitlePreBtn.col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 0.0f );

    // �f�o�C�X�擾
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice ->CreateVertexBuffer((sizeof( VERTEX_2D ) * 4),	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@
								 D3DUSAGE_WRITEONLY,				// ���_�o�b�t�@�̎g�p���@
								 FVF_VERTEX_2D,						// �g�p���钸�_�t�H�[�}�b�g
								 D3DPOOL_MANAGED,					// �o�b�t�@��ێ����郁�����N���X�̎w��
								 &g_TitlePreBtn.vtx,					// ���_�o�b�t�@�ւ̃|�C���^
								 NULL);								// NULL�Œ�

	// ���_���̕ҏW
	//------------------------------------
    VERTEX_2D *pVtx;
	g_TitlePreBtn.vtx->Lock( 0, 0, (void**)&pVtx, 0 );
    {
        pVtx[ 0 ].pos.x = -WIDTH  * 0.5f + TX;
        pVtx[ 0 ].pos.y = -HEIGHT * 0.5f + TY;
        pVtx[ 0 ].pos.z = 0.0f;

        pVtx[ 1 ].pos.x =  WIDTH  * 0.5f + TX;
        pVtx[ 1 ].pos.y = -HEIGHT * 0.5f + TY;
        pVtx[ 1 ].pos.z = 0.0f;

        pVtx[ 2 ].pos.x = -WIDTH  * 0.5f + TX;
        pVtx[ 2 ].pos.y =  HEIGHT * 0.5f + TY;
        pVtx[ 2 ].pos.z = 0.0f;

        pVtx[ 3 ].pos.x =  WIDTH  * 0.5f + TX;
        pVtx[ 3 ].pos.y =  HEIGHT * 0.5f + TY;
        pVtx[ 3 ].pos.z = 0.0f;

        // �@��
        pVtx[ 0 ].rhw = 1.0f;
        pVtx[ 1 ].rhw = 1.0f;
        pVtx[ 2 ].rhw = 1.0f;
        pVtx[ 3 ].rhw = 1.0f;

        // �F
        pVtx[ 0 ].col
            =  pVtx[ 1 ].col
            = pVtx[ 2 ].col
            = pVtx[ 3 ].col
            = g_TitlePreBtn.col;

        // �e�N�X�`��
        pVtx[ 0 ].tex = D3DXVECTOR2( 0.0f, 0.0f );
        pVtx[ 1 ].tex = D3DXVECTOR2( 1.0f, 0.0f );
        pVtx[ 2 ].tex = D3DXVECTOR2( 0.0f, 1.0f );
        pVtx[ 3 ].tex = D3DXVECTOR2( 1.0f, 1.0f );
    }
    g_TitlePreBtn.vtx -> Unlock();

    // �e�N�X�`���̓ǂݍ���
    D3DXCreateTextureFromFile( pDevice, "data/TEXTURE/Title/PressButton.png", &g_TitlePreBtn.tex );
}

/*************************************************************************
|
|   @Brief  : �I��
|
|   @Param  : �Ȃ�
|
|   @Return : �Ȃ�
|
*------------------------------------------------------------------------*/
VOID UninitTitlePreBtn( VOID )
{
    // �e�N�X�`���o�b�t�@�̉��
    if( g_TitlePreBtn.tex != NULL )
    {
        g_TitlePreBtn.tex->Release();
        g_TitlePreBtn.tex = NULL;
    }

    // ���_�o�b�t�@�̉��
    if( g_TitlePreBtn.vtx != NULL )
    {
        g_TitlePreBtn.vtx->Release();
        g_TitlePreBtn.vtx = NULL;
    }
}

/*************************************************************************
|
|   @Brief  : �X�V
|
|   @Param  : �Ȃ�
|
|   @Return : �Ȃ�
|
*------------------------------------------------------------------------*/
VOID UpdateTitlePreBtn( VOID )
{
    if( GetFade() == FADE_OUT )
    {
        g_TitlePreBtn.fAlpha += ( 1.0f - g_TitlePreBtn.col.a ) * 0.15f;
    }
    else
    {
        g_TitlePreBtn.fAlpha += ( 1.0f - g_TitlePreBtn.col.a ) * 0.01f;
    }

    g_TitlePreBtn.col.a += g_TitlePreBtn.fAlpha;

    VERTEX_2D *pVtx;
    g_TitlePreBtn.vtx->Lock( 0, 0, (void**)&pVtx, 0 );
    {
        pVtx[ 0 ].col
            = pVtx[ 1 ].col
            = pVtx[ 2 ].col
            = pVtx[ 3 ].col
            = g_TitlePreBtn.col;
    }
    g_TitlePreBtn.vtx -> Unlock();
}

/*************************************************************************
|
|   @Brief  : �`��
|
|   @Param  : �Ȃ�
|
|   @Return : �Ȃ�
|
*------------------------------------------------------------------------*/
VOID DrawTitlePreBtn( VOID )
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();
    pDevice->SetStreamSource( 0, g_TitlePreBtn.vtx, 0, sizeof( VERTEX_2D ) );
    pDevice->SetFVF( FVF_VERTEX_2D );
    pDevice->SetTexture( 0, g_TitlePreBtn.tex );
    pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );
}

/*************************************************************************
|   End of File
**************************************************************************/
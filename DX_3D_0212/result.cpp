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

//-----------------------------------------------
//�@�}�N����`
//-----------------------------------------------

enum tagRESULT_TEX
{
    RESULT_TEX_BG = 0,
    RESULT_TEX_PLAYER,

    RESULT_TEX_MAX
};

//-----------------------------------------------
//�@�\���̒�`
//-----------------------------------------------

typedef struct tagRESULT
{
    LPDIRECT3DVERTEXBUFFER9 vtx;                    // ���_�o�b�t�@
    LPDIRECT3DTEXTURE9      tex[RESULT_TEX_MAX];    // �e�N�X�`���o�b�t�@
    LPD3DXMESH              pMeshPlayer;            // ���b�V�����ւ̃|�C���^
    LPD3DXBUFFER            pBuffMatPlayer;         // �}�e���A�����ւ̃|�C���^
    DWORD                   nMatPlayer;             // �}�e���A���̐�
}RESULT;

//-----------------------------------------------
//  �O���[�o���ϐ�
//-----------------------------------------------
RESULT g_Result;
int    g_nFrameCount = 0;
bool   g_bResultFade = false;

//-----------------------------------------------
// �w�i����������
//-----------------------------------------------
void InitResult()
{
    // BGM
    PlaySound(RESULT_BGM);

	g_nFrameCount = 0;
	g_bResultFade = false;

	LPDIRECT3DDEVICE9 pDevice;		// �f�o�C�X�I�u�W�F�N�g

	// �f�o�C�X�擾
	//------------------------------------
	pDevice = GetDevice();

	pDevice ->CreateVertexBuffer(( sizeof( VERTEX_2D ) * 4 ),	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@
								 D3DUSAGE_WRITEONLY,				// ���_�o�b�t�@�̎g�p���@
								 FVF_VERTEX_2D,						// �g�p���钸�_�t�H�[�}�b�g
								 D3DPOOL_MANAGED,					// �o�b�t�@��ێ����郁�����N���X�̎w��
								 &g_Result.vtx,					// ���_�o�b�t�@�ւ̃|�C���^
								 NULL);								// NULL�Œ�
		// �f�[�^�̈ʒu���m�ۂ���i���b�N�j
	//------------------------------------
	VERTEX_2D *pVtx;
	g_Result.vtx ->Lock( 0, 0, (void**)&pVtx, 0 );

    pVtx[ 0 ].pos.x = 0.0f;
	pVtx[ 0 ].pos.y = 0.0f;	// ���_�̈ʒu
	pVtx[ 0 ].pos.z = 1.0f;												// ���_�̈ʒu

	pVtx[ 1 ].pos.x = SCREEN_WIDTH;
	pVtx[ 1 ].pos.y = 0.0f;	// ���_�̈ʒu
	pVtx[ 1 ].pos.z = 1.0f;												// ���_�̈ʒu

	pVtx[ 2 ].pos.x = 0.0f;
	pVtx[ 2 ].pos.y = SCREEN_HEIGHT;	// ���_�̈ʒu
	pVtx[ 2 ].pos.z = 1.0f;												// ���_�̈ʒu

	pVtx[ 3 ].pos.x = SCREEN_WIDTH;
	pVtx[ 3 ].pos.y = SCREEN_HEIGHT;	// ���_�̈ʒu
	pVtx[ 3 ].pos.z = 1.0f;												// ���_�̈ʒu

	// �@��
	pVtx[ 0 ].rhw = 1.0f;
	pVtx[ 1 ].rhw = 1.0f;
	pVtx[ 2 ].rhw = 1.0f;
	pVtx[ 3 ].rhw = 1.0f;			

	// �t�F�[�h�̐F
	//------------------------------------
	pVtx[ 0 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );			// ���_���Ƃ̐F�̐ݒ�
	pVtx[ 1 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );			//			��
	pVtx[ 2 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );			//			��
	pVtx[ 3 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );			//			��

	pVtx[ 0 ].tex = D3DXVECTOR2( 0.0f, 0.0f );
	pVtx[ 1 ].tex = D3DXVECTOR2( 1.0f, 0.0f );
	pVtx[ 2 ].tex = D3DXVECTOR2( 0.0f, 1.0f );
	pVtx[ 3 ].tex = D3DXVECTOR2( 1.0f, 1.0f );


	// ���
	//------------------------------------
	g_Result.vtx -> Unlock();

	// �e�N�X�`���̓ǂݍ���
	//------------------------------------
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/result.jpg", &g_Result.tex[RESULT_TEX_BG]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Player.jpg", &g_Result.tex[RESULT_TEX_PLAYER]);

	// x�t�@�C���̓ǂݍ���
	//------------------------------------
	D3DXLoadMeshFromX("data/MODEL/PlayerU.x",		// �ǂݍ��ރt�@�C����
						D3DXMESH_SYSTEMMEM,							// 
						pDevice,										// 
						NULL,											// 
						&g_Result.pBuffMatPlayer,			// 
						NULL,											// 
						&g_Result.nMatPlayer,			// 
						&g_Result.pMeshPlayer );
}

//-----------------------------------------------
// �w�i�I������
//-----------------------------------------------
void UninitResult()
{
    // �v���C���[�f�[�^�I��
    // ���b�V�����̉���Ə�����
	//------------------------------------
	if ( g_Result.pMeshPlayer != NULL)
	{ 
		g_Result.pMeshPlayer ->Release();
		g_Result.pMeshPlayer = NULL;
	}
	
	// �}�e���A�����̉���Ə�����
	//------------------------------------
	if ( g_Result.pBuffMatPlayer != NULL)
	{ 
		g_Result.pBuffMatPlayer ->Release();
		g_Result.pBuffMatPlayer = NULL;
	}


    // �w�i�I��
	for ( int nCnt = 0; nCnt < RESULT_TEX_MAX; nCnt++ )
	{
		if (g_Result.tex[ nCnt ] != NULL)
		{ // �g�����e�N�X�`���̉���Ə�����
		  //------------------------------------
			g_Result.tex[ nCnt ] -> Release();
			g_Result.tex[ nCnt ] = NULL;
		}
	}

	if (g_Result.vtx != NULL)
	{ // ���_�o�b�t�@�̉���Ə�����
	  //------------------------------------
		g_Result.vtx->Release();
		g_Result.vtx = NULL;
	}

    // BGM
	StopSound();
}

//-----------------------------------------------
// ���U���g�X�V����
//-----------------------------------------------
void UpdateResult()
{
    g_nFrameCount++;

	// �J�ڔ���
	if ( ( GetKeyboardTrigger(DIK_RETURN)  || GetPadElecomTrigger( PAD_4 ) || GetPadElecomTrigger( PAD_10 ) || g_nFrameCount > 60 * 8 ) && GetFade() == FADE_NONE )
	{
		PlaySound( DESIDE4_SE );
		SetFade( FADE_OUT, 60 );
		SetMode( MODE_RANKING );

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
								g_Result.vtx,
								0,
								sizeof(VERTEX_2D));	// ���_�f�[�^�̊Ԋu

	// ���_�t�H�[�}�b�g�̐ݒ�
	//------------------------------------
	pDevice -> SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	//------------------------------------
	pDevice -> SetTexture( 0, g_Result.tex[RESULT_TEX_BG] );

	// �|���S���̕`��
	//------------------------------------
	pDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,	// �v���~�e�B�u�̎��
									0,				// �ǂݍ��ލŏ��̔ԍ�
									2);				// �`�悷��v���~�e�B�u��

	// ���[���h�s��ݒ�
    D3DXMATRIX mtxWorld;
    D3DXMatrixIdentity( &mtxWorld );
	pDevice ->SetTransform(D3DTS_WORLD, &mtxWorld );

	// ���݂̃}�e���A������ۑ�
    D3DMATERIAL9 matDef;
	pDevice ->GetMaterial( &matDef );

	// �o�b�t�@�ւ̃|�C���^���擾
	D3DXMATERIAL *pMat = (D3DXMATERIAL*)g_Result.pBuffMatPlayer ->GetBufferPointer();

	// �}�e���A���̐��������[�v
	for (int nCntMat = 0; nCntMat < (int)g_Result.nMatPlayer; nCntMat++)
	{
		pDevice ->SetMaterial( &pMat[ nCntMat ].MatD3D );			// �}�e���A���̐ݒ�
        if( pMat[nCntMat].pTextureFilename == NULL )
        {
		    pDevice ->SetTexture( 0, NULL );							// �e�N�X�`���̃Z�b�g
        }
        else
        {
		    pDevice ->SetTexture( 0, g_Result.tex[RESULT_TEX_PLAYER] );							// �e�N�X�`���̃Z�b�g
        }
		g_Result.pMeshPlayer ->DrawSubset( nCntMat );		// �`��
	}
			
	// �}�e���A�������ɖ߂�
	//------------------------------------
	pDevice ->SetMaterial( &matDef );
}

//-----------------------------------------------
// ���_���Z�b�g
//-----------------------------------------------
void SetupResult()
{
}
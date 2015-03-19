//-----------------------------------------------
//  ���e�F�����L���O����
//  File : ranking.cpp
//  ����ҁF�؍�@��
//-----------------------------------------------

//-----------------------------------------------
//  �C���N���[�h�t�@�C��
//-----------------------------------------------
#include "ranking.h"
#include "score.h"

//------ �}�N����` ------
#define RANK_TEX_MAX	( 5 )

#define RANKING_PASS	"ranking.bin"
#define RANK_TEX_PASS	"data/TEXTURE/ranking.png"
#define RANK_TEX_1ST	"data/TEXTURE/1st.png"
#define RANK_TEX_2ND	"data/TEXTURE/2nd.png"
#define RANK_TEX_3RD	"data/TEXTURE/3rd.png"
#define RANKBACK_TEX	"data/TEXTURE/ringleDash_backTitle.jpg"
#define RANKPOS_X		( SCREEN_WIDTH / 2 - 125.0f )
#define RANKPOS_Y		( 190.0f )
#define RANK_WIDTH		( 30.0f )
#define RANK_HEIGHT		( 40.0f )
#define RANK_DIG		( 6 )

#define RANKPOLI_X		( SCREEN_WIDTH / 2 )
#define RANKPOLI_Y		( 70.0f )
#define RANKPOLI_WIDTH	( 200.0f )
#define RANKPOLI_HEIGHT	( 60.0f )

#define RANK1ST_POS_X	( 130.0f )
#define RANK1ST_POS_Y	( 190.0f )
#define RANK1ST_WIDTH	( 60.0f )
#define RANK1ST_HEIGHT	( 50.0f )

#define RANK2ND_POS_X	( 130.0f )
#define RANK2ND_POS_Y	( 350.0f )
#define RANK2ND_WIDTH	( 60.0f )
#define RANK2ND_HEIGHT	( 50.0f )

#define RANK3RD_POS_X	( 130.0f )
#define RANK3RD_POS_Y	( 510.0f )
#define RANK3RD_WIDTH	( 60.0f )
#define RANK3RD_HEIGHT	( 50.0f )

//------ �O���[�o���ϐ� ------
RANKING ranking;
NUMBER rankingNumber[ RANK_MAX ];
bool g_bJudge = false;
int g_nIndex = 0;
int g_nRankingFrameCount = 0;

LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRanking = NULL;			// ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9		g_pTextureRanking[ RANK_TEX_MAX ];			// �e�N�X�`���ւ̃|�C���^

// �����L���O������
void InitRanking( void )
{
	// ���[�J���ϐ�
	//---------------------------------
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// �f�o�C�X�I�u�W�F�N�g
	VERTEX_2D *pVtx2D;

	// �O���[�o���ϐ�������
	g_bJudge = false;
	g_nIndex = 0;
	g_nRankingFrameCount = 0;

	// ���[�h
	LoadRanking();

	//ranking.nScore[ 0 ] = 1000;
	//ranking.nScore[ 1 ] = 200;
	//ranking.nScore[ 2 ] = 50;

	//// ���[�J���ϐ�
	////------------------------------------
	//FILE *fp;		// �t�@�C���|�C���^

	//// �t�@�C���̃I�[�v��
	//fp = fopen("data/SCRIPT/ranking.bin", "wb");
	//if (fp == NULL)
	//{
	//	MessageBox( NULL, "�����L���O�e�L�X�g�̃I�[�v���Ɏ��s", "�G���[", MB_OK );
	//	return;
	//}

	//// �Z�[�u����
	//fwrite( &ranking, sizeof( RANKING ), RANK_MAX, fp );

	//// �t�@�C���̃N���[�Y
	//fclose( fp );

	//rankingNumber[ 0 ].nNumber = ranking.nScore[ 0 ];
	//rankingNumber[ 1 ].nNumber = ranking.nScore[ 1 ];
	//rankingNumber[ 2 ].nNumber = ranking.nScore[ 2 ];

	// ���_�o�b�t�@�̊m��
	//------------------------------------
	pDevice ->CreateVertexBuffer(( sizeof(VERTEX_2D) * 4 * RANK_TEX_MAX ),				// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@
								 D3DUSAGE_WRITEONLY,					// ���_�o�b�t�@�̎g�p���@
								 FVF_VERTEX_2D,							// �g�p���钸�_�t�H�[�}�b�g
								 D3DPOOL_MANAGED,						// �o�b�t�@��ێ����郁�����N���X�̎w��
								 &g_pVtxBuffRanking,					// ���_�o�b�t�@�ւ̃|�C���^
								 NULL);									// NULL�Œ�	

	// ���b�N
	g_pVtxBuffRanking ->Lock( 0, 0, ( void** )&pVtx2D, 0 );

	// [ �����L���O ]
	pVtx2D[ 0 ].pos = D3DXVECTOR3( RANKPOLI_X - RANKPOLI_WIDTH, RANKPOLI_Y - RANKPOLI_HEIGHT, 0.0f );
	pVtx2D[ 1 ].pos = D3DXVECTOR3( RANKPOLI_X + RANKPOLI_WIDTH, RANKPOLI_Y - RANKPOLI_HEIGHT, 0.0f );
	pVtx2D[ 2 ].pos = D3DXVECTOR3( RANKPOLI_X - RANKPOLI_WIDTH, RANKPOLI_Y + RANKPOLI_HEIGHT, 0.0f );
	pVtx2D[ 3 ].pos = D3DXVECTOR3( RANKPOLI_X + RANKPOLI_WIDTH, RANKPOLI_Y + RANKPOLI_HEIGHT, 0.0f );

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

	// [ �w�i ]
	pVtx2D[ 4 ].pos = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	pVtx2D[ 5 ].pos = D3DXVECTOR3( SCREEN_WIDTH, 0.0f, 0.0f );
	pVtx2D[ 6 ].pos = D3DXVECTOR3( 0.0f, SCREEN_HEIGHT, 0.0f );
	pVtx2D[ 7 ].pos = D3DXVECTOR3( SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f );

	pVtx2D[ 4 ].rhw = 1.0f;
	pVtx2D[ 5 ].rhw = 1.0f;
	pVtx2D[ 6 ].rhw = 1.0f;
	pVtx2D[ 7 ].rhw = 1.0f;

	pVtx2D[ 4 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );
	pVtx2D[ 5 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );
	pVtx2D[ 6 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );
	pVtx2D[ 7 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );

	pVtx2D[ 4 ].tex = D3DXVECTOR2( 0.0f, 0.0f );
	pVtx2D[ 5 ].tex = D3DXVECTOR2( 1.0f, 0.0f );
	pVtx2D[ 6 ].tex = D3DXVECTOR2( 0.0f, 1.0f );
	pVtx2D[ 7 ].tex = D3DXVECTOR2( 1.0f, 1.0f );

	// [ 1st ]
	pVtx2D[ 8 ].pos = D3DXVECTOR3( RANK1ST_POS_X - RANK1ST_WIDTH, RANK1ST_POS_Y - RANK1ST_HEIGHT, 0.0f );
	pVtx2D[ 9 ].pos = D3DXVECTOR3( RANK1ST_POS_X + RANK1ST_WIDTH, RANK1ST_POS_Y - RANK1ST_HEIGHT, 0.0f );
	pVtx2D[ 10 ].pos = D3DXVECTOR3( RANK1ST_POS_X - RANK1ST_WIDTH, RANK1ST_POS_Y + RANK1ST_HEIGHT, 0.0f );
	pVtx2D[ 11 ].pos = D3DXVECTOR3( RANK1ST_POS_X + RANK1ST_WIDTH, RANK1ST_POS_Y + RANK1ST_HEIGHT, 0.0f );

	pVtx2D[ 8 ].rhw = 1.0f;
	pVtx2D[ 9 ].rhw = 1.0f;
	pVtx2D[ 10 ].rhw = 1.0f;
	pVtx2D[ 11 ].rhw = 1.0f;

	pVtx2D[ 8 ].col = D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f );
	pVtx2D[ 9 ].col = D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f );
	pVtx2D[ 10 ].col = D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f );
	pVtx2D[ 11 ].col = D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f );

	pVtx2D[ 8 ].tex = D3DXVECTOR2( 0.0f, 0.0f );
	pVtx2D[ 9 ].tex = D3DXVECTOR2( 1.0f, 0.0f );
	pVtx2D[ 10 ].tex = D3DXVECTOR2( 0.0f, 1.0f );
	pVtx2D[ 11 ].tex = D3DXVECTOR2( 1.0f, 1.0f );

	// [ 2nd ]
	pVtx2D[ 12 ].pos = D3DXVECTOR3( RANK2ND_POS_X - RANK2ND_WIDTH, RANK2ND_POS_Y - RANK2ND_HEIGHT, 0.0f );
	pVtx2D[ 13 ].pos = D3DXVECTOR3( RANK2ND_POS_X + RANK2ND_WIDTH, RANK2ND_POS_Y - RANK2ND_HEIGHT, 0.0f );
	pVtx2D[ 14 ].pos = D3DXVECTOR3( RANK2ND_POS_X - RANK2ND_WIDTH, RANK2ND_POS_Y + RANK2ND_HEIGHT, 0.0f );
	pVtx2D[ 15 ].pos = D3DXVECTOR3( RANK2ND_POS_X + RANK2ND_WIDTH, RANK2ND_POS_Y + RANK2ND_HEIGHT, 0.0f );

	pVtx2D[ 12 ].rhw = 1.0f;
	pVtx2D[ 13 ].rhw = 1.0f;
	pVtx2D[ 14 ].rhw = 1.0f;
	pVtx2D[ 15 ].rhw = 1.0f;

	pVtx2D[ 12 ].col = D3DXCOLOR( 0.8f, 0.8f, 0.8f, 1.0f );
	pVtx2D[ 13 ].col = D3DXCOLOR( 0.8f, 0.8f, 0.8f, 1.0f );
	pVtx2D[ 14 ].col = D3DXCOLOR( 0.8f, 0.8f, 0.8f, 1.0f );
	pVtx2D[ 15 ].col = D3DXCOLOR( 0.8f, 0.8f, 0.8f, 1.0f );

	pVtx2D[ 12 ].tex = D3DXVECTOR2( 0.0f, 0.0f );
	pVtx2D[ 13 ].tex = D3DXVECTOR2( 1.0f, 0.0f );
	pVtx2D[ 14 ].tex = D3DXVECTOR2( 0.0f, 1.0f );
	pVtx2D[ 15 ].tex = D3DXVECTOR2( 1.0f, 1.0f );

	// [ 3rd ]
	pVtx2D[ 16 ].pos = D3DXVECTOR3( RANK3RD_POS_X - RANK3RD_WIDTH, RANK3RD_POS_Y - RANK3RD_HEIGHT, 0.0f );
	pVtx2D[ 17 ].pos = D3DXVECTOR3( RANK3RD_POS_X + RANK3RD_WIDTH, RANK3RD_POS_Y - RANK3RD_HEIGHT, 0.0f );
	pVtx2D[ 18 ].pos = D3DXVECTOR3( RANK3RD_POS_X - RANK3RD_WIDTH, RANK3RD_POS_Y + RANK3RD_HEIGHT, 0.0f );
	pVtx2D[ 19 ].pos = D3DXVECTOR3( RANK3RD_POS_X + RANK3RD_WIDTH, RANK3RD_POS_Y + RANK3RD_HEIGHT, 0.0f );

	pVtx2D[ 16 ].rhw = 1.0f;
	pVtx2D[ 17 ].rhw = 1.0f;
	pVtx2D[ 18 ].rhw = 1.0f;
	pVtx2D[ 19 ].rhw = 1.0f;

	pVtx2D[ 16 ].col = D3DXCOLOR( 1.0f, 0.7f, 0.0f, 1.0f );
	pVtx2D[ 17 ].col = D3DXCOLOR( 1.0f, 0.7f, 0.0f, 1.0f );
	pVtx2D[ 18 ].col = D3DXCOLOR( 1.0f, 0.7f, 0.0f, 1.0f );
	pVtx2D[ 19 ].col = D3DXCOLOR( 1.0f, 0.7f, 0.0f, 1.0f );

	pVtx2D[ 16 ].tex = D3DXVECTOR2( 0.0f, 0.0f );
	pVtx2D[ 17 ].tex = D3DXVECTOR2( 1.0f, 0.0f );
	pVtx2D[ 18 ].tex = D3DXVECTOR2( 0.0f, 1.0f );
	pVtx2D[ 19 ].tex = D3DXVECTOR2( 1.0f, 1.0f );

	// �A�����b�N
	g_pVtxBuffRanking ->Unlock();

	// �e�N�X�`���|�C���^�̏�����
	for ( int i = 0; i < RANK_TEX_MAX; i++ )
	{
		g_pTextureRanking[ i ] = NULL;
	}

	// �e�N�X�`���̓ǂݍ���
	//------------------------------------
	D3DXCreateTextureFromFile( pDevice, RANK_TEX_PASS, &g_pTextureRanking[ 0 ] );
	D3DXCreateTextureFromFile( pDevice, RANKBACK_TEX, &g_pTextureRanking[ 1 ] );
	D3DXCreateTextureFromFile( pDevice, RANK_TEX_1ST, &g_pTextureRanking[ 2 ] );
	D3DXCreateTextureFromFile( pDevice, RANK_TEX_2ND, &g_pTextureRanking[ 3 ] );
	D3DXCreateTextureFromFile( pDevice, RANK_TEX_3RD, &g_pTextureRanking[ 4 ] );

	// �`��̏���
	for ( int i = 0; i < RANK_MAX; i++ )
	{
		SetNumber( &rankingNumber[ i ], D3DXVECTOR3( RANKPOS_X, RANKPOS_Y + ( i * RANK_HEIGHT * 4 ), 0.0f ), RANK_DIG, RANK_WIDTH, RANK_HEIGHT );
	}

	// ���y
	PlaySound( RESULT_BGM );
}

// �����L���O�I������
void UninitRanking( void )
{
	// ���[�J���ϐ�
	//------------------------------------
	FILE *fp;		// �t�@�C���|�C���^

	// �t�@�C���̃I�[�v��
	fp = fopen("data/SCRIPT/ranking.bin", "wb");
	if (fp == NULL)
	{
		//MessageBox( NULL, "�����L���O�e�L�X�g�̃I�[�v���Ɏ��s", "�G���[", MB_OK );
		return;
	}

	// �Z�[�u����
	fwrite( &ranking, sizeof( RANKING ), RANK_MAX, fp );

	// �t�@�C���̃N���[�Y
	fclose( fp );

	// �����`��Ɏg�����o�b�t�@�����
	for ( int i = 0; i < RANK_MAX; i++	 )
	{
		UninitNumber( &rankingNumber[ i ] );
	}

	for ( int i = 0; i < RANK_TEX_MAX; i++ )
	{
		// �g�����e�N�X�`���̉���Ə�����
		//------------------------------------
		if (g_pTextureRanking[ i ] != NULL)
		{ 
			g_pTextureRanking[ i ] ->Release();
			g_pTextureRanking[ i ] = NULL;
		}
	}


	// ���_�o�b�t�@�̉���Ə�����
	//------------------------------------
	if (g_pVtxBuffRanking != NULL)
	{ 
		g_pVtxBuffRanking ->Release();
		g_pVtxBuffRanking = NULL;
	}
}

// �����L���O�X�V����
void UpdateRanking( void )
{
	if ( ( GetKeyboardTrigger(DIK_RETURN) || GetPadElecomTrigger( PAD_4 ) || GetPadElecomTrigger( PAD_10 ) ) && GetFade() == FADE_NONE)
	{
		PlaySound( DESIDE4_SE );
		SetFade( FADE_OUT );
		SetMode( MODE_RESULT );
	}
}

// �����L���O�`�揈��
void DrawRanking( void )
{
	// ���[�J���ϐ�
	//------------------------------------
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�I�u�W�F�N�g
	
	// ���_�o�b�t�@���f�[�^�X�g���[���Ƀo�C���h
	//-----------------------------------------
	pDevice ->SetStreamSource(0,
								g_pVtxBuffRanking,
								0,
								sizeof(VERTEX_2D));	// ���_�f�[�^�̊Ԋu

	
	// ���_�t�H�[�}�b�g�̐ݒ�
	//------------------------------------
	pDevice ->SetFVF( FVF_VERTEX_2D );

	// �e�N�X�`���̐ݒ�[ �w�i ]
	//------------------------------------
	pDevice -> SetTexture(0, g_pTextureRanking[ 1 ]);
			
	// �|���S���̕`��[ �����L���O ]
	//------------------------------------
	pDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,	// �v���~�e�B�u�̎��
									4,				// �ǂݍ��ލŏ��̔ԍ�
									2);				// �`�悷��v���~�e�B�u��

	// �e�N�X�`���̐ݒ�
	//------------------------------------
	pDevice -> SetTexture(0, g_pTextureRanking[ 0 ]);
			
	// �|���S���̕`��[ �����L���O ]
	//------------------------------------
	pDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,	// �v���~�e�B�u�̎��
									0,				// �ǂݍ��ލŏ��̔ԍ�
									2);				// �`�悷��v���~�e�B�u��

	// �e�N�X�`���̐ݒ�[ 1st ]
	//------------------------------------
	pDevice -> SetTexture(0, g_pTextureRanking[ 2 ]);
			
	// �|���S���̕`��
	//------------------------------------
	pDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,	// �v���~�e�B�u�̎��
									8,				// �ǂݍ��ލŏ��̔ԍ�
									2);				// �`�悷��v���~�e�B�u��

	// �e�N�X�`���̐ݒ�[ 2nd ]
	//------------------------------------
	pDevice -> SetTexture(0, g_pTextureRanking[ 3 ]);
			
	// �|���S���̕`��
	//------------------------------------
	pDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,	// �v���~�e�B�u�̎��
									12,				// �ǂݍ��ލŏ��̔ԍ�
									2);				// �`�悷��v���~�e�B�u��

	// �e�N�X�`���̐ݒ�[ 3rd ]
	//------------------------------------
	pDevice -> SetTexture(0, g_pTextureRanking[ 4 ]);
			
	// �|���S���̕`��
	//------------------------------------
	pDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,	// �v���~�e�B�u�̎��
									16,				// �ǂݍ��ލŏ��̔ԍ�
									2);				// �`�悷��v���~�e�B�u��

	// �����̕`��
	if ( g_bJudge == true )
	{
		for ( int i = 0; i < RANK_MAX; i++ )
		{
			if ( i == g_nIndex )
			{
				if ( ( ++g_nRankingFrameCount % 60 ) < 40 )
				{
					DrawNumber( &rankingNumber[ i ], ranking.nScore[ i ] );
				}
			}
			else
			{
				DrawNumber( &rankingNumber[ i ], ranking.nScore[ i ] );
			}
		}
	}
	else
	{
		for ( int i = 0; i < RANK_MAX; i++ )
		{
			DrawNumber( &rankingNumber[ i ], ranking.nScore[ i ] );
		}
	}
}

// �����L���O���[�h����
void LoadRanking( void )
{
	// ���[�J���ϐ�
	//------------------------------------
	FILE *fp;		// �t�@�C���|�C���^

	// �t�@�C���̃I�[�v��
	fp = fopen("data/SCRIPT/ranking.bin", "rb");
	if (fp == NULL)
	{
		//MessageBox( NULL, "�����L���O�e�L�X�g�̃I�[�v���Ɏ��s", "�G���[", MB_OK );
		return;
	}

	// �t�@�C���̓ǂݍ���
	int size = fread( &ranking, sizeof( RANKING ), RANK_MAX, fp );

	// �t�@�C���̃N���[�Y
	fclose( fp );

	// �v���C�������̓��_��z��̍Ō�Ɋi�[
	ranking.nScore[ RANK_MAX ] = GetScore();

	// �\�[�g
	for ( int i = 0; i < RANK_MAX + 1; i++ )
	{
		int n1 = ranking.nScore[ i ];
		int n2 = ranking.nScore[ RANK_MAX ];

		if ( n1 < n2 )
		{
			if ( i == 0 )
			{
				int work = ranking.nScore[ 0 ];
				int work1 = ranking.nScore[ 1 ];
				int work2 = ranking.nScore[ 2 ];
				int work3 = ranking.nScore[ RANK_MAX ];

				ranking.nScore[ 0 ] = work3;
				ranking.nScore[ 1 ] = work;
				ranking.nScore[ 2 ] = work1;
				
				g_bJudge = true;
				
				break;
			}
			else if ( i == 1 )
			{
				int work1 = ranking.nScore[ 1 ];
				int work2 = ranking.nScore[ 2 ];
				int work3 = ranking.nScore[ RANK_MAX ];

				ranking.nScore[ 1 ] = work3;
				ranking.nScore[ 2 ] = work1;

				g_bJudge = true;

				break;
			}
			else if ( i == 2 )
			{
				ranking.nScore[ 2 ] = ranking.nScore[ RANK_MAX ];
				g_bJudge = true;

				break;
			}
		}
	}

	if ( g_bJudge == true )
	{
		for ( int i = 0; i < RANK_MAX; i++ )
		{
			if ( ranking.nScore[ i ] == GetScore() )
			{
				g_nIndex = i;

				break;
			}
		}
	}
}
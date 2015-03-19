//-----------------------------------------------
//  ���e�F�~�j�}�b�v����
//  File : miniMap.cpp
//  ����ҁF�؍�@��
//-----------------------------------------------

//------ �C���N���[�h�t�@�C�� ------
#include "miniMap.h"

//------ �}�N����` ------
#define MINIMAP_TEX_MAX ( 5 )
#define TEX_PLAYER		"data/TEXTURE/miniMapIconPlayer.png"
#define TEX_ENEMY		"data/TEXTURE/miniMapIconEnemy.png"
#define TEX_BACK_A		"data/TEXTURE/rader001.png"
#define TEX_BACK_B		"data/TEXTURE/rader002.png"
#define TEX_OBJECT		"data/TEXTURE/miniMapIconObject.png"

//#define MINIMAP_POS_X	( SCREEN_WIDTH - 80.0f )
#define MINIMAP_POS_X	( SCREEN_WIDTH / 2 )
//#define MINIMAP_POS_Y	( 80.0f )
#define MINIMAP_POS_Y	( SCREEN_HEIGHT - 80.0f )
#define MINIMAP_WIDTH	( 80.0f )
#define MINIMAP_HEIGHT	( 80.0f )

#define MINIMAP_ICON_WIDTH	( 10.0f )
#define MINIMAP_ICON_HEIGHT ( 10.0f )

//------ �O���[�o���ϐ� ------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMiniMap = NULL;				// ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9		g_pTextureMiniMap[ MINIMAP_TEX_MAX ];	// �e�N�X�`���ւ̃|�C���^
MINIMAP g_aMiniMap[ MINIMAP_MAX ];

//------ �����֐��錾 --------------------------

//-----------------------------------------------
// ���_���Z�b�g
//-----------------------------------------------
void SetupVertexMiniMap( void );

//-----------------------------------------------

//-----------------------------------------------
// �~�j�}�b�v������
//-----------------------------------------------
void InitMiniMap( void )
{
	// ���[�J���ϐ�
	//---------------------------------
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�I�u�W�F�N�g

	for ( int i = 0; i < MINIMAP_TEX_MAX; i++ )
	{
		g_pTextureMiniMap[ i ] = NULL;
	}

	// ���_�o�b�t�@�̊m��
	//------------------------------------
	pDevice ->CreateVertexBuffer((sizeof(VERTEX_2D) * 4 * MINIMAP_MAX),	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@
								 D3DUSAGE_WRITEONLY,					// ���_�o�b�t�@�̎g�p���@
								 FVF_VERTEX_2D,							// �g�p���钸�_�t�H�[�}�b�g
								 D3DPOOL_MANAGED,						// �o�b�t�@��ێ����郁�����N���X�̎w��
								 &g_pVtxBuffMiniMap,					// ���_�o�b�t�@�ւ̃|�C���^
								 NULL);									// NULL�Œ�	
	
	// �~�j�}�b�v�Z�b�g
	SetMiniMap( D3DXVECTOR3( MINIMAP_POS_X, MINIMAP_POS_Y, 0.0f ), MINIMAPTYPE_BACK, 0.0f );

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TEX_PLAYER, &g_pTextureMiniMap[ 0 ]);
	D3DXCreateTextureFromFile(pDevice, TEX_ENEMY, &g_pTextureMiniMap[ 1 ]);
	D3DXCreateTextureFromFile(pDevice, TEX_BACK_B, &g_pTextureMiniMap[ 2 ]);
	D3DXCreateTextureFromFile(pDevice, TEX_OBJECT, &g_pTextureMiniMap[ 3 ]);
	D3DXCreateTextureFromFile(pDevice, TEX_BACK_A, &g_pTextureMiniMap[ 4 ]);
}

//-----------------------------------------------
// �~�j�}�b�v�I������
//-----------------------------------------------
void UninitMiniMap( void )
{
	// �g�����e�N�X�`���̉���Ə�����
	//------------------------------------
	for (int nCnt = 0; nCnt < MINIMAP_TEX_MAX; nCnt++)
	{
		if ( g_pTextureMiniMap[ nCnt ] != NULL )
		{ 
			g_pTextureMiniMap[ nCnt ] ->Release();
			g_pTextureMiniMap[ nCnt ] = NULL;
		}
	}

	// ���_�o�b�t�@�̉���Ə�����
	//------------------------------------
	if (g_pVtxBuffMiniMap != NULL)
	{ 
		g_pVtxBuffMiniMap ->Release();
		g_pVtxBuffMiniMap = NULL;
	}
}

//-----------------------------------------------
// �~�j�}�b�v�X�V����
//-----------------------------------------------
void UpdateMiniMap( void )
{
	for ( int i = 1; i < MINIMAP_MAX; i++ )
	{
		if ( g_aMiniMap[ i ].bUse == true )
		{
			if ( i != MINIMAP_MAX - 1 )
			{
				g_aMiniMap[ i ].bUse = false;
			}
		}
	}

	// ���_���Z�b�g
	SetupVertexMiniMap();
}

//-----------------------------------------------
// �~�j�}�b�v�`�揈��
//-----------------------------------------------
void DrawMiniMap( void )
{
	// ���[�J���ϐ�
	//------------------------------------
	LPDIRECT3DDEVICE9 pDevice = GetDevice();						// �f�o�C�X�I�u�W�F�N�g
	
	// ���_�o�b�t�@���f�[�^�X�g���[���Ƀo�C���h
	//-----------------------------------------
	pDevice ->SetStreamSource(0,
								g_pVtxBuffMiniMap,
								0,
								sizeof(VERTEX_2D));	// ���_�f�[�^�̊Ԋu

	
	// ���_�t�H�[�}�b�g�̐ݒ�
	//------------------------------------
	pDevice ->SetFVF( FVF_VERTEX_2D );

	for ( int i = 0; i < MINIMAP_MAX; i++ )
	{
		if ( g_aMiniMap[ i ].bUse == true )
		{
			if ( i == MINIMAP_MAX - 1 )
			{
				// �e�N�X�`���̐ݒ�
				//------------------------------------
				pDevice -> SetTexture( 0, g_pTextureMiniMap[ 4 ] );
			
				// �|���S���̕`��
				//------------------------------------
				pDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,	// �v���~�e�B�u�̎��
													i * 4,			// �ǂݍ��ލŏ��̔ԍ�
													2);				// �`�悷��v���~�e�B�u��
			}
			else
			{
				// �e�N�X�`���̐ݒ�
				//------------------------------------
				pDevice -> SetTexture( 0, g_pTextureMiniMap[ (int)g_aMiniMap[ i ].type ] );
			
				// �|���S���̕`��
				//------------------------------------
				pDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,	// �v���~�e�B�u�̎��
													i * 4,			// �ǂݍ��ލŏ��̔ԍ�
													2);				// �`�悷��v���~�e�B�u��
			}
		}
	}
}

//-----------------------------------------------
// ���_���Z�b�g
//-----------------------------------------------
void SetupVertexMiniMap( void )
{
	// ���[�J���ϐ�
	//------------------------------------
	VERTEX_2D *pVtx2D;

	g_pVtxBuffMiniMap ->Lock( 0, 0, ( void** )&pVtx2D, 0 );

	for ( int i = 0; i < MINIMAP_MAX; i++ )
	{
		if ( g_aMiniMap[ i ].bUse == true )
		{
			if ( g_aMiniMap[ i ].type == MINIMAPTYPE_PLAYER )
			{
				// �ʒu
				pVtx2D[ 0 + i * 4 ].pos = D3DXVECTOR3( g_aMiniMap[ i ].pos.x - g_aMiniMap[ i ].fWidth * -sinf( atan2f( g_aMiniMap[ i ].pos.y, g_aMiniMap[ i ].pos.x ) - g_aMiniMap[ i ].fRotate ), g_aMiniMap[ i ].pos.y - g_aMiniMap[ i ].fHeight * -cosf( atan2f( g_aMiniMap[ i ].pos.y, g_aMiniMap[ i ].pos.x ) - g_aMiniMap[ i ].fRotate ), 0.0f );
				pVtx2D[ 1 + i * 4 ].pos = D3DXVECTOR3( g_aMiniMap[ i ].pos.x + g_aMiniMap[ i ].fWidth * -sinf( atan2f( g_aMiniMap[ i ].pos.y, g_aMiniMap[ i ].pos.x ) + g_aMiniMap[ i ].fRotate ), g_aMiniMap[ i ].pos.y - g_aMiniMap[ i ].fHeight * -cosf( atan2f( g_aMiniMap[ i ].pos.y, g_aMiniMap[ i ].pos.x ) + g_aMiniMap[ i ].fRotate ), 0.0f );
				pVtx2D[ 2 + i * 4 ].pos = D3DXVECTOR3( g_aMiniMap[ i ].pos.x - g_aMiniMap[ i ].fWidth * -sinf( atan2f( g_aMiniMap[ i ].pos.y, g_aMiniMap[ i ].pos.x ) + g_aMiniMap[ i ].fRotate ), g_aMiniMap[ i ].pos.y + g_aMiniMap[ i ].fHeight * -cosf( atan2f( g_aMiniMap[ i ].pos.y, g_aMiniMap[ i ].pos.x ) + g_aMiniMap[ i ].fRotate ), 0.0f );
				pVtx2D[ 3 + i * 4 ].pos = D3DXVECTOR3( g_aMiniMap[ i ].pos.x + g_aMiniMap[ i ].fWidth * -sinf( atan2f( g_aMiniMap[ i ].pos.y, g_aMiniMap[ i ].pos.x ) - g_aMiniMap[ i ].fRotate ), g_aMiniMap[ i ].pos.y + g_aMiniMap[ i ].fHeight * -cosf( atan2f( g_aMiniMap[ i ].pos.y, g_aMiniMap[ i ].pos.x ) - g_aMiniMap[ i ].fRotate ), 0.0f );
			}
			else
			{
				// �ʒu
				pVtx2D[ 0 + i * 4 ].pos = D3DXVECTOR3( g_aMiniMap[ i ].pos.x - g_aMiniMap[ i ].fWidth, g_aMiniMap[ i ].pos.y - g_aMiniMap[ i ].fHeight, 0.0f );
				pVtx2D[ 1 + i * 4 ].pos = D3DXVECTOR3( g_aMiniMap[ i ].pos.x + g_aMiniMap[ i ].fWidth, g_aMiniMap[ i ].pos.y - g_aMiniMap[ i ].fHeight, 0.0f );
				pVtx2D[ 2 + i * 4 ].pos = D3DXVECTOR3( g_aMiniMap[ i ].pos.x - g_aMiniMap[ i ].fWidth, g_aMiniMap[ i ].pos.y + g_aMiniMap[ i ].fHeight, 0.0f );
				pVtx2D[ 3 + i * 4 ].pos = D3DXVECTOR3( g_aMiniMap[ i ].pos.x + g_aMiniMap[ i ].fWidth, g_aMiniMap[ i ].pos.y + g_aMiniMap[ i ].fHeight, 0.0f );
			}

			// �p�[�X�y�N�e�B�u�R���N�g
			pVtx2D[ 0 + i * 4 ].rhw = 1.0f;		
			pVtx2D[ 1 + i * 4 ].rhw = 1.0f;		
			pVtx2D[ 2 + i * 4 ].rhw = 1.0f;		
			pVtx2D[ 3 + i * 4 ].rhw = 1.0f;	
	
			// �F
			pVtx2D[ 0 + i * 4 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f);		
			pVtx2D[ 1 + i * 4 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f);		
			pVtx2D[ 2 + i * 4 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f);		
			pVtx2D[ 3 + i * 4 ].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f);		
			
			// �e�N�X�`�����W
			pVtx2D[ 0 + i * 4 ].tex = D3DXVECTOR2( 0.0f, 0.0f);
			pVtx2D[ 1 + i * 4 ].tex = D3DXVECTOR2( 1.0f, 0.0f);
			pVtx2D[ 2 + i * 4 ].tex = D3DXVECTOR2( 0.0f, 1.0f);
			pVtx2D[ 3 + i * 4 ].tex = D3DXVECTOR2( 1.0f, 1.0f);
		}
	}

	g_pVtxBuffMiniMap ->Unlock();
}

//-----------------------------------------------
// �~�j�}�b�v�Z�b�g
//-----------------------------------------------
void SetMiniMap( D3DXVECTOR3 pos, MINIMAPTYPE type, float fRot )
{
	int nPlayer;

	for ( int i = 0; i < MINIMAP_MAX; i++ )
	{
		if ( g_aMiniMap[ i ].bUse == true )
		{
			if ( g_aMiniMap[ i ].type == MINIMAPTYPE_PLAYER )
			{
				nPlayer = i;
			}
		}

		if ( g_aMiniMap[ i ].bUse == false )
		{
			g_aMiniMap[ i ].worldPos = pos;		// �RD��Ԃ̍��W���擾
			g_aMiniMap[ i ].type = type;		// �\������^�C�v���w��

			// �^�C�v����
			switch ( type )
			{
			case MINIMAPTYPE_PLAYER:
				g_aMiniMap[ i ].fWidth = MINIMAP_ICON_WIDTH;
				g_aMiniMap[ i ].fHeight = MINIMAP_ICON_HEIGHT;
				g_aMiniMap[ i ].fRotate = fRot;

				// �~�j�}�b�v���W�̐^�񒆂ɌŒ�
				g_aMiniMap[ i ].pos = D3DXVECTOR3( MINIMAP_POS_X, MINIMAP_POS_Y, 0.0f );

				break;

			case MINIMAPTYPE_ENEMY:

				// �~�j�}�b�v���W�ɗ��Ƃ�����
				float x, y;
				x = ( g_aMiniMap[ nPlayer ].worldPos.x - g_aMiniMap[ i ].worldPos.x ) / ( SCREEN_WIDTH / MINIMAP_WIDTH );
				y = ( g_aMiniMap[ nPlayer ].worldPos.z - g_aMiniMap[ i ].worldPos.z ) / ( SCREEN_HEIGHT / MINIMAP_HEIGHT );

				// �~�j�}�b�v�O��������`�悵�Ȃ�
				if ( g_aMiniMap[ nPlayer ].pos.x - x < MINIMAP_POS_X - MINIMAP_WIDTH + 5.0f
				||	 g_aMiniMap[ nPlayer ].pos.x - x > MINIMAP_POS_X + MINIMAP_WIDTH - 5.0f
				||	 g_aMiniMap[ nPlayer ].pos.y + y < MINIMAP_POS_Y - MINIMAP_HEIGHT + 5.0f
				||	 g_aMiniMap[ nPlayer ].pos.y + y > MINIMAP_POS_Y + MINIMAP_HEIGHT - 5.0f)
				{
					return;
				}

				// ���W������
				g_aMiniMap[ i ].pos = D3DXVECTOR3( g_aMiniMap[ nPlayer ].pos.x - x, g_aMiniMap[ nPlayer ].pos.y + y, 0.0f );
				g_aMiniMap[ i ].fWidth = MINIMAP_ICON_WIDTH;
				g_aMiniMap[ i ].fHeight = MINIMAP_ICON_HEIGHT;

				break;

			case MINIMAPTYPE_BACK:
				g_aMiniMap[ i ].pos = pos;
				g_aMiniMap[ i ].fWidth = MINIMAP_WIDTH;
				g_aMiniMap[ i ].fHeight = MINIMAP_HEIGHT;

				g_aMiniMap[ MINIMAP_MAX - 1 ].type = type;		// �\������^�C�v���w��
				g_aMiniMap[ MINIMAP_MAX - 1 ].pos = pos;
				g_aMiniMap[ MINIMAP_MAX - 1 ].fWidth = MINIMAP_WIDTH;
				g_aMiniMap[ MINIMAP_MAX - 1 ].fHeight = MINIMAP_HEIGHT;
				g_aMiniMap[ MINIMAP_MAX - 1 ].bUse = true;
				
				break;

			case MINIMAPTYPE_OBJECT:

				// �~�j�}�b�v���W�ɗ��Ƃ�����
				float ox, oy;
				ox = ( g_aMiniMap[ nPlayer ].worldPos.x - g_aMiniMap[ i ].worldPos.x ) / ( SCREEN_WIDTH / MINIMAP_WIDTH );
				oy = ( g_aMiniMap[ nPlayer ].worldPos.z - g_aMiniMap[ i ].worldPos.z ) / ( SCREEN_HEIGHT / MINIMAP_HEIGHT );

				// �~�j�}�b�v�O��������`�悵�Ȃ�
				if ( g_aMiniMap[ nPlayer ].pos.x - ox < MINIMAP_POS_X - MINIMAP_WIDTH + 5.0f
				||	 g_aMiniMap[ nPlayer ].pos.x - ox > MINIMAP_POS_X + MINIMAP_WIDTH - 5.0f
				||	 g_aMiniMap[ nPlayer ].pos.y + oy < MINIMAP_POS_Y - MINIMAP_HEIGHT + 5.0f
				||	 g_aMiniMap[ nPlayer ].pos.y + oy > MINIMAP_POS_Y + MINIMAP_HEIGHT - 5.0f)
				{
					return;
				}

				// ���W������
				g_aMiniMap[ i ].pos = D3DXVECTOR3( g_aMiniMap[ nPlayer ].pos.x - ox, g_aMiniMap[ nPlayer ].pos.y + oy, 0.0f );
				g_aMiniMap[ i ].fWidth = MINIMAP_ICON_WIDTH;
				g_aMiniMap[ i ].fHeight = MINIMAP_ICON_HEIGHT;

				break;
			}

			g_aMiniMap[ i ].bUse = true;
			break;
		}
	}

	// ���_���̃Z�b�g
	SetupVertexMiniMap();
}

//-----------------------------------------------
// �~�j�}�b�v�Z�b�g
//-----------------------------------------------
MINIMAP *GetMiniMap( void )
{
	return &g_aMiniMap[ 0 ];
}
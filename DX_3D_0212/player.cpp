//-----------------------------------------------
//  ���e�F�v���C���[����
//  File : player.cpp
//  ����ҁF�؍�@��
//-----------------------------------------------

//------ �C���N���[�h�t�@�C�� ------
#include "player.h"
#include "camera.h"
#include "collision.h"
#include "cube.h"
#include "Edit.h"
#include "gear.h"
#include "moveCube.h"
#include "billboard.h"
#include "sheet.h"

//------ �}�N����` ------
#define PLAYER_WIDTH ( 20.0f )
#define PLAYER_HEIGHT ( 20.0f )
#define PLAYER_POS_X ( -200.0f )
#define PLAYER_POS_Y ( 40.0f )
#define PLAYER_POS_Z ( 0.0f )

#define MOVE_VAL ( 3.0f )
#define GRAVITY_VAL ( 0.5f )

#define PLAYER_TEX_MAX ( 1 )

//------ �O���[�o���ϐ� ------
D3DXMATRIX mtxWorldPlayer;				// ���[���h�}�g���b�N�X
LPD3DXMESH pMeshModelPlayer;			// ���b�V�����ւ̃|�C���^
LPD3DXBUFFER pBuffMatModelPlayer;		// �}�e���A�����ւ̃|�C���^
DWORD numMatModelPlayer;				// �}�e���A���̐�
LPDIRECT3DTEXTURE9	g_pTexturePlayer[ PLAYER_TEX_MAX ]; 		// �e�N�X�`���ւ̃|�C���^

D3DXVECTOR3 posPlayer;		// �v���C���[�̈ʒu
D3DXVECTOR3 posPlayerOld;	// �v���C���[�̑O��ʒu	
D3DXVECTOR3 rotPlayer;		// �p�x
D3DXVECTOR3 sclPlayer;		// �X�P�[��
COL_RECT rectPlayer;		// �����蔻��
float fJumpVal = 0.0f;		// �W�����v��
int g_nCubeIndex = 0;		// �L���[�u�̃C���f�b�N�X
int g_nRedCubeIndex = 0;	// �ԃL���[�u�̃C���f�b�N�X
bool g_bJump = false;		// �W�����v���H

bool g_bDirect = true;		// ����
	
bool g_bColorChange = false;

//------ �����֐��錾 ------

//-----------------------------------------------
// �v���C���[������
//-----------------------------------------------
void InitPlayer( void )
{
	// ���[�J���ϐ�
	//---------------------------------
	LPDIRECT3DDEVICE9 pDevice = NULL;		// �f�o�C�X�I�u�W�F�N�g
	
	// �f�o�C�X�擾
	//------------------------------------
	pDevice = GetDevice();

	{
		// x�t�@�C���̓ǂݍ���
		//------------------------------------
		D3DXLoadMeshFromX("data/MODEL/player.x",		// �ǂݍ��ރt�@�C����
						  D3DXMESH_SYSTEMMEM,							// 
						  pDevice,										// 
						  NULL,											// 
						  &pBuffMatModelPlayer,			// 
						  NULL,											// 
						  &numMatModelPlayer,			// 
						  &pMeshModelPlayer );
	}

	// �e�N�X�`���|�C���^�̏�����
	g_pTexturePlayer[ 0 ] = NULL;

	// �e�N�X�`���̓ǂݍ���
	//------------------------------------
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Player.jpg", &g_pTexturePlayer[ 0 ]);

	// �O���[�o���ϐ��̏�����
	sclPlayer = D3DXVECTOR3( 1.0f, 1.0f, 1.0f ); 
	rotPlayer = D3DXVECTOR3( 0.0f, 0.0f, 0.0f ); 
	posPlayer = D3DXVECTOR3( PLAYER_POS_X, PLAYER_POS_Y, PLAYER_POS_Z ); 
	posPlayerOld = posPlayer;

	rectPlayer.pos = posPlayer;
	rectPlayer.harfSize = D3DXVECTOR3( PLAYER_WIDTH, PLAYER_HEIGHT, PLAYER_HEIGHT );

	fJumpVal = 0.0f;
	g_nCubeIndex = CUBE_MAX;
	g_nRedCubeIndex = 0;
	g_bDirect = true;
	g_bJump = false;
	g_bColorChange = false;
}

//-----------------------------------------------
// �v���C���[�I������
//-----------------------------------------------
void UninitPlayer( void )
{
	// ���b�V�����̉���Ə�����
	//------------------------------------
	if (pMeshModelPlayer != NULL)
	{ 
		pMeshModelPlayer ->Release();
		pMeshModelPlayer = NULL;
	}
	
	// �}�e���A�����̉���Ə�����
	//------------------------------------
	if (pBuffMatModelPlayer != NULL)
	{ 
		pBuffMatModelPlayer ->Release();
		pBuffMatModelPlayer = NULL;
	}

	// �g�����e�N�X�`���̉���Ə�����
	//------------------------------------
	//for (int nCnt = 0; nCnt < TEXTURE_MAX; nCnt++)
	{
		if (g_pTexturePlayer[ 0 ] != NULL)
		{ 
			g_pTexturePlayer[ 0 ] ->Release();
			g_pTexturePlayer[ 0 ] = NULL;
		}
	}
}

//-----------------------------------------------
// �v���C���[�X�V����
//-----------------------------------------------
void UpdatePlayer( void )
{
	// ���[�J���ϐ�
	CUBE *pCube = GetCube();
	GEAR *pGear = GetGear();
	MOVECUBE *pMoveCube = GetMoveCube();

	if ( GetEdit() == false )
	{
		// �O��ʒu��ۑ�
		posPlayerOld = posPlayer;
		g_nCubeIndex = CUBE_MAX;
		g_bColorChange = false;

		// �f�o�b�O�i�ԃV�[�g�j
		/*if ( ( GetPadElecomTrigger( PAD_6 ) == true || GetKeyboardTrigger( DIK_DOWN ) == true ) && GetSheet() == false )
		{
			GetSheet() = true;
		}
		else if ( ( GetPadElecomTrigger( PAD_6 ) == true || GetKeyboardTrigger( DIK_DOWN ) == true ) && GetSheet() == true )
		{
			GetSheet() = false;
		}*/

		// �d�͂�������
		fJumpVal += GRAVITY_VAL;
		posPlayer.y -= fJumpVal;
		rectPlayer.pos.y = posPlayer.y;

		// ���ԂƂ̓����蔻��
		for ( int i = 0; i < GEAR_MAX; i++ )
		{
			if ( ( pGear + i ) ->bUse == true )
			{
				if ( ColRectXY( &rectPlayer, &( pGear + i ) ->rect ) == true )
				{
					//if ( GetKeyboardTrigger( DIK_RETURN ) == true )
					{
						posPlayer.y = posPlayerOld.y;
						rectPlayer.pos.y = posPlayer.y;
						fJumpVal = 0.0f;
						g_bJump = false;
					}
				}
			}
		}

		if ( GetSheet() == false )
		{
			// �ԃL���[�u�Ƃ̓����蔻��
			for ( int i = 0; i < MOVECUBE_MAX; i++ )
			{
				if ( ( pMoveCube + i ) ->bUse == true )
				{
					if ( ColRectXY( &rectPlayer, &( pMoveCube + i ) ->rect ) == true )
					{
						posPlayer.y = posPlayerOld.y;
						rectPlayer.pos.y = posPlayer.y;
						fJumpVal = 0.0f;
						g_bJump = false;

						g_nCubeIndex = i;
					}
				}
			}
		}
		else
		{
			// �L���[�u�Ƃ̓����蔻��
			for ( int i = 0; i < CUBE_MAX; i++ )
			{
				if ( ( pCube + i ) ->bUse == true )
				{
					if ( ColRectXY( &rectPlayer, &( pCube + i ) ->rect ) == true )
					{
						posPlayer.y = posPlayerOld.y;
						rectPlayer.pos.y = posPlayer.y;
						fJumpVal = 0.0f;
						g_bJump = false;

						g_nCubeIndex = i;
					}
				}
			}
		}

		// �n�ʔ���
		if ( posPlayer.y + PLAYER_HEIGHT <= PLAYER_HEIGHT )
		{
			posPlayer.y = posPlayerOld.y;
			rectPlayer.pos.y = posPlayer.y;
			fJumpVal = 0.0f;			
			g_bJump = false;
		}

		// �ړ������i�L�[�{�[�h�j
		if ( GetKeyboardPress( DIK_A ) == true )
		{
			posPlayer.x -= 3.0f;
			rectPlayer.pos.x = posPlayer.x;
			g_bDirect = false;
		}
		if ( GetKeyboardPress( DIK_D ) == true )
		{
			posPlayer.x += 3.0f;
			rectPlayer.pos.x = posPlayer.x;
			g_bDirect = true;
		}

		// �Q�[���p�b�h�ł̈ړ�����
		if ( ( GetGamePadCrossKeyX() > 0 || GetGamePadCrossKeyY() > 0 || GetGamePadCrossKeyX() < 0 || GetGamePadCrossKeyY() < 0 ) )
		{
			// �����Ă�����̐ݒ�i�����蔻��p�j
			if ( GetGamePadCrossKeyX() > 0 )
			{
				g_bDirect = true;
			}
			else if ( GetGamePadCrossKeyX() < 0 )
			{
				g_bDirect = false;
			}

			// ���[�V�����ύX

			// �ړ�����
			posPlayer.x += GetGamePadCrossKeyX() * MOVE_VAL;
			rectPlayer.pos.x = posPlayer.x;

			// �����̖ڕW�l��ݒ�

		}
		/*else
		{
			if ( g_playerInfo.bMotion == false )
			{
				g_playerInfo.bMove = false;
				g_playerInfo.status = NEUTRAL;
				g_playerInfo.direction = DIRECTION_MAX;
				PrintDebugProc("DIRECTION_MAX\n");
			}
		}*/

		// ���ԂƂ̓����蔻��
		for ( int i = 0; i < GEAR_MAX; i++ )
		{
			if ( ( pGear + i ) ->bUse == true )
			{
				if ( ColRectXY( &rectPlayer, &( pGear + i ) ->rect ) == true )
				{
					//if ( GetKeyboardTrigger( DIK_RETURN ) == true )
					{
						posPlayer.x = posPlayerOld.x;
						rectPlayer.pos.x = posPlayer.x;
					}
				}
			}
		}

		if ( GetSheet() == false )
		{
			// �ԃL���[�u�Ƃ̓����蔻��
			for ( int i = 0; i < MOVECUBE_MAX; i++ )
			{
				if ( ( pMoveCube + i ) ->bUse == true )
				{
					if ( ColRectXY( &rectPlayer, &( pMoveCube + i ) ->rect ) == true )
					{
						posPlayer.x = posPlayerOld.x;
						rectPlayer.pos.x = posPlayer.x;
					}
				}
			}
		}
		else
		{
			// �L���[�u�Ƃ̓����蔻��
			for ( int i = 0; i < CUBE_MAX; i++ )
			{
				if ( ( pCube + i ) ->bUse == true )
				{
					if ( ColRectXY( &rectPlayer, &( pCube + i ) ->rect ) == true )
					{
						posPlayer.x = posPlayerOld.x;
						rectPlayer.pos.x = posPlayer.x;
					}
				}
			}
		}

		// �W�����v����
		if ( ( GetKeyboardTrigger( DIK_SPACE ) == true || GetPadElecomTrigger( PAD_3 ) ) && g_bJump == false )
		{
			fJumpVal -= 8.0f;
			g_bJump = true;
		}

		// �ړ����𔻒肷����
		if ( g_nCubeIndex < CUBE_MAX )
		{
			posPlayer += ( pCube + g_nCubeIndex ) ->move;
			rectPlayer.pos = posPlayer;
		}

		// �F��ς��鏈��
		if ( GetPadElecomTrigger( PAD_4 ) == true || GetKeyboardTrigger( DIK_RETURN ) == true )
		{
			if ( GetSheet() == false )
			{
				// �ԃL���[�u�Ƃ̓����蔻��
				for ( int i = 0; i < MOVECUBE_MAX; i++ )
				{
					if ( ( pMoveCube + i ) ->bUse == true )
					{
						if ( g_bDirect == true )
						{
							// ��Ɨp�ϐ�
							COL_RECT work;

							// ���݂̏�Ԃ�ۑ�
							work = rectPlayer;

							if ( GetKeyboardTrigger( DIK_S ) == true )
							{
								work.pos.y += 10.0f;
							}
							else
							{
								work.pos.x += 10.0f;
							}

							if ( ColRectXY( &work, &( pMoveCube + i ) ->rect ) == true )
							{
								// ���L���[�u�̐���
								SetCube( ( pMoveCube + i ) ->pos );

								SetBillboard( ( pMoveCube + i ) ->pos, 20.0f, 20.0f, TEXTURETYPE_GREEN );
							
								// �ԃL���[�u���Ȃ���
								( pMoveCube + i ) ->bUse = false;

								g_bColorChange = true;

								break;	// ���[�v�𔲂���
							}
						}
						else
						{
							// ��Ɨp�ϐ�
							COL_RECT work;

							// ���݂̏�Ԃ�ۑ�
							work = rectPlayer;

							if ( GetKeyboardTrigger( DIK_S ) == true )
							{
								work.pos.y += 10.0f;
							}
							else
							{
								work.pos.x += 10.0f;
							}
							
							if ( ColRectXY( &work, &( pMoveCube + i ) ->rect ) == true )
							{
								// ���L���[�u�̐���
								SetCube( ( pMoveCube + i ) ->pos );
								
								SetBillboard( ( pMoveCube + i ) ->pos, 20.0f, 20.0f, TEXTURETYPE_GREEN );

								// �ԃL���[�u���Ȃ���
								( pMoveCube + i ) ->bUse = false;

								g_bColorChange = true;

								break;	// ���[�v�𔲂���
							}						
						}
					}
				}
			}

			else //if ( GetSheet() == true )
			{
				// ���L���[�u�Ƃ̓����蔻��
				for ( int i = 0; i < CUBE_MAX; i++ )
				{
					if ( ( pCube + i ) ->bUse == true )
					{
						if ( g_bDirect == true )
						{
							// ��Ɨp�ϐ�
							COL_RECT work;

							// ���݂̏�Ԃ�ۑ�
							work = rectPlayer;

							if ( GetKeyboardTrigger( DIK_S ) == true )
							{
								work.pos.y += 10.0f;
							}
							else
							{
								work.pos.x += 10.0f;
							}
							
							if ( ColRectXY( &work, &( pCube + i ) ->rect ) == true )
							{
								// �ԃL���[�u�̐���
								SetMoveCube( ( pCube + i ) ->pos );

								SetBillboard( ( pCube + i ) ->pos, 20.0f, 20.0f, TEXTURETYPE_RED );
							
								// ���L���[�u���Ȃ���
								( pCube + i ) ->bUse = false;

								break;	// ���[�v�𔲂���
							}
						}
						else
						{
							// ��Ɨp�ϐ�
							COL_RECT work;

							// ���݂̏�Ԃ�ۑ�
							work = rectPlayer;

							if ( GetKeyboardTrigger( DIK_S ) == true )
							{
								work.pos.y += 10.0f;
							}
							else
							{
								work.pos.x -= 10.0f;
							}
							
							if ( ColRectXY( &work, &( pCube + i ) ->rect ) == true )
							{
								// �ԃL���[�u�̐���
								SetMoveCube( ( pCube + i ) ->pos );

								SetBillboard( ( pCube + i ) ->pos, 20.0f, 20.0f, TEXTURETYPE_RED );
							
								// ���L���[�u���Ȃ���
								( pCube + i ) ->bUse = false;

								break;	// ���[�v�𔲂���
							}
						}
					}
				}
			}
		}

		//
		if ( GetKeyboardPress( DIK_RIGHT ) )
		{
			rotPlayer.y += 0.01f;
		}

		PrintDebugProc("[ �v���C���[�ʒu ] %f %f %f\n", posPlayer.x, posPlayer.y, posPlayer.z);
	}
}

//-----------------------------------------------
// �v���C���[�`�揈��
//-----------------------------------------------
void DrawPlayer( void )
{
	// ���[�J���ϐ�
	//------------------------------------
	LPDIRECT3DDEVICE9 pDevice;						// �f�o�C�X�I�u�W�F�N�g
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;		// �v�Z�p
	D3DXMATERIAL *pMat;								// �}�e���A�������擾����ϐ�
	D3DMATERIAL9 matDef;							// ���̃}�e���A�����

	// �f�o�C�X�̎擾
	//------------------------------------
	pDevice = GetDevice();

	// �J�����̃Z�b�g
	SetCamera();

	D3DXMatrixIdentity( &mtxWorldPlayer );	// �t�H�[�}�b�g�̏�����
	D3DXMatrixIdentity( &mtxScl );						// �s��̏�����
	D3DXMatrixIdentity( &mtxRot );						// �s��̏�����
	D3DXMatrixIdentity( &mtxTranslate );				// �s��̏�����

	// �X�P�[���𔽉f
	D3DXMatrixScaling(&mtxScl, sclPlayer.x, sclPlayer.y, sclPlayer.z);
	D3DXMatrixMultiply(&mtxWorldPlayer, &mtxWorldPlayer, &mtxScl);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rotPlayer.y, rotPlayer.x, rotPlayer.z);
	D3DXMatrixMultiply(&mtxWorldPlayer, &mtxWorldPlayer, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTranslate, posPlayer.x, posPlayer.y, posPlayer.z);
	D3DXMatrixMultiply(&mtxWorldPlayer, &mtxWorldPlayer, &mtxTranslate);

	// �ݒ�
	pDevice ->SetTransform(D3DTS_WORLD, &mtxWorldPlayer);

	// ���݂̃}�e���A������ۑ�
	pDevice ->GetMaterial( &matDef );

	// �o�b�t�@�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)pBuffMatModelPlayer ->GetBufferPointer();

	/*strSrc = strrchr(pMat ->pTextureFilename, '\\') + 1;

	strcat(strDest, strSrc);

	D3DXCreateTextureFromFile(pDevice, strDest, &g_pTextureModel[1]);*/
			
	// �}�e���A���̐��������[�v
	for (int nCntMat = 0; nCntMat < (int)numMatModelPlayer; nCntMat++)
	{
		pDevice ->SetMaterial( &pMat[ nCntMat ].MatD3D );			// �}�e���A���̐ݒ�
		pDevice ->SetTexture( 0, g_pTexturePlayer[ 0 ] );							// �e�N�X�`���̃Z�b�g
		pMeshModelPlayer ->DrawSubset( nCntMat );		// �`��
	}
			
	// �}�e���A�������ɖ߂�
	//------------------------------------
	pDevice ->SetMaterial( &matDef );
}

//-----------------------------------------------
// �v���C���[�ʒu�擾����
//-----------------------------------------------
D3DXVECTOR3 GetPlayerPos( void )
{
	return posPlayer;
}

// �f�o�b�O�p
bool GetRed( void )
{
	return GetSheet();
}
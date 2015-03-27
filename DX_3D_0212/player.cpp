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

//------ �}�N����` ------
#define PLAYER_WIDTH ( 20.0f )
#define PLAYER_HEIGHT ( 20.0f )
#define PLAYER_POS_X ( 0.0f )
#define PLAYER_POS_Y ( 30.0f )
#define PLAYER_POS_Z ( 0.0f )

#define GRAVITY_VAL ( 0.5f )

//------ �O���[�o���ϐ� ------
D3DXMATRIX mtxWorldPlayer;				// ���[���h�}�g���b�N�X
LPD3DXMESH pMeshModelPlayer;			// ���b�V�����ւ̃|�C���^
LPD3DXBUFFER pBuffMatModelPlayer;		// �}�e���A�����ւ̃|�C���^
DWORD numMatModelPlayer;				// �}�e���A���̐�

D3DXVECTOR3 posPlayer;	
D3DXVECTOR3 posPlayerOld;	
D3DXVECTOR3 rotPlayer;	
D3DXVECTOR3 sclPlayer;	
COL_RECT rectPlayer;
float fJumpVal = 0.0f;
int g_nCubeIndex = 0;
int g_nRedCubeIndex = 0;
bool g_bJump = false;

bool g_bDirect = true;

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
		D3DXLoadMeshFromX("data/MODEL/sphere.x",		// �ǂݍ��ރt�@�C����
						  D3DXMESH_SYSTEMMEM,							// 
						  pDevice,										// 
						  NULL,											// 
						  &pBuffMatModelPlayer,			// 
						  NULL,											// 
						  &numMatModelPlayer,			// 
						  &pMeshModelPlayer );
	}

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

		// �d�͂�������
		fJumpVal += GRAVITY_VAL;
		posPlayer.y -= fJumpVal;
		rectPlayer.pos.y = posPlayer.y;

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

		// �n�ʔ���
		if ( posPlayer.y + PLAYER_HEIGHT <= PLAYER_HEIGHT )
		{
			posPlayer.y = posPlayerOld.y;
			rectPlayer.pos.y = posPlayer.y;
			fJumpVal = 0.0f;			
			g_bJump = false;
		}

		// �ړ�����
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

		// �ԃL���[�u����鏈��
		if ( GetKeyboardTrigger( DIK_RETURN ) == true )
		{
			if ( g_nRedCubeIndex == 0 )
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

							work.harfSize.x += 10.0f;
							
							if ( ColRectXY( &work, &( pMoveCube + i ) ->rect ) == true )
							{
								g_nRedCubeIndex = 1;
								( pMoveCube + i ) ->bUse = false;
							}

							break;	// ���[�v�𔲂���
						}
						else
						{
							// ��Ɨp�ϐ�
							COL_RECT work;

							// ���݂̏�Ԃ�ۑ�
							work = rectPlayer;

							work.pos.x -= 10.0f;
							
							if ( ColRectXY( &work, &( pMoveCube + i ) ->rect ) == true )
							{
								g_nRedCubeIndex = 1;
								( pMoveCube + i ) ->bUse = false;
							}

							break;	// ���[�v�𔲂���
						}
					}
				}
			}
		}

		// �ԃL���[�u�̐���
		// ���ɐ���
		if ( GetKeyboardTrigger( DIK_P ) == true && GetKeyboardPress( DIK_S ) == false )
		{
			// �L���[�u���������Ă�����
			if ( g_nRedCubeIndex == 1 )
			{
				// �E�����̏ꍇ
				if ( g_bDirect == true )
				{
					if ( ( (int)( posPlayer.x + 40.0f ) % 40 ) != 0 )
					{
						// ��Ɨp�ϐ�
						int nWorkX = ( (int)( posPlayer.x + 40.0f ) / 40 );
						
						// ��������ꏊ���v�Z
						if ( ( (int)( posPlayer.x + 40.0f ) % 40 ) < 10 )
						{
							nWorkX = nWorkX - 1;
						}
						int nWorkY = ( (int)( posPlayer.y + 30.0f ) / 40 );

						// �ԃL���[�u�̐���
						SetMoveCube( D3DXVECTOR3( 40.0f + 40.0f * nWorkX, 40.0f * nWorkY, posPlayer.z ) );
					}
					else
					{
						// �ԃL���[�u�̐���
						SetMoveCube( D3DXVECTOR3( posPlayer.x + 40.0f, posPlayer.y, posPlayer.z ) );
					}
				}

				// �������̏ꍇ
				else
				{
					if ( ( (int)( posPlayer.x - 40.0f ) % 40 ) != 0 )
					{
						// ��������ꏊ���v�Z
						int nWorkX = ( (int)( posPlayer.x - 40.0f ) / 40 );
						if ( ( (int)( posPlayer.x - 40.0f ) % 40 ) > -10 )
						{
							nWorkX = nWorkX + 1;
						}
						int nWorkY = ( (int)( posPlayer.y + 30.0f ) / 40 );

						// �ԃL���[�u�̐���
						SetMoveCube( D3DXVECTOR3( -40.0f + 40.0f * nWorkX, 40.0f * nWorkY, posPlayer.z ) );
					}
					else
					{
						// �ԃL���[�u�̐���
						SetMoveCube( D3DXVECTOR3( posPlayer.x - 40.0f, posPlayer.y, posPlayer.z ) );
					}
				}

				// �����t���O��܂�
				g_nRedCubeIndex = 0;
			}
		}

		// �΂߉��ɐ���
		else if ( GetKeyboardTrigger( DIK_P ) == true && GetKeyboardPress( DIK_S ) == true )
		{
			// �������Ă�����
			if ( g_nRedCubeIndex == 1 )
			{
				// �E�����̏ꍇ
				if ( g_bDirect == true )
				{
					{
						// ��������ꏊ���v�Z
						int nWorkX = ( (int)( posPlayer.x + 40.0f ) / 40 );
						if ( ( (int)( posPlayer.x + 40.0f ) % 40 ) < 10 )
						{
							nWorkX = nWorkX - 1;
						}
						int nWorkY = ( (int)( posPlayer.y + 30.0f ) / 40 );

						// �ԃL���[�u�̐���
						SetMoveCube( D3DXVECTOR3( 40.0f + 40.0f * nWorkX, 40.0f * ( nWorkY - 1 ), posPlayer.z ) );
					}
				}

				// �������̏ꍇ
				else
				{
					{
						// ��������ꏊ���v�Z
						int nWorkX = ( (int)( posPlayer.x - 40.0f ) / 40 );
						if ( ( (int)( posPlayer.x - 40.0f ) % 40 ) > -10 )
						{
							nWorkX = nWorkX + 1;
						}
						int nWorkY = ( (int)( posPlayer.y + 30.0f ) / 40 );

						// �ԃL���[�u�̐���
						SetMoveCube( D3DXVECTOR3( -40.0f + 40.0f * nWorkX, 40.0f * ( nWorkY - 1 ), posPlayer.z ) );
					}
				}

				// �����t���O��܂�
				g_nRedCubeIndex = 0;
			}
		}

		// �W�����v����
		if ( GetKeyboardTrigger( DIK_SPACE ) == true && g_bJump == false )
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

		// ���ԂƂ̓����蔻��
		for ( int i = 0; i < GEAR_MAX; i++ )
		{
			if ( ( pGear + i ) ->bUse == true )
			{
				if ( ColRectXY( &rectPlayer, &( pGear + i ) ->rect ) == true )
				{
					if ( GetKeyboardTrigger( DIK_RETURN ) == true )
					{
						if ( ( pGear + i ) ->bRotation == false )
						{
							SetGearRotation( i, true );
						}
						else
						{
							SetGearRotation( i, false );
						}
					}
				}
			}
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
		pDevice ->SetTexture( 0, NULL );							// �e�N�X�`���̃Z�b�g
		pMeshModelPlayer ->DrawSubset( nCntMat );		// �`��
	}
			
	// �}�e���A�������ɖ߂�
	//------------------------------------
	pDevice ->SetMaterial( &matDef );
}
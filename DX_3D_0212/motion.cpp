//===============================================
//
//  ���e   �F ���[�V�����Ɋւ��鏈��
//  File   �F motion.cpp
//  ����� �F �؍�@��
//
//===============================================

//===============================================
// 
//  �C���N���[�h�t�@�C��
//
//===============================================
#include "motion.h"
#include "camera.h"

//===============================================
//
// ���f���̃��[�V�����Ɋւ��鏉����
//
//===============================================
void InitModelMotionEx( MODEL_INFO *pModel, char *pFileName )
{
	// �t�@�C���̃I�[�v��
	FILE *fp = NULL;
	fp = fopen( pFileName, "rt" );
	if (fp == NULL)
	{
		MessageBox(NULL, "���[�V�����e�L�X�g�̃I�[�v���Ɏ��s", "�G���[", MB_OK);
		return;
	}
	
	// �g�p���郏�[�N
	char strWork[ 100 ];		// �ǂݍ��񂾕�������ꎞ�ۊǂ���z��
	float x = 0.0f;				// �ꎞ�ۊǗp�ϐ�
	float y = 0.0f;				// �ꎞ�ۊǗp�ϐ�
	float z = 0.0f;				// �ꎞ�ۊǗp�ϐ�
	int key = 0;				// �L�[�̃C���f�b�N�X
	int parts = 0;				// �p�[�c�̃C���f�b�N�X
	int nNumModel = 0;			// ���f����
	int nFileName = 0;			// �t�@�C����
	int nNumMotion = 0;			// ���[�V�����ԍ�
	int workInt = 0;			// ���[�N
	float workFloat = 0.0f;		// ���[�N
	bool bFrameCheck = false;	// �d�����Ȃ��g�p���锻��p�ϐ�

	// �Ƃ肠�����ǂݍ���
	fscanf( fp, "%s" , strWork );

	// �t�@�C���̓ǂݍ��݊J�n
	while ( strcmp( strWork, "END_SCRIPT" ) != 0 )
	{
		// ��s�ǂݍ���
		if ( nNumModel == 0 )
		{
			fscanf( fp, "%s = %d" , strWork, &nNumModel );
		}
		else
		{
			fscanf( fp, "%s = %d" , strWork, &workInt );
		}

		// �t�@�C�����̓ǂݍ���
		// ���f���������肵�Ă�����
		if ( nNumModel != nFileName )
		{
			while ( nNumModel != nFileName )
			{
				fscanf( fp, "%s = %s" , strWork, &pModel ->modelFileName[ nFileName ][ 0 ] );
				if ( strcmp( strWork, "MODEL_FILENAME" ) == 0 )
				{
					nFileName++;
				}
			}
		}

		// �L�����N�^�[�̏���ǂݍ���
		if ( strcmp( strWork, "CHARACTERSET" ) == 0 )
		{
			while ( strcmp( strWork, "END_CHARACTERSET" ) != 0 )
			{
				// ��s�ǂݍ���
				fscanf( fp, "%s = %f" , strWork, &workFloat );

				// �ړ���
				if ( strcmp( strWork, "MOVE" ) == 0 )
				{
					pModel ->fMove = workFloat;
				}

				// �W�����v��
				if ( strcmp( strWork, "JUMP" ) == 0 )
				{
					pModel ->fJumpVal = workFloat;
				}

				// �p�[�c��
				if ( strcmp( strWork, "NUM_PARTS" ) == 0 )
				{
					pModel ->nNumParts = ( int )workFloat;
				}

				// �p�[�c�̏���ǂݍ���
				if ( strcmp( strWork, "PARTSSET" ) == 0 )
				{
					// �p�[�c�̐ݒ肪�I���܂�
					while( strcmp( strWork, "END_PARTSSET" ) != 0 )
					{
						// ��s�ǂݍ���
						fscanf( fp, "%s = %f %f %f" , strWork, &x, &y, &z );

						// �C���f�b�N�X�i�z��ԍ��j
						if ( strcmp( strWork, "INDEX" ) == 0 )
						{
							pModel ->nIndex[ parts ] = ( int )x;
						}

						// �e�̃C���f�b�N�X
						if ( strcmp( strWork, "PARENT" ) == 0 )
						{
							pModel ->nParent[ parts ] = ( int )x;
						}

						// �ʒu
						if ( strcmp( strWork, "POS" ) == 0 )
						{
							pModel ->posParts[ parts ].x = x;
							pModel ->posParts[ parts ].y = y;
							pModel ->posParts[ parts ].z = z;
						}

						// �p�x
						if ( strcmp( strWork, "ROT" ) == 0 )
						{
							pModel ->rotParts[ parts ].x = x;
							pModel ->rotParts[ parts ].y = y;
							pModel ->rotParts[ parts ].z = z;
						}
					}

					parts++;	// �p�[�c�̃C���f�b�N�X�𑝂₷
				}
			}
		}

		// ���[�V�����̓ǂݍ���
		if ( strcmp( strWork, "MOTIONSET" ) == 0 )
		{
			key = 0;	// �L�[�̃C���f�b�N�X�̏�����

			// ���[�V�����̐ݒ肪�I���܂�
			while ( strcmp( strWork, "END_MOTIONSET" ) != 0 )
			{
				// ��s�ǂݍ���
				fscanf( fp, "%s = %f" , strWork, &workFloat );

				// ���[�v
				if ( strcmp( strWork, "LOOP" ) == 0 )
				{
					if ( ( int )workFloat == 1 )
					{
						pModel ->aMotion[ nNumMotion ].bLoop = true;
					}
					else
					{
						pModel ->aMotion[ nNumMotion ].bLoop = false;
					}
				}

				// �L�[�̑���
				if ( strcmp( strWork, "NUM_KEY" ) == 0 )
				{
					pModel ->aMotion[ nNumMotion ].nNumKey = ( int )workFloat;
				}

				// �L�[�̏��
				if ( strcmp( strWork, "KEYSET" ) == 0 )
				{
					parts = 0;				// �p�[�c���̏�����
					bFrameCheck = false;	// [ FRAME ]���`�F�b�N��������

					// �P���[�V�����̑S�ẴL�[�̐ݒ肪�I���܂�
					while ( strcmp( strWork, "END_KEYSET" ) != 0 )
					{
						// ��s�ǂݍ���
						fscanf( fp, "%s = %f %f %f" , strWork, &x, &y, &z );	

						// �t���[����
						if ( strcmp( strWork, "FRAME" ) == 0 )
						{
							pModel ->aMotion[ nNumMotion ].nFrame[ key ] = ( int )x;
							bFrameCheck = true;
						}

						// �t���[�����`�F�b�N���Ă�����i�d���Ȃ��s�������j
						if ( bFrameCheck == true )
						{
							// �L�[�̐ݒ�
							if ( strcmp( strWork, "KEY" ) == 0 )
							{
								// �L�[�̐ݒ肪�I���܂�
								while ( strcmp( strWork, "END_KEY" ) != 0 )
								{
									// ��s�ǂݍ���
									fscanf( fp, "%s = %f %f %f" , strWork, &x, &y, &z );

									// �ʒu
									if ( strcmp( strWork, "POS" ) == 0 )
									{
										pModel ->aKeyInfo[ nNumMotion ].pos[ parts ][ key ].x = x;
										pModel ->aKeyInfo[ nNumMotion ].pos[ parts ][ key ].y = y;
										pModel ->aKeyInfo[ nNumMotion ].pos[ parts ][ key ].z = z;
									}

									// �p�x
									if ( strcmp( strWork, "ROT" ) == 0 )
									{
										pModel ->aKeyInfo[ nNumMotion ].rot[ parts ][ key ].x = x;
										pModel ->aKeyInfo[ nNumMotion ].rot[ parts ][ key ].y = y;
										pModel ->aKeyInfo[ nNumMotion ].rot[ parts ][ key ].z = z;
									}
								} //[ END_KEY ]�I���

								parts++;	// �L�[�̃C���f�b�N�X�𑝂₷

							} //[ KEY ]�I���
						} // [ FRAME�`�F�b�N ]�̏I���					
					} //[ END_KEYSET ]�I���
					
					key++;	// �L�[�̃C���f�b�N�X�𑝂₷

				} //[ KEYSET ]�I���
			} //[ END_MOTIONSET ]�I���

			nNumMotion++;	// ���[�V�����̃C���f�b�N�X�𑝂₷
		}//[ MOTIONSET ]�I���
	}
}

//===============================================
//
// ���f���̃��[�V�����Ɋւ���I��
//
//===============================================
void UninitModelMotion( MODEL_INFO *pModel )
{
	for (int nCnt = 0; nCnt < pModel ->nNumParts; nCnt++)
	{
		// ���b�V�����̉���Ə�����
		//------------------------------------
		if (pModel ->pMeshModel[ nCnt ] != NULL)
		{ 
			pModel ->pMeshModel[ nCnt ] ->Release();
			pModel ->pMeshModel[ nCnt ] = NULL;
		}
	
		// �}�e���A�����̉���Ə�����
		//------------------------------------
		if (pModel ->pBuffMatModel[ nCnt ] != NULL)
		{ 
			pModel ->pBuffMatModel[ nCnt ] ->Release();
			pModel ->pBuffMatModel[ nCnt ] = NULL;
		}
	}
}

//===============================================
//
// ���f���̃��[�V�����Ɋւ���X�V
//
//===============================================
void UpdateModelMotion( MODEL_INFO *pModel )
{
	// ���[�v���郂�[�V���������[�v���邩�ǂ����A���ׂ�����
	if ( pModel ->aMotion[ pModel ->status ].bLoop == false && pModel ->bMotion == false )
	{
		pModel ->bMotion = true;
		pModel ->nFrameCount = 0;
		pModel ->nKey = 0;
	}

	if ( pModel ->bMotion == false )
	{
		if ( pModel ->status != pModel ->statusOld )
		{
			pModel ->nFrameCount = 0;
			pModel ->nKey = 0;
		}
	}

	for (int nCnt = 0; nCnt < pModel ->nNumParts; nCnt++)
	{
		if ( pModel ->nFrameCount == 0 )
		{
			pModel ->rotDevide[ nCnt ].x = ( pModel ->aKeyInfo[ pModel ->status ].rot[ nCnt ][ pModel ->nKey ].x - pModel ->targetRot[ nCnt ].x) / pModel ->aMotion[ pModel ->status ].nFrame[ pModel ->nKey ];
			pModel ->rotDevide[ nCnt ].y = ( pModel ->aKeyInfo[ pModel ->status ].rot[ nCnt ][ pModel ->nKey ].y - pModel ->targetRot[ nCnt ].y) / pModel ->aMotion[ pModel ->status ].nFrame[ pModel ->nKey ];
			pModel ->rotDevide[ nCnt ].z = ( pModel ->aKeyInfo[ pModel ->status ].rot[ nCnt ][ pModel ->nKey ].z - pModel ->targetRot[ nCnt ].z) / pModel ->aMotion[ pModel ->status ].nFrame[ pModel ->nKey ];
		
			pModel ->posDevide[ nCnt ].x = ( pModel ->aKeyInfo[ pModel ->status ].pos[ nCnt ][ pModel ->nKey ].x - pModel ->targetPos[ nCnt ].x) / pModel ->aMotion[ pModel ->status ].nFrame[ pModel ->nKey ];
			pModel ->posDevide[ nCnt ].y = ( pModel ->aKeyInfo[ pModel ->status ].pos[ nCnt ][ pModel ->nKey ].y - pModel ->targetPos[ nCnt ].y) / pModel ->aMotion[ pModel ->status ].nFrame[ pModel ->nKey ];
			pModel ->posDevide[ nCnt ].z = ( pModel ->aKeyInfo[ pModel ->status ].pos[ nCnt ][ pModel ->nKey ].z - pModel ->targetPos[ nCnt ].z) / pModel ->aMotion[ pModel ->status ].nFrame[ pModel ->nKey ];
		}

		pModel ->targetRot[ nCnt ].x += pModel ->rotDevide[ nCnt ].x;
		pModel ->targetRot[ nCnt ].y += pModel ->rotDevide[ nCnt ].y;
		pModel ->targetRot[ nCnt ].z += pModel ->rotDevide[ nCnt ].z;

		pModel ->targetPos[ nCnt ].x += pModel ->posDevide[ nCnt ].x;
		pModel ->targetPos[ nCnt ].y += pModel ->posDevide[ nCnt ].y;
		pModel ->targetPos[ nCnt ].z += pModel ->posDevide[ nCnt ].z;
	}

	// �t���[���J�E���g����
	pModel ->nFrameCount++;

	// ���[�V�����̃L�[�ύX����
	if (pModel ->aMotion[ pModel ->status ].nFrame[ pModel ->nKey ] == pModel ->nFrameCount)
	{ 
		pModel ->nKey++;			// �L�[�𑝂₷
		pModel ->nFrameCount = 0;	// �J�E���g�����ɂ��ǂ�

		// �L�[�̑����ɓ��B������
		if (pModel ->aMotion[ pModel ->status ].nNumKey == pModel ->nKey)
		{
			// ���[�v����
			if (pModel ->aMotion[ pModel ->status ].bLoop == true)
			{
				pModel ->nKey = 0;		// �L�[���ŏ��ɂ��ǂ�
			}
			else
			{
				// �L�[�ԍ����Ō�̃L�[�ŌŒ�
				//pModel ->aMotion[ pModel ->status ].nKey = pModel ->aMotion[ pModel ->status ].nNumKey - 1;
				
				pModel ->bMotion = false;
				
				// �W�����v��������
				if ( pModel ->status == JUMP )
				{
					pModel ->nKey = 0;			// �L�[���ŏ��ɂ��ǂ�
					pModel ->nFrameCount = 0;	// �J�E���g�����ɂ��ǂ�
					pModel ->status = NEUTRAL;
					//pModel ->status = RANDING;

					//pModel ->bMotion = true;
				}
				else
				{
					pModel ->nKey = 0;			// �L�[���ŏ��ɂ��ǂ�
					pModel ->nFrameCount = 0;	// �J�E���g�����ɂ��ǂ�
					pModel ->status = NEUTRAL;
				}
			}
		}
	}

	//�f�o�b�O�\��
	//PrintDebugProc("/-------------------------\n");
	//PrintDebugProc("MOTION : NEUTRAL\n");
	//PrintDebugProc("LOOP   : ON\n");
	//PrintDebugProc("KEY    : %d / %d\n", pModel ->nKey, pModel ->aMotion[ pModel ->status ].nNumKey);
	//PrintDebugProc("FRAME  : %d / %d\n", pModel ->nFrameCount, pModel ->aMotion[ pModel ->status ].nFrame[ pModel ->nKey ]);
	//PrintDebugProc("-------------------------/\n\n");
}

//===============================================
//
// ���f���̕`��
//
//===============================================
void DrawModelMotion( MODEL_INFO *pModel )
{
	// ���[�J���ϐ�
	//------------------------------------
	LPDIRECT3DDEVICE9 pDevice;						// �f�o�C�X�I�u�W�F�N�g
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;		// �v�Z�p
	D3DXMATRIX pMtxWorld;
	D3DXMATERIAL *pMat;								// 
	D3DMATERIAL9 matDef;							// ���̃}�e���A�����
	char strDest[256] = "data/TEXTURE/";

	// �f�o�C�X�̎擾
	//------------------------------------
	pDevice = GetDevice();

	SetCamera();

	for (int nCnt = 0; nCnt < pModel ->nNumParts; nCnt++)
	{
		// ���[���h�}�g���b�N�X�̐ݒ�
		//------------------------------------
		D3DXMatrixIdentity( &pModel ->mtxWorld[ nCnt ] );			// �t�H�[�}�b�g�̏�����
		D3DXMatrixIdentity( &mtxScl );
		D3DXMatrixIdentity( &mtxRot );
		D3DXMatrixIdentity( &mtxTranslate );

		// �X�P�[���𔽉f
		D3DXMatrixScaling(&mtxScl, pModel ->scl[ nCnt ].x, pModel ->scl[ nCnt ].y, pModel ->scl[ nCnt ].z);
		D3DXMatrixMultiply(&pModel ->mtxWorld[ nCnt ], &pModel ->mtxWorld[ nCnt ], &mtxScl);

		// �����𔽉f�i���[�V�����p�j
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pModel ->targetRot[ nCnt ].y, pModel ->targetRot[ nCnt ].x, pModel ->targetRot[ nCnt ].z);
		D3DXMatrixMultiply(&pModel ->mtxWorld[ nCnt ], &pModel ->mtxWorld[ nCnt ], &mtxRot);

		// �����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pModel ->rotParts[ nCnt ].y, pModel ->rotParts[ nCnt ].x, pModel ->rotParts[ nCnt ].z);
		D3DXMatrixMultiply(&pModel ->mtxWorld[ nCnt ], &pModel ->mtxWorld[ nCnt ], &mtxRot);

		// �ʒu�𔽉f�i���[�V�����p�j
		D3DXMatrixTranslation(&mtxTranslate, pModel ->targetPos[ nCnt ].x, pModel ->targetPos[ nCnt ].y, pModel ->targetPos[ nCnt ].z);
		D3DXMatrixMultiply(&pModel ->mtxWorld[ nCnt ], &pModel ->mtxWorld[ nCnt ], &mtxTranslate);

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTranslate, pModel ->posParts[ nCnt ].x, pModel ->posParts[ nCnt ].y, pModel ->posParts[ nCnt ].z);
		D3DXMatrixMultiply(&pModel ->mtxWorld[ nCnt ], &pModel ->mtxWorld[ nCnt ], &mtxTranslate);

		// �p�[�c�ɐe��������
		if ( pModel ->nParent[ nCnt ] != -1 )
		{
			// �e�q�ݒ�
			D3DXMatrixMultiply(&pModel ->mtxWorld[ nCnt ], &pModel ->mtxWorld[ nCnt ], &pModel ->mtxWorld[ pModel ->nParent[ nCnt ] ]);
		}

		// �ݒ�
		pDevice ->SetTransform(D3DTS_WORLD, &pModel ->mtxWorld[ nCnt ]);

		// ���݂̃}�e���A������ۑ�
		//------------------------------------
		pDevice ->GetMaterial( &matDef );

		pMat = (D3DXMATERIAL*)pModel ->pBuffMatModel[ nCnt ] ->GetBufferPointer();

		/*strSrc = strrchr(pMat ->pTextureFilename, '\\') + 1;

		strcat(strDest, strSrc);

		D3DXCreateTextureFromFile(pDevice, strDest, &g_pTextureModel[1]);*/
		
		// �}�e���A���̐��������[�v
		for (int nCntMat = 0; nCntMat < (int)pModel ->numMatModel[ nCnt ]; nCntMat++)
		{
			pDevice ->SetMaterial( &pMat[ nCntMat ].MatD3D );			// �}�e���A���̐ݒ�
			pDevice ->SetTexture( 0, NULL );								// �e�N�X�`���̃Z�b�g
			pModel ->pMeshModel[ nCnt ] ->DrawSubset( nCntMat );	// �`��
		}
			
		// �}�e���A�������ɖ߂�
		//------------------------------------
		pDevice ->SetMaterial( &matDef );
	}
}
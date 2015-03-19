//===============================================
//
//  ���e   �F �󏈗�
//  File   �F sky.cpp
//  ����� �F �؍�@��
//
//===============================================

// �C���N���[�h�t�@�C��
#include "object.h"
#include "model.h"
#include "enemy.h"

// �}�N����`

// �O���[�o���ϐ�
D3DXMATRIX mtxWorldSky;			// ���[���h�}�g���b�N�X
D3DXVECTOR3 posSky;				// �ʒu
D3DXVECTOR3 rotSky;				// �p�x
D3DXVECTOR3 sclSky;				// �X�P�[��

LPD3DXMESH pMeshSky;			// ���b�V�����ւ̃|�C���^
LPD3DXBUFFER pBuffMatSky;	// �}�e���A�����ւ̃|�C���^
DWORD numMatSky;				// �}�e���A���̐�
LPDIRECT3DTEXTURE9 pTextureSky;		// �e�N�X�`���ւ̃|�C���^

//===============================================
//
// �I�u�W�F�N�g�Ɋւ��鏉����
//
//===============================================
void InitSky(void)
{
	// ���[�J���ϐ�
	//------------------------------------
	LPDIRECT3DDEVICE9 pDevice;						// �f�o�C�X�I�u�W�F�N�g

	// �f�o�C�X�̎擾
	//------------------------------------
	pDevice = GetDevice();

	pTextureSky = NULL;

	posSky = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	rotSky = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	sclSky = D3DXVECTOR3( 1.0f, 1.0f, 1.0f );
	
	// x�t�@�C���̓ǂݍ���
	//------------------------------------
	D3DXLoadMeshFromX("data/MODEL/sky.x",		// �ǂݍ��ރt�@�C����
						D3DXMESH_SYSTEMMEM,		// 
						pDevice,					// 
						NULL,						// 
						&pBuffMatSky,			// 
						NULL,						// 
						&numMatSky,			// 
						&pMeshSky);

	// �e�N�X�`���̓ǂݍ���
	//------------------------------------
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/sky000.jpg", &pTextureSky);
}

//===============================================
//
// �I�u�W�F�N�g�Ɋւ���I������
//
//===============================================
void UninitSky(void)
{
	if ( pMeshSky != NULL )
	{ 
		// ���b�V���I�u�W�F�N�g�̉��
		//------------------------------------
		pMeshSky -> Release();
		pMeshSky = NULL;
	}
	// 
	//------------------------------------
	if ( pBuffMatSky != NULL )
	{ 
		// �I�u�W�F�N�g�o�b�t�@�̉��
		pBuffMatSky -> Release();
		pBuffMatSky = NULL;
	}
	
	if ( pTextureSky != NULL )
	{ 
		// �g�����e�N�X�`���̉���Ə�����
		//------------------------------------
		pTextureSky ->Release();
		pTextureSky = NULL;
	}
}

//===============================================
//
// �I�u�W�F�N�g�Ɋւ���X�V����
//
//===============================================
void UpdateSky(void)
{
	// ��̉�]
	rotSky.y += 0.0003f;
}

//===============================================
//
// �I�u�W�F�N�g�Ɋւ���`�揈��
//
//===============================================
void DrawSky(void)
{
	// ���[�J���ϐ�
	//------------------------------------
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// �f�o�C�X�I�u�W�F�N�g
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;		// �v�Z�p
	D3DXMATRIX pMtxWorld;
	D3DXMATERIAL *pMat;								// 
	D3DMATERIAL9 matDef;							// ���̃}�e���A�����


	// ���[���h�}�g���b�N�X�̐ݒ�
	//------------------------------------
	D3DXMatrixIdentity(&mtxWorldSky);			// �t�H�[�}�b�g�̏�����
	D3DXMatrixIdentity(&mtxScl);
	D3DXMatrixIdentity(&mtxRot);
	D3DXMatrixIdentity(&mtxTranslate);

	//==============================================
	// 
	// ���f���鏇�Ԃ͌���I�I
	// 
	//==============================================

	// �X�P�[���𔽉f
	D3DXMatrixScaling(&mtxScl, sclSky.x, sclSky.y, sclSky.z);
	D3DXMatrixMultiply(&mtxWorldSky, &mtxWorldSky, &mtxScl);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rotSky.y, rotSky.x, rotSky.z);
	D3DXMatrixMultiply(&mtxWorldSky, &mtxWorldSky, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTranslate, posSky.x, posSky.y, posSky.z);
	D3DXMatrixMultiply(&mtxWorldSky, &mtxWorldSky, &mtxTranslate);
		
	// �ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorldSky);

	// ���݂̃}�e���A������ۑ�
	//------------------------------------
	pDevice->GetMaterial(&matDef);

	pMat = (D3DXMATERIAL*)pBuffMatSky ->GetBufferPointer();

	/*strSrc = strrchr(pMat ->pTextureFilename, '\\') + 1;

	strcat(strDest, strSrc);

	D3DXCreateTextureFromFile(pDevice, strDest, &g_pTextureSky[1]);*/

	// �}�e���A���̐��������[�v
	for (int nCntMat = 0; nCntMat < (int)numMatSky; nCntMat++)
	{
		pDevice ->SetMaterial(&pMat[ nCntMat ].MatD3D);		// �}�e���A���̐ݒ�
		pDevice ->SetTexture(0, pTextureSky);						// �e�N�X�`���̃Z�b�g
		pMeshSky->DrawSubset( nCntMat );	// �`��
	}

	// �}�e���A�������ɖ߂�
	//------------------------------------
	pDevice ->SetMaterial( &matDef );
}
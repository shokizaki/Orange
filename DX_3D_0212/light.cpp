//===============================================
//
//  ���e   �F ���C�g�Ɋւ��鏈��
//  File   �F camera.cpp
//  ����� �F �؍�@��
//
//===============================================

//===============================================
// 
//  �C���N���[�h�t�@�C��
//
//===============================================
#include "light.h"
#include "input.h"

//===============================================
//
//�@�}�N����`
//
//===============================================
//#define MODEL_TEXTURE_MAX 	(1)			// �g�p����e�N�X�`���̐�
//#define TEX_PASS		"data/TEXTURE/airsrock.jpg"

//-----------------------------------------------
//  �O���[�o���ϐ�
//-----------------------------------------------
D3DLIGHT9 g_aLight[3];			// ���C�g���

//===============================================
//
// ���C�g�Ɋւ��鏉����
//
//===============================================
void InitLight(void)
{
	// ���[�J���ϐ�
	//------------------------------------
	LPDIRECT3DDEVICE9 pDevice;		// �f�o�C�X�I�u�W�F�N�g
	D3DXVECTOR3 vecDir;				// �v�Z�p�i�x�N�g���̌����j

	// �f�o�C�X�̎擾
	//------------------------------------
	pDevice = GetDevice();

	ZeroMemory(&g_aLight[0], sizeof(D3DLIGHT9));						// ���g��������
	g_aLight[0].Type = D3DLIGHT_DIRECTIONAL;							// ���C�g�̎�ށi���s�����j
	g_aLight[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);			// ���C�g�̐F
	vecDir = D3DXVECTOR3(1.0f, 0.0f, 1.0f);								// ���C�g�̌���
	D3DXVec3Normalize( (D3DXVECTOR3*)&g_aLight[0].Direction, &vecDir );	// ���C�g�̌������m�[�}���C�Y
	pDevice ->SetLight(0, &g_aLight[0]);								// ���C�g�̐ݒ�
	pDevice ->LightEnable(0, TRUE);										// ���C�g�̗L����

	ZeroMemory(&g_aLight[1], sizeof(D3DLIGHT9));						// ���g��������
	g_aLight[1].Type = D3DLIGHT_DIRECTIONAL;							// ���C�g�̎�ށi���s�����j
	g_aLight[1].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);			// ���C�g�̐F
	vecDir = D3DXVECTOR3(0.0f, -1.0f, 0.0f);							// ���C�g�̌���
	D3DXVec3Normalize( (D3DXVECTOR3*)&g_aLight[1].Direction, &vecDir );	// ���C�g�̌������m�[�}���C�Y
	pDevice ->SetLight(1, &g_aLight[1]);								// ���C�g�̐ݒ�
	pDevice ->LightEnable(1, TRUE);										// ���C�g�̗L����

	ZeroMemory(&g_aLight[2], sizeof(D3DLIGHT9));						// ���g��������
	g_aLight[2].Type = D3DLIGHT_DIRECTIONAL;							// ���C�g�̎�ށi���s�����j
	g_aLight[2].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);			// ���C�g�̐F
	vecDir = D3DXVECTOR3(-1.0f, 0.0f, -1.0f);							// ���C�g�̌���
	D3DXVec3Normalize( (D3DXVECTOR3*)&g_aLight[2].Direction, &vecDir );	// ���C�g�̌������m�[�}���C�Y
	pDevice ->SetLight(2, &g_aLight[2]);								// ���C�g�̐ݒ�
	pDevice ->LightEnable(2, TRUE);
}

//===============================================
//
// ���C�g�Ɋւ���I������
//
//===============================================
void UninitLight(void)
{

}

//===============================================
//
// ���C�g�Ɋւ���X�V����
//
//===============================================
void UpdateLight(void)
{
	
}

//===============================================
//
// ���C�g�Ɋւ���`�揈��
//
//===============================================
void DrawLight(void)
{

}

//===============================================
//
// ���C�g���擾
//
//===============================================
//CAMERA *GetLight()
//{
//	return &g_camera;
//}
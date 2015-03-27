//===============================================
//
//  ���e   �F �J�����Ɋւ��鏈��
//  File   �F camera.cpp
//  ����� �F �؍�@��
//
//===============================================

//===============================================
// 
//  �C���N���[�h�t�@�C��
//
//===============================================
#include "camera.h"
#include "input.h"
#include "cube.h"
#include "Edit.h"
#include "player.h"

//===============================================
//
//�@�}�N����`
//
//===============================================
#define ROT_VAL (D3DX_PI / 180)	// ��]�p�x
#define MOVE_VAL (3.0f)			// �ړ���
#define ROTATE_VAL (0.08f)
#define INTERVAL_CAMERA ( 1000.0f )	// �J�����ƃ��f���̋���
#define HEIGHT_CAMERA	( 200.0f )	// �J�����̎��_�̍���

#define INTERVAL_BATTLECAMERA	( 700.0f )	// �J�����ƃ��f���̋���
#define HEIGHT_BATTLECAMERA		( 250.0f )	// �J�����̎��_�̍���

//-----------------------------------------------
//  �O���[�o���ϐ�
//-----------------------------------------------
CAMERA g_camera;				// CAMERA�^�\���̕ϐ�
D3DXMATRIX g_mtxProjection;		// �v���W�F�N�V�����}�g���b�N�X
bool bRotate = false;

//===============================================
//
// �J�����Ɋւ��鏉����
//
//===============================================
void InitCamera(void)
{
	// �J�����Ɋւ��鏉����
	//------------------------------------
	g_camera.posCameraP = D3DXVECTOR3(0.0f, 100.0f, -500.0f);	// ���_�̏�����
	g_camera.posCameraR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �����_�̏�����
	g_camera.vecCameraU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		// ������x�N�g���̏�����

	{
		g_camera.rotCamera = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �J�����̊p�x
		g_camera.fLengthIntervalCamera = INTERVAL_CAMERA;
		g_camera.fLengthHeightCamera = HEIGHT_CAMERA;
	}

	g_camera.posCameraP_Old = g_camera.posCameraP;				// ���_�̃��Z�b�g�ʒu�̕ۑ�
	g_camera.posCameraR_Old = g_camera.posCameraR;				// �����_�����Z�b�g�ʒu�̕ۑ�

	bRotate = false;

	//SetFadePos( g_camera.posCameraP, D3DXVECTOR3(0.0f, g_camera.rotCamera.y, 0.0f) );
}

//===============================================
//
// �J�����Ɋւ���I������
//
//===============================================
void UninitCamera(void)
{

}

//===============================================
//
// �J�����Ɋւ���X�V����
//
//===============================================
void UpdateCamera(void)
{
	D3DXVECTOR3 pPosPlayer = GetPlayerPos();

	if ( GetEdit() == false )
	{
		g_camera.posCameraP.x = pPosPlayer.x;
		g_camera.posCameraP.y = pPosPlayer.y + 100.0f;
		g_camera.posCameraP.z = pPosPlayer.z - 500.0f;
		
		g_camera.posCameraR.y = pPosPlayer.y + 100.0f;

		// �ړ��������ʂ𒍎��_�ɔ��f
		g_camera.posCameraR.x = g_camera.posCameraP.x + sinf(-g_camera.rotCamera.y) * g_camera.fLengthIntervalCamera;
		g_camera.posCameraR.z = g_camera.posCameraP.z + cosf(g_camera.rotCamera.y) * g_camera.fLengthIntervalCamera;
	}
	else
	{
		// ����
		if (GetKeyboardPress(DIK_Q))
		{
			g_camera.rotCamera.y += 0.05f;
		}
		if (GetKeyboardPress(DIK_E))
		{
			g_camera.rotCamera.y -= 0.05f;
		}

		// �����_�ړ�
		// ��ړ�
		if (GetKeyboardPress(DIK_R))
		{
			g_camera.posCameraP.y += 5.0f;
		}
		// ���ړ�
		if (GetKeyboardPress(DIK_V))
		{
			g_camera.posCameraP.y -= 5.0f;
		}
		
		// ���_�ړ�
		if (GetKeyboardPress(DIK_T))
		{
			g_camera.posCameraP.x -= sinf( g_camera.rotCamera.y ) * 5.0f;
			g_camera.posCameraP.z += cosf( g_camera.rotCamera.y ) * 5.0f;
		}
		if (GetKeyboardPress(DIK_G))
		{
			g_camera.posCameraP.x += sinf( g_camera.rotCamera.y ) * 5.0f;
			g_camera.posCameraP.z -= cosf( g_camera.rotCamera.y ) * 5.0f;
		}
		if (GetKeyboardPress(DIK_F))
		{
			g_camera.posCameraP.x -= cosf( g_camera.rotCamera.y ) * 5.0f;
			g_camera.posCameraP.z -= sinf( g_camera.rotCamera.y ) * 5.0f;
		}
		if (GetKeyboardPress(DIK_H))
		{
			g_camera.posCameraP.x += cosf( g_camera.rotCamera.y ) * 5.0f;
			g_camera.posCameraP.z += sinf( g_camera.rotCamera.y ) * 5.0f;
		}

		if ( GetKeyboardPress(DIK_N) )
		{
			g_camera.posCameraP.y -= 2.0f;
			g_camera.posCameraR.y -= 2.0f;
		}
		if ( GetKeyboardPress(DIK_M) )
		{
			g_camera.posCameraP.y += 2.0f;
			g_camera.posCameraR.y += 2.0f;
		}
	}

	// �J�����̉�荞�ݏ����i�����j
	/*if ( ( GetKeyboardTrigger( DIK_L ) == true || GetGamePadTrigger( GAMEPAD_L ) ) || bRotate == true )
	{
		bRotate = true;
		
		RotationCamera();
	}*/
}

//===============================================
//
// �J�����Ɋւ���`�揈��
//
//===============================================
void SetCamera(void)
{
	// ���[�J���ϐ�
	//------------------------------------
	LPDIRECT3DDEVICE9 pDevice;						// �f�o�C�X�I�u�W�F�N�g

	// �f�o�C�X�̎擾
	//------------------------------------
	pDevice = GetDevice();

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	//------------------------------------
	D3DXMatrixIdentity(&g_mtxProjection);		// �t�H�[�}�b�g�̏�����

	D3DXMatrixPerspectiveFovLH(&g_mtxProjection,							// �쐬�J�n
								(float)DEGTOR(45),							// ����p�i��p�j
								(float)SCREEN_WIDTH / SCREEN_HEIGHT,		// �A�X�y�N�g��
								20.0f,										// Near�l
								10000.0f);									// Far�l

	pDevice ->SetTransform(D3DTS_PROJECTION, &g_mtxProjection);				// �ݒ�

	// �r���[�}�g���b�N�X�̐ݒ�
	//------------------------------------
	D3DXMatrixIdentity(&g_camera.mtxView);			// �t�H�[�}�b�g�̏�����

	D3DXMatrixLookAtLH(&g_camera.mtxView,					// �쐬�J�n
						&g_camera.posCameraP,		// �J�����̎��_
						&g_camera.posCameraR,		// �J�����̒����_
						&g_camera.vecCameraU);		// �J�����̏�����x�N�g��

	pDevice->SetTransform(D3DTS_VIEW, &g_camera.mtxView);	// �ݒ�
}

//===============================================
//
// �J�����̃o�g�����Ɋւ��鏉����
//
//===============================================
void InitBattleCamera( void )
{
	// �J�����Ɋւ��鏉����
	//------------------------------------
	g_camera.posCameraP = D3DXVECTOR3(0.0f, HEIGHT_BATTLECAMERA, -500.0f);	// ���_�̏�����
	g_camera.posCameraR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �����_�̏�����
	g_camera.vecCameraU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		// ������x�N�g���̏�����

	g_camera.rotCamera = D3DXVECTOR3(0.0f, -1.57f, 0.0f);			// �J�����̊p�x
	g_camera.fLengthIntervalCamera = INTERVAL_BATTLECAMERA;
	g_camera.fLengthHeightCamera = HEIGHT_BATTLECAMERA;

	g_camera.posCameraP_Old = g_camera.posCameraP;				// ���_�̃��Z�b�g�ʒu�̕ۑ�
	g_camera.posCameraR_Old = g_camera.posCameraR;				// �����_�����Z�b�g�ʒu�̕ۑ�

	bRotate = false;
}

//===============================================
//
// �J�������擾
//
//===============================================
CAMERA *GetCamera()
{
	return &g_camera;
}
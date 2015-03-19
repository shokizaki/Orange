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
#include "model.h"
#include "input.h"
#include "billboard.h"
#include "pause.h"
#include "stringDraw.h"
#include "enemy.h"

//===============================================
//
//�@�}�N����`
//
//===============================================
#define ROT_VAL (D3DX_PI / 180)	// ��]�p�x
#define MOVE_VAL (3.0f)			// �ړ���
#define ROTATE_VAL (0.08f)

#define INTERVAL_CAMERA ( 800.0f )	// �J�����ƃ��f���̋���
#define HEIGHT_CAMERA	( 200.0f )	// �J�����̎��_�̍���

#define INTERVAL_BATTLECAMERA	( 600.0f )	// �J�����ƃ��f���̋���
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
	if ( GetMode() == MODE_TITLE )
	{
		g_camera.posCameraP = D3DXVECTOR3(0.0f, 100.0f, -500.0f);	// ���_�̏�����
		g_camera.posCameraR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �����_�̏�����
		g_camera.vecCameraU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		// ������x�N�g���̏�����

		g_camera.rotCamera = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �J�����̊p�x
		g_camera.fLengthIntervalCamera = sqrtf((g_camera.posCameraP.x * g_camera.posCameraP.x) + (g_camera.posCameraP.z * g_camera.posCameraP.z));
		g_camera.fLengthHeightCamera = HEIGHT_BATTLECAMERA;
	}
	else
	{
		g_camera.posCameraP = D3DXVECTOR3(0.0f, 300.0f, -600.0f);	// ���_�̏�����
		g_camera.posCameraR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �����_�̏�����
		g_camera.vecCameraU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		// ������x�N�g���̏�����

		g_camera.rotCamera = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �J�����̊p�x
		g_camera.fLengthIntervalCamera = sqrtf((g_camera.posCameraP.x * g_camera.posCameraP.x) + (g_camera.posCameraP.z * g_camera.posCameraP.z));
		g_camera.fLengthHeightCamera = HEIGHT_BATTLECAMERA;
		g_camera.fLengthIntervalCamera = INTERVAL_BATTLECAMERA;
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
	MODEL_INFO *pModelInfo = GetModelInfo();
	MODEL_INFO *pEnemyInfo = GetEnemyInfo();
	bool *bEnemy = GetEnemyUse();

	if ( GetPauseDraw() == false )
	{
		// �����_�ړ�
		if (GetKeyboardPress(DIK_Q))
		{
			g_camera.rotCamera.y += 0.05f;
		}
		if (GetKeyboardPress(DIK_E))
		{
			g_camera.rotCamera.y -= 0.05f;
		}
		// ��ړ�
		if (GetKeyboardPress(DIK_Y))
		{
			g_camera.posCameraR.y += 5.0f;
		}
		// ���ړ�
		if (GetKeyboardPress(DIK_N))
		{
			g_camera.posCameraR.y -= 5.0f;
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

		// ��ړ�
		if (GetKeyboardPress(DIK_U))
		{
			g_camera.posCameraP.y += 5.0f;
		}
		// ���ړ�
		if (GetKeyboardPress(DIK_M))
		{
			g_camera.posCameraP.y -= 5.0f;
		}

		// �ړ��������ʂ𒍎��_�ɔ��f
		g_camera.posCameraR.x = g_camera.posCameraP.x + sinf(-g_camera.rotCamera.y) * g_camera.fLengthIntervalCamera;
		g_camera.posCameraR.z = g_camera.posCameraP.z + cosf(g_camera.rotCamera.y) * g_camera.fLengthIntervalCamera;

		return;
	}

	//==============================================
	// 
	// �J�����ړ�����
	// 
	//==============================================

	// ��ړ�
	if (GetKeyboardPress(DIK_Y))
	{
		g_camera.posCameraP.y += 2.0f;
	}

	// ���ړ�
	if (GetKeyboardPress(DIK_N))
	{
		g_camera.posCameraP.y -= 2.0f;
	}

	//==============================================
	// 
	// �����_����
	// 
	//==============================================

	// ��ړ�
	if (GetKeyboardPress(DIK_T))
	{
		g_camera.posCameraR.y += 5.0f;
	}

	// ���ړ�
	if (GetKeyboardPress(DIK_B))
	{
		g_camera.posCameraR.y -= 5.0f;
	}

	if ( ( GetKeyboardTrigger( DIK_L ) == true || GetGamePadTrigger( GAMEPAD_L ) ) || bRotate == true )
	{
		bRotate = true;
		
		// �J�����̉�荞�ݏ���
		RotationCamera();
	}

	// �J�����̉�荞�ݏ����i�蓮�j
	// �Q�[���p�b�h���g�p���Ă���ꍇ
	//if ( GetGamePadUse() == true )
	{
		if ( GetPadElecomStickRX() == -1 )
		{
			g_camera.rotCamera.y += 0.03f;
		}

		if ( GetPadElecomStickRX() == 1 )
		{
			g_camera.rotCamera.y -= 0.03f;
		}
	}

	// �L�[�{�[�h�̏ꍇ
	//else
	{
		if ( GetKeyboardPress( DIK_U ) == true )
		{
			g_camera.rotCamera.y += 0.03f;
		}

		if ( GetKeyboardPress( DIK_O ) == true )
		{
			g_camera.rotCamera.y -= 0.03f;
		}
	}

	// �K��l�𒴂�����ۂߍ���
	if (g_camera.rotCamera.y > D3DX_PI)
	{
		g_camera.rotCamera.y = ( ( g_camera.rotCamera.y ) - ( D3DX_PI * 2 ) );
	}
	if (g_camera.rotCamera.y < -D3DX_PI)
	{
		g_camera.rotCamera.y = ( ( g_camera.rotCamera.y ) + ( D3DX_PI * 2 ) );
	}

	if ( GetStringMessage() == GOTOUCH )
	{
		for ( int i = 0; i < ENEMY_MAX; i++ )
		{
			if ( *( bEnemy + i ) == true )
			{
				g_camera.posCameraR.x = ( pEnemyInfo + i ) ->posParts[ 0 ].x;
				break;
			}
		}
	}
	else
	{
		if ( GetMode() == MODE_TITLE )
		{
			// ���_�̈ʒu��ݒ�
			g_camera.posCameraP.x = pModelInfo->posParts[ 0 ].x + 400.0f * sinf( g_camera.rotCamera.y );
			g_camera.posCameraP.y = pModelInfo->posParts[ 0 ].y + 200.0f;
			g_camera.posCameraP.z = pModelInfo->posParts[ 0 ].z + 500.0f * -cosf( g_camera.rotCamera.y );

			// �����_�̈ʒu��ݒ�
			g_camera.posCameraR.x += ((pModelInfo ->posParts[ 0 ].x + -sinf(pModelInfo ->rotParts[ 0 ].y) * 30.0f + (pModelInfo ->move.x * 15.0f)) - g_camera.posCameraR.x) * 0.1f;
			g_camera.posCameraR.z += ((pModelInfo ->posParts[ 0 ].z + -cosf(pModelInfo ->rotParts[ 0 ].y) * 50.0f + (pModelInfo ->move.z * 15.0f)) - g_camera.posCameraR.z) * 0.1f;	
		}
		else
		{
			// ���_�̈ʒu��ݒ�
			g_camera.posCameraP.x = pModelInfo->posParts[ 0 ].x + g_camera.fLengthIntervalCamera * sinf( g_camera.rotCamera.y );
			g_camera.posCameraP.y = pModelInfo->posParts[ 0 ].y + g_camera.fLengthHeightCamera;
			g_camera.posCameraP.z = pModelInfo->posParts[ 0 ].z + g_camera.fLengthIntervalCamera * -cosf( g_camera.rotCamera.y );

			// �����_�̈ʒu��ݒ�
			g_camera.posCameraR.x += ((pModelInfo ->posParts[ 0 ].x + -sinf(pModelInfo ->rotParts[ 0 ].y) * 30.0f + (pModelInfo ->move.x * 15.0f)) - g_camera.posCameraR.x) * 0.1f;
			g_camera.posCameraR.z += ((pModelInfo ->posParts[ 0 ].z + -cosf(pModelInfo ->rotParts[ 0 ].y) * 50.0f + (pModelInfo ->move.z * 15.0f)) - g_camera.posCameraR.z) * 0.1f;	
		}
	}

		
	// �f�o�b�O�\��
	PrintDebugProc("/----- �J������� -------------/\n");
	PrintDebugProc("[ ���_ ]\nX : %f  Y : %f  Z : %f\n", g_camera.posCameraP.x, g_camera.posCameraP.y, g_camera.posCameraP.z);
	//PrintDebugProc("[ �����_ ]\nX : %f  Y : %f  Z : %f\n", g_camera.posCameraR.x, g_camera.posCameraR.y, g_camera.posCameraR.z);
	//PrintDebugProc("[ ���� ]\nY : %f\n", g_camera.rotCamera.y);
	//PrintDebugProc("[ �ڕW�̌��� ]\nY : %f\n", g_camera.rotCameraGoal.y);
	PrintDebugProc("/------------------------------/\n\n");
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
// �J�����̉�荞�ݏ���
//
//===============================================
void RotationCamera(void)
{
	// ���[�J���ϐ�
	MODEL_INFO *pModelInfo = GetModelInfo();
	static bool bSave = false;

	// �J�����̖ڕW�n�_���i�[
	if ( bSave == false )
	{
		g_camera.rotCameraGoal.y = D3DX_PI - pModelInfo ->targetRotParts.y;
		bSave = true;
	}

	if (g_camera.rotCameraGoal.y > D3DX_PI)
	{
		g_camera.rotCameraGoal.y = ((g_camera.rotCameraGoal.y) - (D3DX_PI * 2));
	}
	else if (g_camera.rotCameraGoal.y < -D3DX_PI)
	{
		g_camera.rotCameraGoal.y = ((g_camera.rotCameraGoal.y) + (D3DX_PI * 2));
	}

	if ( fabs( g_camera.rotCamera.y - g_camera.rotCameraGoal.y ) < 0.02f )
	{
		bRotate = false;
		bSave = false;
		return;
	}

	// �J�����̉�荞�ݏ���
	//if (pModel ->bMove == false)
	{
		if (g_camera.rotCameraGoal.y - g_camera.rotCamera.y > D3DX_PI)
		{
			g_camera.rotCamera.y += ((g_camera.rotCameraGoal.y - g_camera.rotCamera.y) - (D3DX_PI * 2)) * ROTATE_VAL;

			if (g_camera.rotCamera.y > D3DX_PI)
			{
				g_camera.rotCamera.y = ((g_camera.rotCamera.y) - (D3DX_PI * 2));
			}
			else if (g_camera.rotCamera.y < -D3DX_PI)
			{
				g_camera.rotCamera.y = ((g_camera.rotCamera.y) + (D3DX_PI * 2));
			}
		}
		else if (g_camera.rotCameraGoal.y - g_camera.rotCamera.y < -D3DX_PI)
		{
			g_camera.rotCamera.y += ((g_camera.rotCameraGoal.y - g_camera.rotCamera.y) + (D3DX_PI * 2)) * ROTATE_VAL;

			if (g_camera.rotCamera.y > D3DX_PI)
			{
				g_camera.rotCamera.y = (g_camera.rotCamera.y) - (D3DX_PI * 2);
			}
			else if (g_camera.rotCamera.y < -D3DX_PI)
			{
				g_camera.rotCamera.y = (g_camera.rotCamera.y) + (D3DX_PI * 2);
			}
		}
		else
		{
			g_camera.rotCamera.y += (g_camera.rotCameraGoal.y - g_camera.rotCamera.y) * ROTATE_VAL;
		}
	}
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
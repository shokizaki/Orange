//===============================================
//
//  ���e   �F �f�o�b�O�\������
//  File   �F debugproc.h
//  ����� �F �؍�@��
//
//===============================================

//===============================================
//
//  �C���N���[�h�t�@�C��
//
//===============================================
#include "debugproc.h"
#include "input.h"

//===============================================
//
//  �O���[�o���ϐ�
//
//===============================================
LPD3DXFONT g_pD3DFont = NULL;	// 
char g_aStrDebug[1024];

void InitDebugProc(void)
{
	// ���[�J���ϐ�
	//------------------------------------
	LPDIRECT3DDEVICE9 pDevice;						// �f�o�C�X�I�u�W�F�N�g

	pDevice = GetDevice();

	// �t�H���g�̐ݒ�
	//------------------------------------
	D3DXCreateFont(pDevice,
				   18,
				   0,
				   0,
				   0,
				   FALSE,
				   SHIFTJIS_CHARSET,
				   OUT_DEFAULT_PRECIS,
				   DEFAULT_QUALITY,
				   DEFAULT_PITCH,
				   "terminal",
				   &g_pD3DFont);

	// �[���Œ��g��������
	memset(&g_aStrDebug[0], 0, sizeof g_aStrDebug);
}

void UninitDebugProc(void)
{
	if (g_pD3DFont != NULL)
	{
		g_pD3DFont -> Release();			// �t�H���g�̉��
		g_pD3DFont = NULL;					// �t�H���g�̉��
	}
}

void UpdateDebugProc(void)
{

}

void DrawDebugProc(void)
{
	static bool bFlag = true;

	RECT rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

	if ( GetKeyboardTrigger( DIK_F1 ) && bFlag == false )
	{
		bFlag = true;
	}
	else if ( GetKeyboardTrigger( DIK_F1 ) && bFlag == true )
	{
		bFlag = false;
	}

	if ( bFlag == false )
	{
		// �e�L�X�g�̕`��
		//------------------------------------
		g_pD3DFont ->DrawTextA(NULL, &g_aStrDebug[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255,255,255,255));
	}

	// �[���Œ��g��������
	memset(&g_aStrDebug[0], 0, sizeof(g_aStrDebug));
}

void PrintDebugProc(char *fmt, ...)
{
	va_list list;	
	char buff[256];

	//cp = (char*)fmt;

	// �ψ����ɃA�N�Z�X�J�n
	va_start(list, fmt);

	// �ψ����̔��菈��
	while (*fmt != '\0')
	{
		switch (*fmt)
		{
			// �����������ꍇ
		case '%':
			fmt++;		// �|�C���^�̈ړ�

			switch (*fmt)
			{
				// ������\��
			case 's':
				strcat_s(g_aStrDebug, va_arg(list, char*));	// ������̘A��

				break;

				// �����\��
			case 'd':
				itoa(va_arg(list, int), buff, 10);	// int�^��10�i���̏�Ԃ�char�^�ɕϊ�
				strcat_s(g_aStrDebug, buff);			// ������̘A��

				// �[���Œ��g��������
				memset(&buff[0], 0, sizeof(buff));
				break;

				// �����_�\��
			case 'f':
				sprintf_s(buff, "%.2f", va_arg(list, double));		// double�^��char�^�ɕϊ�
				strcat_s(g_aStrDebug, buff);							// ������A��

				// �[���Œ��g��������
				memset(&buff[0], 0, sizeof(buff));
				break;

			default:
				//strcat(g_aStrDebug, '\0');			// ������̘A��

				break;
			}

			fmt++;		// �|�C���^�̈ړ�
			break;

			// ����ȊO�̏ꍇ
		default:

			// ������̎w�萔�A��
			strncat_s(g_aStrDebug, fmt, 1);
			
			fmt++;		// �|�C���^�̈ړ�
			break;
		}
	}

	// �ψ����ɃA�N�Z�X������̏I������
	va_end( list );
}
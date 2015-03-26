//===================================================================
//  ���e�F�����j���O�}��
//  Fail:main.cpp
//  ����ҁF�؍�@��
//===================================================================

//===================================================================
//  �C���N���[�h�t�@�C��
//===================================================================
#include "main.h"
#include "field.h"
#include "input.h"
#include "model.h"
#include "camera.h"
#include "light.h"
#include "meshField.h"
#include "shadow.h"
#include "billboard.h"
#include "wall.h"
#include "title.h"
#include "game.h"
#include "result.h"
#include "tutorial.h"
#include "number.h"
#include "ranking.h"

//===================================================================
//�@�}�N����`
//===================================================================
#define CLASS_NAME  "DirectX"				// �E�C���h�E�N���X�̖��O
#define WINDOW_NAME "�����O���_�b�V��"		// �E�C���h�E�̖��O

//===================================================================
//  �֐��v���g�^�C�v�錾
//===================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);	// �E�C���h�E�v���V�[�W��

HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);	// ����������
void Uninit();		// �I������
void Update();		// �X�V����
void Draw();		// �`�揈��

void DrawFPS(void);	// FPS�`�揈��

//===================================================================
//  �O���[�o���ϐ�
//===================================================================
LPDIRECT3D9 g_pD3D = NULL;					// �_�C���N�g�R�c�I�u�W�F�N�g
LPDIRECT3DDEVICE9  g_pD3DDevice = NULL;		// �f�o�C�X�I�u�W�F�N�g
//LPD3DXFONT g_pD3DFont = NULL;				// 
int g_nCountFPS = 0;
bool bFlag = true;

//===================================================================
//  ���C���֐�
//===================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// ���[�J���ϐ�
	//------------------------------------
	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),				// ���̑傫��
		CS_CLASSDC,						// �\������E�C���h�E�̃X�^�C����ݒ�
		WndProc,						// �E�C���h�E�v���V�[�W���̃A�h���X�i�֐����j��ݒ�
		0,								// �g�p���Ȃ�
		0,								// �g�p���Ȃ�
		hInstance,						// ���C���֐��̈����̃C���X�^���X�n���h��
		NULL,							// �g�p����A�C�R����ݒ�
		LoadCursor(NULL, IDC_ARROW),	// �}�E�X�J�[�\�����w��
		(HBRUSH)(COLOR_WINDOW + 1),		// �N���C�A���g�̈�̔w�i�F���w��i���j
		NULL,							// �E�C���h�E�ɕt���郁�j���[��ݒ�
		CLASS_NAME,						// �E�C���h�E�N���X�̖��O�i�d�v�I�j
		NULL							// �g�p����X���[���A�C�R�����w��
	};

	HWND hWnd;		// �\������E�C���h�E�̃n���h��
	MSG msg;		// ���b�Z�[�W

	// �E�C���h�E�N���X�̓o�^
	//------------------------------------
	RegisterClassEx(&wcex);

	// �E�C���h�E�̐���
	//------------------------------------
	hWnd = CreateWindowEx(
		0,
		CLASS_NAME,
		WINDOW_NAME,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL);

	// ����������
	//------------------------------------
	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{
		return -1;
	}

	// ����\��ݒ�
	//------------------------------------
	timeBeginPeriod(1);

	// ���݂̎������擾
	//------------------------------------
	dwExecLastTime =
	dwFPSLastTime = timeGetTime();

	// ������
	//------------------------------------
	dwCurrentTime =
	dwFrameCount = 0;

	// �E�C���h�E�̕\��
	//------------------------------------
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// ���C�����[�v
	//------------------------------------
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{ // window�v���O����
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				// ���b�Z�\�W�̖|��Ƒ��o
				//------------------------------------
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{ // DirectX�v���O����

			// ���݂̎������擾
			//------------------------------------
			dwCurrentTime = timeGetTime();

			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{ 
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);	// FPS���v��
				dwFPSLastTime = dwCurrentTime;
				dwFrameCount= 0;
			}

			// ��60�t���[�����v��������
			//------------------------------------
			if ((dwCurrentTime - dwExecLastTime) >= (1000/60))
			{ 
				dwExecLastTime = dwCurrentTime;

				// �X�V����
				//------------------------------------
				Update();

				// �`�揈��
				//------------------------------------
				Draw();

				dwFrameCount++;
			}
		}
	}

	// �E�C���h�E�N���X�̓o�^����
	//------------------------------------
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// �I������
	//------------------------------------
	Uninit();

	// ����\��߂�
	//------------------------------------
	timeEndPeriod(1);

	return (int)msg.wParam;
}

//===================================================================
//  �E�C���h�E�v���V�[�W��
//===================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// �ϐ��̐錾
	//------------------------------------
	const RECT rect = {0,0,600,600};

	switch (uMsg)
	{
		// �Ώۂ̃E�C���h�E�Ƀt�H�[�J�X�����킹��
	case WM_LBUTTONDOWN:
		SetFocus(hWnd);
		break;

		// �E�C���h�E���~�{�^���i����{�^���j�ŕ���Ƃ�
	case WM_CLOSE:
		//nID = MessageBox(NULL, "�I�����܂����H", "�I���R�}���h", MB_YESNO);		// �E�C���h�E����邩�𕷂�
		//if (nID == IDYES)
		{
			// �u�͂��v���������ꍇ
			//------------------------------------
			DestroyWindow(hWnd);		// �E�C���h�E�����֐�
			PostQuitMessage(0);			// WM_QUIT���b�Z�[�W��Ԃ�
		}
		return 0;

		// �E�C���h�E�����P�[�X
	case WM_DESTROY:
		PostQuitMessage(0);			// WM_QUIT���b�Z�[�W��Ԃ�
		break;
		
		// �����̃L�[��������Ă�����
	case WM_KEYDOWN:				
		switch (wParam)
		{
			// �G�X�P�[�v�������ꂽ��
		case VK_ESCAPE:
			//nID = MessageBox(NULL, "�I�����܂����H", "�I���R�}���h", MB_YESNO);		// �E�C���h�E����邩�𕷂�
			//if (nID == IDYES)
			{
				// �u�͂��v���������ꍇ
				//------------------------------------
				DestroyWindow( hWnd );		// �E�C���h�E�����֐�
				PostQuitMessage( 0 );			// WM_QUIT���b�Z�[�W��Ԃ�
				//break;
			}
			break;
		}
		break;

		// ���������ɔ�����
	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

// ����������
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	// ���[�J���ϐ�
	//------------------------------------
	D3DDISPLAYMODE d3ddm;
	D3DPRESENT_PARAMETERS d3dpp;

	// Direct3D�I�u�W�F�N�g�̐���
	//------------------------------------
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	// ���݂̃f�B�X�v���C���[�h���擾
	//------------------------------------
	if (FAILED (g_pD3D ->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)) )
	{
		return E_FAIL;
	}

	// ���g��������
	//------------------------------------
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	//int nID = MessageBox(NULL, "�t���X�N���[���ŋN�����܂����H", "�t���X�N���[��", MB_YESNO);		// �E�C���h�E����邩�𕷂�
	//if (nID == IDYES)
	//{
	//	// �t���X�N���[��
	//	bWindow = false;
	//}

	// �v���[���e�[�V�����p�����[�^�̐ݒ�
	//------------------------------------
	d3dpp.BackBufferWidth = SCREEN_WIDTH;			// �Q�[����ʂ̕�
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;			// �Q�[����ʂ̍���
	d3dpp.BackBufferFormat = d3ddm.Format;			// �J���[���[�h
	d3dpp.BackBufferCount = 1;						// �o�b�N�o�b�t�@�̐�
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;		// 
	d3dpp.Windowed = bWindow;						// �E�C���h�E���[�h /�@�t���X�N���[�����[�h
	d3dpp.EnableAutoDepthStencil = TRUE;			// �f�v�X�o�b�t�@�A�X�e���V���o�b�t�@�̍쐬
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;		// 16bit�̃f�v�X�o�b�t�@���g�p
	if (bWindow == TRUE)
	{
		d3dpp.FullScreen_RefreshRateInHz = 0;
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	}
	else
	{
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	}

	// �f�o�C�X�I�u�W�F�N�g�̐���
	//------------------------------------
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,				// �`��ƒ��_�������n�[�h�E�F�A�ōs��
							 D3DDEVTYPE_HAL,
							 hWnd,
							 D3DCREATE_HARDWARE_VERTEXPROCESSING,
							 &d3dpp,
							 &g_pD3DDevice)))
	{
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,			// �`����n�[�h�E�F�A�A���_������CPU�ōs��
							 D3DDEVTYPE_HAL,
							 hWnd,
							 D3DCREATE_SOFTWARE_VERTEXPROCESSING,
							 &d3dpp,
							 &g_pD3DDevice)))
		{
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,		// �`��ƒ��_������CPU�ōs��
								 D3DDEVTYPE_REF,
								 hWnd,
								 D3DCREATE_SOFTWARE_VERTEXPROCESSING,
								 &d3dpp,
								 &g_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

	// �����_�[�X�e�[�g�̐ݒ�
	//------------------------------------
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// �J�����O�̐ݒ�
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// ���u�����h���s��
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// �����x�̊���
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// �����x�̊���

	// �T���v���[�X�e�[�g�̐ݒ�
	//------------------------------------
	g_pD3DDevice ->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	g_pD3DDevice ->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	g_pD3DDevice ->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice ->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	// �e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
	//------------------------------------
	g_pD3DDevice ->SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);
	g_pD3DDevice ->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice ->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	//// �t�H���g�̐ݒ�
	////------------------------------------
	//D3DXCreateFont(g_pD3DDevice,
	//			   18,
	//			   0,
	//			   0,
	//			   0,
	//			   FALSE,
	//			   SHIFTJIS_CHARSET,
	//			   OUT_DEFAULT_PRECIS,
	//			   DEFAULT_QUALITY,
	//			   DEFAULT_PITCH,
	//			   "terminal",
	//			   &g_pD3DFont);

	// �T�E���h������
	//------------------------------------
	if ( InitSound( hWnd )/* || InitSound( hWnd ) == E_FAIL || InitSound( hWnd ) == S_FALSE*/ )
	{
		DestroyWindow( hWnd );		// �E�C���h�E�����֐�

		return E_FAIL;
	}

	// ���͏�������������
	//------------------------------------
	InitKeyboard( hInstance, hWnd );

	InitGamePad(  hInstance, hWnd  );

	// �J�����̏�����
	//------------------------------------
	InitCamera();

	// ���C�g�̏�����
	//------------------------------------
	InitLight();

	// �t�F�[�h�̏�����
	//------------------------------------
	InitFade();

	// �����`��Ɋւ��鏉��������
	//------------------------------------
	InitNumber();

	// �f�o�b�O����������
	//------------------------------------
	InitDebugProc();

	// ���[�h�J��
	//------------------------------------
	ChangeScene();

	return S_OK;
}

// �I������
void Uninit()
{
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice -> Release();		// �f�o�C�X�̉��
		g_pD3DDevice = NULL;			// �f�o�C�X������
	}
	if (g_pD3D != NULL)
	{
		g_pD3D -> Release();			// Direct3D�I�u�W�F�N�g�̉��
		g_pD3D = NULL;					// Direct3D�I�u�W�F�N�g�̉��
	}

	//if (g_pD3DFont != NULL)
	//{
	//	g_pD3DFont -> Release();			// �t�H���g�̉��
	//	g_pD3DFont = NULL;					// �t�H���g�̉��
	//}

	// ���b�V���t�B�[���h�I������
	//------------------------------------
	UninitMeshField();

	// ���f���I������
	//------------------------------------
	UninitModel();

	// �J�����I������
	//------------------------------------
	UninitCamera();

	// ���͏I������
	//------------------------------------
	UninitKeyboard();

	UninitGamePad();

	// �e�I������
	//------------------------------------
	UninitShadow();

	// �r���{�[�h�I������
	//------------------------------------
	UninitBillboard();

	// �ǏI������
	//------------------------------------
	UninitWall();

	// �t�F�[�h�I��
	//------------------------------------
	UninitFade();

	UninitNumberTexture();

	UninitDebugProc();
}

void Update()
{
	switch ( GetMode() )
	{
	case MODE_TITLE:
		UpdateTitle();

		break;

	case MODE_TUTORIAL:
		UpdateTutorial();

		break;

	case MODE_GAME:
		UpdateGame();

		break;

	case MODE_RANKING:
		UpdateRanking();

		break;

	case MODE_RESULT:
		UpdateResult();

		break;
	}

	// �t�F�[�h�̍X�V
	//------------------------------------
	UpdateFade();

	// ���͏����̍X�V
	//------------------------------------
	UpdateKeyboard();

	UpdateGamePad();

	// �f�o�b�O�`��X�V����
	//------------------------------------
	UpdateDebugProc();
}

void Draw()
{
	// ��ʂ̃N���A
	//------------------------------------
	g_pD3DDevice -> Clear(0,
						 NULL,
						 (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
						 D3DCOLOR_RGBA(0,0,0,0),
						 1.0f,
						 0);

	// �`��̊J�n
	//------------------------------------
	if(SUCCEEDED(g_pD3DDevice -> BeginScene()))
	{ // �`�悵�������̂������ɏ���
		
		switch ( GetMode() )
		{
		case MODE_TITLE:
			DrawTitle();

			break;

		case MODE_TUTORIAL:
			DrawTutorial();

			break;

		case MODE_GAME:
			DrawGame();

			break;

		case MODE_RANKING:
			DrawRanking();

			break;

		case MODE_RESULT:
			DrawResult();

			break;
		}

		// FPS�\���֐�
		//------------------------------------
//#ifdef _DEBUG

		// �f�o�b�O���̕`��
		//------------------------------------
		DrawDebugProc();
//#endif

		// �t�F�[�h�̕`��
		//------------------------------------
		DrawFade();

		// �`��̏I��
		//------------------------------------
		g_pD3DDevice -> EndScene();		
	}

	// �o�b�N�o�b�t�@���t�����g�o�b�t�@�Ɠ���ւ���
	//----------------------------------------------
	g_pD3DDevice -> Present(NULL, NULL, NULL, NULL);
}

//===================================================================
// �f�o�C�X�̎擾�֐�
//===================================================================
LPDIRECT3DDEVICE9 GetDevice()
{
	return g_pD3DDevice;
}


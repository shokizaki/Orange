//===================================================================
//  内容：ランニングマン
//  Fail:main.cpp
//  制作者：木崎　翔
//===================================================================

//===================================================================
//  インクルードファイル
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
//　マクロ定義
//===================================================================
#define CLASS_NAME  "DirectX"				// ウインドウクラスの名前
#define WINDOW_NAME "リングルダッシュ"		// ウインドウの名前

//===================================================================
//  関数プロトタイプ宣言
//===================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);	// ウインドウプロシージャ

HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);	// 初期化処理
void Uninit();		// 終了処理
void Update();		// 更新処理
void Draw();		// 描画処理

void DrawFPS(void);	// FPS描画処理

//===================================================================
//  グローバル変数
//===================================================================
LPDIRECT3D9 g_pD3D = NULL;					// ダイレクト３Ｄオブジェクト
LPDIRECT3DDEVICE9  g_pD3DDevice = NULL;		// デバイスオブジェクト
//LPD3DXFONT g_pD3DFont = NULL;				// 
int g_nCountFPS = 0;
bool bFlag = true;

//===================================================================
//  メイン関数
//===================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// ローカル変数
	//------------------------------------
	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),				// 情報の大きさ
		CS_CLASSDC,						// 表示するウインドウのスタイルを設定
		WndProc,						// ウインドウプロシージャのアドレス（関数名）を設定
		0,								// 使用しない
		0,								// 使用しない
		hInstance,						// メイン関数の引数のインスタンスハンドル
		NULL,							// 使用するアイコンを設定
		LoadCursor(NULL, IDC_ARROW),	// マウスカーソルを指定
		(HBRUSH)(COLOR_WINDOW + 1),		// クライアント領域の背景色を指定（白）
		NULL,							// ウインドウに付けるメニューを設定
		CLASS_NAME,						// ウインドウクラスの名前（重要！）
		NULL							// 使用するスモールアイコンを指定
	};

	HWND hWnd;		// 表示するウインドウのハンドル
	MSG msg;		// メッセージ

	// ウインドウクラスの登録
	//------------------------------------
	RegisterClassEx(&wcex);

	// ウインドウの生成
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

	// 初期化処理
	//------------------------------------
	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{
		return -1;
	}

	// 分解能を設定
	//------------------------------------
	timeBeginPeriod(1);

	// 現在の時刻を取得
	//------------------------------------
	dwExecLastTime =
	dwFPSLastTime = timeGetTime();

	// 初期化
	//------------------------------------
	dwCurrentTime =
	dwFrameCount = 0;

	// ウインドウの表示
	//------------------------------------
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// メインループ
	//------------------------------------
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{ // windowプログラム
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				// メッセ―ジの翻訳と送出
				//------------------------------------
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{ // DirectXプログラム

			// 現在の時刻を取得
			//------------------------------------
			dwCurrentTime = timeGetTime();

			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{ 
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);	// FPSを計測
				dwFPSLastTime = dwCurrentTime;
				dwFrameCount= 0;
			}

			// 約60フレームを計測したら
			//------------------------------------
			if ((dwCurrentTime - dwExecLastTime) >= (1000/60))
			{ 
				dwExecLastTime = dwCurrentTime;

				// 更新処理
				//------------------------------------
				Update();

				// 描画処理
				//------------------------------------
				Draw();

				dwFrameCount++;
			}
		}
	}

	// ウインドウクラスの登録解除
	//------------------------------------
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// 終了処理
	//------------------------------------
	Uninit();

	// 分解能を戻す
	//------------------------------------
	timeEndPeriod(1);

	return (int)msg.wParam;
}

//===================================================================
//  ウインドウプロシージャ
//===================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// 変数の宣言
	//------------------------------------
	const RECT rect = {0,0,600,600};

	switch (uMsg)
	{
		// 対象のウインドウにフォーカスを合わせる
	case WM_LBUTTONDOWN:
		SetFocus(hWnd);
		break;

		// ウインドウを×ボタン（閉じるボタン）で閉じるとき
	case WM_CLOSE:
		//nID = MessageBox(NULL, "終了しますか？", "終了コマンド", MB_YESNO);		// ウインドウを閉じるかを聞く
		//if (nID == IDYES)
		{
			// 「はい」を押した場合
			//------------------------------------
			DestroyWindow(hWnd);		// ウインドウを閉じる関数
			PostQuitMessage(0);			// WM_QUITメッセージを返す
		}
		return 0;

		// ウインドウを閉じるケース
	case WM_DESTROY:
		PostQuitMessage(0);			// WM_QUITメッセージを返す
		break;
		
		// 何かのキーが押されていたら
	case WM_KEYDOWN:				
		switch (wParam)
		{
			// エスケープが押されたら
		case VK_ESCAPE:
			//nID = MessageBox(NULL, "終了しますか？", "終了コマンド", MB_YESNO);		// ウインドウを閉じるかを聞く
			//if (nID == IDYES)
			{
				// 「はい」を押した場合
				//------------------------------------
				DestroyWindow( hWnd );		// ウインドウを閉じる関数
				PostQuitMessage( 0 );			// WM_QUITメッセージを返す
				//break;
			}
			break;
		}
		break;

		// 何もせずに抜ける
	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

// 初期化処理
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	// ローカル変数
	//------------------------------------
	D3DDISPLAYMODE d3ddm;
	D3DPRESENT_PARAMETERS d3dpp;

	// Direct3Dオブジェクトの生成
	//------------------------------------
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
	//------------------------------------
	if (FAILED (g_pD3D ->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)) )
	{
		return E_FAIL;
	}

	// 中身を初期化
	//------------------------------------
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	//int nID = MessageBox(NULL, "フルスクリーンで起動しますか？", "フルスクリーン", MB_YESNO);		// ウインドウを閉じるかを聞く
	//if (nID == IDYES)
	//{
	//	// フルスクリーン
	//	bWindow = false;
	//}

	// プレゼンテーションパラメータの設定
	//------------------------------------
	d3dpp.BackBufferWidth = SCREEN_WIDTH;			// ゲーム画面の幅
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;			// ゲーム画面の高さ
	d3dpp.BackBufferFormat = d3ddm.Format;			// カラーモード
	d3dpp.BackBufferCount = 1;						// バックバッファの数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;		// 
	d3dpp.Windowed = bWindow;						// ウインドウモード /　フルスクリーンモード
	d3dpp.EnableAutoDepthStencil = TRUE;			// デプスバッファ、ステンシルバッファの作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;		// 16bitのデプスバッファを使用
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

	// デバイスオブジェクトの生成
	//------------------------------------
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,				// 描画と頂点処理をハードウェアで行う
							 D3DDEVTYPE_HAL,
							 hWnd,
							 D3DCREATE_HARDWARE_VERTEXPROCESSING,
							 &d3dpp,
							 &g_pD3DDevice)))
	{
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,			// 描画をハードウェア、頂点処理をCPUで行う
							 D3DDEVTYPE_HAL,
							 hWnd,
							 D3DCREATE_SOFTWARE_VERTEXPROCESSING,
							 &d3dpp,
							 &g_pD3DDevice)))
		{
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,		// 描画と頂点処理をCPUで行う
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

	// レンダーステートの設定
	//------------------------------------
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// カリングの設定
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// αブレンドを行う
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// 透明度の割合
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// 透明度の割合

	// サンプラーステートの設定
	//------------------------------------
	g_pD3DDevice ->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	g_pD3DDevice ->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	g_pD3DDevice ->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice ->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	// テクスチャステージステートの設定
	//------------------------------------
	g_pD3DDevice ->SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);
	g_pD3DDevice ->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice ->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	//// フォントの設定
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

	// サウンド初期化
	//------------------------------------
	if ( InitSound( hWnd )/* || InitSound( hWnd ) == E_FAIL || InitSound( hWnd ) == S_FALSE*/ )
	{
		DestroyWindow( hWnd );		// ウインドウを閉じる関数

		return E_FAIL;
	}

	// 入力処理初期化処理
	//------------------------------------
	InitKeyboard( hInstance, hWnd );

	InitGamePad(  hInstance, hWnd  );

	// カメラの初期化
	//------------------------------------
	InitCamera();

	// ライトの初期化
	//------------------------------------
	InitLight();

	// フェードの初期化
	//------------------------------------
	InitFade();

	// 数字描画に関する初期化処理
	//------------------------------------
	InitNumber();

	// デバッグ初期化処理
	//------------------------------------
	InitDebugProc();

	// モード遷移
	//------------------------------------
	ChangeScene();

	return S_OK;
}

// 終了処理
void Uninit()
{
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice -> Release();		// デバイスの解放
		g_pD3DDevice = NULL;			// デバイス初期化
	}
	if (g_pD3D != NULL)
	{
		g_pD3D -> Release();			// Direct3Dオブジェクトの解放
		g_pD3D = NULL;					// Direct3Dオブジェクトの解放
	}

	//if (g_pD3DFont != NULL)
	//{
	//	g_pD3DFont -> Release();			// フォントの解放
	//	g_pD3DFont = NULL;					// フォントの解放
	//}

	// メッシュフィールド終了処理
	//------------------------------------
	UninitMeshField();

	// モデル終了処理
	//------------------------------------
	UninitModel();

	// カメラ終了処理
	//------------------------------------
	UninitCamera();

	// 入力終了処理
	//------------------------------------
	UninitKeyboard();

	UninitGamePad();

	// 影終了処理
	//------------------------------------
	UninitShadow();

	// ビルボード終了処理
	//------------------------------------
	UninitBillboard();

	// 壁終了処理
	//------------------------------------
	UninitWall();

	// フェード終了
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

	// フェードの更新
	//------------------------------------
	UpdateFade();

	// 入力処理の更新
	//------------------------------------
	UpdateKeyboard();

	UpdateGamePad();

	// デバッグ描画更新処理
	//------------------------------------
	UpdateDebugProc();
}

void Draw()
{
	// 画面のクリア
	//------------------------------------
	g_pD3DDevice -> Clear(0,
						 NULL,
						 (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
						 D3DCOLOR_RGBA(0,0,0,0),
						 1.0f,
						 0);

	// 描画の開始
	//------------------------------------
	if(SUCCEEDED(g_pD3DDevice -> BeginScene()))
	{ // 描画したいものをここに書く
		
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

		// FPS表示関数
		//------------------------------------
//#ifdef _DEBUG

		// デバッグ情報の描画
		//------------------------------------
		DrawDebugProc();
//#endif

		// フェードの描画
		//------------------------------------
		DrawFade();

		// 描画の終了
		//------------------------------------
		g_pD3DDevice -> EndScene();		
	}

	// バックバッファをフロントバッファと入れ替える
	//----------------------------------------------
	g_pD3DDevice -> Present(NULL, NULL, NULL, NULL);
}

//===================================================================
// デバイスの取得関数
//===================================================================
LPDIRECT3DDEVICE9 GetDevice()
{
	return g_pD3DDevice;
}


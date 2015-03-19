//===============================================
//
//  内容   ： デバッグ表示処理
//  File   ： debugproc.h
//  制作者 ： 木崎　翔
//
//===============================================

//===============================================
//
//  インクルードファイル
//
//===============================================
#include "debugproc.h"
#include "input.h"

//===============================================
//
//  グローバル変数
//
//===============================================
LPD3DXFONT g_pD3DFont = NULL;	// 
char g_aStrDebug[1024];

void InitDebugProc(void)
{
	// ローカル変数
	//------------------------------------
	LPDIRECT3DDEVICE9 pDevice;						// デバイスオブジェクト

	pDevice = GetDevice();

	// フォントの設定
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

	// ゼロで中身を初期化
	memset(&g_aStrDebug[0], 0, sizeof g_aStrDebug);
}

void UninitDebugProc(void)
{
	if (g_pD3DFont != NULL)
	{
		g_pD3DFont -> Release();			// フォントの解放
		g_pD3DFont = NULL;					// フォントの解放
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
		// テキストの描画
		//------------------------------------
		g_pD3DFont ->DrawTextA(NULL, &g_aStrDebug[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255,255,255,255));
	}

	// ゼロで中身を初期化
	memset(&g_aStrDebug[0], 0, sizeof(g_aStrDebug));
}

void PrintDebugProc(char *fmt, ...)
{
	va_list list;	
	char buff[256];

	//cp = (char*)fmt;

	// 可変引数にアクセス開始
	va_start(list, fmt);

	// 可変引数の判定処理
	while (*fmt != '\0')
	{
		switch (*fmt)
		{
			// ％を見つけた場合
		case '%':
			fmt++;		// ポインタの移動

			switch (*fmt)
			{
				// 文字列表示
			case 's':
				strcat_s(g_aStrDebug, va_arg(list, char*));	// 文字列の連結

				break;

				// 整数表示
			case 'd':
				itoa(va_arg(list, int), buff, 10);	// int型を10進数の状態でchar型に変換
				strcat_s(g_aStrDebug, buff);			// 文字列の連結

				// ゼロで中身を初期化
				memset(&buff[0], 0, sizeof(buff));
				break;

				// 小数点表示
			case 'f':
				sprintf_s(buff, "%.2f", va_arg(list, double));		// double型をchar型に変換
				strcat_s(g_aStrDebug, buff);							// 文字列連結

				// ゼロで中身を初期化
				memset(&buff[0], 0, sizeof(buff));
				break;

			default:
				//strcat(g_aStrDebug, '\0');			// 文字列の連結

				break;
			}

			fmt++;		// ポインタの移動
			break;

			// それ以外の場合
		default:

			// 文字列の指定数連結
			strncat_s(g_aStrDebug, fmt, 1);
			
			fmt++;		// ポインタの移動
			break;
		}
	}

	// 可変引数にアクセスした後の終了処理
	va_end( list );
}
/*=======================================================================================================
概　略：ウィンドウの管理（エントリーポイント）
作成日：2015.05.29
更新日：2015.10.15
制作者：藤原順基
=======================================================================================================*/

//---------------インクルード-----------------------
#include "Global.h"
#include <time.h>
#include "Game.h"
#include "Sprite.h"

//-------------------定数宣言------------------
GLOBAL global;

//---------------メモリリーク検出---------------
#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif



//---------------プロトタイプ宣言---------------
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
ATOM InitApp(HINSTANCE);
BOOL InitInstance(HINSTANCE, int);


//----------------グローバル変数----------------
TCHAR szClassName[] = "OriginalGame";   //ウィンドウクラス名
Game  *game;							//ゲームオブジェクト

//ウィンドウハンドル
HWND hWnd;

BOOL isEnd = FALSE;

/////////////////////////////////////////////////////////////////////////////////////////////////////

//――――――――――――――――――――――
//エントリーポイント
//――――――――――――――――――――――
int WINAPI WinMain(HINSTANCE hCurInst, HINSTANCE hPrevInst, LPSTR lpsCmdLine, int nCmdShow)
{

#if _DEBUG
	_CrtDumpMemoryLeaks();
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	MSG msg;

	//ウィンドウクラスの登録
	if (!InitApp(hCurInst))
		return FALSE;

	//ウィンドウ生成
	if (!InitInstance(hCurInst, nCmdShow))
		return FALSE;

	//ランダムの準備
	srand((unsigned)time(NULL));

	//ゲームオブジェクトを作成
	game = new Game;

	//Direct3Dの初期化
	if (FAILED(game->InitD3d(hWnd)))
	{
		return FALSE;
	}

	//入力処理の初期化
	g_pInput = new Input();
	g_pInput->Init(hWnd);

	//読み込み処理
	if (FAILED(game->Load()))
	{
		return FALSE;
	}

	// メッセージを取得
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			if (isEnd)
			{
				break;
			}
			//ゲームの更新
			if (FAILED(game->Update()))
			{
				return FALSE;
			}

			//衝突判定
			if (FAILED(game->Hit()))
			{
				return FALSE;
			}

			//ゲーム画面の描画
			if (FAILED(game->Render()))
			{
				return FALSE;
			}
		}
	}
	return (int)msg.wParam;
}

//――――――――――――――――――――――
// ウィンドウクラスの登録
//――――――――――――――――――――――
ATOM InitApp(HINSTANCE hInst)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);           // 構造体のサイズ
	wc.style = CS_HREDRAW | CS_VREDRAW;       // クラスのスタイル
	wc.lpfnWndProc = WndProc;                 // プロシージャ名
	wc.cbClsExtra = 0;                        // 補助メモリ
	wc.cbWndExtra = 0;                        // 補助メモリ
	wc.hInstance = hInst;                     // インスタンス
	wc.hIcon = (HICON)LoadImage(              // アイコン
		NULL, MAKEINTRESOURCE(IDI_APPLICATION),
		IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
	wc.hCursor = (HCURSOR)LoadImage(          // カーソル
		NULL, MAKEINTRESOURCE(IDC_ARROW),
		IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
	wc.hbrBackground =                        // 背景ブラシ
		(HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;                   // メニュー名
	wc.lpszClassName = szClassName;           // クラス名
	wc.hIconSm = (HICON)LoadImage(            // 小さいアイコン
		NULL, MAKEINTRESOURCE(IDI_APPLICATION),
		IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED);

	return (RegisterClassEx(&wc));
}

//――――――――――――――――――――――
// ウィンドウの生成
//――――――――――――――――――――――
BOOL InitInstance(HINSTANCE hInst, int nCmdShow)
{

	//クライアント領域サイズから、ウィンドウサイズを計算
	RECT r = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
	AdjustWindowRect(&r, WS_OVERLAPPEDWINDOW, FALSE);

	//ウィンドウの作成
	hWnd = CreateWindow(
		szClassName,			// クラス名
		szClassName,			// ウィンドウ名
		WS_OVERLAPPEDWINDOW,	// ウィンドウスタイル
		CW_USEDEFAULT,			// x位置
		CW_USEDEFAULT,			// y位置
		r.right - r.left,		// ウィンドウ幅
		r.bottom - r.top,		// ウィンドウ高さ
		NULL,  // 親ウィンドウのハンドル、親を作るときはNULL
		NULL,  // メニューハンドル、クラスメニューを使うときはNULL
		hInst, // インスタンスハンドル
		NULL   // ウィンドウ作成データ
		);

	//ウィンドウの作成に失敗した場合
	if (!hWnd)
	{
		return FALSE;
	}

	// ウィンドウの表示状態を設定
	ShowWindow(hWnd, nCmdShow);

	// ウィンドウを更新
	UpdateWindow(hWnd);

	return TRUE;
}

//――――――――――――――――――――――
// ウィンドウプロシージャ
//――――――――――――――――――――――
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	
	switch (msg)
	{
		
	//■ウィンドウが消された
	case WM_DESTROY:
		isEnd = TRUE;

		//入力処理を開放
		SAFE_DELETE(g_pInput);

		//ゲームオブジェクトの開放
		SAFE_DELETE(game);

		//プログラム終了
		PostQuitMessage(0);
		break;
	
	//■その他
	default:
		return (DefWindowProc(hWnd, msg, wp, lp));
	}
	return 0;
}

//おおよそデストロイは最後に呼ばれるだけなのでゲームの速度に支障はない、はず。
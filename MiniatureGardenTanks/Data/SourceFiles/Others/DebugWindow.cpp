#include "../Others/DebugWindow.h"

const std::string DebugWindow::CLASS_NAME  = "DebugInfo";		// ウィンドウクラスの名前

/*-------------------------------------------*/
/* コンストラクタ
/*-------------------------------------------*/
DebugWindow::DebugWindow()
{
	hWindow = NULL;
}

/*-------------------------------------------*/
/* デストラクタ
/*-------------------------------------------*/
DebugWindow::~DebugWindow()
{
	// 処理なし
}

/*-------------------------------------------*/
/* メッセージプロシージャ
/*-------------------------------------------*/
LRESULT DebugWindow::MessageProcedure(HWND hWindow, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
		// Windowsにこのプログラムを終了するように伝える
		case WM_DESTROY : PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWindow, iMessage, wParam, lParam);
}

/*-------------------------------------------*/
/* ウィンドウプロシージャ
/*（実際の処理はメッセージプロシージャ関数で行う）
/*-------------------------------------------*/
LRESULT CALLBACK WindowProcedure(HWND hWindow, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	return DebugWindow::GetInstance().MessageProcedure(hWindow, iMessage, wParam, lParam);
}

/*-------------------------------------------*/
/* 初期化
/*-------------------------------------------*/
HRESULT DebugWindow::Initialize(HINSTANCE hInstance)
{
	// デバッグ文のフォントサイズを設定
	SetFontSize(24);

	// ウィンドウの初期化
	WNDCLASSEX windowClass;

	windowClass.cbSize        = sizeof(windowClass);					// クラスのサイズ
	windowClass.style         = CS_HREDRAW | CS_VREDRAW;				// ウィンドウのスタイル
	windowClass.lpfnWndProc   = WindowProcedure;						// ウィンドウプロシージャ関数へのポインタ
	windowClass.cbClsExtra    = 0;										// 拡張クラスメモリ（なし）
	windowClass.cbWndExtra    = 0;										// 拡張ウィンドウメモリ（なし）
	windowClass.hInstance     = hInstance;								// インスタンスへのハンドル
	windowClass.hIcon         = NULL;									// Windowsプログラムの大きなアイコン（なし）
	windowClass.hCursor       = LoadCursor(NULL,IDC_ARROW);				// ウィンドウで使用するマウスカーソル
	windowClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	// ウィンドウの背景を塗りつぶす色
	windowClass.lpszMenuName  = NULL;									// デフォルトメニュー（なし）
	windowClass.lpszClassName = CLASS_NAME.c_str();						// ウィンドウクラスの名前
	windowClass.hIconSm       = NULL;									// Windowsプログラムの小さなアイコン（なし）

	// ウィンドウクラスを登録
	if (RegisterClassEx(&windowClass) == 0)
	{
		// エラーの場合
		MessageBox(hWindow, "ウィンドウクラスの登録に失敗しました。", "", MB_OK);
		return E_FAIL;;
	}

	// ウィンドウの作成（取得した解像度をもとに、画面の中央に表示されるように調整）
	hWindow = CreateWindow(CLASS_NAME.c_str(),		// ウィンドウクラスの名前
						   CLASS_NAME.c_str(),		// タイトルバーの表示テキスト
						   WS_OVERLAPPED,			// ウィンドウのスタイル（コントロールなしの固定サイズ）
						   0,						// ウィンドウの左上隅の座標（X軸）
						   0,						// ウィンドウの左上隅の座標（Y軸）
						   800,						// ウィンドウの幅
						   600,						// ウィンドウの高さ
						   NULL,					// 親ウィンドウ（なし）
						   NULL,					// ウィンドウメニュー（なし）
						   hInstance,				// インスタンスへのハンドル
						   NULL);					// 他のウィンドウのパラメータ（なし）

	if (hWindow == NULL)
	{
		// エラーの場合
		MessageBox(hWindow, "ウィンドウの作成に失敗しました。", "", MB_OK);
		return E_FAIL;
	}

	// ウィンドウの表示
	ShowWindow(hWindow, SW_SHOW);
	// ウィンドウプロシージャーにWM_PAINTを送る
	UpdateWindow(hWindow);

	return S_OK;
}
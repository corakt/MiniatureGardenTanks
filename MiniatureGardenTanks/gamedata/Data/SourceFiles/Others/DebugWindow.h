#pragma once
#include "../Others/Singleton.h"
#include <DxLib.h>
#include <string>

/*-------------------------------------------*/
/* デバッグ用のウィンドウクラス（シングルトン）
/*-------------------------------------------*/
class DebugWindow final : public Singleton< DebugWindow>
{
	friend class Singleton< DebugWindow>;

public:
			 DebugWindow();		// コンストラクタ
	virtual ~DebugWindow();		// デストラクタ

	HRESULT Initialize(HINSTANCE);								// 初期化
	LRESULT MessageProcedure(HWND, UINT, WPARAM, LPARAM);		// メッセージプロシージャ

	HWND hWindow;	// ウィンドウハンドル
private:
	static const std::string CLASS_NAME;	// ウィンドウクラスの名前
};

#define DEBUG_WINDOW DebugWindow::GetInstance()
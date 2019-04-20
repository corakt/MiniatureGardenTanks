#include "../Scene/SceneManager.h"
#include "../Scene/SceneBase.h"
#include "../Others/ErrorMessage.h"
#include "../Others/Define.h"
#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include <iostream>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInst, LPSTR lpszCmdLine, int nCmdShow)
{
	// DirectX9を使用する
	SetUseDirect3DVersion(DX_DIRECT3D_9);

	// ウィンドウタイトルバーの名前
	SetWindowText("箱庭戦車 - MiniatureGardenTanks");
	// 画面解像度を設定
	SetGraphMode(RESOLUTION_WIDTH,RESOLUTION_HEIGHT, 16);
	// ウィンドウサイズを設定
	SetWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	// ウィンドウモード
	//ChangeWindowMode(TRUE);

	// DXライブラリの初期化処理
	if (DxLib_Init() == -1)
	{
		// DXライブラリ初期化失敗時のエラーメッセージ
		MessageBox(GetMainWindowHandle(), "DXライブラリの初期化に失敗しました。\nプログラムを終了します。", NULL, MB_OK);
		// プログラムを終了する
		return -1;
	}

	// フルスクリーンウィンドウの切り替えでリソースが消えるのを防ぐ
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

	// Effekseerを初期化する
	if (Effkseer_Init(EFFEKSEER_EFFECT_MAXNUM) == -1)
	{
		// Effekseer初期化失敗時のエラーメッセージ
		MessageBox(GetMainWindowHandle(), "Effekseerの初期化に失敗しました。\nプログラムを終了します。", NULL, MB_OK);
		// プログラムを終了する
		return -1;
	}

	// Effekseerの2Dの表示の設定を行う
	Effekseer_Set2DSetting(WINDOW_WIDTH, WINDOW_HEIGHT);

	// 描画対象の画面を裏画面に設定
	SetDrawScreen(DX_SCREEN_BACK);

	// 3DのZバッファを使用する
	SetUseZBuffer3D(TRUE);
	// 3DのZバッファの書き込みを行う
	SetWriteZBuffer3D(TRUE);

	// シーンマネージャーのメモリを確保
	SceneManager* sceneManager = new SceneManager();

	// メインループ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == false)
	{
		// シーンの更新
		sceneManager->Update();

		// 画面をクリアする
		ClearDrawScreen();

		// シーンの描画
		sceneManager->Draw();

		// Effekseerの更新
		sceneManager->UpdateEffekseer();
		// Effekseerの描画
		sceneManager->DrawEffekseer();

		// プログラム内のエラーメッセージを表示
		ERROR_MESSAGE.Draw();

		// 裏画面の内容を表画面に反映
		ScreenFlip();

		// 画面内の"printfDx"の内容をクリア
		clsDx();

		// シーン変更の処理
		sceneManager->ChangeScene();
	}
	
	// Effekseer終了処理
	Effkseer_End();

	// DXライブラリの終了処理
	DxLib_End();

	// プログラムを終了
	return 0;
}
#include "../Scene/SceneManager.h"
#include "../Scene/SceneBase.h"
#include "../Others/ErrorMessage.h"
#include "../Others/Define.h"
#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include <iostream>

static int startTime;      //測定開始時刻
static int count;          //カウンタ
static float fps;          //fps
static const int N = 60;   //平均を取るサンプル数
static const int FPS = 60; //設定したFPS

// 待機
static void WaitFrame()
{
	int tookTime = GetNowCount() - startTime;	//かかった時間
	int waitTime = count * 1000 / FPS - tookTime;	//待つべき時間
	if (waitTime > 0) 
	{
		Sleep(waitTime);	//待機
	}
}

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
		if (count == 0) { //1フレーム目なら時刻を記憶
			startTime = GetNowCount();
		}
		if (count == N) { //60フレーム目なら平均を計算する
			int t = GetNowCount();
			fps = 1000.f / ((t - startTime) / (float)N);
			count = 0;
			startTime = t;
		}
		count++;

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
		// 60FPSになるように待機する
		WaitFrame();

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
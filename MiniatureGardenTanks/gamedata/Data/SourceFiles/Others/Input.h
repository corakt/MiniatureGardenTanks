#pragma once
#include "../Others/Singleton.h"
#include <DxLib.h>

/*-------------------------------------------*/
/* インプットクラス（シングルトン）
/*-------------------------------------------*/
class Input final : public Singleton<Input>
{
	friend class Singleton<Input>;

public:
	         Input();		// コンストラクタ
	virtual ~Input();		// デストラクタ

	void Initialize();		// 初期化
	void Update();			// 更新

	// キー関連
	bool GetKey(int keyCode);				// 押されている間
	bool GetKeyDown(int keyCode);			// 押された瞬間
	bool GetKeyUp(int keyCode);				// 離された瞬間

	// ボタン関連
	bool GetButton(UCHAR buttonCode);		// 押されている間
	bool GetButtonDown(UCHAR buttonCode);	// 押された瞬間
	bool GetButtonUp(UCHAR buttonCode);		// 離された瞬間

	// スティック関連
	float GetLeftStickX();					// 左スティック横軸
	float GetLeftStickY();					// 左スティック縦軸
	float GetRightStickX();					// 右スティック横軸
	float GetRightStickY();					// 左スティック縦軸
	
	// トリガー関連
	UCHAR GetLeftTrigger();					// 左トリガー
	UCHAR GetRightTrigger();				// 右トリガー

	// バイブレーション
	void StartVibration(int power,int time);

private:
	char*        prevKeyInput;				// 前フレームのキーの状態
	char*        currentKeyInput;			// 現在のキーの状態
	XINPUT_STATE prevXInput;				// 前フレームのパッドの状態
	XINPUT_STATE currentXInput;				// 現在のパッドの状態

	static const int KEY_BUFFER_SIZE;		// キーのバッファのサイズ
};

#define INPUT Input::GetInstance()
#include "../Others/Input.h"
#include "../Others/Define.h"

// キーのバッファのサイズ
const int Input::KEY_BUFFER_SIZE = 256;

/*-------------------------------------------*/
/* コンストラクタ
/*-------------------------------------------*/
Input::Input()
{
	
}

/*-------------------------------------------*/
/* デストラクタ
/*-------------------------------------------*/
Input::~Input()
{
	// キーのバッファーで使用したメモリを解放
	SafeDeleteArray(currentKeyInput);
	SafeDeleteArray(prevKeyInput);
}

/*-------------------------------------------*/
/* 初期化
/*-------------------------------------------*/
void Input::Initialize()
{
	// バッファの初期化
	currentKeyInput = NULL;
	prevKeyInput    = NULL;
	// キーバッファのメモリを確保
	currentKeyInput = new char[KEY_BUFFER_SIZE];
	prevKeyInput    = new char[KEY_BUFFER_SIZE];
	// 確保したメモリを初期化
	for (int i = 0; i < KEY_BUFFER_SIZE; i++)
	{
		currentKeyInput[i] = 0;
		prevKeyInput[i]    = 0;
	}

	// ジョイパッド関連の初期化
	for (int i = 0; i < 16; i++)
	{
		currentXInput.Buttons[i] = 0;
		prevXInput.Buttons[i] = 0;
	}
	currentXInput.LeftTrigger = 0;
	currentXInput.RightTrigger = 0;
	currentXInput.ThumbLX = 0;
	currentXInput.ThumbLY = 0;
	currentXInput.ThumbRX = 0;
	currentXInput.ThumbRY = 0;

	prevXInput.LeftTrigger = 0;
	prevXInput.RightTrigger = 0;
	prevXInput.ThumbLX = 0;
	prevXInput.ThumbLY = 0;
	prevXInput.ThumbRX = 0;
	prevXInput.ThumbRY = 0;

	// ジョイパッドのデッドゾーンを設定
	SetJoypadDeadZone(DX_INPUT_PAD1, 0.3);
}

/*-------------------------------------------*/
/* 更新
/*-------------------------------------------*/
void Input::Update()
{
	for (int i = 0; i < KEY_BUFFER_SIZE; i++)
	{
		// 現在のキーの状態を前フレームの状態として記録
		prevKeyInput[i] = currentKeyInput[i];
	}
	// 現在のパッドの状態を前フレームの状態として記録
	prevXInput = currentXInput;

	// 現在のキーの状態を取得
	GetHitKeyStateAll(currentKeyInput);
	// 現在のパッドの状態を取得
	GetJoypadXInputState(DX_INPUT_PAD1, &currentXInput);
}

/*-------------------------------------------*/
/* キー：押されている間
/*-------------------------------------------*/
bool Input::GetKey(int keyCode)
{
	if (prevKeyInput[keyCode])
	{
		if (currentKeyInput[keyCode])
		{
			return true;
		}
	}
	return false;
}

/*-------------------------------------------*/
/* キー：押された瞬間
/*-------------------------------------------*/
bool Input::GetKeyDown(int keyCode)
{
	if (prevKeyInput[keyCode] == false)
	{
		if (currentKeyInput[keyCode])
		{
			return true;
		}
	}
	return false;
}

/*-------------------------------------------*/
/* キー：離された瞬間
/*-------------------------------------------*/
bool Input::GetKeyUp(int keyCode)
{
	if (prevKeyInput[keyCode])
	{
		if (currentKeyInput[keyCode] == false)
		{
			return true;
		}
	}
	return false;
}

/*-------------------------------------------*/
/* ボタン：押されている間
/*-------------------------------------------*/
bool Input::GetButton(UCHAR buttonCode)
{
	if (prevXInput.Buttons[buttonCode])
	{
		if (currentXInput.Buttons[buttonCode])
		{
			return true;
		}
	}
	return false;
}

/*-------------------------------------------*/
/* ボタン：押された瞬間
/*-------------------------------------------*/
bool Input::GetButtonDown(UCHAR buttonCode)
{
	if (prevXInput.Buttons[buttonCode] == false)
	{
		if (currentXInput.Buttons[buttonCode])
		{
			return true;
		}
	}
	return false;
}

/*-------------------------------------------*/
/* ボタン：離された瞬間
/*-------------------------------------------*/
bool Input::GetButtonUp(UCHAR buttonCode)
{
	if (prevXInput.Buttons[buttonCode])
	{
		if (currentXInput.Buttons[buttonCode] == false)
		{
			return true;
		}
	}
	return false;
}

/*-------------------------------------------*/
/* 左スティック横軸
/*-------------------------------------------*/
float Input::GetLeftStickX()
{
	// DXライブラリで設定されているスティックのデッドゾーンの値を取得する
	short deadZoneNum = (short)(GetJoypadDeadZone(DX_INPUT_PAD1) * 32767);
	// 現在入力されているスティックの値がデッドゾーンの範囲外だったら、その値を返す
	if (currentXInput.ThumbLX >  deadZoneNum ||
		currentXInput.ThumbLX < -deadZoneNum)
	{
		// スティックの角度を0~1.0に丸め込む
		float percent = (float)currentXInput.ThumbLX / 32767.0f * 100.0f;
		return percent * 0.01f;
	}
	// デッドゾーンの範囲内だったら、０を返す
	return 0;
}

/*-------------------------------------------*/
/* 左スティック縦軸
/*-------------------------------------------*/
float Input::GetLeftStickY()
{
	// DXライブラリで設定されているスティックのデッドゾーンの値を取得する
	short deadZoneNum = (short)(GetJoypadDeadZone(DX_INPUT_PAD1) * 32767);
	// 現在入力されているスティックの値がデッドゾーンの範囲外だったら、その値を返す
	if (currentXInput.ThumbLY >  deadZoneNum ||
		currentXInput.ThumbLY < -deadZoneNum)
	{
		// スティックの角度を0~1.0に丸め込む
		float percent = (float)currentXInput.ThumbLY / 32767.0f * 100.0f;
		return percent * 0.01f;
	}
	// デッドゾーンの範囲内だったら、０を返す
	return 0;
}

/*-------------------------------------------*/
/* 右スティック横軸
/*-------------------------------------------*/
float Input::GetRightStickX()
{
	// DXライブラリで設定されているスティックのデッドゾーンの値を取得する
	short deadZoneNum = (short)(GetJoypadDeadZone(DX_INPUT_PAD1) * 32767);
	// 現在入力されているスティックの値がデッドゾーンの範囲外だったら、その値を返す
	if (currentXInput.ThumbRX >  deadZoneNum ||
		currentXInput.ThumbRX < -deadZoneNum)
	{
		// スティックの角度を0~1.0に丸め込む
		float percent = (float)currentXInput.ThumbRX / 32767.0f * 100.0f;
		return percent * 0.01f;
	}
	// デッドゾーンの範囲内だったら、０を返す
	return 0;
}

/*-------------------------------------------*/
/* 右スティック縦軸
/*-------------------------------------------*/
float Input::GetRightStickY()
{
	// DXライブラリで設定されているスティックのデッドゾーンの値を取得する
	short deadZoneNum = (short)(GetJoypadDeadZone(DX_INPUT_PAD1) * 32767);
	// 現在入力されているスティックの値がデッドゾーンの範囲外だったら、その値を返す
	if (currentXInput.ThumbRY >  deadZoneNum || 
		currentXInput.ThumbRY < -deadZoneNum)
	{
		// スティックの角度を0~1.0に丸め込む
		float percent = (float)currentXInput.ThumbRY / 32767.0f * 100.0f;
		return percent * 0.01f;
	}
	// デッドゾーンの範囲内だったら、０を返す
	return 0;
}

/*-------------------------------------------*/
/* 左トリガー
/*-------------------------------------------*/
UCHAR Input::GetLeftTrigger()
{
	return currentXInput.LeftTrigger;
}

/*-------------------------------------------*/
/* 右トリガー
/*-------------------------------------------*/
UCHAR Input::GetRightTrigger()
{
	return currentXInput.RightTrigger;
}

/*-------------------------------------------*/
/* バイブレーション
/*-------------------------------------------*/
void Input::StartVibration(int power, int time)
{
	StartJoypadVibration(DX_INPUT_PAD1,power,time);
}

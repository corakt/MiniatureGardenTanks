#include "../Camera/PlayerCamera.h"
#include "../BaseObject/Transform.h"
#include "../Others/Input.h"
#include "../Others/Define.h"
#include <EffekseerForDXLib.h>

const float  PlayerCamera::CAMERA_HEIGHT              = 100;			// カメラの高さ
const float  PlayerCamera::LOOK_AT_POS_DISTANCE       = 1500;			// 注視点位置との距離
const float  PlayerCamera::ROTATION_SPEED             = 0.02f;			// 回転スピード

/*-------------------------------------------*/
/* コンストラクタ
/*-------------------------------------------*/
PlayerCamera::PlayerCamera()
	:CameraBase::CameraBase()
{

}

/*-------------------------------------------*/
/* デストラクタ
/*-------------------------------------------*/
PlayerCamera::~PlayerCamera()
{

}

/*-------------------------------------------*/
/* 初期化
/*-------------------------------------------*/
void PlayerCamera::Initialize(const VECTOR position, const VECTOR lookAtPos)
{
	// 各変数の初期化
	// 揺らし処理のパラメータ
	shakeParam.isShake     = false;		// 揺らし処理を実行中かどうか
	shakeParam.width       = 0;			// 揺れ幅
	shakeParam.timeCounter = 0;			// 揺らす時間で扱うカウンター
	shakeParam.time        = 0;			// 揺らす時間
	shakeParam.angle       = 0;			// 回転角
	shakeParam.angleSpeed  = 0;			// 回転スピード

	transform.position  = position;		// カメラの初期位置をセット
	lookAtPosition      = lookAtPos;	// 注視点の座標

	horizontalAngle     = DX_PI_F;		// カメラの水平回転角度
	cameraHeight        = 0;			// カメラの高さ
	lookAtPosToDistance = 0;			// 注視点位置との距離

	// 移動カウント
	moveCount = 0;
}

/*-------------------------------------------*/
/* 更新
/*-------------------------------------------*/
void PlayerCamera::Update()
{
	// 右スティックの入力方向を得る
	VECTOR inputDir = ZERO_VECTOR;
	inputDir.x = INPUT.GetRightStickX();	// X軸の入力方向

	// キーの入力判定
	     if (INPUT.GetKey(KEY_INPUT_LEFT))  { inputDir.x = -1; }
	else if (INPUT.GetKey(KEY_INPUT_RIGHT)) { inputDir.x = +1; }

	// 回転制御のパラメータをセット
	horizontalAngle    += (-inputDir.x * ROTATION_SPEED);	// スティックの水平方向をもとにカメラを回転させる
	cameraHeight        = CAMERA_HEIGHT;					// カメラの高さ
	lookAtPosToDistance = LOOK_AT_POS_DISTANCE;				// 注視点との距離

	// パラメータをもとにカメラを回転させる
	RotationCamera();

	// カメラの揺らし処理
	shakeCamera();
	// カメラの座標と注視点座標をセット
	SetCameraPositionAndTarget_UpVecY(transform.position, lookAtPosition);
	// DXライブラリのカメラとEffekseerのカメラを同期する
	Effekseer_Sync3DSetting();
	// ３Dサウンドのリスナー位置とリスナーの前方位置を設定
	Set3DSoundListenerPosAndFrontPos_UpVecY(transform.position, lookAtPosition);
}
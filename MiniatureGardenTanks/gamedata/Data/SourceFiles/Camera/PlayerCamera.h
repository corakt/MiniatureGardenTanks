#pragma once
#include "../Camera/CameraBase.h"

class Transform;

/*-------------------------------------------*/
/* カメラ：プレイヤー / 派生クラス
/*-------------------------------------------*/
class PlayerCamera final : public CameraBase
{
public:
	         PlayerCamera();	// コンストラクタ
	virtual ~PlayerCamera();	// デストラクタ

	void Initialize(const VECTOR position, const VECTOR lookAtPos) override;	// 初期化
	void Update() override;														// 更新

private:
	static const float  CAMERA_HEIGHT;				// カメラの高さ
	static const float  LOOK_AT_POS_DISTANCE;		// 注視点位置との距離
	static const float  ROTATION_SPEED;				// 回転スピード
};

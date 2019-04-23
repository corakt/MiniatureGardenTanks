#pragma once
#include "../BaseObject/GameObject.h"
#include <DxLib.h>
#include <list>

// カメラの揺らし処理のパラメータ
struct CameraShakeParameter
{
	bool  isShake;			// 揺らし処理を実行中かどうか
	float width;			// 揺れ幅
	float timeCounter;		// 揺らす時間で扱うカウンター
	float time;				// 揺らす時間
	float angle;			// 回転角
	float angleSpeed;		// 回転スピード
};

// カメラの移動パラメータ
struct CameraMoveParameter
{
	int    lineId;				// 行ID
	int    startCountNum;		// 開始カウント数
	int    endCountNum;			// 終了カウント数
	VECTOR startPosition;		// 移動開始位置
	VECTOR lookAtPosOffset;		// 注視点位置のオフセット値
	VECTOR axisMoveSpeed;		// 各軸の移動スピード
};

/*-------------------------------------------*/
/* カメラ / 基底クラス
/*-------------------------------------------*/
class CameraBase : public GameObject
{
public:
	         CameraBase();		// コンストラクタ
	virtual ~CameraBase();		// デストラクタ

	virtual void Initialize(const VECTOR position, const VECTOR lookAtPos);		// 初期化
	virtual void Update();														// 更新

	// カメラのパラメータを設定
	void SettingCameraParameter();

	// 注視点を中心とした回転制御
	void RotationCamera();

	// カメラの揺らし処理呼び出し
	void CallingToShakeCamera(float shakeWidth,			// 揺れ幅
							  float shakeTime,			// 揺らし時間
							  float shakeAngleSpeed);	// 揺れスピード

	// カメラの移動パラメータをファイルから読み込む
	bool ReadMoveData(LPCSTR fileName);
	// 現在の移動地点をリセット
	void ResetMovePoint();
	// カメラの移動制御
	int MoveCamera();

	// 注視点位置
	const VECTOR& GetLookAtPosition() const             { return lookAtPosition; }
	void          SetLookAtPosition(  const VECTOR set) { lookAtPosition = set;  }

	// カメラの水平回転角
	const float& GetHorizontalAngle() const            { return horizontalAngle; }
	void         SetHorizontalAngle(  const float set) { horizontalAngle = set; }
	
	// カメラの高さ
	const float& GetCameraHeight() const            { return cameraHeight; }
	void         SetCameraHeight(  const float set) { cameraHeight = set;  }

	// 注視点位置との距離
	const float& GetLookAtPosToDistance() const            { return lookAtPosToDistance; }
	void         SetLookAtPosToDistance(  const float set) { lookAtPosToDistance = set;  }

protected:
	void shakeCamera();				// カメラの揺らし処理

	VECTOR lookAtPosition;			// 注視点の座標
	float  horizontalAngle;			// カメラの水平回転角
	float  cameraHeight;			// カメラの高さ
	float  lookAtPosToDistance;		// 注視点位置との距離

	int	   antialiasScreen;			// アンチエイリアス処理のスクリーン

	std::list<CameraMoveParameter>           moveParam;				// 移動パラメータ
	std::list<CameraMoveParameter>::iterator moveParamIterator;		// 移動パラメータ用イテレータ
	int                                      moveCount;				// 移動カウント
	CameraShakeParameter                     shakeParam;			// 揺らし処理のパラメータ

	static const float NEAR_DISTANCE;		// 手前の描画距離
	static const float FAR_DISTANCE;		// 最奥の描画距離
};
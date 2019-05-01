#include "../Camera/CameraBase.h"
#include "../Others/Input.h"
#include "../Others/Define.h"
#include <EffekseerForDXLib.h>
#include <math.h>
#include <fstream>
#include <sstream>
#include <string>
#include <stdlib.h>

const float CameraBase::NEAR_DISTANCE = 300;		// 手前の描画距離
const float CameraBase::FAR_DISTANCE  = 80000;		// 最奥の描画距離

/*-------------------------------------------*/
/* コンストラクタ
/*-------------------------------------------*/
CameraBase::CameraBase()
{
	
}

/*-------------------------------------------*/
/* デストラクタ
/*-------------------------------------------*/
CameraBase::~CameraBase()
{
	// リストを削除する
	moveParam.clear();
}

/*-------------------------------------------*/
/* カメラのパラメータを設定
/*-------------------------------------------*/
void CameraBase::SettingCameraParameter()
{
	// カメラの手前と最奥の描画距離をセット
	SetCameraNearFar(NEAR_DISTANCE, FAR_DISTANCE);

	// カメラの座標と注視点座標をセット
	SetCameraPositionAndTarget_UpVecY(transform.position, lookAtPosition);

	// DXライブラリのカメラとEffekseerのカメラを同期する。
	Effekseer_Sync3DSetting();

	// ３Dサウンドのリスナー位置とリスナーの前方位置を設定
	Set3DSoundListenerPosAndFrontPos_UpVecY(transform.position, lookAtPosition);
}

/*-------------------------------------------*/
/* 初期化
/*-------------------------------------------*/
void CameraBase::Initialize(const VECTOR position, const VECTOR lookAtPos)
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
	horizontalAngle     = 0;			// 水平方向の回転角度
	cameraHeight        = 0;			// カメラの高さ
	lookAtPosToDistance = 0;			// 注視点との距離

	// 移動カウント
	moveCount = 0;
}

/*-------------------------------------------*/
/* 更新
/*-------------------------------------------*/
void CameraBase::Update()
{
	// カメラの揺らし処理
	ShakeCamera();
	// カメラの座標と注視点座標をセット
	SetCameraPositionAndTarget_UpVecY(transform.position, lookAtPosition);
	// DXライブラリのカメラとEffekseerのカメラを同期する
	Effekseer_Sync3DSetting();
	// ３Dサウンドのリスナー位置とリスナーの前方位置を設定
	Set3DSoundListenerPosAndFrontPos_UpVecY(transform.position, lookAtPosition);
}

/*-------------------------------------------*/
/* カメラの揺らし処理呼び出し
/*-------------------------------------------*/
void CameraBase::CallingToShakeCamera(float shakeWidth, float shakeTime, float shakeAngleSpeed)
{
	// 揺らし処理のパラメータをセット
	shakeParam.isShake     = true;					// 揺らし処理を実行中かどうか
	shakeParam.width       = shakeWidth;			// 揺れ幅
	shakeParam.timeCounter = 0;						// 揺らす時間で扱うカウンター
	shakeParam.time        = shakeTime;				// 揺らす時間
	shakeParam.angle       = 0;						// 回転角
	shakeParam.angleSpeed  = shakeAngleSpeed;		// 回転スピード
}

/*-------------------------------------------*/
/* カメラの揺らし処理
/*-------------------------------------------*/
void CameraBase::ShakeCamera()
{
	if (shakeParam.isShake)
	{
		// パラメータの情報をもとに、カメラを上下に揺らす
		transform.position.y += sinf(shakeParam.angle) * (1.0f - (shakeParam.timeCounter /
			shakeParam.time)) *shakeParam.width;

		shakeParam.angle += shakeParam.angleSpeed * 0.01f;
		shakeParam.timeCounter += 0.01f;

		// "time"が"timeCounter"の値を上回ったら、揺らし処理を解除する
		if (shakeParam.timeCounter >= shakeParam.time)
		{
			shakeParam.isShake = false;
		}
	}
}

/*-------------------------------------------*/
/* カメラの移動パラメータをファイルから読み込む
/*-------------------------------------------*/
bool CameraBase::ReadMoveData(LPCSTR fileName)
{
	// ファイルを開く
	std::fstream file(fileName);
	// ファイルが開けなかったらfalseを返す
	if (file.is_open() == false)
	{
		return false;
	}

	// 一行分の文字列
	std::string line;

	// 最初の三行を読み飛ばす
	for (int i = 0; i < 3; i++)
	{
		std::getline(file, line);
	}

	// 一行分の文字列を読み込む
	while (std::getline(file, line))
	{
		std::string token;					// 一セル分の文字列
		std::istringstream stream(line);	// 文字列ストリーム
		CameraMoveParameter tmpParam;		// 一時パラメータ
		
		// 一セル分のデータをそれぞれ格納していく
		// 行ID
		std::getline(stream, token, ',');
		tmpParam.lineId = atoi(token.c_str());
		
		// 開始カウント数
		std::getline(stream, token, ',');
		tmpParam.startCountNum = atoi(token.c_str());
		// 終了カウント数
		std::getline(stream, token, ',');
		tmpParam.endCountNum = atoi(token.c_str());
		
		// 移動開始位置：X軸
		std::getline(stream, token, ',');
		tmpParam.startPosition.x = (float)atoi(token.c_str());
		// 移動開始位置：Y軸
		std::getline(stream, token, ',');
		tmpParam.startPosition.y = (float)atoi(token.c_str());
		// 移動開始位置：Z軸
		std::getline(stream, token, ',');
		tmpParam.startPosition.z = (float)atoi(token.c_str());
		
		// 注視点位置のオフセット値：X軸
		std::getline(stream, token, ',');
		tmpParam.lookAtPosOffset.x = (float)atoi(token.c_str());
		// 注視点位置のオフセット値：Y軸
		std::getline(stream, token, ',');
		tmpParam.lookAtPosOffset.y = (float)atoi(token.c_str());
		// 注視点位置のオフセット値：Z軸
		std::getline(stream, token, ',');
		tmpParam.lookAtPosOffset.z = (float)atoi(token.c_str());

		// 各軸の移動スピード：X軸
		std::getline(stream, token, ',');
		tmpParam.axisMoveSpeed.x = (float)atoi(token.c_str());
		// 各軸の移動スピード：Y軸
		std::getline(stream, token, ',');
		tmpParam.axisMoveSpeed.y = (float)atoi(token.c_str());
		// 各軸の移動スピード：Z軸
		std::getline(stream, token, ',');
		tmpParam.axisMoveSpeed.z = (float)atoi(token.c_str());

		// パラメータをリストに格納する
		moveParam.push_back(tmpParam);
	}

	// ファイルを閉じる
	file.close();
	// ファイルストリームの内容を削除
	file.clear();

	return true;
}

/*-------------------------------------------*/
/* 現在の移動地点をリセット
/*-------------------------------------------*/
void CameraBase::ResetMovePoint()
{
	// 移動カウントをゼロに初期化
	moveCount = 0;
	// イテレータを先頭に移動
	moveParamIterator = moveParam.begin();
}

/*-------------------------------------------*/
/* カメラの移動制御
/*-------------------------------------------*/
int CameraBase::MoveCamera()
{
	if (moveParam.empty() == false)
	{
		if (moveParamIterator != moveParam.end())
		{
			
			if (moveCount == (*moveParamIterator).startCountNum)
			{
				// 移動開始位置をセット
				transform.position = (*moveParamIterator).startPosition;
			}

			// カメラの位置をパラメータによって移動させる
			transform.position = transform.position + (*moveParamIterator).axisMoveSpeed;
			// 注視点位置のオフセット値をセット
			lookAtPosition = lookAtPosition + (*moveParamIterator).lookAtPosOffset;

			// 移動終了カウント数に達したら、次の行の要素に移動
			if (moveCount >= (*moveParamIterator).endCountNum)
			{
				moveParamIterator++;
			}

			// 移動処理が最後まで終わったら、-1を返す
			if (moveParamIterator == moveParam.end()) { return -1; }

			moveCount++;

			// 現在の行のIDを返す
			return (*moveParamIterator).lineId;
		}
	}

	// 移動処理が既に終わっている場合は、-1を返す
	return -1;
}

/*-------------------------------------------*/
/*　注視点を中心とした回転制御
/*-------------------------------------------*/
void CameraBase::RotationCamera()
{
	// 水平方向の回転範囲角度の制御
	if (horizontalAngle < -DX_PI_F)
	{
		horizontalAngle += DX_PI_F * 2.0f;
	}
	if (horizontalAngle > DX_PI_F)
	{
		horizontalAngle -= DX_PI_F * 2.0f;
	}

	// 回転角からカメラの位置を算出
	transform.position.x = -sinf(horizontalAngle) * lookAtPosToDistance;
	transform.position.z =  cosf(horizontalAngle) * lookAtPosToDistance;
	// カメラの高さをセット
	transform.position.y = cameraHeight;

	transform.position = transform.position + lookAtPosition;

	// カメラの方向を算出
	transform.direction = VNorm(lookAtPosition - transform.position);
}
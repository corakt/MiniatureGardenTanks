#include "../CharacterController/ControllerBase.h"
#include "../Character/CharacterBase.h"
#include "../Others/Math3D.h"
#include <math.h>

/*-------------------------------------------*/
/* コンストラクタ
/*-------------------------------------------*/
ControllerBase::ControllerBase(CharacterBase* controlCharacter)
{
	// 制御するキャラクターのポインタを取得
	this->controlCharacter = controlCharacter;

	// キャラクターの各部分のオブジェクトを取得
	characterBody   = controlCharacter->GetParts(CharacterBase::PartsType::BODY);		// 車体部分
	characterTurret = controlCharacter->GetParts(CharacterBase::PartsType::TURRET);		// 砲塔部分
}

/*-------------------------------------------*/
/* デストラクタ
/*-------------------------------------------*/
ControllerBase::~ControllerBase()
{
	// 各ポインタにNULLを代入
	controlCharacter = NULL;
	characterBody    = NULL;
	characterTurret  = NULL;
}

/*-------------------------------------------*/
/* 共通の初期化
/*-------------------------------------------*/
void ControllerBase::commonInitialize()
{
	// 各変数の初期化
	easingTime   = 0;		// イージングの進行度
	currentSpeed = 0;		// 現在の移動スピード
}

/*-------------------------------------------*/
/* 移動
/*-------------------------------------------*/
void ControllerBase::move(bool isInput)
{
	// キャラクターから移動パラメータを取得
	const float  MOVING_SPEED_MAX  = controlCharacter->GetMovingSpeedMax();		// 移動の最高スピード
	const float  MOVING_ACCEL_RATE = controlCharacter->GetMovingAccelRate();	// 移動の加速率
	const float  MOVING_DECEL_RATE = controlCharacter->GetMovingDecelRate();	// 移動の減速率
	
	// 加速
	if (isInput)
	{
		// イージング処理で加速
		easingTime += MOVING_ACCEL_RATE;
		currentSpeed = CubicEaseInOut(easingTime, 0, MOVING_SPEED_MAX);
	}
	// 減速
	else
	{
		//	イージング処理で減速
		easingTime -= MOVING_DECEL_RATE;
		currentSpeed = QuadraticEaseOut(easingTime, 0, MOVING_SPEED_MAX);
	}

	// イージングの経過時間が０～１の範囲内から超えないようにする
	if (easingTime < 0)
	{
		easingTime = 0;			// ０を代入
	}
	else if (easingTime > 1)
	{
		easingTime = 1;			// １を代入
	}

	// 現在の移動スピードが０～最高速度の範囲を超えないようにする
	//（floatの誤差があるため、0.2f以下は０にする）
	if (currentSpeed < 0.2f)
	{
		currentSpeed = 0;				// 移動速度を０にする
	}
	else if (currentSpeed > MOVING_SPEED_MAX)
	{
		currentSpeed = MOVING_SPEED_MAX;		// 移動速度を最高速度にする
	}

	// 移動の操作中フラグをセット
	controlCharacter->SetMoveInputtingFlag(isInput);

	// 移動スピードをセット
	controlCharacter->SetMovingSpeed(currentSpeed);

	// 向きに移動量を加える
	characterBodyTrans.velocity = VScale(characterBodyTrans.direction, currentSpeed);
	// キャラクターを移動させる
	characterBodyTrans.position = VAdd(characterBodyTrans.position, characterBodyTrans.velocity);
}

/*-------------------------------------------*/
/* 車体の回転
/*-------------------------------------------*/
void ControllerBase::rotationBody(VECTOR moveDir,bool isInput)
{
	if (isInput)
	{
		// 移動する向きを回転角に変換する
		float rotationY = atan2f(moveDir.x, moveDir.z);
		// 算出した角度を車体部分の回転角に反映
		characterBodyTrans.rotation.y = LerpRadian(characterBodyTrans.rotation.y, rotationY, 0.05f);
	}
}

/*-------------------------------------------*/
/* 砲塔の回転
/*-------------------------------------------*/
void ControllerBase::rotationTurret(VECTOR turretDir)
{
	// 向きを角度に変換する
	float rotationY = atan2f(turretDir.x, turretDir.z);
	// 算出した角度を砲塔部分の回転角に反映
	characterTurretTrans.rotation.y = LerpRadian(characterTurretTrans.rotation.y, rotationY, 0.04f);
}

/*-------------------------------------------*/
/* ショット発射時の反動
/*-------------------------------------------*/
void ControllerBase::firingReaction()
{
	// 現在の反動時の状態を表すステートを取得
	CharacterBase::FiringReactionState reactionState = controlCharacter->GetReactionState();
	// 現在の反動力を取得
	float currentReactionForce = controlCharacter->GetCurrentReactionForce();

	// 反動力の最大値を取得
	const float REACTION_FORCE_MAX = controlCharacter->GetReactionForceMax();
	// 反動力の減衰率を取得
	const float REACTION_ATTENUATION = controlCharacter->GetReactionAttenuation();

	// 砲塔の向きと逆のベクトルを算出
	VECTOR turretInverseDir = VScale(characterTurretTrans.direction, -1);
	turretInverseDir.y = 0;

	// 反動の状態"REACTION_START"の際の処理
	// （反動処理開始時の処理）
	if (reactionState == CharacterBase::REACTION_START)
	{
		// 車体の向きと逆ベクトルの内積を算出
		currentReactionForce = VDot(characterBodyTrans.direction, turretInverseDir);
		// 反動力の最大値を掛ける
		currentReactionForce *= REACTION_FORCE_MAX;
		// 反動力の状態を"ATTENUATION"に変更
		reactionState = CharacterBase::ATTENUATION;
	}

	// 向きに反動力を加える
	characterBodyTrans.velocity =  VScale(characterBodyTrans.direction, currentReactionForce);
	// キャラクターの座標を変更する
	characterBodyTrans.position = VAdd(characterBodyTrans.position, characterBodyTrans.velocity);

	// 反動の状態"ATTENUATION"の際の処理
	// （反動力が減衰していく処理）
	if (reactionState == CharacterBase::ATTENUATION)
	{
		// 反動の向きによって、符号がプラスかマイナスか変わってくる
		// 反動力がプラス値だった場合は値を減らし、
		// 反動力がマイナス値だった場合は値を増やしていき、反動力を減衰（０に近づける）させていく。
		if (currentReactionForce > 0) { currentReactionForce -= REACTION_ATTENUATION; }
		else                          { currentReactionForce += REACTION_ATTENUATION; }

	}
	
	// 反動力が-1～1の範囲内になったら、反動力を０にして終了
	if (IsCountBetween((int)(currentReactionForce), -1, 1))
	{
		// 反動力の値を０にする
		currentReactionForce = 0;
		// 反動の状態を"NO_REACTION"に変更
		reactionState = CharacterBase::NON_REACTION;

		// 反動の際中かどうかを表すフラグをオフにする
		controlCharacter->SetReactionFlag(false);
	}

	// 変更した各パラメータをキャラクターに反映させる
	controlCharacter->SetReactionState(reactionState);						// 反動の状態を表すステート
	controlCharacter->SetCurrentReactionForce(currentReactionForce);		// 現在の反動力
}
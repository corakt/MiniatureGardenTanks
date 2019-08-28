#include "../CharacterController/ControllerPlayer.h"
#include "../Camera/CameraBase.h"
#include "../Camera/PlayerCamera.h"
#include "../Others/Input.h"
#include "../Others/Define.h"
#include "../CharacterShot/ShotManager.h"

/*-------------------------------------------*/
/* コンストラクタ
/*-------------------------------------------*/
ControllerPlayer::ControllerPlayer(CharacterBase* controlCharacter, CameraBase* playerCamera)
	:ControllerBase::ControllerBase(controlCharacter)
{
	// プレイヤーカメラを取得
	this->playerCamera = playerCamera;
}

/*-------------------------------------------*/
/* デストラクタ
/*-------------------------------------------*/
ControllerPlayer::~ControllerPlayer()
{

}


/*-------------------------------------------*/
/* 初期化
/*-------------------------------------------*/
void ControllerPlayer::Initialize()
{
	// キャラクターの制御共通の初期化
	commonInitialize();
}

/*-------------------------------------------*/
/* 更新
/*-------------------------------------------*/
void ControllerPlayer::Update()
{
	// キャラクターの制御フラグがfalseだったら、
	// 何も処理せずに関数を抜ける
	if (controlCharacter->GetControlingFlag() == false) { return; }

	// キャラクターの各部分のトランスフォーム取得
	characterBodyTrans   = characterBody->GetTransform();		// 車体部分
	characterTurretTrans = characterTurret->GetTransform();		// 砲塔部分

	// カメラのトランスフォームを取得
	Transform cameraTrans = playerCamera->GetTransform();

	/*-------------------------------------------*/
	/* 移動処理
	/*-------------------------------------------*/
	VECTOR moveInputDir = ZERO_VECTOR;
	// 左スティックの入力方向を取得
	moveInputDir.x = INPUT.GetLeftStickX();
	moveInputDir.z = INPUT.GetLeftStickY();
	// キーの入力判定
	 if (INPUT.GetKey(KEY_INPUT_W)) { moveInputDir.z = +1; }
	 if (INPUT.GetKey(KEY_INPUT_S)) { moveInputDir.z = -1; }
	 if (INPUT.GetKey(KEY_INPUT_A)) { moveInputDir.x = -1; }
	 if (INPUT.GetKey(KEY_INPUT_D)) { moveInputDir.x = +1; }

	// 入力中かどうかのフラグ
	bool isMoveInput = false;
	// 左スティックが入力中かどうかを判定
	if (VSquareSize(moveInputDir) != 0) { isMoveInput = true;  }
	else                                { isMoveInput = false; }
	// 関数から移動処理を行う
	move(isMoveInput);

	
	/*-------------------------------------------*/
	/* 車体の回転
	/*-------------------------------------------*/
	// 内積を使い、Z軸を基準としたカメラの角度を求める
	float z = VDot(cameraTrans.direction, VGet(0, 0, 1));
	z = acosf(z);

	// 外積を使いZ軸を基準としたカメラの向きを調べる
	VECTOR upVec = VCross(cameraTrans.direction, VGet(0, 0, 1));
	MATRIX rotationY = { 0 };
	// 求めたカメラの向きをもとに、回転行列を受け取る
	if (upVec.y < 0) { rotationY = MGetRotY(z);  }
	else			 { rotationY = MGetRotY(-z); }
	// 回転行列とパッドの入力方向から移動方向を決める
	VECTOR moveDirection = VTransform(moveInputDir, rotationY);
	// 車体を回転させる
	rotationBody(moveDirection, isMoveInput);


	/*-------------------------------------------*/
	/* 砲塔の回転
	/*-------------------------------------------*/
	rotationTurret(cameraTrans.direction);


	/*-------------------------------------------*/
	/* ショットの発射
	/*-------------------------------------------*/
	// リロード中かどうか調べる
	bool isReload = controlCharacter->GetReloadFlag();
	
	if (isReload == false)
	{
		if (INPUT.GetButtonDown(XINPUT_BUTTON_RIGHT_SHOULDER) ||	// ジョイパッド
			INPUT.GetKeyDown(KEY_INPUT_SPACE))						// キーボード
		{
			// ショットを発射する数を取得
			int firingShotNum = controlCharacter->GetFiringShotNum();

			// 弾を発射する
			SHOT_MANAGER.FiringShot(controlCharacter, firingShotNum);

			// ショットを撃った際のコールバック関数を呼ぶ
			controlCharacter->OnFiringShot();

			// カメラを揺らす処理
			const float SHAKE_WIDTH       = 50.0f;		// 揺れ幅
			const float SHAKE_TIME        = 0.2f;		// 揺れ時間
			const float SHAKE_ANGLE_SPEED = 100.0f;		// 揺れ速度
			// カメラを揺らす
			playerCamera->CallingToShakeCamera(SHAKE_WIDTH, SHAKE_TIME, SHAKE_ANGLE_SPEED);

			// コントローラーの振動処理
			const int VIVE_POWER = 1000;				// 振動の強さ
			const int VIVE_TIME  = 100;					// 振動の時間
			// 振動を開始する
			INPUT.StartVibration(VIVE_POWER, VIVE_TIME);

			// リロードフラグをオンにする
			controlCharacter->SetReloadFlag(true);
			// 反動の最中かどうかのフラグをオンにする
			controlCharacter->SetReactionFlag(true);
			// 反動の状態を"REACTION_START"に変更
			controlCharacter->SetReactionState(CharacterBase::REACTION_START);
		}
	}


	/*-------------------------------------------*/
	/* ショットの発射時の反動
	/*-------------------------------------------*/
	// 反動の最中かどうかのフラグを取得
	bool isReaction = controlCharacter->GetReactionFlag();
	// フラグがたっている場合は、反動の処理にうつる
	if (isReaction)
	{
		firingReaction();
	}

	/*-------------------------------------------*/
	/* スキルモード発動
	/*-------------------------------------------*/
	// キャラクターの現在の状態を取得
	const CharacterBase::State CHARACTER_STATE = controlCharacter->GetState();
	// キャラクターの現在のアクティブポイントを取得
	const int ACTIVE_POINT = controlCharacter->GetActivePoint();
	// アクティブポイントの最大値を取得
	const int ACTIVE_POINT_MAX = controlCharacter->GetActivePointMax();

	if (INPUT.GetButtonDown(XINPUT_BUTTON_LEFT_SHOULDER) ||	// ジョイパッド
		INPUT.GetKeyDown(KEY_INPUT_LSHIFT))					// キーボード
	{
		if (CHARACTER_STATE == CharacterBase::ACTIVEGAUGE_FULL)
		{
			// スキルモードを発動する
			controlCharacter->ActivateSkill();
		}
	}

	// キャラクターの各部分のトランスフォームセット
	characterBody->SetTransform(characterBodyTrans);		// 車体部分
	characterTurret->SetTransform(characterTurretTrans);	// 砲塔部分
}
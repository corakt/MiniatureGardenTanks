#include "../Character/CharacterBase.h"
#include "../Stage/TerrainBase.h"
#include "../Stage/TerrainWall.h"
#include "../ResourcesManager/ResourceModelManager.h"
#include "../ResourcesManager/ResourceSoundManager.h"
#include "../Raycast/Raycast.h"
#include "../Raycast/RaycastManager.h"
#include "../Collision/BoxCollider.h"
#include "../Collision/CollisionManager.h"
#include "../Others/Math3D.h"
#include "../Others/Define.h"
#include <algorithm>
#include <utility>
#include <vector>

const int    CharacterBase::HITPOINT_MAX              = 5;						// ヒットポイントの最大値
const int    CharacterBase::ACTIVEPOINT_MAX           = 8000;					// アクティブポイントの最大値
const float  CharacterBase::MOVING_SPEED_MAX          = 30;						// 最高速度
const float  CharacterBase::MOVING_ACCEL_RATE         = 0.03f;					// 加速率
const float  CharacterBase::MOVING_DECEL_RATE         = 0.05f;					// 減速率
const int    CharacterBase::SKILL_FIRINGSHOT_NUM      = 3;						// スキル発動時の発射するショットの数
const VECTOR CharacterBase::MODEL_SCALE               = VGet(1,1,1);			// モデルのスケール
const VECTOR CharacterBase::COLLISION_SIZE            = VGet(670,170,670);		// 衝突判定の大きさ
const VECTOR CharacterBase::COLLIDER_POS_OFFSET       = VGet(0,200,0);			// コライダーの位置のオフセット
const int    CharacterBase::INVINCIBLE_FLASH_INTERVAL = 5;						// 無敵中のモデルの点滅間隔
const int    CharacterBase::INVINCIBLE_COUNT          = 60;						// 無敵時間
const int    CharacterBase::RELOAD_TIME               = 180;					// リロード時間
const float  CharacterBase::REACTION_FORCE_MAX        = 20;						// ショット発射時の反動力
const float  CharacterBase::REACTION_ATTENUATION      = 0.2f;					// 反動の減衰率
const float  CharacterBase::TURRET_RAYCAST_DISTANCE   = 100000;					// 砲塔のレイキャストの距離

/*-------------------------------------------*/
/* コンストラクタ
/*-------------------------------------------*/
CharacterBase::CharacterBase(UINT id, CharacterType type)
{
	// キャラクターのIDを取得
	this->id = id;

	// キャラクターの種類を取得
	this->type = type;

	// NULLで初期化
	for (int i = 0; i < PartsType::TYPE_NUM; i++) { tank[i] = NULL; }	// キャラクター

	boxCollider         = NULL;		// 車体のボックスコライダーを生成
	turretDirRaycast    = NULL;		// 砲塔と同じ向きのレイキャスト

	sounds.damage       = NULL;		// ダメージ
	sounds.shotFiring   = NULL;		// 砲撃音
	sounds.shotReload   = NULL;		// 砲弾の装填
	sounds.engineStart  = NULL;		// エンジン：始動
	sounds.engineIdling = NULL;		// エンジン：アイドリング
	sounds.engineAccel  = NULL;		// エンジン：加速
	sounds.engineMoving = NULL;		// エンジン：移動中
	sounds.engineDecel  = NULL;		// エンジン：減速
	sounds.crawler      = NULL;		// 履帯音

	// キャラクターの各部品を表すモデルコンポーネントを生成
	tank[PartsType::BODY]   = AddComponent<ModelObject>();
	tank[PartsType::TURRET] = AddComponent<ModelObject>();
	// モデルをセットする
	tank[PartsType::BODY]->SetHandle(MODEL_MANAGER.GetHandle(ResourceModelManager::ModelType::TANK_BODY));
	tank[PartsType::TURRET]->SetHandle(MODEL_MANAGER.GetHandle(ResourceModelManager::ModelType::TANK_TURRET));
	// モデルコンポーネントの種類を設定
	tank[PartsType::BODY]->SetObjectType(ObjectType::CHARACTER_TANKBODY);
	tank[PartsType::TURRET]->SetObjectType(ObjectType::CHARACTER_TANKTURRET);

	// 車体部分のモデルをレイキャストの対象のモデルとして登録
	RAYCAST_MANAGER.SetTargetModel(tank[PartsType::BODY]);
	// 砲塔部分のモデルをレイキャストの対象のモデルとして登録
	RAYCAST_MANAGER.SetTargetModel(tank[PartsType::TURRET]);

	// レイキャストを追加（ゼロで初期化）
	turretDirRaycast = RAYCAST_MANAGER.AddRaycast(ZERO_VECTOR, ZERO_VECTOR, 0);

	// ボックスコライダーを生成
	boxCollider = COLLISION_MANAGER.AddBoxCollider();

	// 各サウンドのオブジェクトを生成
	sounds.damage       = new SoundObject(SOUND_MANAGER.GetHandle(ResourceSoundManager::SoundType::SE_TANK_SHOTHIT));			// ダメージ
	sounds.shotFiring   = new SoundObject(SOUND_MANAGER.GetHandle(ResourceSoundManager::SoundType::SE_TANK_SHOTFIRING));		// 砲撃音
	sounds.shotReload   = new SoundObject(SOUND_MANAGER.GetHandle(ResourceSoundManager::SoundType::SE_TANK_SHOTRELOAD));		// 砲弾の装填
	sounds.engineStart  = new SoundObject(SOUND_MANAGER.GetHandle(ResourceSoundManager::SoundType::SE_TANK_ENGINESTART));		// エンジン：始動
	sounds.engineIdling = new SoundObject(SOUND_MANAGER.GetHandle(ResourceSoundManager::SoundType::SE_TANK_ENGINEIDLING));		// エンジン：アイドリング
	sounds.engineAccel  = new SoundObject(SOUND_MANAGER.GetHandle(ResourceSoundManager::SoundType::SE_TANK_ENGINEACCEL));		// エンジン：加速
	sounds.engineMoving = new SoundObject(SOUND_MANAGER.GetHandle(ResourceSoundManager::SoundType::SE_TANK_ENGINEMOVING));		// エンジン：移動中
	sounds.engineDecel  = new SoundObject(SOUND_MANAGER.GetHandle(ResourceSoundManager::SoundType::SE_TANK_ENGINEDECEL));		// エンジン：減速
	sounds.crawler      = new SoundObject(SOUND_MANAGER.GetHandle(ResourceSoundManager::SoundType::SE_TANK_CRAWLER));			// 履帯音
}

/*-------------------------------------------*/
/* デストラクタ
/*-------------------------------------------*/
CharacterBase::~CharacterBase()
{
	// キャラクターの各部品を削除
	for (int i = 0; i < PartsType::TYPE_NUM; i++)
	{
		SafeDelete(tank[i]);
	}

	// レイキャスト
	turretDirRaycast = NULL;
	// ボックスコライダー
	boxCollider   = NULL;

	// 各サウンドのオブジェクトを削除
	SafeDelete(sounds.damage);				// ダメージ
	SafeDelete(sounds.shotFiring);			// 砲撃音
	SafeDelete(sounds.shotReload);			// 砲弾の装填
	SafeDelete(sounds.engineStart);			// エンジン：始動
	SafeDelete(sounds.engineIdling);		// エンジン：アイドリング
	SafeDelete(sounds.engineAccel);			// エンジン：加速
	SafeDelete(sounds.engineMoving);		// エンジン：移動中
	SafeDelete(sounds.engineDecel);			// エンジン：減速
	SafeDelete(sounds.crawler);				// 履帯音
}

/*-------------------------------------------*/
/* 砲塔の相対位置を算出
/*-------------------------------------------*/
void CharacterBase::CalculationTurretRelativePosition()
{
	// 各部品の相対位置
	VECTOR BODY_RELATIVE_POS   = VGet(0, 0, 0);
	VECTOR TURRET_RELATIVE_POS = VGet(0, 200, 0);

	// 相対位置から部品の位置を算出
	bodyTransform.position   = transform.position + BODY_RELATIVE_POS;
	turretTransform.position = transform.position + TURRET_RELATIVE_POS;
}

/*-------------------------------------------*/
/* アクティブポイントの加算
/*-------------------------------------------*/
void CharacterBase::AddActivePoint(int addNum)
{
	// 通常モードのみポイントを加算する
	if (state == State::NORMAL)
	{
		// アクティブポイントはHPが少ないほど多く獲得できる。

		// HPの最大値から現在のHP-1を引いた差分を算出
		const int DIFF_HP_NUM = HITPOINT_MAX - (hitPoint - 1);
		// もともとの加算値に算出した差分をかける
		const int SCALED_ADD_NUM = addNum * DIFF_HP_NUM;

		// 差分を掛けたものを最終的に加算値にする
		activePoint += SCALED_ADD_NUM;
	}
}

/*-------------------------------------------*/
/* 共通の初期化処理
/*-------------------------------------------*/
void CharacterBase::commonInitialize()
{
	// 各変数の初期化
	hitPoint             = HITPOINT_MAX;						// キャラクターのヒットポイント
	activePoint          = 0;									// キャラクターのアクティブポイント
	isInvincible         = false;								// 無敵状態かどうか
	currentReloadTime    = 0;									// 現在のリロード時間
	movingSpeed          = 0;									// 移動スピード
	isReload             = false;								// リロード中かどうか
	isReaction           = false;								// 反動の処理の最中かどうか
	isEngineSoundPlaying = false;								// エンジンサウンドの再生を行うかどうか
	isControling         = false;								// 制御中かどうか
	isMoveInputting      = false;								// 移動の操作中か

	state                = State::NORMAL;						// キャラクターの状態

	turretTipPosition    = ZERO_VECTOR;							// 砲塔の先端の座標
	impactPosition       = ZERO_VECTOR;							// 砲弾の着弾地点

	reactionState        = FiringReactionState::NON_REACTION;	// ショット発射時の反動の状態
	currentReactionForce = 0;									// 現在の反動力

	moveState            = MoveState::STOP;						// 移動の状態

	// エフェクトハンドル
	effects.damage           = -1;		// ダメージ
	effects.brokenExplosion  = -1;		// 戦車の破壊
	effects.brokenBlackSmoke = -1;		// 破壊時の黒煙
	effects.firingExplosion  = -1;		// 砲撃時の爆発
	effects.exhaustGas       = -1;		// 排気ガス
	effects.moveSandSmoke    = -1;		// 移動時の土煙

	// サウンドの音量を設定
	sounds.damage->SetPlayVolume(220);				// ダメージ
	sounds.shotFiring->SetPlayVolume(255);			// 砲撃音
	sounds.shotReload->SetPlayVolume(200);			// 砲弾の装填
	sounds.engineStart->SetPlayVolume(255);			// エンジン：始動
	sounds.engineIdling->SetPlayVolume(255);		// エンジン：アイドリング
	sounds.engineAccel->SetPlayVolume(255);			// エンジン：加速
	sounds.engineMoving->SetPlayVolume(255);		// エンジン：移動中
	sounds.engineDecel->SetPlayVolume(255);			// エンジン：減速
	sounds.crawler->SetPlayVolume(150);				// 履帯音

	// サウンドの可聴範囲を設定
	sounds.damage->SetPlayingRadius(25000);			// ダメージ
	sounds.shotFiring->SetPlayingRadius(30000);		// 砲撃音
	sounds.shotReload->SetPlayingRadius(7000);		// 砲弾の装填
	sounds.engineStart->SetPlayingRadius(7000);		// エンジン：始動
	sounds.engineIdling->SetPlayingRadius(7000);	// エンジン：アイドリング
	sounds.engineAccel->SetPlayingRadius(7000);		// エンジン：加速
	sounds.engineMoving->SetPlayingRadius(7000);	// エンジン：移動中
	sounds.engineDecel->SetPlayingRadius(7000);		// エンジン：減速
	sounds.crawler->SetPlayingRadius(7000);			// 履帯音

	// カウンター
	counters.brokenSmokeEffect   = 0;	// 破壊時の黒煙エフェクトで扱うカウント
	counters.exhaustGasEffect    = 0;	// 排気ガスエフェクトで扱うカウント
	counters.moveSandSmokeEffect = 0;	// 移動時の土煙で扱うカウント
	counters.engineSound         = 0;	// エンジンサウンドを制御する際に扱うカウント
	counters.invincible          = 0;	// 無敵処理で扱うカウント
	counters.reload              = 0;	// リロード時に扱うカウント

	// コライダー
	boxCollider->center        = bodyTransform.position;	// コライダーの基準位置
	boxCollider->size          = COLLISION_SIZE;			// コライダーのサイズ
	boxCollider->attachedModel = tank[PartsType::BODY];		// コライダーにアタッチするモデル
	boxCollider->isCollCheck   = true;						// 衝突判定を行うかどうか

	// レイキャスト
	turretDirRaycast->origin        = turretTransform.position;		// レイキャストの原点を設定
	turretDirRaycast->direction     = turretTransform.direction;	// レイキャストの向きを設定
	turretDirRaycast->distance      = TURRET_RAYCAST_DISTANCE;		// レイキャストの距離を設定
	turretDirRaycast->attachedModel = tank[PartsType::TURRET];		// レイキャストに砲塔のモデルオブジェクトをアタッチ

	for (int i = 0; i < PartsType::TYPE_NUM; i++)
	{
		tank[i]->Initialize();				// トランスフォームの初期化
		tank[i]->SetActiveFlag(true);		// アクティブフラグを立てる
		tank[i]->SetDrawFlag(true);			// 描画フラグを立てる
	}
}

/*-------------------------------------------*/
/* 共通の更新処理
/*-------------------------------------------*/
void CharacterBase::commonUpdate()
{
	// トランスフォームを取得
	bodyTransform   = tank[PartsType::BODY]->GetTransform();		// 車体
	turretTransform = tank[PartsType::TURRET]->GetTransform();		// 砲塔

	// 砲塔の相対位置を算出
	CalculationTurretRelativePosition();
	// 各部品の向きベクトルを算出
	calculationPartsDirection();
	// 砲塔の先端の座標を算出
	calculationTurretTipPos();
	// ショットの着弾地点を算出
	calculationImpactPosition();
	// 無敵状態の制御
	controlInvincibleState();
	// 移動の状態を更新
	updateMoveState();
	// レイキャストのパラメータを更新
	updateRaycastParameter();
	// コライダーのパラメータを更新
	updateColliderParameter();
	// サウンドの再生位置を更新
	updateSoundPosition();

	// エンジンサウンドの再生フラグがtrueだった場合のみ、再生する
	if (isEngineSoundPlaying)
	{
		// HPが残っている場合のみ、エンジン音を再生する
		if (hitPoint > 0)
		{
			// エンジン音の制御を行う
			controlEngineSound();
		}
		else
		{
			// 再生中のエンジン音を停止する
			StopEngineSound();
		}
	}

	// 戦車が破壊されていなければ、以下の処理を行う
	if (state != State::BROKEN)
	{
		// スキル発動時の発射するショットの数を設定
		if (state == State::SKILL_ACTIVE)
		{
			// スキル発動時
			firingShotNum = SKILL_FIRINGSHOT_NUM;
		}
		else
		{
			// スキル未発動時は１発のみ
			firingShotNum = 1;
		}

		// ショットのリロード処理
		if (isReload)
		{
			// 現在のリロード時間を表すカウントを加算していく
			currentReloadTime++;

			// リロード完了手前で、装填音を再生
			if (currentReloadTime == RELOAD_TIME-40)
			{
				sounds.shotReload->Playing(DX_PLAYTYPE_BACK);
			}

			// 現在のリロード時間が指定のリロード時間を超えたら、
			// リロードフラグを倒す
			if (currentReloadTime >= RELOAD_TIME)
			{
				isReload = false;
				// カウントを０に初期化
				currentReloadTime = 0;
			}
		}

		// 各エフェクトを再生
		playEffectForExhaustGas();			// 排気ガス
		playEffectforMoveSandSmoke();		// 移動中の土煙
		playEffectForBrokenBlackSmoke();	// 破壊後の黒煙

		// 移動中の処理
		if (moveState != MoveState::STOP)
		{
			// アクティブポイントを加算していく
			AddActivePoint(1);
		}
	}
	// 戦車が破壊された場合は、以下の処理を行う
	else
	{
		// 破壊後の黒煙のエフェクトを再生
		playEffectForBrokenBlackSmoke();
		// キャラクターの制御フラグをfalseに変更
		isControling = false;
	}

	// スキルモード発動中の処理
	if (state == State::SKILL_ACTIVE)
	{
		// スキル発動中はアクティブポイントを減らしていく
		activePoint -= 7;

		// アクティブポイントが０を下回らないようにする
		if (activePoint <= 0)
		{
			// 通常の状態に変更する
			state = State::NORMAL;
			activePoint = 0;
		}
	}
	
	// アクティブポイントが満タンになったら、状態を表すステートを変更する
	if (activePoint >= ACTIVEPOINT_MAX) { state = State::ACTIVEGAUGE_FULL; }
	// アクティブポイントが最大値を上回らないようにする
	if (activePoint > ACTIVEPOINT_MAX)
	{
		// アクティブポイントに最大値を代入
		activePoint = ACTIVEPOINT_MAX;
	}


	// 衝突関連の処理
	if (boxCollider->GetCollModelInfo().empty() == false)
	{
		for (const CollModelInfo& collModelInfoElem : boxCollider->GetCollModelInfo())
		{
			// 衝突したモデルを取得
			ModelObject* collModel = collModelInfoElem.collModel;
			// オブジェクトがNULLだったら、スキップして次の要素へ
			if (collModel == NULL) { continue; }

			// ショットと衝突
			if (collModel->GetObjectType() == ObjectType::CHARACTER_SHOT)
			{
				// コールバック関数
				onCollisionShot(collModelInfoElem);
			}
		}
	}

	// トランスフォームをセット
	tank[PartsType::BODY]->SetTransform(bodyTransform);			// 車体
	tank[PartsType::TURRET]->SetTransform(turretTransform);		// 砲塔
}

/*-------------------------------------------*/
/* 共通の描画処理
/*-------------------------------------------*/
void CharacterBase::commonDraw()
{
	for (int i = 0; i < PartsType::TYPE_NUM; i++)
	{
		// 戦車の各部品を描画
		tank[i]->DrawModel();
	}
}

/*-------------------------------------------*/
/* 各部品の向きベクトルを算出
/*-------------------------------------------*/
void CharacterBase::calculationPartsDirection()
{
	// 回転角から回転行列を算出して、回転行列から向きベクトルを算出する
	MATRIX rotationMatrix = GetRotMatrixFromRot(bodyTransform.rotation);
	bodyTransform.direction = VTransform(VGet(0, 0, 1), rotationMatrix);

	rotationMatrix = GetRotMatrixFromRot(turretTransform.rotation);
	turretTransform.direction = VTransform(VGet(0, 0, 1), rotationMatrix);
}

/*-------------------------------------------*/
/* 砲塔の先端の座標を算出
/*-------------------------------------------*/
void CharacterBase::calculationTurretTipPos()
{
	// 先端座標算出用ローカル変数
	const float TURRETTIP_DISTANCE = 600;	// 砲塔の先端までの距離
	const float TURRETTIP_HEIGHT   = 100;	// 砲塔の先端までの高さ

	// 先端座標算出
	turretTipPosition = turretTransform.direction * TURRETTIP_DISTANCE;		// 向きに移動量を加える
	turretTipPosition = VGet(turretTransform.position.x,
							 turretTransform.position.y  + TURRETTIP_HEIGHT,
							 turretTransform.position.z) + turretTipPosition;
}

/*-------------------------------------------*/
/* ショットの着弾地点の算出
/*-------------------------------------------*/
void CharacterBase::calculationImpactPosition()
{
	// レイキャストに衝突しているモデルが無ければ、
	// 何も処理せずにそのまま関数を抜ける
	if (turretDirRaycast->GetHitModelInfo().empty()) { return; }

	// 衝突しているモデルでキャラクターに近い順でソートする
	turretDirRaycast->GetHitModelInfo().sort();

	// キャラクターに一番近いモデルを取得
	HitModelInfo nearModel = turretDirRaycast->GetHitModelInfo().front();

	// レイキャストとモデルの衝突位置をショットの着弾地点にする
	impactPosition = nearModel.hitPosition;
}

/*-------------------------------------------*/
/* 無敵状態の制御
/*-------------------------------------------*/
void CharacterBase::controlInvincibleState()
{
	// 無敵状態のフラグが立っている場合のみ処理を行う
	if (isInvincible)
	{
		// 一定間隔でモデルを点滅させる
		counters.invincible++;
		for (int i = 0; i < PartsType::TYPE_NUM; i++)
		{
			if (counters.invincible % INVINCIBLE_FLASH_INTERVAL == 0)
			{
				MV1SetOpacityRate(tank[i]->GetHandle(), 0.5f);
			}
			else
			{
				MV1SetOpacityRate(tank[i]->GetHandle(), 1.0f);
			}
		}

		// カウントが６０フレームを超えたら、点滅処理を終了させる
		// 無敵状態を解除する
		if (counters.invincible >= INVINCIBLE_COUNT)
		{
			for (int i = 0; i < PartsType::TYPE_NUM; i++)
			{
				// 透明度を０にする
				MV1SetOpacityRate(tank[i]->GetHandle(), 1.0f);
			}

			// カウントを０に初期化して、無敵状態を解除する
			counters.invincible = 0;
			isInvincible = false;
		}
	}
}

/*-------------------------------------------*/
/* 移動の状態を更新
/*-------------------------------------------*/
void CharacterBase::updateMoveState()
{
	// 停止
	if ((int)movingSpeed <= 0)
	{
		moveState = MoveState::STOP;
	}

	// 加速
	else if (isMoveInputting && (int)movingSpeed < (int)MOVING_SPEED_MAX)
	{
		moveState = MoveState::ACCEL;
	}

	// 最高速度
	else if (isMoveInputting && (int)movingSpeed == (int)MOVING_SPEED_MAX)
	{
		moveState = MoveState::SPEED_MAX;
	}

	// 減速
	else if (isMoveInputting == false && (int)movingSpeed != 0)
	{
		moveState = MoveState::DECEL;
	}
}

/*-------------------------------------------*/
/* レイキャストのパラメータを更新
/*-------------------------------------------*/
void CharacterBase::updateRaycastParameter()
{
	// レイキャストの原点を設定
	turretDirRaycast->origin = turretTransform.position;
	// レイキャストの向きを設定
	turretDirRaycast->direction = turretTransform.direction;
	// レイキャストの距離を設定
	turretDirRaycast->distance = TURRET_RAYCAST_DISTANCE;
}

/*-------------------------------------------*/
/* コライダーのパラメータを更新
/*-------------------------------------------*/
void CharacterBase::updateColliderParameter()
{
	// 車体のボックスコライダーの基準位置
	boxCollider->center = bodyTransform.position + COLLIDER_POS_OFFSET;
}

/*-------------------------------------------*/
/* サウンドの再生位置を更新
/*-------------------------------------------*/
void CharacterBase::updateSoundPosition()
{
	// エンジン音
	sounds.engineStart->SetPlayingPosition(bodyTransform.position);		// エンジン：始動
	sounds.engineIdling->SetPlayingPosition(bodyTransform.position);	// エンジン：アイドリング
	sounds.engineAccel->SetPlayingPosition(bodyTransform.position);		// エンジン：加速
	sounds.engineMoving->SetPlayingPosition(bodyTransform.position);	// エンジン：移動中
	sounds.engineDecel->SetPlayingPosition(bodyTransform.position);		// エンジン：減速
	
	// 履帯音
	sounds.crawler->SetPlayingPosition(bodyTransform.position);

	// 砲弾の装填音
	sounds.shotReload->SetPlayingPosition(turretTransform.position);
}

/*-------------------------------------------*/
/* エンジン音の制御
/*-------------------------------------------*/
void CharacterBase::controlEngineSound()
{
	// 停止中
	if (moveState == MoveState::STOP)
	{
		// エンジン音：加速を停止
		sounds.engineAccel->Stop();
		// エンジン音：最高スピードを停止
		sounds.engineMoving->Stop();

		// エンジン音：減速の再生が終了していたなら
		if (sounds.engineDecel->IsPlaying() == false)
		{
			// エンジン音：アイドリングを再生
			sounds.engineIdling->Playing(DX_PLAYTYPE_LOOP);
		}
	}

	// 加速
	else if (moveState == MoveState::ACCEL)
	{
		// エンジン音：アイドリングを停止
		sounds.engineIdling->Stop();
		// エンジン音：最高スピードを停止
		sounds.engineMoving->Stop();
		// エンジン音：減速を停止
		sounds.engineDecel->Stop();

		// エンジン音：加速を再生
		sounds.engineAccel->Playing(DX_PLAYTYPE_BACK);
	}

	// 最高スピード
	else if (moveState == MoveState::SPEED_MAX)
	{
		// エンジン音：アイドリングを停止
		sounds.engineIdling->Stop();
		// エンジン音：減速を停止
		sounds.engineDecel->Stop();

		// エンジン音：加速の再生が終了していたなら
		if (sounds.engineAccel->IsPlaying() == false)
		{
			// エンジン音：移動中を再生
			sounds.engineMoving->Playing(DX_PLAYTYPE_LOOP);
		}
	}

	// 減速
	else if (moveState == MoveState::DECEL)
	{
		// エンジン音：加速を停止
		sounds.engineAccel->Stop();
		// エンジン音：最高スピードを停止
		sounds.engineMoving->Stop();
		// エンジン音：アイドリングを停止
		sounds.engineIdling->Stop();
		
		// エンジン音：減速を再生
		sounds.engineDecel->Playing(DX_PLAYTYPE_BACK);
	}

	// 移動中は履帯音を再生
	if (moveState != MoveState::STOP)
	{
		sounds.crawler->Playing(DX_PLAYTYPE_BACK);
	}
	else
	{
		// 停止中は再生しない
		sounds.crawler->Stop();
	}
}

/*-------------------------------------------*/
/* 衝突コールバック関数：ショットと衝突
/*-------------------------------------------*/
void CharacterBase::onCollisionShot(const CollModelInfo& shot)
{
	// 再生位置をセット
	sounds.damage->SetPlayingPosition(bodyTransform.position);
	// ショットによる被弾音を再生する
	sounds.damage->Playing(DX_PLAYTYPE_BACK);


	// 無敵中だった場合は、無視して関数を抜ける
	if (isInvincible) { return; }

	// HPを減らす
	hitPoint--;

	// HPが残っている場合
	if (hitPoint > 0)
	{
		// ダメージエフェクトを描画
		playEffectForDamage();

		// 無敵フラグをtrueにする
		isInvincible = true;
	}
	// HPが残っていなければ
	else
	{
		// 破壊エフェクトを描画
		playEffectForBrokenExplosion();

		// 状態を"破壊"に変更
		state = State::BROKEN;
	}
}
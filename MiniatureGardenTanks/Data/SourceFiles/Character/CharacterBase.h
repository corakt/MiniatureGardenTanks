#pragma once
#include "../BaseObject/ModelObject.h"
#include "../BaseObject/SoundObject.h"
#include "../Others/ErrorMessage.h"
#include <cassert>

struct CollModelInfo;
class ControllerBase;
class Raycast;
class BoxCollider;
class TerrainBase;

// キャラクターで扱うサウンド
struct CharacterSounds
{
	SoundObject* damage;		// ダメージ

	SoundObject* shotFiring;	// 砲撃音
	SoundObject* shotReload;	// 砲弾の装填
	
	SoundObject* engineStart;	// エンジン：始動
	SoundObject* engineIdling;	// エンジン：アイドリング
	SoundObject* engineAccel;	// エンジン：加速
	SoundObject* engineMoving;	// エンジン：移動中
	SoundObject* engineDecel;	// エンジン：減速
	SoundObject* crawler;		// 履帯音
};

// キャラクターで扱うエフェクト
struct CharacterEffects
{
	int damage;					// ダメージ
	int brokenExplosion;		// 戦車の破壊
	int brokenBlackSmoke;		// 破壊時の黒煙
	int firingExplosion;		// 砲撃時の爆発
	int exhaustGas;				// 排気ガス
	int moveSandSmoke;			// 移動時の土煙
};

// キャラクターで扱うカウンター
struct CharacterCounters
{
	int brokenSmokeEffect;		// 破壊時の黒煙エフェクトで扱うカウント
	int exhaustGasEffect;		// 排気ガスエフェクトで扱うカウント
	int moveSandSmokeEffect;	// 移動時の土煙で扱うカウント
	int engineSound;			// エンジンサウンドを制御する際に扱うカウント
	int invincible;				// 無敵処理で扱うカウント
	int reload;					// リロード時に扱うカウント
};

/*-------------------------------------------*/
/* キャラクター / 基底クラス
/*-------------------------------------------*/
class CharacterBase : public GameObject
{
public:
	// キャラクターの種類
	enum CharacterType
	{
		PLAYER,		// プレイヤー
		ENEMY,		// エネミー
	};

			 CharacterBase(UINT id, CharacterType type);
	virtual ~CharacterBase();

	// キャラクターの部品の種類
	enum PartsType
	{
		BODY,				// 車体
		TURRET,				// 砲塔

		TYPE_NUM,			// 部品の種類数
	};

	// キャラクターの状態
	enum State
	{
		NORMAL,				// 通常
		ACTIVEGAUGE_FULL,	// アクティブゲージ満タン
		SKILL_ACTIVE,		// スキル発動
		BROKEN,				// 破壊
	};

	// ショット発射時の反動の状態
	enum FiringReactionState
	{
		NON_REACTION,		// 非反動時
		REACTION_START,		// 開始
		ATTENUATION,		// 減衰
		END,				// 終了
	};

	// 移動の状態
	enum MoveState
	{
		STOP,		// 停止
		SPEED_MAX,	// 最高スピード
		ACCEL,		// 加速
		DECEL,		// 減速
	};
	
	virtual void Initialize() = 0;		// 初期化
	virtual void Update()     = 0;		// 更新
	virtual void Draw()       = 0;		// 描画
	virtual void DrawUi()     = 0;		// UIの描画

	// 砲塔の相対位置を算出
	void CalculationTurretRelativePosition();

	// アクティブポイントの加算
	void AddActivePoint(int addNum);

	// スキルを発動
	virtual void ActivateSkill() = 0;

	// コールバック関数群
	virtual void OnFiringShot() = 0;		// 砲弾を発射したとき
	virtual void OnCharacterHitShot() = 0;	// 砲弾がキャラクターにヒットしたとき

	// キャラクターID
	const int& GetId      () const          { return id; }
	void       SetId      (  const int set) { id = set;  }

	// キャラクターHP
	const int& GetHitPoint() const          { return hitPoint; }
	void       SetHitPoint(  const int set) { hitPoint = set;  }

	// アクティブポイント
	const int& GetActivePoint   () const		  { return activePoint; }
	void       SetActivePoint   (  const int set) { activePoint = set;  }

	// 移動スピード
	const float& GetMovingSpeed   () const		      { return movingSpeed; }
	void         SetMovingSpeed   (  const float set) { movingSpeed = set;  }

	// 発射するショットの数を取得
	const int& GetFiringShotNum() const { return firingShotNum; }

	// リロード中かどうか
	const bool& GetReloadFlag() const           { return isReload; }
	void        SetReloadFlag(  const bool set) { isReload = set;  }

	// 反動の処理の際中か
	const bool& GetReactionFlag() const           { return isReaction; }
	void		SetReactionFlag(  const bool set) { isReaction = set;  }

	// エンジンサウンドの再生を行うかどうか
	const bool& GetEngineSoundPlayingFlag() const           { return isEngineSoundPlaying; }
	void		SetEngineSoundPlayingFlag(  const bool set) { isEngineSoundPlaying = set;  }

	// 制御中かどうか
	const bool& GetControlingFlag() const           { return isControling; }
	void		SetControlingFlag(  const bool set) { isControling = set;  }

	// 移動の操作中か
	void SetMoveInputtingFlag(const bool set) { isMoveInputting = set; }

	// 砲塔の先端部分の座標
	const VECTOR& GetTurretTipPos() const { return turretTipPosition; }

	// 砲弾の着弾地点
	void SetImpactPos(const VECTOR set) { impactPosition = set; }

	// キャラクターの種類
	const CharacterType& GetObjectType() const { return type; }

	// キャラクターの状態
	const State& GetState() const { return state; }

	// ショット発射時の反動の状態
	const FiringReactionState& GetReactionState() const							 { return reactionState; }
	void					   SetReactionState(  const FiringReactionState set) { reactionState = set ; }

	// ショット発射時の反動力
	const float& GetCurrentReactionForce() const            { return currentReactionForce; }
	void		 SetCurrentReactionForce(  const float set) { currentReactionForce = set;  }

	// キャラクターHPの最大値を取得
	const int& GetHitPointMax() const { return HITPOINT_MAX; }
	// キャラクターのアクティブポイントの最大値を取得
	const int& GetActivePointMax() const { return ACTIVEPOINT_MAX; }

	// 移動の最高スピードを取得
	const float& GetMovingSpeedMax() const { return MOVING_SPEED_MAX; }

	// 移動の加速率を取得
	const float& GetMovingAccelRate() const { return MOVING_ACCEL_RATE; }

	// 移動の減速率を取得
	const float& GetMovingDecelRate() const { return MOVING_DECEL_RATE; }

	// ショット発射時の反動力の最大値
	const float& GetReactionForceMax() const { return REACTION_FORCE_MAX; }
	// 反動力の減衰率
	const float& GetReactionAttenuation() const { return REACTION_ATTENUATION; }

	// モデルの色を取得
	const COLOR_F GetModelColor() const
	{ 
		return MV1GetDifColorScale(tank[PartsType::BODY]->GetHandle());
	}
	// モデルの色をセット
	void SetModelColor(  const COLOR_F set)
	{
		// キャラクターのオブジェクトが一つでもNULLであれば中断
		assert(tank[PartsType::BODY]   != NULL);
		assert(tank[PartsType::TURRET] != NULL);

		// 車体
		MV1SetDifColorScale(tank[PartsType::BODY]->GetHandle(), set);
		MV1SetEmiColorScale(tank[PartsType::BODY]->GetHandle(), set);

		// 砲塔
		MV1SetDifColorScale(tank[PartsType::TURRET]->GetHandle(), set);
		MV1SetEmiColorScale(tank[PartsType::TURRET]->GetHandle(), set);
	}

	// キャラクターの部品の取得
	ModelObject* GetParts(const PartsType type) const
	{
		// キャラクターのオブジェクトが一つでもNULLであれば中断
		assert(tank[PartsType::BODY]   != NULL);
		assert(tank[PartsType::TURRET] != NULL);

		     if (type == PartsType::BODY)   { return tank[PartsType::BODY];   }
		else if (type == PartsType::TURRET) { return tank[PartsType::TURRET]; }

		else
		{
			ErrorData errorData;
			errorData.errorMessage           = "キャラクターの部品を取得する関数で、引数に不正なデータを指定しています。";
			errorData.detectedErrorClassName = "CharacterBase";

			// エラーデータをリストに挿入する
			ERROR_MESSAGE.SetErrorData(errorData);

			return NULL;
		}
	}

	// エンジン音を停止する
	void StopEngineSound()
	{
		sounds.engineStart->Stop();		// 始動
		sounds.engineIdling->Stop();	// アイドリング
		sounds.engineAccel->Stop();		// 加速
		sounds.engineMoving->Stop();	// 最高スピード
		sounds.engineDecel->Stop();		// 減速
	}

protected:
	// 継承クラス共通の処理を行う関数群
	void InitializeCommonParameter();		// 共通のパラメータを初期化
	void UpdateCommonParameter();			// 共通のパラメータを更新
	void DrawCommonModel();					// 共通のモデルを描画

	// 各エフェクトの再生
	void PlayEffectForDamage();						// ダメージ
	void PlayEffectForBrokenExplosion();			// 戦車の破壊
	void PlayEffectForBrokenBlackSmoke();			// 破壊後の黒煙
	void PlayEffectForFiringExplosion();			// 砲撃時の爆発
	void PlayEffectForExhaustGas();					// 排気ガス
	void PlayEffectforMoveSandSmoke();				// 移動時の土煙

	// 衝突コールバック関数
	void OnCollisionShot(const CollModelInfo& shot);// ショットと衝突

	UINT  id;										// キャラクターID
	int   hitPoint;									// キャラクターのヒットポイント
	int   activePoint;								// キャラクターのアクティブポイント
	int   firingShotNum;							// 発射するショットの数
	int   currentReloadTime;						// 現在のリロード時間
	float movingSpeed;								// 移動スピード
	bool  isInvincible;								// 無敵状態かどうか
	bool  isReload;									// リロード中かどうか
	bool  isReaction;								// 反動の処理の最中かどうか
	bool  isEngineSoundPlaying;						// エンジンサウンドの再生を行うかどうか
	bool  isControling;								// 制御中かどうか
	bool  isMoveInputting;							// 移動の操作中か

	ModelObject* tank[PartsType::TYPE_NUM];			// キャラクター（戦車）の部品

	Transform bodyTransform;						// 車体のトランスフォーム
	Transform turretTransform;						// 砲塔のトランスフォーム

	Raycast*     turretDirRaycast;					// 砲塔の向きと同じレイキャスト
	BoxCollider* boxCollider;						// 車体のボックスコライダー

	CharacterType type;								// キャラクターの種類
	
	State     state;								// キャラクターの状態
	MoveState moveState;							// 移動の状態

	VECTOR turretTipPosition;						// 砲塔の先端の座標
	VECTOR impactPosition;							// 砲弾の着弾地点
	ModelType impactModelType;						// 着弾地点のモデルの種類

	FiringReactionState reactionState;				// ショット発射時の反動の状態
	float currentReactionForce;						// 現在の反動力

	CharacterSounds  sounds;						// サウンド

	CharacterEffects effects;						// エフェクト
	CharacterCounters counters;						// カウンター

	static const int    HITPOINT_MAX;				// ヒットポイントの最大値
	static const int    ACTIVEPOINT_MAX;			// アクティブポイントの最大値
	static const float  MOVING_SPEED_MAX;			// 移動の最高スピード
	static const float  MOVING_ACCEL_RATE;			// 移動の加速率
	static const float  MOVING_DECEL_RATE;			// 移動の減速率
	static const int    SKILL_FIRINGSHOT_NUM;		// スキル発動時の発射するショットの数
	static const VECTOR MODEL_SCALE;				// モデルのスケール
	static const VECTOR COLLISION_SIZE;				// 衝突判定の大きさ
	static const VECTOR COLLIDER_POS_OFFSET;		// コライダーの位置のオフセット
	static const int    INVINCIBLE_FLASH_INTERVAL;	// 無敵中のモデルの点滅間隔
	static const int    INVINCIBLE_COUNT;			// 無敵時間
	static const int    RELOAD_TIME;				// リロード時間
	static const float  REACTION_FORCE_MAX;			// ショット発射時の反動力
	static const float  REACTION_ATTENUATION;		// 反動の減衰率
	static const float  TURRET_RAYCAST_DISTANCE;	// 砲塔のレイキャストの距離

private:
	void CalculationPartsDirection();				// 各部品の向きベクトルを算出
	void CalculationTurretTipPos();					// 砲塔の先端の座標を算出
	void CalculationImpactPosition();				// ショットの着弾地点の算出
	void UpdateInvincibleState();					// 無敵状態を更新
	void UpdateCharacterState();					// キャラクターの状態を更新
	void UpdateReloadState();						// リロードの状態を更新
	void UpdateMoveState();							// 移動の状態を更新
	void UpdateFiringShotNum();						// 発射するショットの数を更新
	void UpdateActivePoint();						// アクティブポイントを更新
	void UpdateRaycastParameter();					// レイキャストのパラメータを更新
	void UpdateColliderParameter();					// コライダーのパラメータを更新
	void UpdateSoundPosition();						// サウンドの再生位置を更新
	void UpdateEngineSound();						// エンジン音を更新
	void CallingCollisionCallback();				// 衝突時のコールバック関数を呼ぶ
};
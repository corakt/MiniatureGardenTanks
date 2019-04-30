#pragma once
#include "../BaseObject/GameObject.h"
#include <EffekseerForDXLib.h>
#include <functional>

class CharacterBase;
class ModelObject;
class TerrainBase;
class BoxCollider;
struct CollModelInfo;

// ショットで扱うエフェクト
struct ShotEffects
{
	int impactExplosion;	// 着弾時の爆発
	int boostSmoke;			// 後ろから出る煙
};

// ショットで扱うカウンター
struct ShotCounters
{
	int boostEffect;		// 煙エフェクト時に扱うカウント
};

/*-------------------------------------------*/
/* キャラクターショット
/*-------------------------------------------*/
class CharacterShot : public GameObject
{
public:
			 CharacterShot();		// コンストラクタ
	virtual ~CharacterShot();		// デストラクタ

	virtual void Initialize();		// 初期化
	virtual void Update();			// 更新
	virtual void Draw();			// 描画

	// キャラクターにショットがヒットした際のコールバック関数
	std::function<void(void)> OnCharacterHitShot;

	// ショットを撃ったキャラクターのオブジェクトID
	const std::uint32_t& GetFiringCharacterObjectId()        { return firingCharacterObjectId; }
	void SetFiringCharacterObjectId(const std::uint32_t set) { firingCharacterObjectId = set;  }

private:
	// エフェクトの再生
	void PlayEffectForShotBoost();			// ショットの後ろから出る煙
	void PlayEffectForImpactExplosion();	// ショット着弾時の爆発

	// 衝突コールバック関数群
	void OnCollisionCharacter(const CollModelInfo& character);		// キャラクター
	void OnCollisionTerrainWall(const CollModelInfo& terrainWall);	// ステージの壁

	ModelObject*  shotModel;				// ショットのモデル

	std::uint32_t firingCharacterObjectId;	// ショットを撃ったキャラクターのオブジェクトID

	ShotEffects  effects;					// エフェクト
	ShotCounters counters;					// カウンター
											
	BoxCollider* boxCollider;				// ボックスコライダー
											
	static const float  SPEED;				// ショットのスピード
	static const VECTOR COLLISION_SIZE;		// 衝突範囲のサイズ
};
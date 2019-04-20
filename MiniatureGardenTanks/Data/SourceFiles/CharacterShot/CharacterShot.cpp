#include "../CharacterShot/CharacterShot.h"
#include "../Collision/CollisionManager.h"
#include "../Collision/BoxCollider.h"
#include "../ResourcesManager/ResourceEffectManager.h"
#include "../ResourcesManager/ResourceSoundManager.h"
#include "../Others/Define.h"

const float  CharacterShot::SPEED          = 400;					// ショットのスピード
const VECTOR CharacterShot::COLLISION_SIZE = VGet(150, 100, 150);	// 衝突範囲のサイズ

/*-------------------------------------------*/
/* コンストラクタ
/*-------------------------------------------*/
CharacterShot::CharacterShot(int modelHandle,ModelType type)
	:ModelObject::ModelObject(modelHandle,type)
{
	// ボックスコライダーを生成
	boxCollider = COLLISION_MANAGER.AddBoxCollider();
}

/*-------------------------------------------*/
/* デストラクタ
/*-------------------------------------------*/
CharacterShot::~CharacterShot()
{
	
}

/*-------------------------------------------*/
/* 初期化
/*-------------------------------------------*/
void CharacterShot::Initialize()
{
	// 各変数を初期化
	transform.position  = ZERO_VECTOR;				// 現在の座標
	transform.direction = VGet(0, 0, 1);			// 現在の向き
	transform.rotation  = ZERO_VECTOR;				// 現在の回転角
	transform.scale     = VGet(0.5f,0.5f,0.5f);		// 現在の大きさ
	isActive			= false;					// アクティブ中か否か

	firingCharacterObjectId = 0;					// ショットを撃ったキャラクターのオブジェクトID

	// エフェクトハンドルを初期化
	effects.impactExplosion = -1;		// 着弾時の爆発
	effects.boostSmoke      = -1;		// 後ろから出る煙

	// カウンターの初期化
	counters.boostEffect = -1;			// 煙エフェクト時に扱うカウント

	// コライダー
	boxCollider->center        = transform.position;	// ボックスコライダーの基準位置
	boxCollider->size          = COLLISION_SIZE;		// コライダーのサイズ
	boxCollider->attachedModel = this;					// コライダーにアタッチするモデル
	boxCollider->isCollCheck   = true;
}

/*-------------------------------------------*/
/* 更新
/*-------------------------------------------*/
void CharacterShot::Update()
{
	// ボックスコライダーの基準位置を更新
	boxCollider->center = transform.position;

	if (isActive)
	{
		// ショットの生成時に飛ばす
		transform.velocity = VScale(transform.direction, SPEED);			// 速度を算出
		transform.position = VAdd(transform.position, transform.velocity);	// 速度を座標に加える

		// 各エフェクトの再生
		playEffectForShotBoost();		// ショットの後ろから出る煙
		
		// モデルが衝突した際のコールバック関数を呼ぶ
		if (boxCollider->GetCollModelInfo().empty() == false)
		{
			for (const CollModelInfo& collModelInfoElem : boxCollider->GetCollModelInfo())
			{
				// 衝突したモデルを取得
				ModelObject* collModel = collModelInfoElem.collModel;
				// オブジェクトがNULLだったら、スキップして次の要素へ
				if (collModel == NULL) { continue; }

				// キャラクタと衝突
				if (collModel->GetModelType() == ModelType::TANK_BODY)
				{
					// コールバック関数
					onCollisionCharacter(collModelInfoElem);
				}
				// ステージの壁と衝突
				else if (collModel->GetModelType() == ModelType::TERRAIN_WALL)
				{
					// コールバック関数
					onCollisionTerrainWall(collModelInfoElem);
				}
			}
		}
	}
}

/*-------------------------------------------*/
/* 描画
/*-------------------------------------------*/
void CharacterShot::Draw()
{
	if (isActive)
	{
		DrawModel();
	}
}

/*-------------------------------------------*/
/* エフェクトの再生：ショットの後ろから出る煙
/*-------------------------------------------*/
void CharacterShot::playEffectForShotBoost()
{
	// エフェクトのパラメータ
	const VECTOR BOOSTEFFECT_SCALE = VGet(0.6f, 0.6f, 0.6f);

	// 後ろから出る煙エフェクトを描画
	effects.boostSmoke = PlayEffekseer3DEffect(EFFECT_MANAGER.GetHandle(ResourceEffectManager::EffectType::TANK_SHOTSMOKE));	// エフェクトハンドルをセット
	SetPosPlayingEffekseer3DEffect(effects.boostSmoke, transform.position.x, transform.position.y, transform.position.z);		// 座標をセット
	SetRotationPlayingEffekseer3DEffect(effects.boostSmoke, 0, transform.rotation.y, 0);										// 回転角をセット
	SetScalePlayingEffekseer3DEffect(effects.boostSmoke, BOOSTEFFECT_SCALE.x, BOOSTEFFECT_SCALE.y, BOOSTEFFECT_SCALE.z);		// スケールをセット
}

/*-------------------------------------------*/
/* エフェクトの再生：ショット着弾時の爆発
/*-------------------------------------------*/
void CharacterShot::playEffectForImpactExplosion()
{
	// エフェクトのパラメータ
	const VECTOR SCALE = VGet(2, 2, 2);

	// 再生するエフェクトをセット
	effects.impactExplosion = PlayEffekseer3DEffect(EFFECT_MANAGER.GetHandle(ResourceEffectManager::TANK_IMPACTEXPLOSION));
	// 再生位置をセット
	SetPosPlayingEffekseer3DEffect(effects.impactExplosion, transform.position.x, transform.position.y, transform.position.z);
	// 再生スケールをセット
	SetScalePlayingEffekseer3DEffect(effects.impactExplosion, SCALE.x, SCALE.y, SCALE.z);
}

/*-------------------------------------------*/
/* 衝突コールバック関数：キャラクター
/*-------------------------------------------*/
void CharacterShot::onCollisionCharacter(const CollModelInfo& character)
{
	// ショットに衝突したキャラクターが
	// 撃ったキャラクターと同一であれば、無視してそのまま関数を抜ける
	if (character.collModel->GetObjectId() == firingCharacterObjectId) { return; }

	// 衝突したモデルのコライダーがボックス以外だったら、無視してそのまま関数を抜ける
	if (character.colliderType != CollisionManager::ColliderType::BOX) { return; }

	// ショットがキャラクターにヒットした際のコールバック関数を呼ぶ
	OnCharacterHitShot();

	// 稼働フラグをfalseにする
	isActive = false;
	// 描画フラグをオフにする
	isDraw = false;
}

/*-------------------------------------------*/
/* 衝突コールバック関数：ステージの壁
/*-------------------------------------------*/
void CharacterShot::onCollisionTerrainWall(const CollModelInfo& terrainWall)
{
	// 壁の着弾音を取得
	int shotImpactSound = SOUND_MANAGER.GetHandle(ResourceSoundManager::SoundType::SE_TANK_SHOTIMPACT);
	// 再生位置をセット
	SetNextPlay3DPositionSoundMem(transform.position, shotImpactSound);
	// 可聴範囲をセット
	SetNextPlay3DRadiusSoundMem(30000, shotImpactSound);
	// 再生ボリュームを設定
	ChangeNextPlayVolumeSoundMem(220, shotImpactSound);
	// サウンドを再生する
	PlaySoundMem(shotImpactSound,DX_PLAYTYPE_BACK);

	// ショット着弾時の爆発エフェクトを再生
	playEffectForImpactExplosion();

	// 稼働フラグをfalseにする
	isActive = false;
	// 描画フラグをオフにする
	isDraw = false;
}
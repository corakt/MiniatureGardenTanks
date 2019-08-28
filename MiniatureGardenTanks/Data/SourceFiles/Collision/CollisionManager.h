#pragma once
#include "../Others/Singleton.h"
#include "../BaseObject/Transform.h"
#include "../ResourcesManager/ResourceModelManager.h"
#include <list>
#include <utility>

class ModelObject;
class BoxCollider;
class SphereCollider;
class CircularSectorCollider;

/*-------------------------------------------*/
/* コライダー / 管理クラス
/*-------------------------------------------*/
class CollisionManager final : public Singleton<CollisionManager>
{
	friend class Singleton<CollisionManager>;

public:
			 CollisionManager();	// コンストラクタ
	virtual ~CollisionManager();	// デストラクタ

	// コライダーの種類
	enum ColliderType
	{
		BOX,				// ボックス
		SPHERE,				// スフィア
		CIRCULAR_SECTOR,	// サーキュラーセクター（扇形）
	};

	BoxCollider*            AddBoxCollider();				// ボックスコライダーを追加
	SphereCollider*         AddSphereCollider();			// スフィアコライダーを追加
	CircularSectorCollider* AddCircularSectorCollider();	// 扇形のコライダーを追加

	void Initialize();			// 初期化
	void Update();				// 更新

	void Finalize();		// 終了処理

private:
	// 衝突判定関数群
	void collisionCheckBoxAndBox();				// ボックスとボックス
	void collisionCheckSphereAndSphere();		// スフィアとスフィア
	void collisionCheckBoxAndSphere();			// ボックスとスフィア
	void collisionCheckCircularSectorAndBox();	// 扇形とボックス

	// ボックス同士が衝突した際の処理
	void onCollisionBoxAndBox(BoxCollider* collider1,BoxCollider* collider2);

	BoxCollider**            boxCollider;				// ボックスコライダー
	SphereCollider**         sphereCollider;			// スフィアコライダー
	CircularSectorCollider** circularSectorCollider;	// 扇形のコライダー

	std::list<BoxCollider*>            activeBoxCollider;				// 稼働中のボックスコライダー
	std::list<SphereCollider*>         activeSphereCollider;			// 稼働中のスフィアコライダー
	std::list<CircularSectorCollider*> activeCircularSectorCollider;	// 稼働中の扇形のコライダー

	static const int BOX_COLLIDER_MAX;						// ボックスコライダーの最大数
	static const int SPHERE_COLLIDER_MAX;					// スフィアコライダーの最大数
	static const int CIRCULARSECTOR_COLLIDER_MAX;			// 扇形のコライダーの最大数
};
#define COLLISION_MANAGER CollisionManager::GetInstance()

// モデルの衝突情報
struct CollModelInfo
{
	ModelObject* collModel;							// 衝突したモデルのポインタ
	CollisionManager::ColliderType colliderType;	// 衝突したモデルが使用していたコライダーの種類
};
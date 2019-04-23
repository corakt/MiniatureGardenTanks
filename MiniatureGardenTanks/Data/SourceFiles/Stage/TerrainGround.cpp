#include "../Stage/TerrainGround.h"
#include "../Collision/CollisionManager.h"
#include "../Collision/BoxCollider.h"

/*-------------------------------------------*/
/* コンストラクタ
/*-------------------------------------------*/
TerrainGround::TerrainGround()
	:TerrainBase::TerrainBase()
{
	terrainModel = NULL;
	boxCollider  = NULL;

	// モデルコンポーネントを生成
	terrainModel = AddComponent<ModelObject>();
	// モデルのハンドルを設定
	terrainModel->SetHandle(MODEL_MANAGER.GetHandle(ResourceModelManager::ModelType::TERRAIN_GROUND));
	// モデルの種類を設定
	terrainModel->SetObjectType(ObjectType::TERRAIN_GROUND);

	// ボックスコライダーを生成
	boxCollider = COLLISION_MANAGER.AddBoxCollider();
}

/*-------------------------------------------*/
/* デストラクタ
/*-------------------------------------------*/
TerrainGround::~TerrainGround()
{
	// 処理なし
}

/*-------------------------------------------*/
/* 初期化
/*-------------------------------------------*/
void TerrainGround::Initialize()
{
	// 共通の初期化処理
	commonInitialize();
}

/*-------------------------------------------*/
/* 更新
/*-------------------------------------------*/
void TerrainGround::Update()
{
	boxCollider->center = transform.position;
	boxCollider->size = VGet(1500, 10, 1500);
	//boxCollider->attachedModel = this;
}
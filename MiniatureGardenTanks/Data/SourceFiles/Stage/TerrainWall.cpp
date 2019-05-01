#include "../Stage/TerrainWall.h"
#include "../Raycast/RaycastManager.h"
#include "../Collision/BoxCollider.h"
#include "../Collision/CollisionManager.h"

// 衝突判定のサイズ
const VECTOR TerrainWall::COLLISION_SIZE = VGet(1500,1500,1500);

/*-------------------------------------------*/
/* コンストラクタ
/*-------------------------------------------*/
TerrainWall::TerrainWall()
	:TerrainBase::TerrainBase()
{	
	// モデルコンポーネントを生成
	terrainModel = AddComponent<ModelObject>();
	// モデルのハンドルを設定
	terrainModel->SetHandle(MODEL_MANAGER.GetHandle(ResourceModelManager::ModelType::TERRAIN_WALL));
	// モデルの種類を設定
	terrainModel->SetObjectType(ObjectType::TERRAIN_WALL);

	// ボックスコライダーを生成
	boxCollider = COLLISION_MANAGER.AddBoxCollider();
}

/*-------------------------------------------*/
/* デストラクタ
/*-------------------------------------------*/
TerrainWall::~TerrainWall()
{
	
}

/*-------------------------------------------*/
/* 初期化
/*-------------------------------------------*/
void TerrainWall::Initialize()
{
	// 共通の初期化パラメータ
	InitializeCommonParameter();
}

/*-------------------------------------------*/
/* 更新
/*-------------------------------------------*/
void TerrainWall::Update()
{
	boxCollider->center = transform.position;
	boxCollider->size = COLLISION_SIZE;
	//boxCollider->attachedModel = this;
}
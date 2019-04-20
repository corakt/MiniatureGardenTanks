#include "../Stage/TerrainWall.h"
#include "../Raycast/RaycastManager.h"
#include "../Collision/BoxCollider.h"
#include "../Collision/CollisionManager.h"

// 衝突判定のサイズ
const VECTOR TerrainWall::COLLISION_SIZE = VGet(1500,1500,1500);

/*--------------6-----------------------------*/
/* コンストラクタ
/*-------------------------------------------*/
TerrainWall::TerrainWall(int modelHandle, TerrainId terrainId, ModelType type)
	:TerrainBase::TerrainBase(modelHandle,terrainId,type)
{
	// レイキャストの対象のモデルとして登録
	RAYCAST_MANAGER.SetTargetModel(this);
	
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
	// 共通の初期化処理
	commonInitialize();
}

/*-------------------------------------------*/
/* 更新
/*-------------------------------------------*/
void TerrainWall::Update()
{
	boxCollider->center = transform.position;
	boxCollider->size = COLLISION_SIZE;
	boxCollider->attachedModel = this;
}
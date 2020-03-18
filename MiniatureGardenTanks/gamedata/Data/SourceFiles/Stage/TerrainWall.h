#pragma once
#include "../Stage/TerrainBase.h"

class BoxCollider;

/*-------------------------------------------*/
/* 地形：地面 / 派生クラス
/*-------------------------------------------*/
class TerrainWall final : public TerrainBase
{
public:
	         TerrainWall(int modelHandle, TerrainId terrainId,ModelType type);		// コンストラクタ
	virtual ~TerrainWall();															// デストラクタ

	void     Initialize()override;			// 初期化
	void     Update()    override;			// 更新

private:
	BoxCollider* boxCollider;				// ボックスコライダー
	static const VECTOR	COLLISION_SIZE;		// 衝突判定の大きさ
};
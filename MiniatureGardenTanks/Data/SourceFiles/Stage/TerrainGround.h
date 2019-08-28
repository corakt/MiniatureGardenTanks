#pragma once
#include "../Stage/TerrainBase.h"

class BoxCollider;

/*-------------------------------------------*/
/* 地形：地面 / 派生クラス
/*-------------------------------------------*/
class TerrainGround final : public TerrainBase
{
public:
	         TerrainGround(int modelHandle, TerrainId terrainId, ModelType type);	// コンストラクタ
	virtual ~TerrainGround();														// デストラクタ

	void     Initialize()override;		// 初期化
	void     Update()    override;		// 更新

private:
	BoxCollider* boxCollider;				// ボックスコライダー
};
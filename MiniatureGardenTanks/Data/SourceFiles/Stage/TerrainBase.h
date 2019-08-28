#pragma once
#include "../BaseObject/ModelObject.h"

class CharacterShot;

// 地形ID
class TerrainId final
{
public:
	int x;
	int z;

	// IDの比較
	bool operator==(const TerrainId& right)
	{
		if (x == right.x && z == right.z)
		{
			return true;
		}
		return false;
	}

	// IDの加算
	TerrainId operator+(const TerrainId& right)
	{
		TerrainId ret;
		ret.x = x + right.x;
		ret.z = z + right.z;
		return ret;
	}
};

/*-------------------------------------------*/
/* 地形 / 基底クラス
/*-------------------------------------------*/
class TerrainBase : public ModelObject
{
public:
			 TerrainBase(int modelHandle, TerrainId terrainId,ModelType type);	// コンストラクタ
	virtual ~TerrainBase();														// デストラクタ

	virtual void Initialize() = 0;		// 初期化
	virtual void Update()     = 0;		// 更新
	virtual void Draw();				// 描画
	
	// 地形ID
	const TerrainId& GetId() const                { return id; }
	void             SetId(  const TerrainId set) { id = set;  }

protected:
	void commonInitialize();			// 継承クラス共通の初期化
	TerrainId        id;				// 地形ID
};
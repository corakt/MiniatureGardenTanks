#pragma once
#include <DxLib.h>
#include <list>
#include "../ResourcesManager/ResourceModelManager.h"

class ModelObject;

// ヒットしたモデルの情報
class HitModelInfo
{
public:
	ModelObject* hitModel;		// ヒットしたモデル
	VECTOR    hitPosition;		// ヒットした位置
	float     distance;			// ヒットした際の原点からの距離

	// 原点からの距離で比較する
	bool operator<(const HitModelInfo& right) const
	{
		return distance < right.distance;
	}
};

/*-------------------------------------------*/
/* レイキャスト
/*-------------------------------------------*/
class Raycast final
{
public:
	 Raycast(VECTOR origin,VECTOR direction,float distance);				// コンストラクタ
	~Raycast();																// デストラクタ

	// レイにヒットしたモデルの情報をセットする
	void SetHitModelInfo(HitModelInfo hitModelInfo);

	// モデルの情報を格納しているリストを削除
	void ClearHitModelInfo();

	VECTOR origin;					// レイの原点
	VECTOR direction;				// レイの向き
	float  distance;				// レイの距離
	ModelObject* attachedModel;		// レイキャストにアタッチするモデル

	// レイにヒットしたモデルを取得
	std::list<HitModelInfo>& GetHitModelInfo() { return hitModelInfo; }

private:
	std::list<HitModelInfo> hitModelInfo;			// レイにヒットしたモデルの情報
};
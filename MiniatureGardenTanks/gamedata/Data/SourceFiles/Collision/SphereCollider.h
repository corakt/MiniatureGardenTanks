#pragma once
#include "../Collision/CollisionManager.h"
#include <vector>
#include <DxLib.h>

class ModelObject;

/*-------------------------------------------*/
/* スフィアコライダー
/*-------------------------------------------*/
class SphereCollider final 
{
public:
	 SphereCollider();		// コンストラクタ
	~SphereCollider();		// デストラクタ

	VECTOR       center;			// 衝突判定範囲の中心位置
	float        radius;			// 衝突判定範囲の半径
	ModelObject* attachedModel;		// コライダーにアタッチするモデル
	bool         isCollCheck;		// 衝突判定を行うかどうか

	// 衝突したモデルを取得
	const std::vector<CollModelInfo>& GetCollModelInfo() const { return collModelInfo; }
	// 衝突したモデルをセット
	void SetCollModelInfo(CollModelInfo set) { collModelInfo.push_back(set); }
	// 衝突したモデルのリストを削除
	void ClearCollModelVector() { collModelInfo.clear(); }

private:
	std::vector<CollModelInfo> collModelInfo;	// 衝突したモデルの情報
};
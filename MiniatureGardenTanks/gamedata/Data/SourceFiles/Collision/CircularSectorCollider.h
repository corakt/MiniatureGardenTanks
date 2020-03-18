#pragma once
#include "../Collision/CollisionManager.h"
#include <DxLib.h>
#include <vector>

class ModelObject;

/*-------------------------------------------*/
/* サーキュラーセクターコライダー（扇形）
/*-------------------------------------------*/
class CircularSectorCollider final
{
public:
	 CircularSectorCollider();			// コンストラクタ
	~CircularSectorCollider();			// デストラクタ

	VECTOR       center;			// 衝突判定範囲の中心位置
	VECTOR       direction;			// 衝突範囲の向き
	float        width;				// 衝突範囲の幅
	float        distance;			// 衝突範囲の距離
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
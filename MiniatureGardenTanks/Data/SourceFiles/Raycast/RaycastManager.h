#pragma once
#include "../Others/Singleton.h"
#include <list>
#include <DxLib.h>

class Raycast;
class ModelObject;

/*-------------------------------------------*/
/* レイキャスト / 管理クラス（シングルトン）
/*-------------------------------------------*/
class RaycastManager final : public Singleton<RaycastManager>
{
	friend class Singleton<RaycastManager>;

public:
			 RaycastManager();		// コンストラクタ
	virtual ~RaycastManager();		// デストラクタ

	// レイキャストを追加
	Raycast* AddRaycast(VECTOR origin, VECTOR direction, float distance);
	// レイキャストの対象のモデルをセット
	void SetTargetModel(ModelObject* model);

	void Initialize();		// 初期化
	void Update();			// 更新
	void Finalize();		// 終了処理

private:
	Raycast** raycast;						// レイキャスト

	std::list<Raycast*>     activeRaycast;	// 稼働中のレイキャスト
	std::list<ModelObject*> targetModel;	// 対象のモデル

	static const int RAYCAST_MAX;			// レイキャストの最大数
};

#define RAYCAST_MANAGER RaycastManager::GetInstance()
#pragma once
#include "../Others/Singleton.h"
#include <list>
#include <random>

/*-------------------------------------------*/
/* オブジェクトの識別IDを管理（シングルトン）
/*-------------------------------------------*/
class ObjectIdManager final : public Singleton<ObjectIdManager>
{
	friend class Singleton<ObjectIdManager>;

public:
			 ObjectIdManager();		// コンストラクタ
	virtual ~ObjectIdManager();		// デストラクタ

	// 初期化
	void Initialize();				// 初期化

	// オブジェクトIDを生成
	std::uint32_t CreateId();
	// オブジェクトIDを削除
	void DestroyId(std::uint32_t& id);

private:
	std::random_device randomDevice;	// ランダムデバイス
	std::mt19937       mt19937;			// メルセンヌ・ツイスト乱数生成器

	std::list<std::uint32_t> idList;	// IDのリスト
};

#define OBJECTID_MANAGER ObjectIdManager::GetInstance()
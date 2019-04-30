#pragma once
#include "../Others/Singleton.h"
#include <list>
#include <random>

/*-------------------------------------------*/
/* IDの生成クラス
/*-------------------------------------------*/
class IdGenerator final : public Singleton<IdGenerator>
{
	friend class Singleton<IdGenerator>;

public:
			 IdGenerator();		// コンストラクタ
	virtual ~IdGenerator();		// デストラクタ

	// IDを生成
	std::uint32_t Generate();
	// IDを削除
	void Delete(std::uint32_t& id);

private:
	std::random_device randomDevice;		// ランダムデバイス
	std::mt19937       randomGenerator;		// メルセンヌ・ツイスト乱数生成器

	std::list<std::uint32_t> idList;	// IDのリスト
};

#define ID_GENERATOR IdGenerator::GetInstance()
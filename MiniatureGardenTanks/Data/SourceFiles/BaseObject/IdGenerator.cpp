#include "../BaseObject/IdGenerator.h"

/*-------------------------------------------*/
/* コンストラクタ
/*-------------------------------------------*/
IdGenerator::IdGenerator()
{
	// 乱数生成器の初期シードを設定
	mt19937.seed(randomDevice());
}

/*-------------------------------------------*/
/* デストラクタ
/*-------------------------------------------*/
IdGenerator::~IdGenerator()
{

}

/*-------------------------------------------*/
/* IDを生成
/*-------------------------------------------*/
std::uint32_t IdGenerator::Generate()
{
	// 一時保存ID
	std::uint32_t tmpId = 0;
	// イテレータ
	std::list<std::uint32_t>::iterator idIterator;

	do
	{
		// IDとして扱うランダム値を生成
		tmpId = mt19937();

		// 生成されたIDが重複していないか調べる
		idIterator = std::find(idList.begin(), idList.end(), tmpId);
	}
	// IDが重複していた場合は生成し直す
	while (idIterator != idList.end());

	// 重複していなければ、そのIDをリストに追加する
	idList.push_back(tmpId);

	// IDを戻り値として返す
	return tmpId;
}

/*-------------------------------------------*/
/* IDを削除
/*-------------------------------------------*/
void IdGenerator::Delete(uint32_t& id)
{
	// 指定されたIDをリストから削除する
	idList.remove(id);
	// IDを０クリア
	id = 0;
}
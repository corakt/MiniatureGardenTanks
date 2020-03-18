#include "../BaseObject/ObjectIdManager.h"

/*-------------------------------------------*/
/* コンストラクタ
/*-------------------------------------------*/
ObjectIdManager::ObjectIdManager()
{

}

/*-------------------------------------------*/
/* デストラクタ
/*-------------------------------------------*/
ObjectIdManager::~ObjectIdManager()
{

}

/*-------------------------------------------*/
/* 初期化
/*-------------------------------------------*/
void ObjectIdManager::Initialize()
{
	// 乱数生成器の初期シードを設定
	mt19937.seed(randomDevice());
}

/*-------------------------------------------*/
/* オブジェクトIDを生成
/*-------------------------------------------*/
std::uint32_t ObjectIdManager::CreateId()
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
/* オブジェクトIDを削除
/*-------------------------------------------*/
void ObjectIdManager::DestroyId(uint32_t& id)
{
	// 指定されたIDをリストから削除する
	idList.remove(id);
	// IDを０クリア
	id = 0;
}
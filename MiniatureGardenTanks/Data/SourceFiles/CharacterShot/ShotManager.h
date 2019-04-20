#pragma once
#include "../Others/Singleton.h"
#include <DxLib.h>
#include <list>

class CharacterShot;
class CharacterBase;

/*-------------------------------------------*/
/* ショット / 管理クラス（シングルトン）
/*-------------------------------------------*/
class ShotManager final : public Singleton<ShotManager>
{
	friend class Singleton<ShotManager>;

public:
			 ShotManager();		// コンストラクタ
	virtual ~ShotManager();		// デストラクタ

	void CreateShot();			// ショットの生成
	void DestroyShot();			// ショットの削除

	// ショットを飛ばす
	bool FiringShot(CharacterBase* firingCharacter, UINT firingNum);

	void Initialize();			// 初期化
	void Update();				// 更新
	void Draw();				// 描画

	void ReplaceShotList();		// 未使用リストと使用リストのショットを入れ替える

	const int& GetUseShotSize() { return useShot.size(); }

private:
	CharacterShot** shot;					// キャラクターショット

	std::list<CharacterShot*> freeShot;		// ショットの未使用リスト
	std::list<CharacterShot*> useShot;		// ショットの使用リスト

	static const int FREESHOT_MAX;			// 未使用リストのショットの最大数
};

#define SHOT_MANAGER ShotManager::GetInstance()
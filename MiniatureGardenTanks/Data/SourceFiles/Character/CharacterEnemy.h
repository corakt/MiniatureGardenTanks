#pragma once
#include "../Character/CharacterBase.h"

/*-------------------------------------------*/
/* キャラクター：エネミー / 派生クラス
/*-------------------------------------------*/
class CharacterEnemy final : public CharacterBase
{
public:
			 CharacterEnemy(UINT id,CharacterType type);	// コンストラクタ
	virtual	~CharacterEnemy();								// デストラクタ

	void Initialize() override;			// 初期化
	void Update()     override;			// 更新
	void Draw()       override;			// 描画
	void DrawUi()     override;			// UIの描画

	// コールバック関数群
	virtual void OnFiringShot()       override;		// 砲弾を発射したとき
	virtual void OnCharacterHitShot() override;		// 砲弾がキャラクターにヒットしたとき

	// スキルを発動
	void ActivateSkill() override;
};
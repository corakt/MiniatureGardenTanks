#pragma once
#include "../Character/CharacterBase.h"

// エフェクトハンドル構造体
struct PlayerEffects
{
	int reloadGauge;			// リロードゲージ
};

// プレイヤー関係のカウンター
struct PlayerCounters
{
	int apGauge;				// アクティブゲージの表示にで扱うカウント
	int apGaugeFilterParam;		// アクティブゲージのフィルターのパラメータで扱うカウント
	int hitLogo;				// ヒットロゴ表示で扱うカウント
	int hitLogoBlendParam;		// ヒットロゴのブレンド用パラメータ
	int skillCutin;				//スキル発動時のカットイン描画で扱うカウント
};

/*-------------------------------------------*/
/* キャラクター：プレイヤー / 派生クラス
/*-------------------------------------------*/
class CharacterPlayer final : public CharacterBase
{
public:
			 CharacterPlayer(UINT id, CharacterType type);		// コンストラクタ
	virtual ~CharacterPlayer();									// デストラクタ

	void Initialize() override;			// 初期化
	void Update()     override;			// 更新
	void Draw()       override;			// 描画
	void DrawUi()     override;			// UIの描画

	// コールバック関数群
	virtual void OnFiringShot()       override;		// 砲弾を発射したとき
	virtual void OnCharacterHitShot() override;		// 砲弾がキャラクターにヒットしたとき

	// スキルを発動
	void ActivateSkill() override;

	void DrawHpGauge();				// HPゲージの表示
	void DrawActiveGauge();			// アクティブゲージの表示
	void DrawHitLogo();				// 攻撃ヒット時のロゴを描画
	void DrawSightUi();				// 照準UIの表示
	void DrawLaser();				// レーザー描画
	void DrawReloadGauge();			// リロードゲージの描画
	void DrawSkillCutin();			// スキル使用時のカットインを描画

private:
	ModelObject*   laser;			// 戦車から伸びるレーザー
	PlayerEffects  effects;			// エフェクト
	PlayerCounters counters;		// カウンター
	int    isDrawHitLogo;			// ヒットロゴを表示するかどうか
	int    isDrawCutin;				// カットインを表示するかどうか
	VECTOR cutinSpritePos;			// カットインの表示座標
};
#pragma once
#include "../Others/Singleton.h"
#include <string>

/*-------------------------------------------*/
/* エフェクトのリソースを管理するクラス
/*-------------------------------------------*/
class ResourceEffectManager final : public Singleton<ResourceEffectManager>
{
	friend class Singleton<ResourceEffectManager>;

public:
			 ResourceEffectManager();	// コンストラクタ
	virtual ~ResourceEffectManager();	// デストラクタ

	// エフェクトの種類
	enum EffectType
	{
		SHOT_RELOADGAUGE,		// リロードゲージ
		TANK_EXHAUSTGAS,		// 戦車：排気ガス
		TANK_MOVESANDSMOKE,		// 戦車：移動中の土煙
		TANK_FIRINGEXPLOSION,	// 戦車：砲弾発射時の爆発
		TANK_SHOTSMOKE,			// 戦車：砲弾の煙
		TANK_HITSPARK,			// 戦車：被弾時の火花
		TANK_IMPACTEXPLOSION,	// 戦車：着弾時の爆発
		TANK_BREAKEXPLOSION,	// 戦車：破壊時の爆発
		TANK_BROKENSMOKE,		// 戦車：破壊後の黒煙
		
		TRANSITION_FADE,		// トランジション：フェード
		TRANSITION_TANKMOVE,	// トランジション：戦車移動

		TYPE_NUM,				// エフェクトの種類数
	};

	// 全てのエフェクトを読み込む
	void LoadAllEffect();
	// 全てのエフェクトを削除する
	void DeleteAllEffect();

	// 指定のエフェクトハンドルを取得
	const int& GetHandle(EffectType type) const;

private:
	int effectHandle[EffectType::TYPE_NUM];			// エフェクトハンドル

	static const std::string EFFECT_FOLDER_RELATIVE_PATH;						// エフェクトフォルダへの相対パス
	static const std::string EFFECT_FILE_RELATIVE_PATH[EffectType::TYPE_NUM];	// エフェクトファイルへの相対パス
};

#define EFFECT_MANAGER ResourceEffectManager::GetInstance()
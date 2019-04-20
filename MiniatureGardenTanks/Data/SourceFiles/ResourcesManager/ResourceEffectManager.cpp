#include "../ResourcesManager/ResourceEffectManager.h"
#include "../Others/ErrorMessage.h"
#include "../Others/Define.h"
#include <EffekseerForDXLib.h>
#include <sstream>
#include <assert.h>

// エフェクトフォルダへの相対パス
const std::string ResourceEffectManager::EFFECT_FOLDER_RELATIVE_PATH = "Data/ResourceFiles/Effects/";

// エフェクトファイルへの相対パス
const std::string ResourceEffectManager::EFFECT_FILE_RELATIVE_PATH[] =
{
	"Character/Player/ReloadGauge.efk",		// リロードゲージ
	
	"Character/ExhaustGas.efk",				// 戦車：排気ガス
	"Character/MoveSandSmoke.efk",			// 戦車：移動中の土煙
	"Character/ShotFiringExplosion.efk",	// 戦車：砲弾発射時の爆発
	"Character/ShotBoost.efk",				// 戦車：砲弾の煙
	"Character/HitSpark.efk",				// 戦車：被弾時の火花
	"Character/ImpactExplosion.efk",		// 戦車：着弾時の爆発
	"Character/BrokenExplosion.efk",		// 戦車：破壊時の爆発
	"Character/BrokenBlackSmoke.efk",		// 戦車：破壊後の黒煙
	
	"Transition/Fade.efk",					// トランジション：フェード
	"Transition/TankMove.efk",				// トランジション：戦車移動
};

/*-------------------------------------------*/
/* コンストラクタ
/*-------------------------------------------*/
ResourceEffectManager::ResourceEffectManager()
{
	// 処理なし
}

/*-------------------------------------------*/
/* デストラクタ
/*-------------------------------------------*/
ResourceEffectManager::~ResourceEffectManager()
{
	// 処理なし
}

/*-------------------------------------------*/
/* 全てのエフェクトを読み込む
/*-------------------------------------------*/
void ResourceEffectManager::LoadAllEffect()
{
	for (int i = 0; i < EffectType::TYPE_NUM; i++)
	{
		// ファイルへのフルパスを取得
		std::string fullPath = EFFECT_FOLDER_RELATIVE_PATH + EFFECT_FILE_RELATIVE_PATH[i];

		// エフェクトを読み込む
		effectHandle[i] = LoadEffekseerEffect(fullPath.c_str());

		// エフェクトの読み込みに失敗した場合
		if (effectHandle[i] == -1)
		{
			std::ostringstream tmpMessage;
			tmpMessage << "エフェクトの読み込みに失敗しました。,EffectType:" << i;

			ErrorData errorData;
			errorData.errorMessage = tmpMessage.str();
			errorData.detectedErrorClassName = "ResourceEffectManager";

			// エラーデータをリストに入れる
			ERROR_MESSAGE.SetErrorData(errorData);
		}
	}
}

/*-------------------------------------------*/
/* 全てのエフェクトを削除
/*-------------------------------------------*/
void ResourceEffectManager::DeleteAllEffect()
{
	for (int i = 0; i < EffectType::TYPE_NUM; i++)
	{
		// エフェクトを全て削除する
		if (effectHandle[i] != NULL)
		{
			SafeDeleteEffect(effectHandle[i]);
		}
	}	
}

/*-------------------------------------------*/
/* 指定のエフェクトハンドルを取得
/*-------------------------------------------*/
const int& ResourceEffectManager::GetHandle(ResourceEffectManager::EffectType type) const
{
	// エフェクトハンドルがNULLであれば例外を投げる
	assert(effectHandle != NULL);
	// エフェクトハンドル以外の定数を指定していれば例外を投げる
	assert(type != EffectType::TYPE_NUM);

	// 指定のエフェクトハンドルを返す
	return effectHandle[type];
}
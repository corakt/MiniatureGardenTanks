#include "../Others/HelperFunction.h"
#include "../Others/Define.h"
#include "../ResourcesManager/ResourceEffectManager.h"
#include <EffekseerForDXLib.h>

/*-------------------------------------------*/
/* 指定したボックスの頂点位置を取得
/*-------------------------------------------*/
void GetBoxVerticesPosition(VECTOR center, VECTOR size, VECTOR *verticesPosBuffer)
{
	// ボックスの８つの頂点位置を算出
	VECTOR vertices[8] = 
	{
		// 左下手前
		center - (size * 0.5),
		
		// 左下奥
		VGet(center.x - (size.x * 0.5f),
			 center.y - (size.y * 0.5f),
			 center.z + (size.z * 0.5f)),

		// 左上手前
		VGet(center.x - (size.x * 0.5f),
			 center.y + (size.y * 0.5f),
			 center.z - (size.z * 0.5f)),

		// 左上奥
		VGet(center.x - (size.x * 0.5f),
			 center.y + (size.y * 0.5f),
			 center.z + (size.z * 0.5f)),

		// 右上奥
		center + (size * 0.5),

		// 右上手前
		VGet(center.x + (size.x * 0.5f),
			 center.y + (size.y * 0.5f),
			 center.z - (size.z * 0.5f)),

		// 右下奥
		VGet(center.x + (size.x * 0.5f),
			 center.y - (size.y * 0.5f),
			 center.z + (size.z * 0.5f)),

		// 右下手前
		VGet(center.x + (size.x * 0.5f),
			 center.y - (size.y * 0.5f),
			 center.z - (size.z * 0.5f)),
	};

	// 算出した位置をバッファに渡す
	for (int i = 0; i < 8; i++)
	{
		verticesPosBuffer[i] = vertices[i];
	}
}

// エフェクトハンドル
int transitionEffectHandle = -1;

/*-------------------------------------------*/
/* トランジションエフェクト「フェード」を再生
/*-------------------------------------------*/
void PlayTransitionEffectForFade(float playSpeed)
{
	// 既に再生中の場合はスキップ
	if (IsEffekseer2DEffectPlaying(transitionEffectHandle) == 0) { return; }

	// エフェクトを再生
	transitionEffectHandle = PlayEffekseer2DEffect(EFFECT_MANAGER.GetHandle(ResourceEffectManager::EffectType::TRANSITION_FADE));
	// 再生位置をセット
	SetPosPlayingEffekseer2DEffect(transitionEffectHandle, RESOLUTION_WIDTH / 2, RESOLUTION_HEIGHT / 2, 0);
	// 再生スケールをセット
	SetScalePlayingEffekseer2DEffect(transitionEffectHandle, 50, 50, 50);
	// 再生スピードをセット（引数）
	SetSpeedPlayingEffekseer2DEffect(transitionEffectHandle, playSpeed);
}

/*-------------------------------------------*/
/* トランジションエフェクト「戦車移動」を再生
/*-------------------------------------------*/
void PlayTransitionEffectForTankMove(float playSpeed)
{
	// 既に再生中の場合はスキップ
	if (IsEffekseer2DEffectPlaying(transitionEffectHandle) == 0) { return; }

	// エフェクトを再生
	transitionEffectHandle = PlayEffekseer2DEffect(EFFECT_MANAGER.GetHandle(ResourceEffectManager::EffectType::TRANSITION_TANKMOVE));
	// 再生位置をセット
	SetPosPlayingEffekseer2DEffect(transitionEffectHandle, RESOLUTION_WIDTH+1000, (RESOLUTION_HEIGHT / 2)-50, 0);
	// 再生スケールをセット
	SetScalePlayingEffekseer2DEffect(transitionEffectHandle, 2.7f, 2.7f, 2.7f);
	// 再生スピードをセット（引数）
	SetSpeedPlayingEffekseer2DEffect(transitionEffectHandle, playSpeed);
}

/*-------------------------------------------*/
/* トランジションエフェクトが再生中かどうかを判定
/*-------------------------------------------*/
bool IsTransitionEffectPlaying()
{
	// 再生中であればtrue、そうでなければfalseを返す

	if (IsEffekseer2DEffectPlaying(transitionEffectHandle) == 0) { return true; }
	return false;
}
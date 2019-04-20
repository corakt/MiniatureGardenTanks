#include "../Character/CharacterBase.h"
#include "../ResourcesManager/ResourceEffectManager.h"

/*-------------------------------------------*/
/* エフェクト：ダメージ
/*-------------------------------------------*/
void CharacterBase::playEffectForDamage()
{
	// エフェクト用パラメータ
	const VECTOR POS_OFFSET = VGet(0,100,0);		// 再生位置オフセット
	const VECTOR SCALE      = VGet(300,300,300);	// 再生スケール

	// 再生するエフェクトをセット
	effects.damage = PlayEffekseer3DEffect(EFFECT_MANAGER.GetHandle(ResourceEffectManager::EffectType::TANK_HITSPARK));
	// 再生位置をセット
	SetPosPlayingEffekseer3DEffect(effects.damage,bodyTransform.position.x + POS_OFFSET.x,
												  bodyTransform.position.y + POS_OFFSET.y,
												  bodyTransform.position.z + POS_OFFSET.z);
	// 再生スケールをセット
	SetScalePlayingEffekseer3DEffect(effects.damage,SCALE.x, SCALE.y, SCALE.z);
}

/*-------------------------------------------*/
/* エフェクト：戦車の破壊
/*-------------------------------------------*/
void CharacterBase::playEffectForBrokenExplosion()
{
	// エフェクト用パラメータ
	const VECTOR SCALE = VGet(3, 3, 3);	// 再生スケール
	const float  SPEED = 1.5f;			// 再生スピード

	// 再生するエフェクトをセット
	effects.brokenExplosion = PlayEffekseer3DEffect(EFFECT_MANAGER.GetHandle(ResourceEffectManager::EffectType::TANK_BREAKEXPLOSION));
	// 再生位置をセット
	SetPosPlayingEffekseer3DEffect(effects.brokenExplosion, bodyTransform.position.x, 
															bodyTransform.position.y, 
															bodyTransform.position.z);
	// 再生スケールをセット
	SetScalePlayingEffekseer3DEffect(effects.brokenExplosion, SCALE.x, SCALE.y, SCALE.z);
	// 再生スピードをセット
	SetSpeedPlayingEffekseer3DEffect(effects.brokenExplosion, SPEED);
}

/*-------------------------------------------*/
/* エフェクト：破壊後の黒煙
/*-------------------------------------------*/
void CharacterBase::playEffectForBrokenBlackSmoke()
{
	// エフェクト用パラメータ
	const VECTOR POS_OFFSET = VGet(0, 0,-200);
	const VECTOR SCALE = VGet(2, 1, 2);
	const int PLAY_INTERVAL = 250;

	// HPが無くなっていたら
	if (hitPoint <= 0)
	{
		counters.brokenSmokeEffect++;
		// 指定の間隔でエフェクトを再生する
		if (counters.brokenSmokeEffect % PLAY_INTERVAL == 1)
		{
			// 再生するエフェクトをセット
			effects.brokenBlackSmoke = PlayEffekseer3DEffect(EFFECT_MANAGER.GetHandle(ResourceEffectManager::EffectType::TANK_BROKENSMOKE));
		}
	}
	// 再生位置をセット
	SetPosPlayingEffekseer3DEffect(effects.brokenBlackSmoke, bodyTransform.position.x + POS_OFFSET.x,
															 bodyTransform.position.y + POS_OFFSET.y,
															 bodyTransform.position.z + POS_OFFSET.z);
	// 再生スケールをセット
	SetScalePlayingEffekseer3DEffect(effects.brokenBlackSmoke, SCALE.x, SCALE.y, SCALE.z);
	// エフェクトの色を黒にセット
	SetColorPlayingEffekseer3DEffect(effects.brokenBlackSmoke, 0, 0, 0,255);
}

/*-------------------------------------------*/
/* エフェクト：砲撃時の爆発
/*-------------------------------------------*/
void CharacterBase::playEffectForFiringExplosion()
{
	// エフェクト用パラメータ
	const float SPEED = 2;		// エフェクトの再生スピード

	// 再生するエフェクトをセット
	effects.firingExplosion = PlayEffekseer3DEffect(EFFECT_MANAGER.GetHandle(ResourceEffectManager::TANK_FIRINGEXPLOSION));
	// 再生位置をセット
	SetPosPlayingEffekseer3DEffect(effects.firingExplosion, turretTipPosition.x, turretTipPosition.y, turretTipPosition.z);
	// 再生スピードをセット
	SetSpeedPlayingEffekseer3DEffect(effects.firingExplosion, SPEED);
}

/*-------------------------------------------*/
/* エフェクト：排気ガス
/*-------------------------------------------*/
void CharacterBase::playEffectForExhaustGas()
{
	// エフェクト用パラメータ
	VECTOR position         = ZERO_VECTOR;				// 再生位置
	const VECTOR POS_OFFSET = VGet(-330,100,0);			// 再生位置オフセット
	const VECTOR SCALE      = VGet(1.5f,1.5f,1.5f);		// スケール
	const float  SPEED      = 2;						// 再生スピード

	int playInterval = 0;
	// 移動中か停止中かで再生する間隔を変更する
	if (moveState != MoveState::STOP) { playInterval = 2; }
	else                              { playInterval = 20;}

	// エフェクトの再生位置を算出
	position = bodyTransform.direction * POS_OFFSET.x;
	position = VGet(bodyTransform.position.x, bodyTransform.position.y + POS_OFFSET.y, bodyTransform.position.z) + position;
	
	// 指定の間隔でエフェクトを再生する
	counters.exhaustGasEffect++;
	if (counters.exhaustGasEffect % playInterval == 0)
	{
		// 再生するエフェクトをセット
		effects.exhaustGas = PlayEffekseer3DEffect(EFFECT_MANAGER.GetHandle(ResourceEffectManager::EffectType::TANK_EXHAUSTGAS));
	}
	// 再生位置をセット
	SetPosPlayingEffekseer3DEffect(effects.exhaustGas,position.x, position.y, position.z);
	// 回転角をセット
	SetRotationPlayingEffekseer3DEffect(effects.exhaustGas, bodyTransform.rotation.x, bodyTransform.rotation.y, bodyTransform.rotation.z);
	// 再生スケールをセット
	SetScalePlayingEffekseer3DEffect(effects.exhaustGas,SCALE.x, SCALE.y, SCALE.z);
	// 再生スピードをセット
	SetSpeedPlayingEffekseer3DEffect(effects.exhaustGas,SPEED);
}

/*-------------------------------------------*/
/* エフェクト：移動時の土煙
/*-------------------------------------------*/
void CharacterBase::playEffectforMoveSandSmoke()
{
	// エフェクト用パラメータ
	const VECTOR POS_OFFSET    = VGet(0,-50,0);
	const VECTOR SCALE         = VGet(1.5f,2.5f,1.5f);
	const float  SPEED         = 1.5f;
	const int    PLAY_INTERVAL = 5;

	// 移動中
	if (moveState != MoveState::STOP)
	{
		// カウンター
		counters.moveSandSmokeEffect++;
		// 指定の間隔で再生させる
		if (counters.moveSandSmokeEffect % PLAY_INTERVAL == 0)
		{
			// 再生するエフェクトをセット
			effects.moveSandSmoke = PlayEffekseer3DEffect(EFFECT_MANAGER.GetHandle(ResourceEffectManager::EffectType::TANK_MOVESANDSMOKE));
		}
	}

	// 再生位置をセット
	SetPosPlayingEffekseer3DEffect(effects.moveSandSmoke, bodyTransform.position.x + POS_OFFSET.x,
														  bodyTransform.position.y + POS_OFFSET.y,
														  bodyTransform.position.z + POS_OFFSET.z);
	// 再生角度をセット
	SetRotationPlayingEffekseer3DEffect(effects.moveSandSmoke, bodyTransform.rotation.x, 
															   bodyTransform.rotation.y, 
															   bodyTransform.rotation.z);
	// 再生スケールをセット
	SetScalePlayingEffekseer3DEffect(effects.moveSandSmoke,SCALE.x, SCALE.y, SCALE.z);
	// 再生スピードをセット
	SetSpeedPlayingEffekseer3DEffect(effects.moveSandSmoke,SPEED);
}
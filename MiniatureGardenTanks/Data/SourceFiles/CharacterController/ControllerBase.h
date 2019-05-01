#pragma once
#include "../Character/CharacterManager.h"
#include <DxLib.h>

class CharacterBase;
class ModelObject;

/*-------------------------------------------*/
/* キャラクターの制御 / 基底クラス
/*-------------------------------------------*/
class ControllerBase
{
public:
			 ControllerBase(CharacterBase* controlCharacter);	// コンストラクタ
	virtual ~ControllerBase();									// デストラクタ

	virtual void Initialize() = 0;						// 初期化
	virtual void Update()     = 0;						// 更新

protected:
	void InitializeCommonParameter();					// 共通のパラメータを初期化

	void Move(bool isInput);							// 移動
	void RotationBody(VECTOR moveDir,bool isInput);		// 車体の回転
	void RotationTurret(VECTOR turretDir);				// 砲塔の回転
	void FiringReaction();								// ショット発射時の反動

	float easingTime;						// イージングの進行度
	float currentSpeed;						// 現在の移動スピード

	CharacterBase*  controlCharacter;		// 制御するキャラクター
	ModelObject*    characterBody;			// キャラクターの車体部分
	ModelObject*    characterTurret;		// キャラクターの砲塔部分

	Transform       characterBodyTrans;		// 車体部分のトランスフォーム
	Transform       characterTurretTrans;	// 砲塔部分のトランスフォーム
};
#pragma once
#include "../CharacterController/ControllerBase.h"
#include <DxLib.h>

class CameraBase;

/*-------------------------------------------*/
/* キャラクターの制御 : プレイヤー / 派生クラス
/*-------------------------------------------*/
class ControllerPlayer : public ControllerBase
{
public:
	// コンストラクタ
	ControllerPlayer(CharacterBase* controlCharacter,
				     CameraBase*    playerCamera);
	// デストラクタ
	virtual ~ControllerPlayer();

	void Initialize() override;			// 初期化
	void Update()     override;			// 更新

private:
	CameraBase* playerCamera;		// プレイヤーカメラ
};
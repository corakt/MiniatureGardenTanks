#pragma once
#include "../Character/CharacterBase.h"
#include "../Others/Singleton.h"
#include <DxLib.h>
#include <list>
#include <string>

class ControllerBase;
class CameraBase;

/*-------------------------------------------*/
/* キャラクター / 管理クラス（シングルトン）
/*-------------------------------------------*/
class CharacterManager final : public Singleton<CharacterManager>
{
	friend class Singleton<CharacterManager>;

public:
			 CharacterManager();		// コンストラクタ
	virtual ~CharacterManager();		// デストラクタ

	CharacterBase* CreateCharacter(CharacterBase::CharacterType type);	// キャラクターを生成
	void DestroyCharacter();											// キャラクターを削除

	void Initialize(const VECTOR position[], const float bodyAngle[], const COLOR_F modelColor[]);	// 初期化
	void Update();			// 更新
	void Draw();			// 描画
	void DrawUi();			// UIの描画

	// 指定のキャラクターを取得
	CharacterBase* GetCharacter(UINT id) const { return character[id];}
	// プレイヤーカメラを取得
	void GetPlayerCamera(CameraBase* playerCamera) { this->playerCamera = playerCamera; }

	// 生きている敵戦車の数を取得
	UINT GetAliveEnemyTankNum();
	// プレイヤーが生きているかどうかのフラグを取得
	bool isAlivePlayerTank();

	// 全てのキャラクターの制御フラグをセットする
	void SetAllCharacterControlingFlag(const bool set);

private:
	CharacterBase**  character;								// キャラクター
	ControllerBase** characterController;					// キャラクターの制御

	std::list<CharacterBase*>  activeCharacter;				// 活動中のキャラクター
	std::list<ControllerBase*> activeCharacterController;	// 活動中のキャラクターの制御

	CameraBase* playerCamera;								// プレイヤーカメラ

	static const int CHARACTER_MAX;							// キャラクターの最大数
};

#define CHARACTER_MANAGER CharacterManager::GetInstance()
#include "../Character/CharacterManager.h"
#include "../Character/CharacterPlayer.h"
#include "../Character/CharacterEnemy.h"
#include "../CharacterController/ControllerBase.h"
#include "../CharacterController/ControllerPlayer.h"
#include "../CharacterController/ControllerEnemy.h"
#include "../Others/Define.h"

const int CharacterManager::CHARACTER_MAX = 5;		// キャラクターの最大数

/*-------------------------------------------*/
/* コンストラクタ
/*-------------------------------------------*/
CharacterManager::CharacterManager()
{
	// NULLで初期化
	character    = NULL;
	playerCamera = NULL;

	// キャラクターの数だけ、メモリを確保
	character           = new CharacterBase*[CHARACTER_MAX];	// キャラクター
	characterController = new ControllerBase*[CHARACTER_MAX];	// キャラクターの制御

	// 確保したメモリをNULLで初期化
	for (UINT i = 0; i < CHARACTER_MAX; i++)
	{
		character[i]           = NULL;			// キャラクター
		characterController[i] = NULL;			// キャラクターの制御
	}
}

/*-------------------------------------------*/
/* デストラクタ
/*-------------------------------------------*/
CharacterManager::~CharacterManager()
{
	// キャラクター生成用に確保していたメモリを全て解放
	SafeDeleteArray(character);
	SafeDeleteArray(characterController);
}

/*-------------------------------------------*/
/* キャラクターを生成
/*-------------------------------------------*/
CharacterBase* CharacterManager::CreateCharacter(CharacterBase::CharacterType type)
{
	// ループしながらメモリを確保できる変数を探す
	for (UINT i = 0; i < CHARACTER_MAX; i++)
	{
		// 既に確保されていれば、次の変数に移る
		if (character[i] != NULL) { continue; }
		// 確保されていない変数を見つけた場合、その変数にメモリを確保する
		else
		{
			// キャラクターを生成（ループの添え字をIDとしてセット）
			if (type == CharacterBase::CharacterType::PLAYER)
			{
				// プレイヤーを生成
				character[i] = new CharacterPlayer(i, type);
				// プレイヤーの制御オブジェクトを生成
				characterController[i] = new ControllerPlayer(character[i],playerCamera);
			}
			else
			{
				// エネミーを生成
				character[i] = new CharacterEnemy(i, type);
				// エネミーの制御オブジェクトを生成
				characterController[i] = new ControllerEnemy(character[i]);
			}

			// キャラクターをリストに追加
			activeCharacter.push_back(character[i]);
			// キャラクターの制御オブジェクトをリストに追加
			activeCharacterController.push_back(characterController[i]);

			// キャラクターのポインタを返す
			return character[i];
		}
	}
	return NULL;
}

/*-------------------------------------------*/
/* キャラクターを削除
/*-------------------------------------------*/
void CharacterManager::DestroyCharacter()
{
	// 各リストを全削除
	activeCharacter.clear();				// キャラクター
	activeCharacterController.clear();		// キャラクターの制御

	// 各キャラクターのインスタンスを削除してメモリを解放
	for (UINT i = 0; i < CHARACTER_MAX; i++)
	{
		SafeDelete(character[i]);				// キャラクター
		SafeDelete(characterController[i]);		// キャラクターの制御
	}
}

/*-------------------------------------------*/
/* 初期化
/*-------------------------------------------*/
void CharacterManager::Initialize(const VECTOR position[],const float bodyAngle[],const COLOR_F modelColor[])
{
	// ループカウンター
	int loopCounter = 0;

	if (activeCharacter.empty() == false)
	{
		for (CharacterBase*& characterElem : activeCharacter)
		{
			if (characterElem != NULL)
			{
				// 初期化処理
				characterElem->Initialize();

				// トランスフォームを取得
				Transform bodyTrans   = characterElem->GetParts(CharacterBase::PartsType::BODY)->GetTransform();		// 車体
				Transform turretTrans = characterElem->GetParts(CharacterBase::PartsType::TURRET)->GetTransform();	// 砲塔

				// 初期化用パラメータをセット
				// 初期位置をセット
				bodyTrans.position = position[loopCounter];
				// 初期角度をセット
				bodyTrans.rotation.y   = bodyAngle[loopCounter];	// 車体
				turretTrans.rotation.y = bodyAngle[loopCounter];	// 砲塔
				// モデルの色をセット
				characterElem->SetModelColor(modelColor[loopCounter]);

				// トランスフォームをセット
				characterElem->GetParts(CharacterBase::PartsType::BODY)->SetTransform(bodyTrans);
				characterElem->GetParts(CharacterBase::PartsType::TURRET)->SetTransform(turretTrans);
				
				// ループカウンターのインクリメント
				loopCounter++;

				// 砲塔の相対座標を算出
				characterElem->CalculationTurretRelativePosition();
			}
		}
	}

	if (activeCharacterController.empty() == false)
	{
		for (ControllerBase*& characterControllerElem : activeCharacterController)
		{
			if (characterControllerElem != NULL)
			{
				// キャラクターの制御オブジェクトの初期化
				characterControllerElem->Initialize();
			}
		}
	}
}

/*-------------------------------------------*/
/* 更新
/*-------------------------------------------*/
void CharacterManager::Update()
{
	if (activeCharacter.empty() == false)
	{
		for (CharacterBase*& characterElem : activeCharacter)
		{
			if (characterElem != NULL)
			{
				// 更新処理
				characterElem->Update();
			}
		}
	}

	if (activeCharacterController.empty() == false)
	{
		for (ControllerBase*& characterControllerElem : activeCharacterController)
		{
			if (characterController != NULL)
			{
				// キャラクターの制御オブジェクトの更新
				characterControllerElem->Update();
			}
		}
	}
}

/*-------------------------------------------*/
/* 描画
/*-------------------------------------------*/
void CharacterManager::Draw()
{
	if (activeCharacter.empty() == false)
	{
		for (CharacterBase*& characterElem : activeCharacter)
		{
			if (character != NULL)
			{
				// 描画処理
				characterElem->Draw();
			}
		}
	}
}

/*-------------------------------------------*/
/* UIの描画
/*-------------------------------------------*/
void CharacterManager::DrawUi()
{
	if (activeCharacter.empty() == false)
	{
		for (CharacterBase*& character : activeCharacter)
		{
			if (character != NULL)
			{
				// UIの描画処理
				character->DrawUi();
			}
		}
	}
}

/*-------------------------------------------*/
/* 生きている敵戦車の数を取得
/*-------------------------------------------*/
UINT CharacterManager::GetAliveEnemyTankNum()
{
	// カウント
	UINT count = 0;

	if (activeCharacter.empty() == false)
	{
		for (CharacterBase*& character : activeCharacter)
		{
			// NULLだった場合はスキップ
			if (character == NULL) { continue; }
			
			// プレイヤーは除外する
			if (character->GetObjectType() == CharacterBase::CharacterType::PLAYER) { continue; }

			// HPが残っているキャラクターのみ
			// 生きている戦車としてカウント
			if (character->GetHitPoint() > 0) { count++; }
		}
	}

	// カウントの数を生きている敵戦車の数として返す
	return count;
}

/*-------------------------------------------*/
/* プレイヤーが生きているかどうかのフラグを取得
/*-------------------------------------------*/
bool CharacterManager::isAlivePlayerTank()
{
	if (activeCharacter.empty() == false)
	{
		for (CharacterBase*& character : activeCharacter)
		{
			// NULLだった場合はスキップ
			if (character == NULL) { continue; }
			
			// エネミーは除外する
			if (character->GetObjectType() == CharacterBase::CharacterType::ENEMY) { continue; }

			// HPが残っていた
			if (character->GetHitPoint() > 0) { return true; }
		}
	}

	return false;
}

/*-------------------------------------------*/
/* 全てのキャラクターの制御フラグをセットする
/*-------------------------------------------*/
void CharacterManager::SetAllCharacterControlingFlag(const bool set)
{
	if (activeCharacter.empty() == false)
	{
		for (CharacterBase*& character : activeCharacter)
		{
			// NULLだった場合はスキップ
			if (character == NULL) { continue; }

			// 制御フラグをセット
			character->SetControlingFlag(set);
		}
	}
}
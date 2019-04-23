#include "../CharacterShot/ShotManager.h"
#include "../CharacterShot/CharacterShot.h"
#include "../Character/CharacterBase.h"
#include "../Others/Math3D.h"
#include "../Others/Define.h"
#include "../ResourcesManager/ResourceModelManager.h"

// 未使用リストのショットの最大数
const int ShotManager::FREESHOT_MAX = 100;

/*-------------------------------------------*/
/* コンストラクタ
/*-------------------------------------------*/
ShotManager::ShotManager()
{
	if (shot == NULL)
	{
		// ショットの数だけ相的確保を行う
		shot = new CharacterShot*[FREESHOT_MAX]();
		for (int i = 0; i < FREESHOT_MAX; i++)
		{
			// 確保したメモリをNULLで初期化
			shot[i] = NULL;
		}
	}
}

/*-------------------------------------------*/
/* デストラクタ
/*-------------------------------------------*/
ShotManager::~ShotManager()
{
	// ショット生成用に確保したメモリを全て解放する
	SafeDeleteArray(shot);
}

/*-------------------------------------------*/
/* ショットの生成
/*-------------------------------------------*/
void ShotManager::CreateShot()
{
	for (int j = 0; j < FREESHOT_MAX; j++)
	{
		if (shot[j] == NULL)
		{
			// ショットのインスタンスを生成する
			shot[j] = new CharacterShot();
		}
		
		// 生成したショットを未使用リストに挿入
		freeShot.push_back(shot[j]);
	}
}

/*-------------------------------------------*/
/* ショットの削除
/*-------------------------------------------*/
void ShotManager::DestroyShot()
{
	for (int i = 0; i < FREESHOT_MAX; i++)
	{
		if (shot[i] != NULL)
		{
			// ショットのインスタンスを破棄して、メモリを解放
			SafeDelete(shot[i]);
		}
	}

	// リストを削除する
	freeShot.clear();	// ショットの未使用リスト
	useShot.clear();	// ショットの使用リスト
}

/*-------------------------------------------*/
/* ショットを飛ばす
/*-------------------------------------------*/
bool ShotManager::FiringShot(CharacterBase* firingCharacter, UINT firingNum)
{
//	// キャラクターのポインタがNULLだったら終了して関数を抜ける
//	if (firingCharacter == NULL) { return false; }
//
//	// 飛ばすショットの数から最初に設置するショットのY軸角度を算出
//	float rotY = (float)((firingNum / 2) / -10.0f);
//
//	// キャラクターの車体オブジェクトを取得
//	ModelObject* tankBody = firingCharacter->GetParts(CharacterBase::PartsType::BODY);
//
//	// 砲塔のトランスフォームをセットする
//	Transform turretTrans = firingCharacter->GetParts(CharacterBase::PartsType::TURRET)->GetTransform();
//
//	// 発射するショットの数だけセットする
//	for (UINT i = 0; i < firingNum; i++)
//	{
//		// 未使用リストの先頭からショットを一つ取り出す
//		CharacterShot* shot = freeShot.front();
//		// ショットのトランスフォームを取得
//		Transform shotTrans = shot->GetTransform();
//
//		// 砲塔の先端位置をショットの発射位置としてセット
//		shotTrans.position = firingCharacter->GetTurretTipPos();
//
//		// ショットを撃ったキャラクターのオブジェクトIDを取得
//		shot->SetFiringCharacterObjectId(tankBody->GetObjectId());
//
//		// ショットの回転角を砲塔と同じ回転角にセットする
//		shotTrans.rotation = turretTrans.rotation;
//		// ショットのY軸の角度を0.1ずつずらしながら設置していく
//		shotTrans.rotation.y += rotY;
//		rotY += 0.1f;
//		
//		// ショットの回転角から回転行列を取得
//		MATRIX rotMatrix = GetRotMatrixFromRot(shotTrans.rotation);
//		// 回転行列を使用してベクトルを回転させる
//		shotTrans.direction = VTransform(shotTrans.direction, rotMatrix);
//
//		// トランスフォームをセット
//		shot->SetTransform(shotTrans);
//
//		// ショットがキャラクターにヒットした際のコールバック関数をセット
//		shot->OnCharacterHitShot = std::bind(&CharacterBase::OnCharacterHitShot,firingCharacter);
//
//		// ショットの稼働フラグをtrueにする
//		shot->SetActiveFlag(true);
//		// ショットの描画フラグをtrueにする
//		shot->SetDrawFlag(true);
//
//		// ショットを使用リストにセットする
//		useShot.push_back(shot);
//
//		// 未使用リストの先頭要素を削除する
//		freeShot.pop_front();
//	}
	return true;
}

/*-------------------------------------------*/
/* 未使用リストと使用リストのショットを入れ替える
/*-------------------------------------------*/
void ShotManager::ReplaceShotList()
{
	// ショットイテレータ
	std::list<CharacterShot*>::iterator shotIterator;	

	if (useShot.empty() == false)
	{
		for (shotIterator  = useShot.begin();
			 shotIterator != useShot.end();)
		{
			if ((*shotIterator) != NULL)
			{
				// 使用リスト内の未稼働のショットを未使用リストに戻す
				if ((*shotIterator)->GetActiveFlag() == false)
				{
					// 未稼働のショットを受け取る
					CharacterShot* nonActiveShot = (*shotIterator);

					// 受け取ったショットを未使用リストに挿入
					freeShot.push_back(nonActiveShot);
					// 使用リストに残ったショットを削除する
					shotIterator = useShot.erase(shotIterator);
					continue;
				}
			}
			shotIterator++;
		}
	}
}

/*-------------------------------------------*/
/* 初期化
/*-------------------------------------------*/
void ShotManager::Initialize()
{
	if (freeShot.empty() == false)
	{
		for (CharacterShot*& shot : freeShot)
		{
			if (shot != NULL)
			{
				// ショットの初期化
				shot->Initialize();
			}
		}
	}
}

/*-------------------------------------------*/
/* 更新
/*-------------------------------------------*/
void ShotManager::Update()
{
	if (useShot.empty() == false)
	{
		for (CharacterShot*& shot : useShot)
		{
			if (shot != NULL)
			{
				if (shot->GetActiveFlag())
				{
					// ショットの更新
					shot->Update();
				}
			}
		}
	}
}

/*-------------------------------------------*/
/* 描画
/*-------------------------------------------*/
void ShotManager::Draw()
{
	if (useShot.empty() == false)
	{
		for (CharacterShot*& shot : useShot)
		{
			if (shot != NULL)
			{
				// ショットの描画
				shot->Draw();
			}
		}
	}
}
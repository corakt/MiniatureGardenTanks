#include "../CharacterController/ControllerEnemy.h"
#include "../Collision/SphereCollider.h"
#include "../Collision/CircularSectorCollider.h"
#include "../Stage/StageManager.h"
#include "../Others/Define.h"
#include "../Others/Math3D.h"
#include "../Raycast/Raycast.h"
#include <cmath>

// 関数の数が多いので、AIの制御関数はこっちにまとめておく

/*-------------------------------------------*/
/* 移動目標位置を設定
/*-------------------------------------------*/
void ControllerEnemy::setTargetMovePosition()
{
	counters.changeMovePosInterval++;

	// カウントを使用して、一定間隔で目標位置を変更する
	if (counters.changeMovePosInterval %
		MOVETARGETPOS_CHANGEINTERVAL_COUNTNUM == 1)
	{
		// ステージの範囲内で目標位置を決定する
		moveTargetPosition = VGet((float)moveTargetPosRand.x(mt19937),0,
		                          (float)moveTargetPosRand.z(mt19937));
	}
}

/*-------------------------------------------*/
/* 通過した地面のIDを取得
/*-------------------------------------------*/
void ControllerEnemy::getThroughTerrainData()
{
	// 現在立っている地面のデータ
	TerrainData currentGroundData;
	// キャラクターの座標から、現在立っている地面のIDを取得
	currentGroundData.id = getTerrainIdFromPos(characterBodyTrans.position);
	// IDから位置を逆算する
	currentGroundData.position = getTerrainPosFromId(currentGroundData.id);

	// 最後に通過した地面として登録
	lastThroughGroundData = currentGroundData;

	if (throughGroundData.empty() == false)
	{
		for (TerrainData& throughGroundElem : throughGroundData)
		{
			// 登録しようとしている地形データが既に登録されているか調べる
			if (throughGroundElem.id == currentGroundData.id)
			{
				// 既に登録済みの場合は、そのまま関数を抜ける
				return;
			}
		}
	}

	// 未登録の場合は、リストに登録する
	throughGroundData.push_back(currentGroundData);

	// 登録しておくデータの最大数を超えた場合は、古いものから削除
	if (throughGroundData.size() > THROUGHGROUND_SAVE_MAX)
	{
		throughGroundData.pop_front();
	}
}

/*-------------------------------------------*/
/* キャラクターの位置を基準に上下左右の地面のデータを取得
/*-------------------------------------------*/
void ControllerEnemy::getCharacterFourDirGroundData()
{
	// 前フレームで保存していたデータを削除する
	fourDirGroundData.clear();

	// 上下左右の四方向
	TerrainId direction[4] = { {0,-1},{0,+1},{-1,0},{+1,0} };

	// 現在立っている地面から、上下左右の地面のデータを取得
	for (int i = 0; i < 4; i++)
	{
		// 一時保存用の地面データ
		TerrainData tmpGroundData;

		// 地面のIDを取得
		tmpGroundData.id = lastThroughGroundData.id + direction[i];

		// IDから座標を算出
		tmpGroundData.position = getTerrainPosFromId(tmpGroundData.id);

		// 移動目標位置との距離を算出
		tmpGroundData.distance = VSize(tmpGroundData.position - moveTargetPosition);

		// 地面のデータをリストに追加
		fourDirGroundData.push_back(tmpGroundData);
	}
}

/*-------------------------------------------*/
/* 視野の範囲内に存在している敵キャラクターを取得
/*-------------------------------------------*/
void ControllerEnemy::getViewingRangeEnemyCharacter()
{
	// 視野の範囲内に存在するオブジェクトが無ければ
	// そのまま関数を抜ける
	if (charaViewingCollider->GetCollModelInfo().empty()) { return; }

	// 前フレームで保存していたリストのデータを削除
	viewingRangeCharacterData.clear();

	// 視野の扇形コライダーに衝突しているオブジェクトから、敵キャラクターのみを登録する
	for (const CollModelInfo& collModelInfoElem : charaViewingCollider->GetCollModelInfo())
	{
		// 衝突したモデルを取得
		ModelObject* collModel = collModelInfoElem.collModel;
		// オブジェクトがNULLだったら、スキップして次の要素へ
		if (collModel == NULL) { continue; }

		// 敵キャラクターのみを登録する
		if (collModel->GetObjectType() != ObjectType::CHARACTER_ENEMY) { continue; }

		// 敵キャラクターの車体のトランスフォームを取得
		Transform enemyCharaBodyTrans = collModel->GetTransform();

		// 一時保存用のキャラクターのデータ
		EnemyCharacterData tmpCharacterData;

		// 位置を取得
		tmpCharacterData.position = enemyCharaBodyTrans.position;

		// 敵キャラクターとの距離を算出
		tmpCharacterData.distance = VSize(enemyCharaBodyTrans.position - characterBodyTrans.position);

		// 敵キャラクターのデータをリストに追加
		viewingRangeCharacterData.push_back(tmpCharacterData);
	}
}

/*-------------------------------------------*/
/* キャラクターの付近に存在している壁のデータを取得
/*-------------------------------------------*/
void ControllerEnemy::getAroundRangeWallData()
{
	// キャラクター中心の円形コライダーに衝突しているオブジェクトが無ければ
	// そのまま関数を抜ける
	if (charaAroundCollidr->GetCollModelInfo().empty()) { return; }

	// 前フレームで保存していた壁のデータを削除
	aroundRangeWallData.clear();

	// コライダーに衝突しているオブジェクトから、壁のオブジェクトのみを登録する
	for (const CollModelInfo& collModelInfoElem : charaAroundCollidr->GetCollModelInfo())
	{
		// 衝突したモデルを取得
		ModelObject* collModel = collModelInfoElem.collModel;
		// オブジェクトがNULLだったら、スキップして次の要素へ
		if (collModel == NULL) { continue; }

		// モデルの種類が壁以外だったら、スキップして次の要素へ
		if (collModel->GetObjectType() != ObjectType::TERRAIN_WALL) { continue; }

		// ヒットしたモデルのトランスフォームを取得
		Transform collModelTrans = collModel->GetTransform();

		// 一時保存用の壁のデータ
		TerrainData tmpWallData;

		// 位置からIDを算出
		tmpWallData.id = getTerrainIdFromPos(collModelTrans.position);

		// 位置を取得
		tmpWallData.position = collModelTrans.position;

		// キャラクターとの距離を算出
		tmpWallData.distance = VSize(collModelTrans.position - characterBodyTrans.position);

		// 壁のデータをリストに登録
		aroundRangeWallData.push_back(tmpWallData);

	}
}

/*-------------------------------------------*/
/* 狭い範囲にキャラクターが留まり続けているか
/*-------------------------------------------*/
bool ControllerEnemy::isNarrowRangeStayCharacter()
{
	// 通過済みの地面のデータが空であれば、そのまま関数を抜ける
	if (throughGroundData.empty()) { return false; }

	// 通過済みの地面データで一番古いものを取得
	TerrainData oldThroughGroundData = throughGroundData.front();

	// キャラクターと地面とのベクトルを算出
	VECTOR charaToGroundVec = oldThroughGroundData.position - characterBodyTrans.position;

	// 一番古い地面を中心とした指定の半径の範囲内にキャラクターが存在しているか調べる
	if (VSquareSize(charaToGroundVec) < std::pow(CHARACTER_NARROWRANGE_RADIUS, 2))
	{
		// 存在していた場合は、狭い範囲に留まり続けている時間を表すカウントを加算
		counters.areaStayTime++;

		// 狭い範囲に留まり続けている時間が指定の時間を超えたかどうか
		if (counters.areaStayTime > NARROWRANGE_STAY_TIME)
		{
			// 指定の時間を超えたら、留まり続けていると判断してtrueを返す
			return true;
		}
	}
	else
	{
		counters.areaStayTime = 0;
	}
	return false;
}

/*-------------------------------------------*/
/* 自分と敵との間に壁が存在しているか
/*-------------------------------------------*/
bool ControllerEnemy::existWallBetweenEnemyAndSelf(EnemyCharacterData enemy)
{
	// レイキャストに衝突しているオブジェクトが無ければ
	// そのままfalseを返す
	if (charaViewingDirRaycast->GetHitModelInfo().empty()) { return false; }

	// 敵とのベクトルを算出
	VECTOR selfToEnemyVec = enemy.position - characterBodyTrans.position;
	// ベクトルを正規化する
	selfToEnemyVec = SafeNormlizeVector(selfToEnemyVec);

	// レイキャストの向きを敵キャラクタに向ける
	charaViewingDirRaycast->direction = selfToEnemyVec;

	// レイキャストに衝突しているモデルで自分に近い順でソートする
	charaViewingDirRaycast->GetHitModelInfo().sort();

	// 一番近いモデルの情報を取得
	HitModelInfo nearModel = charaViewingDirRaycast->GetHitModelInfo().front();

	// モデルが壁かどうか調べる
	if (nearModel.hitModel->GetObjectType() == ObjectType::TERRAIN_WALL)
	{
		// 敵との間に壁が存在しているとみなし、trueを返す
		return true;
	}

	return false;
}

/*-------------------------------------------*/
/* 地形IDから地形の座標を取得
/*-------------------------------------------*/
VECTOR ControllerEnemy::getTerrainPosFromId(TerrainId id)
{
	// 地形のサイズを取得
	const VECTOR TERRAIN_SIZE = StageManager::GetTerrainSize();

	// IDから地形の座標を算出
	VECTOR ret = ZERO_VECTOR;
	ret.x = id.x *  TERRAIN_SIZE.x;
	ret.z = id.z * -TERRAIN_SIZE.z;

	return ret;
}

/*-------------------------------------------*/
/* 地形の座標から地形IDを取得
/*-------------------------------------------*/
TerrainId ControllerEnemy::getTerrainIdFromPos(VECTOR position)
{
	// 地形のサイズを取得
	const VECTOR TERRAIN_SIZE = StageManager::GetTerrainSize();

	// 指定の座標から地形のIDを算出
	TerrainId ret;
	ret.x = (int)std::round(position.x /  TERRAIN_SIZE.x);
	ret.z = (int)std::round(position.z / -TERRAIN_SIZE.z);

	return ret;
}
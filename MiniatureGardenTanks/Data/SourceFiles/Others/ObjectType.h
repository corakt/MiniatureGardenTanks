#pragma once

/*-------------------------------------------*/
/* プログラム全体で扱うオブジェクトの種類
/*（オブジェクトの識別に使用する）
/*-------------------------------------------*/
enum class ObjectType
{
	CHARACTER_PLAYER,		// キャラクター：プレイヤー
	CHARACTER_ENEMY,		// キャラクター：エネミー
	CHARACTER_TANKBODY,		// キャラクター：戦車の車体
	CHARACTER_TANKTURRET,	// キャラクター：戦車砲塔
	CHARACTER_LASER,		// キャラクター：レーザー
	CHARACTER_SHOT,			// キャラクター：ショット
	TERRAIN_GROUND,			// ステージの地形：地形の地面
	TERRAIN_WALL,			// ステージの地形：地形の壁
};
#pragma once
#include "../CharacterController/ControllerBase.h"
#include "../Stage/TerrainBase.h"
#include <DxLib.h>
#include <list>
#include <random>

class CharacterBase;
class SphereCollider;
class CircularSectorCollider;
class Raycast;

// ベクトルのランダム値生成
struct VectorRandom
{
	std::uniform_int_distribution<int> x;	// X軸
	std::uniform_int_distribution<int> z;	// Y軸
};

// エネミーの制御で扱うカウンター
struct ControllerEnemyCounters
{
	int changeMovePosInterval;			// 目標位置が変化する間隔
	int firingShot;						// ショット発射用のカウント
	int areaStayTime;					// 同じエリアに留まり続けている時間
	int changeTimeToNormalMove;			// 状態を通常移動に戻す際の時間
};

// エネミーの制御で扱う地形データ
class TerrainData
{
public:
	TerrainId    id;	// 地形ID
	VECTOR position;	// 地形の座標
	float  distance;	// 他のオブジェクトとの距離

	// 距離で比較を行う（近い順）
	bool operator<(const TerrainData& right) const
	{
		return distance < right.distance;
	}
};

// エネミーの制御で扱う敵キャラクターのデータ
class EnemyCharacterData
{
public:
	VECTOR position;	// キャラクターの位置
	float  distance;	// 敵キャラクターとの距離

	// 距離で比較を行う（近い順）
	bool operator<(const EnemyCharacterData& right) const
	{
		return distance < right.distance;
	}
};

/*-------------------------------------------*/
/* キャラクターの制御 : エネミー / 派生クラス
/*-------------------------------------------*/
class ControllerEnemy : public ControllerBase
{
public:
			 ControllerEnemy(CharacterBase* controlCharacter);
	virtual ~ControllerEnemy();

	void Initialize() override;		// 初期化
	void Update()     override;		// 更新

	void ControlBehaviorPattern();	// 行動パターンを制御
	void ControlMovePattern();		// 移動パターンを制御

	// 行動パターン
	//（敵に対する攻撃は追跡時に行う）
	enum BehaviorPattern
	{
		STOP,		// 停止
		SEARCH,		// 探索
		CHASE,		// 追跡
		ESCAPE,		// 逃避
	};

	// 移動パターン
	enum MovePattern
	{
		TOWARDS_TARGETPOS,		// 目標位置に向かって移動
		ALONG_WALL,				// 壁に沿って移動
	};

private:
	// 行動パターン関数群
	void behaviorOfSearch();	// 探索
	void behaviorOfChase();		// 追跡
	void behaviorOfEscape();	// 逃避

	// 行動パターン関数群
	void moveOfTowardsTargetPos();		// 目標位置に向かって移動
	void moveOfAlongWall();				// 壁に沿って移動

	// AI処理関数群
	void setTargetMovePosition();									// 移動目標位置を設定
	void getThroughTerrainData();									// 通過した地面のIDを取得
	void getCharacterFourDirGroundData();							// キャラクターの位置を基準に上下左右の地面のデータを取得
	void getViewingRangeEnemyCharacter();							// 視野の範囲内に存在している敵キャラクターを取得
	void getAroundRangeWallData();									// キャラクターの付近に存在している壁のデータを取得
	bool isNarrowRangeStayCharacter();								// 狭い範囲にキャラクターが留まり続けているか
	bool existWallBetweenEnemyAndSelf(EnemyCharacterData enemy);	// 自分と敵との間に壁が存在しているか
	VECTOR    getTerrainPosFromId(TerrainId id);					// 地形IDから地形の座標を取得
	TerrainId getTerrainIdFromPos(VECTOR position);					// 地形の座標から地形IDを取得

	VECTOR                 moveTargetPosition;					// 移動目標位置
	VECTOR                 moveTargetDirection;					// 移動目標への方向
	VECTOR                 turretDirection;						// 砲塔の向き
	EnemyCharacterData     targetCharacterData;					// 攻撃対象のキャラクターのデータ
	TerrainData            lastThroughGroundData;				// 最後に通過した地面のデータ
	std::list<TerrainData> fourDirGroundData;					// キャラクター中心に上下左右四面の地面のデータ
	std::list<TerrainData> aroundRangeWallData;					// キャラクターの付近に存在している壁のデータ
	std::list<TerrainData> throughGroundData;					// 通過済みの地面のデータ
	std::list<EnemyCharacterData> viewingRangeCharacterData;	// 視野の範囲内に存在しているキャラクターのデータ

	BehaviorPattern          behaviorPattern;					// 行動パターン
	MovePattern              movePattern;						// 移動パターン
	ControllerEnemyCounters  counters;							// エネミーの制御で扱うカウンター

	// コライダー関連
	SphereCollider*          charaAroundCollidr;				// キャラクターを中心としたコライダー
	CircularSectorCollider*  charaViewingCollider;				// キャラクターの視野のコライダー
	Raycast*                 charaViewingDirRaycast;			// キャラクターの視野と同じ向きのレイキャスト

	// ランダム関連
	std::mt19937             mt19937;							// メルセンヌ・ツイスタ乱数生成器
	VectorRandom             moveTargetPosRand;					// 移動目標位置のランダム値

	static const VECTOR NORMALMOVE_TARGETPOS_MIN;				// 通常移動の目標位置の最小値
	static const VECTOR NORMALMOVE_TARGETPOS_MAX;				// 通常移動の目標位置の最大値
	static const float  TRACKING_ENEMY_DISTANCE_MIN;			// 追跡時の敵との距離の最小値
	static const float  TRACKING_ENEMY_DISTANCE_MAX;			// 追跡時の敵との距離の最大値
	static const float  FIRINGSHOT_ENEMY_DISTANCE;				// ショットを発射する際の敵との距離
	static const float  CHARACTER_VIEWING_WIDTH;				// キャラクターの視野の横幅
	static const float  CHARACTER_VIEWING_DISTANCE;				// キャラクターの視野の距離
	static const UINT   MOVETARGETPOS_CHANGEINTERVAL_COUNTNUM;	// 移動目標位置を変更する間隔
	static const UINT   THROUGHGROUND_SAVE_MAX;					// 通過済みの地面を登録しておく最大数
	static const float  EVADE_WALL_DISTANCE;					// 回避する壁との距離
	static const float  CHARACTER_NARROWRANGE_RADIUS;			// 狭い範囲の半径
	static const int    NARROWRANGE_STAY_TIME;					// 狭い範囲に留まり続けているとみなす時間
	static const float  PARALLELMOVE_CHANGE_DISTANCE;			// 追跡中に平行移動に切り替える敵との距離
	static const float  RAYCAST_DISTANCE;						// レイキャストの距離
};
#include "../CharacterController/ControllerEnemy.h"
#include "../CharacterShot/ShotManager.h"
#include "../Collision/SphereCollider.h"
#include "../Collision/CircularSectorCollider.h"
#include "../Raycast/Raycast.h"
#include "../Collision/CollisionManager.h"
#include "../Raycast/RaycastManager.h"
#include "../Others/Define.h"
#include "../Others/Math3D.h"

const VECTOR ControllerEnemy::NORMALMOVE_TARGETPOS_MIN              = VGet(2000,  0, 2000);		// 通常移動の目標位置の最小値
const VECTOR ControllerEnemy::NORMALMOVE_TARGETPOS_MAX              = VGet(28000, 0, 28000);	// 通常移動の目標位置の最大値
const float  ControllerEnemy::TRACKING_ENEMY_DISTANCE_MIN           = 4000;						// 追跡時の敵との距離の最小値
const float  ControllerEnemy::TRACKING_ENEMY_DISTANCE_MAX           = 15000;					// 追跡時の敵との距離の最大値
const float  ControllerEnemy::FIRINGSHOT_ENEMY_DISTANCE             = 10000;					// ショットを発射する際の敵との距離
const float  ControllerEnemy::CHARACTER_VIEWING_WIDTH               = 0.8f;						// キャラクターの視野の横幅
const float  ControllerEnemy::CHARACTER_VIEWING_DISTANCE            = 20000;					// キャラクターの視野の距離
const UINT   ControllerEnemy::MOVETARGETPOS_CHANGEINTERVAL_COUNTNUM = 300;						// 移動目標位置を変更する間隔
const UINT   ControllerEnemy::THROUGHGROUND_SAVE_MAX                = 6;						// 通過済みの地面を登録しておく最大数
const float  ControllerEnemy::EVADE_WALL_DISTANCE                   = 1500;						// 回避する壁との距離
const float  ControllerEnemy::CHARACTER_NARROWRANGE_RADIUS          = 8000;						// キャラクターの周辺範囲の半径
const int    ControllerEnemy::NARROWRANGE_STAY_TIME                 = 100;						// 狭い範囲に留まり続けているとみなす時間
const float  ControllerEnemy::PARALLELMOVE_CHANGE_DISTANCE          = 2000;						// 追跡中に平行移動に切り替える敵との距離
const float  ControllerEnemy::RAYCAST_DISTANCE                      = 20000;					// レイキャストの距離

// ランダムパラメータ
typedef std::uniform_int_distribution<int>::param_type RandParam;

/*-------------------------------------------*/
/* コンストラクタ
/*-------------------------------------------*/
ControllerEnemy::ControllerEnemy(CharacterBase* controlCharacter)
	:ControllerBase::ControllerBase(controlCharacter)
{
	// NULLで初期化
	charaAroundCollidr     = NULL;
	charaViewingCollider   = NULL;
	charaViewingDirRaycast = NULL;

	// キャラクターを中心とした球体コライダーを生成
	charaAroundCollidr = COLLISION_MANAGER.AddSphereCollider();
	// キャラクターの視野の扇形コライダーを生成
	charaViewingCollider = COLLISION_MANAGER.AddCircularSectorCollider();
	// キャラクターの視野と同じ向きのレイキャストを生成（０で初期化）
	charaViewingDirRaycast = RAYCAST_MANAGER.AddRaycast(ZERO_VECTOR, ZERO_VECTOR, 0);
}

/*-------------------------------------------*/
/* デストラクタ
/*-------------------------------------------*/
ControllerEnemy::~ControllerEnemy()
{
	// 各ポインタにNULLを代入
	charaAroundCollidr     = NULL;
	charaViewingCollider   = NULL;
	charaViewingDirRaycast = NULL;

	// 各リストを削除
	fourDirGroundData.clear();				// キャラクター中心に上下左右四面の地面のデータ
	aroundRangeWallData.clear();			// キャラクターの付近に存在している壁のデータ
	throughGroundData.clear();				// 通過済みの地面のデータ
	viewingRangeCharacterData.clear();		// 視野の範囲内に存在しているキャラクターのデータ
}

/*-------------------------------------------*/
/* 初期化
/*-------------------------------------------*/
void ControllerEnemy::Initialize()
{
	// 共通の初期化処理
	commonInitialize();

	// 各変数の初期化
	moveTargetPosition  = ZERO_VECTOR;						// 移動目標位置
	moveTargetDirection = ZERO_VECTOR;						// 移動目標への方向
	turretDirection     = ZERO_VECTOR;						// 砲塔の向き
	
	targetCharacterData.position = ZERO_VECTOR;				// キャラクターの位置
	targetCharacterData.distance = 0;						// 自キャラクターとの距離

	counters.changeMovePosInterval  = 0;					// 目標位置が変化する間隔
	counters.firingShot             = 0;					// ショット発射用のカウント
	counters.areaStayTime           = 0;					// 同じエリアに留まり続けている時間
	counters.changeTimeToNormalMove = 0;					// 状態を通常移動に戻す際の時間

	lastThroughGroundData.id.x     = 0;						// 地形ID（X軸）
	lastThroughGroundData.id.z     = 0;						// 地形ID（Z軸）
	lastThroughGroundData.position = ZERO_VECTOR;			// 地形の座標
	lastThroughGroundData.distance = 0;						// 自キャラクターとの距離

	behaviorPattern = BehaviorPattern::SEARCH;				// 行動パターンを初期化
	movePattern     = MovePattern::TOWARDS_TARGETPOS;		// 移動パターンを初期化（目標位置に向かって移動）

	// ランダム関連のパラメータを設定
	std::random_device randomDevice;						// ランダムデバイスを生成
	mt19937.seed(randomDevice());							// ランダムのシード値を設定

	// キャラクターの車体のオブジェクトを取得
	ModelObject* tankBody = controlCharacter->GetParts(CharacterBase::PartsType::BODY);
	// 車体のトランスフォームを取得
	Transform bodyTrans = tankBody->GetTransform();

	// キャラクターの砲塔のオブジェクトを取得
	ModelObject* tankTurret = controlCharacter->GetParts(CharacterBase::PartsType::TURRET);
	// 砲塔のトランスフォームを取得
	Transform turretTrans = tankTurret->GetTransform();

	// 各コライダーのパラメータを設定
	charaAroundCollidr->center         = bodyTrans.position;				// キャラクターの位置をコライダーの基準位置として設定
	charaAroundCollidr->radius         = CHARACTER_NARROWRANGE_RADIUS;		// コライダーの半径
	charaAroundCollidr->attachedModel  = tankBody;							// キャラクターの車体をコライダーにアタッチするモデルとして設定
	charaAroundCollidr->isCollCheck    = true;								// 衝突判定を行うかどうかのフラグを立てる

	charaViewingCollider->center        = bodyTrans.position;				// キャラクターの位置をコライダーの基準位置として設定
	charaViewingCollider->direction     = turretTrans.direction;			// キャラクターの砲塔の向きをコライダーの向きとして設定
	charaViewingCollider->width         = CHARACTER_VIEWING_WIDTH;			// キャラクターの視野の横幅
	charaViewingCollider->distance      = CHARACTER_VIEWING_DISTANCE;		// キャラクターの視野の距離
	charaViewingCollider->attachedModel = tankBody;							// キャラクターの車体をコライダーにアタッチするモデルとして設定
	charaViewingCollider->isCollCheck   = true;								// 衝突判定を行うかどうかのフラグを立てる

	// レイキャストのパラメータを設定
	charaViewingDirRaycast->origin        = turretTrans.position;			// レイキャストの原点をキャラクターと同じ位置にする
	charaViewingDirRaycast->direction     = turretTrans.direction;			// レイキャストの向きをキャラクターと同じにする	
	charaViewingDirRaycast->distance      = RAYCAST_DISTANCE;				// レイキャストの距離
	charaViewingDirRaycast->attachedModel = tankTurret;						// キャラクターの車体をレイキャストにアタッチする
}

/*-------------------------------------------*/
/* 更新
/*-------------------------------------------*/
void ControllerEnemy::Update()
{
	// キャラクターの制御フラグがfalseだったら、
	// 何も処理せずに関数を抜ける
	if (controlCharacter->GetControlingFlag() == false) { return; }

	// キャラクターの各部分のトランスフォーム取得
	characterBodyTrans = characterBody->GetTransform();					// 車体部分
	characterTurretTrans = characterTurret->GetTransform();				// 砲塔部分

	// コライダーのパラメータを更新する
	// キャラクターを中心とした球体コライダー
	charaAroundCollidr->center = characterBodyTrans.position;			// キャラクターの位置をコライダーの基準位置として設定
	// キャラクターの視野の扇形コライダー
	charaViewingCollider->center = characterBodyTrans.position;			// キャラクターの位置をコライダーの基準位置として設定
	charaViewingCollider->direction = characterTurretTrans.direction;	// キャラクターの砲塔の向きをコライダーの向きとして設定

	// レイキャストのパラメータを更新する
	charaViewingDirRaycast->origin = characterTurretTrans.position;		// レイキャストの原点をキャラクターと同じ位置にする

	// 移動目標位置を設定
	setTargetMovePosition();
	// キャラクターの位置を基準に上下左右の地面のデータを取得
	getCharacterFourDirGroundData();
	// 通過した地面のIDを取得
	getThroughTerrainData();
	// 視野の範囲内に存在している敵キャラクターを取得
	getViewingRangeEnemyCharacter();
	// コライダーに衝突している壁のデータを取得
	getAroundRangeWallData();

	// 戦車が破壊されていなければ、状態の制御を行う
	if (controlCharacter->GetState() != CharacterBase::State::BROKEN)
	{
		// 行動パターンを制御
		ControlBehaviorPattern();
		// 移動パターンを制御
		ControlMovePattern();
	}
	// 戦車が破壊された場合は
	else
	{
		// 停止状態に変更する
		behaviorPattern = BehaviorPattern::STOP;
	}

	/*-------------------------------------------*/
	/* 行動パターンによって、関数を変更する
	/*-------------------------------------------*/
	switch (behaviorPattern)
	{
		// 行動パターン：停止
	case BehaviorPattern::STOP:
		// 停止中は何もしない
		break;

		// 行動パターン：探索
	case BehaviorPattern::SEARCH:
		behaviorOfSearch();
		break;

		// 行動パターン：追跡
	case BehaviorPattern::CHASE:
		behaviorOfChase();
		break;

		// 行動パターン：逃避
	case BehaviorPattern::ESCAPE:
		behaviorOfEscape();
		break;

	default:
		// デフォルトは"探索"に設定しておく
		behaviorOfSearch();
		break;
	}

	/*-------------------------------------------*/
	/* 移動パターンによって、関数を変更する
	/*-------------------------------------------*/
	// 状態が"停止"じゃなければ、移動の処理を行う
	if (behaviorPattern != BehaviorPattern::STOP)
	{
		switch (movePattern)
		{
			// 移動パターン：目標位置に向かって移動
		case MovePattern::TOWARDS_TARGETPOS:
			moveOfTowardsTargetPos();
			break;

			// 移動パターン：壁に沿って移動
		case MovePattern::ALONG_WALL:
			moveOfAlongWall();
			break;

		default:
			// デフォルトは"目標位置に向かって移動"に設定しておく
			moveOfTowardsTargetPos();
			break;
		}

		/*-------------------------------------------*/
		/* 車体の移動
		/*-------------------------------------------*/
		move(true);

		/*-------------------------------------------*/
		/* 車体の回転
		/*-------------------------------------------*/
		rotationBody(moveTargetDirection, true);

		/*-------------------------------------------*/
		/* 砲塔の回転
		/*-------------------------------------------*/
		rotationTurret(turretDirection);

		/*-------------------------------------------*/
		/* ショットの発射時の反動
		/*-------------------------------------------*/
		// 反動の最中かどうかのフラグを取得
		bool isReaction = controlCharacter->GetReactionFlag();
		// フラグがたっている場合は、反動の処理にうつる
		if (isReaction)
		{
			firingReaction();
		}
	}

	// キャラクターの各部分のトランスフォームセット
	characterBody->SetTransform(characterBodyTrans);		// 車体部分
	characterTurret->SetTransform(characterTurretTrans);	// 砲塔部分
}

/*-------------------------------------------*/
/* 行動パターンを制御
/*-------------------------------------------*/
void ControllerEnemy::ControlBehaviorPattern()
{
	// 敵を見つけていなければ
	if (viewingRangeCharacterData.empty())
	{
		// 行動パターンを[探索]に切り替える
		behaviorPattern = BehaviorPattern::SEARCH;
	}
	// 敵を一体見つけたら
	else if (viewingRangeCharacterData.size() == 1)
	{
		// 追跡対象のキャラクターとして登録
		targetCharacterData = viewingRangeCharacterData.front();

		// 敵キャラクターとの間に壁が存在しているか
		bool isWall = existWallBetweenEnemyAndSelf(targetCharacterData);
		// 壁が存在していなければ、追跡を開始する
		if (isWall == false)
		{
			// 行動パターンを[追跡]に切り替える
			behaviorPattern = BehaviorPattern::CHASE;
		}
		// 前方に壁が存在していれば、
		// 敵キャラクターを見つけていないとみなし、探索を続行する
		else
		{
			behaviorPattern = BehaviorPattern::SEARCH;
		}
	}
	// 敵を複数見つけたら
	else if (viewingRangeCharacterData.size() > 1)
	{
		// 行動パターンを[逃避]に切り替える
		behaviorPattern = BehaviorPattern::ESCAPE;
	}
}

/*-------------------------------------------*/
/* 移動パターンを制御
/*-------------------------------------------*/
void ControllerEnemy::ControlMovePattern()
{
	// キャラクターが狭い範囲内に留まり続けていないか調べる
	bool isAreaStay = isNarrowRangeStayCharacter();
	if (isAreaStay)
	{
		// 留まり続けていた場合、
		// 一番近い壁に沿って移動するモードに切り替える
		movePattern = MovePattern::ALONG_WALL;
	}
	else
	{
		// 移動目標位置の最小値と最大値をセット（X軸）
		moveTargetPosRand.x.param(RandParam((int)NORMALMOVE_TARGETPOS_MIN.x,
											(int)NORMALMOVE_TARGETPOS_MAX.x));
		// 移動目標位置の最小値と最大値をセット（Z軸）
		moveTargetPosRand.z.param(RandParam((int)NORMALMOVE_TARGETPOS_MIN.z,
											(int)NORMALMOVE_TARGETPOS_MAX.z));

		// 狭い範囲内から抜け出したら、
		// 目標位置に向かって移動するモードに変更する
		movePattern = MovePattern::TOWARDS_TARGETPOS;
	}
}

/*-------------------------------------------*/
/* 行動パターン：探索
/*-------------------------------------------*/
void ControllerEnemy::behaviorOfSearch()
{
	// 砲塔を車体と同じ向きにする
	turretDirection = characterBodyTrans.direction;
}

/*-------------------------------------------*/
/* 行動パターン：追跡
/*-------------------------------------------*/
void ControllerEnemy::behaviorOfChase()
{
	// 敵キャラクターの座標を取得
	VECTOR enemyPos = targetCharacterData.position;

	// 敵キャラクターとのベクトルを算出
	VECTOR charaPairVec = enemyPos - characterBodyTrans.position;
	// 敵キャラクターとの距離の２乗を算出
	float enemyDistance = VSquareSize(charaPairVec);
	// ベクトルを正規化する
	charaPairVec = SafeNormlizeVector(charaPairVec);

	// 追跡時の移動目標位置の最小値と最大値を設定（X軸）
	moveTargetPosRand.x.param(RandParam((int)(enemyPos.x + TRACKING_ENEMY_DISTANCE_MIN),
										(int)(enemyPos.x + TRACKING_ENEMY_DISTANCE_MAX)));
	// 追跡時の移動目標位置の最小値と最大値を設定（Z軸）
	moveTargetPosRand.z.param(RandParam((int)(enemyPos.z + TRACKING_ENEMY_DISTANCE_MIN),
										(int)(enemyPos.z + TRACKING_ENEMY_DISTANCE_MAX)));

	// キャラクターと移動目標位置とのベクトルを算出
	VECTOR targetPosToChara = moveTargetPosition - characterBodyTrans.position;
	// ベクトルを正規化する
	targetPosToChara = SafeNormlizeVector(targetPosToChara);
	// キャラクターの向きを移動目標位置のほうに向ける
	moveTargetDirection = targetPosToChara;

	// 敵キャラクターとの距離がある程度縮まったら、ショットを発射する
	if (enemyDistance < std::pow(FIRINGSHOT_ENEMY_DISTANCE, 2))
	{
		// リロードフラグを取得
		bool isReload = controlCharacter->GetReloadFlag();
		// リロード中じゃなければ
		if (isReload == false)
		{
			// 現在の発射するショットの数を取得
			int firingShotNum = controlCharacter->GetFiringShotNum();
			// ショットを発射する
			SHOT_MANAGER.FiringShot(controlCharacter, firingShotNum);

			// リロードフラグをオンにする
			controlCharacter->SetReloadFlag(true);
			// ショット発射時の反動フラグをオンにする
			controlCharacter->SetReactionFlag(true);
		}
	}

	// 砲塔を敵キャラクターに向ける
	turretDirection = charaPairVec;
}

/*-------------------------------------------*/
/* 行動パターン：逃避
/*-------------------------------------------*/
void ControllerEnemy::behaviorOfEscape()
{
	// 一番近いキャラクターを取得
	EnemyCharacterData nearCharacter = viewingRangeCharacterData.front();
	// 一番近いキャラクターとのベクトルを取得
	VECTOR charaPairVec = nearCharacter.position - characterBodyTrans.position;
	// ベクトルを正規化
	charaPairVec = SafeNormlizeVector(charaPairVec);

	// 一番近いキャラクターに対して、逆方向に逃げる
	moveTargetDirection = charaPairVec * -1;
}

/*-------------------------------------------*/
/* 移動パターン：目標位置に向かって移動
/*-------------------------------------------*/
void ControllerEnemy::moveOfTowardsTargetPos()
{
	// キャラクターを中心に上下左右の地面のデータが存在していなければ
	// そのまま関数を抜ける
	if (fourDirGroundData.empty()) { return; }

	// 上下左右の地面で目標位置に一番近い順でソートする
	fourDirGroundData.sort();

	// 目標位置に一番近い地面に向かって移動する
	moveTargetDirection = SafeNormlizeVector(fourDirGroundData.front().position - characterBodyTrans.position);
}

/*-------------------------------------------*/
/* 移動パターン：壁に沿って移動
/*-------------------------------------------*/
void ControllerEnemy::moveOfAlongWall()
{
	// コライダーに衝突している壁のオブジェクトが無ければ
	// そのまま関数を抜ける
	if (aroundRangeWallData.empty()) { return; }

	// コライダーに衝突している壁の中で、
	// キャラクターに近い順でソートする
	aroundRangeWallData.sort();

	// キャラクターから一番近い壁への向きベクトルを算出
	VECTOR charaToWallDir = SafeNormlizeVector(aroundRangeWallData.front().position - characterBodyTrans.position);
	charaToWallDir.y = 0;

	// 一番近い壁に沿った移動を行う
	moveTargetDirection = VCross(charaToWallDir, VGet(0, 1, 0));	// 車体の向きを回転
}
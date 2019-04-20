#pragma once
#include "../BaseObject/Transform.h"
#include "../Others/Singleton.h"
#include <DxLib.h>
#include <list>
#include <vector>

class TerrainBase;
class ModelObject;

// ステージ用パラメータ
struct StageParameter
{
	int groundModelHandle;		// 地面のモデルハンドル
	int wallModelHandle;		// 壁のモデルハンドル
	int skydomeModelHandle;		// スカイドームのモデルハンドル
	LPCSTR wallMapFileName;		// 壁の配置のcsvファイル
};

/*-------------------------------------------*/
/* ステージ / 管理クラス（シングルトン）
/*-------------------------------------------*/
class StageManager final : public Singleton<StageManager>
{
	friend class Singleton<StageManager>;

public:
	         StageManager();				// コンストラクタ
	virtual ~StageManager();				// デストラクタ

	void CreateStage();						// ステージの生成
	void DestroyStage();					// ステージの削除

	void ReadWallMapData(LPCSTR fileName);	// 壁の配置データをマップから読み込む
	void SetTerrainPosition();				// 地形の配置位置を計算

	void Initialize();						// 初期化
	void Update();							// 更新
	void Draw();							// 描画
	void DrawSkydome();						// スカイドームの描画

	static const VECTOR GetTerrainSize() { return TERRAIN_SIZE; }	// 地形のサイズを取得

private:
	TerrainBase*** terrainGround;					// 地形：地面
	TerrainBase*** terrainWall;						// 地形：壁

	std::list<TerrainBase*>   activeAllTerrain;		// 活動中の全ての地形のリスト
	std::vector<TerrainBase*> activeTerrainGround;	// 活動中の地面のリスト
	std::list<TerrainBase*>   activeTerrainWall;	// 活動中の壁のリスト

	std::vector<std::vector<int> > wallMapData;		// 壁の配置データ

	ModelObject* skydome;							// スカイドーム

	static const int    STAGE_WIDTH;				// ステージの横幅
	static const int    STAGE_DEPTH;				// ステージの奥行
	static const VECTOR TERRAIN_SIZE;				// 地形のサイズ
	static const VECTOR SKYDOME_POSITION;			// スカイドームの座標
	static const VECTOR SKYDOME_SCALE;				// スカイドームのスケール
};

#define STAGE_MANAGER StageManager::GetInstance()
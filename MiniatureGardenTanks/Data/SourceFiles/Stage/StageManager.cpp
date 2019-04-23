#include "../Stage/StageManager.h"
#include "../Stage/TerrainBase.h"
#include "../Stage/TerrainGround.h"
#include "../Stage/TerrainWall.h"
#include "../Collision/CollisionManager.h"
#include "../ResourcesManager/ResourceModelManager.h"
#include "../BaseObject/ModelObject.h"
#include "../Others/Define.h"
#include <string>
#include <fstream>
#include <sstream>

const int    StageManager::STAGE_WIDTH      = 20;							// ステージの横幅
const int    StageManager::STAGE_DEPTH      = 20;							// ステージの奥行
const VECTOR StageManager::TERRAIN_SIZE     = VGet(1500,1500,1500);			// 地形のサイズ
const VECTOR StageManager::SKYDOME_POSITION = VGet(22500, -2000, 22500);	// スカイドームの座標
const VECTOR StageManager::SKYDOME_SCALE    = VGet(400, 400, 400);			// スカイドームのスケール

/*-------------------------------------------*/
/* コンストラクタ
/*-------------------------------------------*/
StageManager::StageManager()
{
	// NULLで初期化する
	terrainGround = NULL;		// 地形：地面
	terrainWall   = NULL;		// 地形：壁
	skydome       = NULL;		// スカイドーム

	// ステージの奥行の数だけメモリを確保
	terrainGround = new TerrainBase**[STAGE_DEPTH]();
	terrainWall   = new TerrainBase**[STAGE_DEPTH]();
	// ステージの横幅の数だけメモリを確保
	for (int z = 0; z < STAGE_DEPTH; z++)
	{
		terrainGround[z] = new TerrainBase*[STAGE_WIDTH]();
		terrainWall[z]   = new TerrainBase*[STAGE_WIDTH]();
	}

	// 確保したメモリをNULLで初期化
	for (int z = 0; z < STAGE_DEPTH; z++)
	{
		for (int x = 0; x < STAGE_WIDTH; x++)
		{
			terrainGround[z][x] = NULL;
			terrainWall[z][x]   = NULL;
		}
	}
}

/*-------------------------------------------*/
/* デストラクタ
/*-------------------------------------------*/
StageManager::~StageManager()
{
	// ステージ生成用に確保していたメモリを全て解放する
	for (int z = 0; z < STAGE_DEPTH; z++)
	{
		SafeDeleteArray(terrainGround[z]);
		SafeDeleteArray(terrainWall[z]);
	}
	SafeDeleteArray(terrainGround);
	SafeDeleteArray(terrainWall);
}

/*-------------------------------------------*/
/* ステージの生成
/*-------------------------------------------*/
void StageManager::CreateStage()
{
	// 壁の配置の際に使用するモデルの識別ID
	const int WALL_MAPDATA_ID = 1;

	// 配置データをもとに壁を配置していく
	for (int z = 0; z < STAGE_DEPTH; z++)
	{
		for (int x = 0; x < STAGE_WIDTH; x++)
		{
			// 配列の要素番号を地形IDとして取得
			TerrainId tmpId;
			tmpId.x = x;
			tmpId.z = z;

			// 地形：壁
			if (wallMapData[z][x] == WALL_MAPDATA_ID)
			{
				// 地形の壁を生成
				terrainWall[z][x] = new TerrainWall();
				// 地形IDをセット
				terrainWall[z][x]->SetId(tmpId);
				// 生成した地形のインスタンスをリストに挿入
				activeTerrainWall.push_back(terrainWall[z][x]);
				activeAllTerrain.push_back(terrainWall[z][x]);
			}

			// 地形：地面
			// 地形の地面を生成
			terrainGround[z][x] = new TerrainGround();
			// 地形IDをセット
			terrainGround[z][x]->SetId(tmpId);
			// 生成した地形のインスタンスをリストに挿入
			activeTerrainGround.push_back(terrainGround[z][x]);
			activeAllTerrain.push_back(terrainGround[z][x]);
		}
	}

	// スカイドームを生成する
	//skydome = new ModelObject(MODEL_MANAGER.GetHandle(ResourceModelManager::ModelType::SKYDOME),ModelType::SKYDOME);
}

/*-------------------------------------------*/
/* ステージの削除
/*-------------------------------------------*/
void StageManager::DestroyStage()
{
	// 各リストを全削除する
	activeAllTerrain.clear();
	activeTerrainGround.clear();
	activeTerrainWall.clear();
	// 地形の配置データを削除
	wallMapData.clear();

	// ステージの地面と壁のインスタンスを破棄してメモリを解放
	for (int z = 0; z < STAGE_DEPTH; z++)
	{
		for (int x = 0; x < STAGE_WIDTH; x++)
		{
			SafeDelete(terrainGround[z][x]);
			SafeDelete(terrainWall[z][x]);
		}

	}

	// スカイドームのインスタンスを削除してメモリを解放
	SafeDelete(skydome);
}

/*-------------------------------------------*/
/* 壁の配置データをマップから読み込む
/*-------------------------------------------*/
void StageManager::ReadWallMapData(LPCSTR fileName)
{
	// vector配列のサイズを変更
	wallMapData.resize(STAGE_DEPTH, std::vector<int>(STAGE_WIDTH));

	std::ifstream file(fileName);			            // ファイル
	std::vector<std::vector<std::string>> dataStr;		// 読み込んだを文字列を格納
	std::string str;							        // 読み込んだ文字列の一行分のデータ

	// vector配列のサイズを変更
	dataStr.resize(STAGE_DEPTH, std::vector<std::string>(STAGE_WIDTH));

	for (int iz = 0; iz < STAGE_DEPTH; iz++)
	{
		std::string tmp;						// 分割された文字列
		std::getline(file, str);				// 一行分の文字列を取得
		std::istringstream stream(str);

		for (int ix = 0; ix < STAGE_WIDTH; ix++)
		{
			// 一行分の文字列からカンマごとに分割する
			std::getline(stream, tmp, ',');
			// 分割した文字列を格納する
			dataStr[iz][ix] = tmp;

			// 文字列を数値に変換して格納する
			wallMapData[iz][ix] = atoi(dataStr[iz][ix].c_str());
		}
	}

	// vector配列の後始末
	dataStr.clear();

	// ファイルを閉じる
	file.close();
	// ファイル関連のデータを削除
	file.clear();
}

/*-------------------------------------------*/
/* 地形の配置位置を計算
/*-------------------------------------------*/
void StageManager::SetTerrainPosition()
{
	// 地面の配置
	if (activeTerrainGround.empty() == false)
	{
		for (TerrainBase*& terrainGroundElem : activeTerrainGround)
		{
			// NULLだったらスキップ
			if (terrainGroundElem == NULL) { continue; }

			// 地形IDの取得
			TerrainId terrainId = terrainGroundElem->GetId();

			// 配置する座標を計算
			VECTOR groundPos = VGet(terrainId.x * TERRAIN_SIZE.x, 0, terrainId.z * TERRAIN_SIZE.z);

			// トランスフォームを取得
			Transform groundTrans = terrainGroundElem->GetTransform();
			// 計算済みの座標を代入
			groundTrans.position = groundPos;
			// トランスフォームをセット
			terrainGroundElem->SetTransform(groundTrans);
		}
	}

	// 壁の配置
	if (activeTerrainWall.empty() == false)
	{
		for (TerrainBase*& terrainWallElem : activeTerrainWall)
		{
			// NULLだったら、スキップ
			if (terrainWallElem == NULL) { continue; }

			// 地形IDの取得
			TerrainId terrainId = terrainWallElem->GetId();

			// 配置する座標を計算
			VECTOR wallPos = VGet(terrainId.x * TERRAIN_SIZE.x, -110, terrainId.z * TERRAIN_SIZE.z);

			// トランスフォームを取得
			Transform wallTrans = terrainWallElem->GetTransform();
			// 計算済みの座標を代入
			wallTrans.position = wallPos;
			// トランスフォームをセット
			terrainWallElem->SetTransform(wallTrans);
		}
	}
}

/*-------------------------------------------*/
/* 初期化
/*-------------------------------------------*/
void StageManager::Initialize()
{
	if (activeAllTerrain.empty() == false)
	{
		for (TerrainBase*& terrainElem : activeAllTerrain)
		{
			// 地形インスタンスの初期化処理
			terrainElem->Initialize();
		}
	}

	// スカイドームのパラメータをセット
	if (skydome != NULL)
	{
		// スカイドームのモデルの初期化
		skydome->Initialize();

		// スカイドームのトランスフォームを取得
		Transform skydomeTrans = skydome->GetTransform();
		// スカイドームの座標を設定
		skydomeTrans.position = SKYDOME_POSITION;
		// スカイドームのスケールを設定
		skydomeTrans.scale = SKYDOME_SCALE;

		// スカイドームのトランスフォームをセット
		skydome->SetTransform(skydomeTrans);

		// スカイドームのアクティブフラグを立てる
		skydome->SetActiveFlag(true);
		// スカイドームの描画フラグを立てる
		skydome->SetDrawFlag(true);
	}
}

/*-------------------------------------------*/
/* 更新
/*-------------------------------------------*/
void StageManager::Update()
{
	if (activeAllTerrain.empty() == false)
	{
		for (TerrainBase*& terrainElem : activeAllTerrain)
		{
			// 地形インスタンスの更新処理
			terrainElem->Update();
		}
	}
}

/*-------------------------------------------*/
/* 描画
/*-------------------------------------------*/
void StageManager::Draw()
{
	if (activeAllTerrain.empty() == false)
	{
		for (TerrainBase*& terrainElem : activeAllTerrain)
		{
			// 地形インスタンスの描画処理
			terrainElem->Draw();
		}
	}
}

/*-------------------------------------------*/
/* スカイドームの描画
/*-------------------------------------------*/
void StageManager::DrawSkydome()
{
	if (skydome != NULL)
	{
		// スカイドームを描画
		skydome->DrawModel();
	}
}
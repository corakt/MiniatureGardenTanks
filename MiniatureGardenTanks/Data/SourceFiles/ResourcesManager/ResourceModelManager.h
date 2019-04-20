#pragma once
#include "../Others/Singleton.h"
#include <string>

struct ImageFileParam;

/*-------------------------------------------*/
/* モデルのリソースを管理するクラス
/*-------------------------------------------*/
class ResourceModelManager final : public Singleton<ResourceModelManager>
{
	friend class Singleton<ResourceModelManager>;

public:
			 ResourceModelManager();	// コンストラクタ
	virtual ~ResourceModelManager();	// デストラクタ

	// モデルの種類
	enum ModelType
	{
		TANK_BODY,			// 戦車：車体
		TANK_TURRET,		// 戦車：砲塔
		TANK_LASER,			// 戦車：レーザー
		TANK_SHOT,			// 戦車：ショット

		TERRAIN_GROUND,		// 地形：地面
		TERRAIN_WALL,		// 地形：壁
		SKYDOME,			// スカイドーム

		TYPE_NUM,			// モデルの種類数
	};

	// 全てのモデルを読み込む
	void LoadAllModel(ImageFileParam* imageFileParam);
	// 全てのモデルを削除する
	void DeleteAllModel();

	// 指定のモデルハンドルを取得
	const int& GetHandle(ModelType type) const;
	// 指定のモデルハンドルのファイルパスを取得
	const std::string GetFilePath(ModelType type) const;

private:
	int modelHandle[ModelType::TYPE_NUM];		// モデルハンドル

	static const std::string MODEL_FOLDER_RELATIVE_PATH;						// モデルフォルダへの相対パス
	static const std::string MODEL_FILE_RELATIVE_PATH[ModelType::TYPE_NUM];		// モデルファイルへの相対パス
};

#define MODEL_MANAGER ResourceModelManager::GetInstance()
typedef ResourceModelManager::ModelType ModelType;
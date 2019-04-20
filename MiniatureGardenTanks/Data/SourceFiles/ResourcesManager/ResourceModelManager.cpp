#include "../ResourcesManager/ResourceModelManager.h"
#include "../Others/AsyncLoadData.h"
#include "../Others/ErrorMessage.h"
#include "../Others/Define.h"
#include <DxLib.h>
#include <sstream>
#include <assert.h>

// モデルフォルダへの相対パス
const std::string ResourceModelManager::MODEL_FOLDER_RELATIVE_PATH = "Data/ResourceFiles/Models/";

// モデルファイルへの相対パス
const std::string ResourceModelManager::MODEL_FILE_RELATIVE_PATH[] = 
{ 
	"Character/Tank_Body.mqo",		// 戦車：車体
	"Character/Tank_Turret.mqo",	// 戦車：砲塔
	"Character/Tank_Laser.mqo",		// 戦車：レーザー
	"Character/Tank_Shot.mqo",		// 戦車：ショット
	
	"Stage/Terrain_Ground.mqo",		// 地形：地面
	"Stage/Terrain_Wall.mqo",		// 地形：壁
	"Stage/Skydome.x"				// スカイドーム
};

/*-------------------------------------------*/
/* コンストラクタ
/*-------------------------------------------*/
ResourceModelManager::ResourceModelManager()
{
	// 処理なし
}

/*-------------------------------------------*/
/* デストラクタ
/*-------------------------------------------*/
ResourceModelManager::~ResourceModelManager()
{
	// 処理なし
}

/*-------------------------------------------*/
/* 全てのモデルを読み込む
/*-------------------------------------------*/
void ResourceModelManager::LoadAllModel(ImageFileParam* imageFileParam)
{
	for (int i = 0; i < ModelType::TYPE_NUM; i++)
	{
		// メモリ内のイメージファイルからモデルを展開する
		modelHandle[i] = MV1LoadModelFromMem(imageFileParam[i].fileImage,
											 imageFileParam[i].fileSize,
											 LoadImageFileFunc,
											 ReleaseFileFunc,
											 NULL);

		// イメージファイルからの展開に失敗した場合
		if (modelHandle[i] == -1)
		{
			std::ostringstream tmpMessage;
			tmpMessage << "イメージファイルからモデルの展開に失敗しました。,ModelType:" << i;

			ErrorData errorData;
			errorData.errorMessage = tmpMessage.str();
			errorData.detectedErrorClassName = "ResourceModelManager";

			// エラーデータをリストに入れる
			ERROR_MESSAGE.SetErrorData(errorData);
		}
	}
}

/*-------------------------------------------*/
/* 全てのモデルを削除
/*-------------------------------------------*/
void ResourceModelManager::DeleteAllModel()
{
	for (int i = 0; i < ModelType::TYPE_NUM; i++)
	{
		// モデルを全て削除する
		if (modelHandle[i] != NULL)
		{
			SafeDeleteModel(modelHandle[i]);
		}
	}
	
	// モデルのデータが残っていれば、全て削除する
	MV1InitModel();
}

/*-------------------------------------------*/
/* 指定のモデルハンドルを取得
/*-------------------------------------------*/
const int& ResourceModelManager::GetHandle(ResourceModelManager::ModelType type) const
{
	// モデルハンドルがNULLであれば例外を投げる
	assert(modelHandle != NULL);
	// モデルハンドル以外の定数を指定していれば例外を投げる
	assert(type != ModelType::TYPE_NUM);

	// 指定のモデルハンドルを返す
	return modelHandle[type];
}

/*-------------------------------------------*/
/* 指定のモデルハンドルのファイルパスを取得
/*-------------------------------------------*/
const std::string ResourceModelManager::GetFilePath(ResourceModelManager::ModelType type) const
{
	// モデルハンドル以外の定数を指定していれば例外を投げる
	assert(type != ModelType::TYPE_NUM);

	// 指定のモデルハンドルのファイルパスを返す
	return MODEL_FOLDER_RELATIVE_PATH + MODEL_FILE_RELATIVE_PATH[type];
}
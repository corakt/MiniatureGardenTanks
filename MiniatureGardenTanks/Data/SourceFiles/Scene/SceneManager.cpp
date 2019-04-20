#include "../Scene/SceneManager.h"
#include "../Scene/SceneTitle.h"
#include "../Scene/SceneMain.h"
#include "../Scene/SceneLoading.h"
#include "../Scene/SceneResult.h"
#include "../Character/CharacterManager.h"
#include "../Others/Define.h"
#include "../Others/Input.h"
#include <cassert>

SceneBase* SceneManager::currentScene = NULL;
SceneManager::SceneType SceneManager::nextSceneType;
bool SceneManager::isSceneChange = false;

/*-------------------------------------------*/
/* コンストラクタ
/*-------------------------------------------*/
SceneManager::SceneManager()
{
	// 最初のシーンを生成
	currentScene = new SceneLoading();
	// シーンに必要なインスタンスを生成
	currentScene->CreateObject();
	// シーンに存在するインスタンスを初期化
	currentScene->Initialize();
}

/*-------------------------------------------*/
/* デストラクタ
/*-------------------------------------------*/
SceneManager::~SceneManager()
{
	// シーンのメモリを解放
	SafeDelete(currentScene);
}

/*-------------------------------------------*/
/* 更新
/*-------------------------------------------*/
void SceneManager::Update()
{
	if (currentScene != NULL)
	{
		currentScene->Update();
	}
}

/*-------------------------------------------*/
/* 描画
/*-------------------------------------------*/
void SceneManager::Draw()
{
	if (currentScene != NULL)
	{
		currentScene->Draw();
	}
}

/*-------------------------------------------*/
/* Effekseerの更新
/*-------------------------------------------*/
void SceneManager::UpdateEffekseer()
{
	// Effekseer用のダミー画像を取得
	int dummySprite = SPRITE_MANAGER.GetHandle(ResourceSpriteManager::SpriteType::EFFEKSEER_DUMMY);
	// ダミー画像を描画
	DrawGraph(0, 0, dummySprite, TRUE);
	// Effekseerにより再生中のエフェクトを更新する
	UpdateEffekseer3D();	// 3Dエフェクト
	UpdateEffekseer2D();	// 2Dエフェクト
}

/*-------------------------------------------*/
/* Effekseerの描画
/*-------------------------------------------*/
void SceneManager::DrawEffekseer()
{
	// Effekseerにより再生中のエフェクトを描画する
	DrawEffekseer3D();		// 3Dエフェクト
	DrawEffekseer2D();		// 2Dエフェクト
}

/*-------------------------------------------*/
/* 次のシーンの種類をセット
/*-------------------------------------------*/
void SceneManager::SetNextScene(SceneType sceneType)
{
	// ロード画面を呼ぼうとしていたら、例外を投げる
	//（ロードは一度しか行わないため、ロード画面を呼ぶことは許されない）
	assert(sceneType != SceneType::LOADING);

	// 次のシーンの種類をセットする
	nextSceneType = sceneType;
	// シーン変更のフラグをたてる
	isSceneChange = true;
}

/*-------------------------------------------*/
/* シーンの変更
/*-------------------------------------------*/
void SceneManager::ChangeScene()
{
	if (isSceneChange)
	{
		// シーンに存在するインスタンスを削除
		currentScene->DestroyObject();
		// シーンのインスタンスを削除
		SafeDelete(currentScene);

		
		switch (nextSceneType)
		{
		case SceneType::TITLE:
			currentScene = new SceneTitle();
			break;

		case SceneType::MAIN:
			currentScene = new SceneMain();
			break;

		case SceneType::RESULT:
			currentScene = new SceneResult();
			break;

		default:
			break;
		}

		// シーンに必要なインスタンスを生成
		currentScene->CreateObject();
		// シーンに存在するインスタンスを初期化
		currentScene->Initialize();

		// シーン変更のフラグをオフにする
		isSceneChange = false;
	}
}
#pragma once

class SceneBase;

/*-------------------------------------------*/
/* シーン / 管理クラス
/*-------------------------------------------*/
class SceneManager final
{
public:
	 SceneManager();			// コンストラクタ
	~SceneManager();			// デストラクタ
	
	void Update();				// 更新
	void Draw();				// 描画
	
	void UpdateEffekseer();		// Effekseerの更新
	void DrawEffekseer();		// Effekseerの描画

	// シーンの種類
	enum class SceneType
	{
		LOADING,	// ロード画面
		TITLE,		// タイトル
		MAIN,		// メイン
		RESULT,		// リザルト
	};

	// 次のシーンの種類をセット
	static void SetNextScene(SceneType sceneType);

	// シーンの変更
	void ChangeScene();

private:
	// 現在のシーンのインスタンス
	static SceneBase *currentScene;
	// 次のシーンの種類
	static SceneType nextSceneType;

	// シーンを変更するかどうかのフラグ
	static bool isSceneChange;

protected:
	// なし
};
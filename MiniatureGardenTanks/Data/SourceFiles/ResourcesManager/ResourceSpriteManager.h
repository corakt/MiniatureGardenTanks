#pragma once
#include "../Others/Singleton.h"
#include <string>

struct ImageFileParam;

/*-------------------------------------------*/
/* スプライトのリソースを管理するクラス
/*-------------------------------------------*/
class ResourceSpriteManager final : public Singleton<ResourceSpriteManager>
{
	friend class Singleton<ResourceSpriteManager>;

public:
			 ResourceSpriteManager();	// コンストラクタ
	virtual ~ResourceSpriteManager();	// デストラクタ

	// スプライトの種類
	enum SpriteType
	{
		// プレイヤー関連
		HPGAUGE,				// HPゲージ
		HPGAUGE_FRAME,			// HPゲージフレーム
		APGAUGE,				// アクティブゲージ
		APGAUGE_FRAME,			// アクティブゲージフレーム
		HITLOGO,				// 攻撃ヒット時のロゴ
		SKILLCUTIN,				// スキル使用時のカットイン
		TARGETSIGHT,			// 照準器

		// タイトル関連
		TITLE_LOGO,				// タイトルロゴ
		START_BUTTON,			// ゲーム開始ボタンの文字列

		// メイン関連
		COUNTDOWN_NUMBER_0,		// カウントダウンの数字_0
		COUNTDOWN_NUMBER_1,		// カウントダウンの数字_1
		COUNTDOWN_NUMBER_2,		// カウントダウンの数字_2
		COUNTDOWN_NUMBER_3,		// カウントダウンの数字_3
		COUNTDOWN_START,		// カウントダウン_スタート
		GAMESET_LOGO,			// ゲームセットロゴ

		// リザルト関連
		WIN_LOGO,				// 勝利ロゴ
		LOSE_LOGO,				// 敗北ロゴ
		BACKTITLE_BUTTON,		// タイトルへ戻るボタンの文字列

		EFFEKSEER_DUMMY,		// Effekseer用ダミー画像

		TYPE_NUM,				// スプライトの種類数
	};

	// 全てのスプライトを読み込む
	void LoadAllSprite(ImageFileParam* imageFileParam);
	// 全てのスプライトを削除する
	void DeleteAllSprite();

	// 指定のスプライトハンドルを取得
	const int& GetHandle(SpriteType type) const;
	// 指定のスプライトハンドルのファイルパスを取得
	const std::string GetFilePath(SpriteType type) const;

private:
	int spriteHandle[SpriteType::TYPE_NUM];			// スプライトハンドル

	static const std::string SPRITE_FOLDER_RELATIVE_PATH;							// スプライトフォルダへの相対パス
	static const std::string SPRITE_FILE_RELATIVE_PATH[SpriteType::TYPE_NUM];		// スプライトファイルへの相対パス
};

#define SPRITE_MANAGER ResourceSpriteManager::GetInstance()
typedef ResourceSpriteManager::SpriteType SpriteType;
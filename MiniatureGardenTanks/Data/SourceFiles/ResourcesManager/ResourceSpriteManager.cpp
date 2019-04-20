#include "../ResourcesManager/ResourceSpriteManager.h"
#include "../Others/AsyncLoadData.h"
#include "../Others/ErrorMessage.h"
#include "../Others/Define.h"
#include <DxLib.h>
#include <sstream>
#include <assert.h>

// スプライトフォルダへの相対パス
const std::string ResourceSpriteManager::SPRITE_FOLDER_RELATIVE_PATH = "Data/ResourceFiles/Sprites/";

// スプライトファイルへの相対パス
const std::string ResourceSpriteManager::SPRITE_FILE_RELATIVE_PATH[] = 
{ 
	// プレイヤー関連
	"Player/HpGauge.png",					// HPゲージ
	"Player/HpGauge_Frame.png",				// HPゲージフレーム
	"Player/ApGauge.png",					// アクティブゲージ
	"Player/ApGauge_Frame.png",				// アクティブゲージフレーム
	"Player/Shot_Hit.png",					// 攻撃ヒット時のロゴ
	"Player/Skill_Cutin.png",				// スキル使用時のカットイン
	"Player/Sight.png",						// 照準器
	
	// タイトル関連
	"Scene/Title/Title_Logo.png",			// タイトルロゴ
	"Scene/Title/Start_Button.png",			// ゲーム開始ボタンの文字列
	
	// メイン関連
	"Scene/Main/Countdown_Number_0.png",	// カウントダウンの数字_0
	"Scene/Main/Countdown_Number_1.png",	// カウントダウンの数字_1
	"Scene/Main/Countdown_Number_2.png",	// カウントダウンの数字_2
	"Scene/Main/Countdown_Number_3.png",	// カウントダウンの数字_3
	"Scene/Main/Countdown_Start.png",		// カウントダウン_スタート
	"Scene/Main/GameSet.png",				// ゲームセットロゴ
	
	// リザルト関連
	"Scene/Result/Win.png",					// 勝利ロゴ
	"Scene/Result/Lose.png",				// 敗北ロゴ
	"Scene/Result/BackTitle_Button.png",	// タイトルへ戻るボタンの文字列

	"Effeksser_Dummy.png"					// Effekseer用ダミー画像
};

/*-------------------------------------------*/
/* コンストラクタ
/*-------------------------------------------*/
ResourceSpriteManager::ResourceSpriteManager()
{
	// 処理なし
}

/*-------------------------------------------*/
/* デストラクタ
/*-------------------------------------------*/
ResourceSpriteManager::~ResourceSpriteManager()
{
	// 処理なし
}

/*-------------------------------------------*/
/* 全てのスプライトを読み込む
/*-------------------------------------------*/
void ResourceSpriteManager::LoadAllSprite(ImageFileParam* imageFileParam)
{
	for (int i = 0; i < SpriteType::TYPE_NUM; i++)
	{
		// メモリ内のイメージファイルからスプライトを展開する
		spriteHandle[i] = CreateGraphFromMem(imageFileParam[i].fileImage,
											 imageFileParam[i].fileSize);

		// イメージファイルからの展開に失敗した場合
		if (spriteHandle[i] == -1)
		{
			std::ostringstream tmpMessage;
			tmpMessage << "イメージファイルからスプライトの展開に失敗しました。,SpriteType:" << i;

			ErrorData errorData;
			errorData.errorMessage = tmpMessage.str();
			errorData.detectedErrorClassName = "ResourceSpriteManager";

			// エラーデータをリストに入れる
			ERROR_MESSAGE.SetErrorData(errorData);
		}
	}
}

/*-------------------------------------------*/
/* 全てのスプライトを削除
/*-------------------------------------------*/
void ResourceSpriteManager::DeleteAllSprite()
{
	for (int i = 0; i < SpriteType::TYPE_NUM; i++)
	{
		// スプライトを全て削除する
		if (spriteHandle[i] != NULL)
		{
			SafeDeleteGraph(spriteHandle[i]);
		}
	}

	// スプライトのデータが残っていれば、全て削除する
	InitGraph();
}

/*-------------------------------------------*/
/* 指定のスプライトハンドルを取得
/*-------------------------------------------*/
const int& ResourceSpriteManager::GetHandle(ResourceSpriteManager::SpriteType type) const
{
	// スプライトハンドルがNULLであれば例外を投げる
	assert(spriteHandle != NULL);
	// スプライトハンドル以外の定数を指定していれば例外を投げる
	assert(type != SpriteType::TYPE_NUM);

	// 指定のスプライトハンドルを返す
	return spriteHandle[type];
}

/*-------------------------------------------*/
/* 指定のスプライトハンドルのファイルパスを取得
/*-------------------------------------------*/
const std::string ResourceSpriteManager::GetFilePath(ResourceSpriteManager::SpriteType type) const
{
	// スプライトハンドル以外の定数を指定していれば例外を投げる
	assert(type != SpriteType::TYPE_NUM);

	// 指定のスプライトハンドルのファイルパスを返す
	return SPRITE_FOLDER_RELATIVE_PATH + SPRITE_FILE_RELATIVE_PATH[type];
}
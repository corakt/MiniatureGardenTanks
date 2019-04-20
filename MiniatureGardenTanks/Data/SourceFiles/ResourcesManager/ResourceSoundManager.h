#pragma once
#include "../Others/Singleton.h"
#include <string>

struct ImageFileParam;

/*-------------------------------------------*/
/* サウンドのリソースを管理するクラス
/*-------------------------------------------*/
class ResourceSoundManager final : public Singleton<ResourceSoundManager>
{
	friend class Singleton<ResourceSoundManager>;

public:
			 ResourceSoundManager();	// コンストラクタ
	virtual ~ResourceSoundManager();	// デストラクタ

	// サウンドの種類
	enum SoundType
	{
		BGM_SCENETITLE,			// タイトルBGM
		BGM_SCENEMAIN,			// メインBGM
		BGM_RESULTWIN,			// 勝利リザルトBGM
		BGM_RESULTLOSE,			// 敗北リザルトBGM

		SE_TANK_ENGINESTART,	// 戦車：エンジン始動
		SE_TANK_ENGINEIDLING,	// 戦車：エンジンアイドリング
		SE_TANK_ENGINEACCEL,	// 戦車：エンジン加速
		SE_TANK_ENGINEMOVING,	// 戦車：エンジン移動中
		SE_TANK_ENGINEDECEL,	// 戦車：エンジン減速
		SE_TANK_CRAWLER,		// 戦車：履帯音
		SE_TANK_SHOTFIRING,		// 戦車：砲撃音
		SE_TANK_SHOTRELOAD,		// 戦車：装填音
		SE_TANK_SHOTHIT,		// 戦車：被弾音
		SE_TANK_SHOTIMPACT,		// 戦車：着弾音
		SE_BATTLEFINISH,		// バトルフィニッシュ

		TYPE_NUM,				// サウンドの種類数
	};

	// 初期化
	void Initialize();

	// 全てのサウンドを読み込む
	void LoadAllSound(ImageFileParam* imageFileParam);
	// 全てのサウンドを削除する
	void DeleteAllSound();

	// 指定のサウンドハンドルを取得
	const int& GetHandle(SoundType type) const;
	// 指定のサウンドハンドルのファイルパスを取得
	const std::string GetFilePath(SoundType type) const;

private:
	int  soundHandle[SoundType::TYPE_NUM];			// サウンドハンドル
	bool isLoad3dSoundMode[SoundType::TYPE_NUM];	// 3Dサウンド用にサウンドを読み込むかどうか

	static const std::string SOUND_FOLDER_RELATIVE_PATH;						// サウンドフォルダへの相対パス
	static const std::string SOUND_FILE_RELATIVE_PATH[SoundType::TYPE_NUM];		// サウンドファイルへの相対パス
};

#define SOUND_MANAGER ResourceSoundManager::GetInstance()
typedef ResourceSoundManager::SoundType SoundType;
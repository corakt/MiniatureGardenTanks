#include "../ResourcesManager/ResourceSoundManager.h"
#include "../Others/AsyncLoadData.h"
#include "../Others/ErrorMessage.h"
#include "../Others/Define.h"
#include <DxLib.h>
#include <sstream>
#include <assert.h>

// サウンドフォルダへの相対パス
const std::string ResourceSoundManager::SOUND_FOLDER_RELATIVE_PATH = "Data/ResourceFiles/Sounds/";

// サウンドファイルへの相対パス
const std::string ResourceSoundManager::SOUND_FILE_RELATIVE_PATH[] = 
{ 
	"Bgm/Title.wav",						// タイトルBGM
	"Bgm/Main.wav",							// メインBGM
	"Bgm/Result_Win.wav",					// 勝利リザルトBGM
	"Bgm/Result_Lose.wav",					// 敗北リザルトBGM

	"Se/Character/Tank_Engine_Start.wav",	// 戦車：エンジン始動
	"Se/Character/Tank_Engine_Idling.wav",	// 戦車：エンジンアイドリング
	"Se/Character/Tank_Engine_Accel.wav",	// 戦車：エンジン加速
	"Se/Character/Tank_Engine_Moving.wav",	// 戦車：エンジン移動中
	"Se/Character/Tank_Engine_Decel.wav",	// 戦車：エンジン減速
	"Se/Character/Tank_Crawler.wav",		// 戦車：履帯音
	"Se/Character/Tank_Shot_Firing.wav",	// 戦車：砲撃音
	"Se/Character/Tank_Reload.wav",			// 戦車：装填音
	"Se/Character/Tank_Shot_Hit.wav",		// 戦車：被弾音
	"Se/Character/Tank_Shot_Impact.wav",	// 戦車：着弾音
	"Se/Scene/Battle_Finish.wav",			// バトルフィニッシュ
};

/*-------------------------------------------*/
/* コンストラクタ
/*-------------------------------------------*/
ResourceSoundManager::ResourceSoundManager()
{
	// ３Ｄサウンドのリスナーの移動速度を設定
	Set3DSoundListenerVelocity(VGet(500, 500, 500));
}

/*-------------------------------------------*/
/* デストラクタ
/*-------------------------------------------*/
ResourceSoundManager::~ResourceSoundManager()
{
	// 処理なし
}

/*-------------------------------------------*/
/* 初期化
/*-------------------------------------------*/
void ResourceSoundManager::Initialize()
{
	// 3Dサウンド用にサウンドを読み込むかどうかのフラグをセット
	isLoad3dSoundMode[SoundType::BGM_SCENETITLE]       = false;		// タイトルBGM
	isLoad3dSoundMode[SoundType::BGM_SCENEMAIN]        = false;		// メインBGM
	isLoad3dSoundMode[SoundType::BGM_RESULTWIN]        = false;		// 勝利リザルトBGM
	isLoad3dSoundMode[SoundType::BGM_RESULTLOSE]       = false;		// 敗北リザルトBGM
	
	isLoad3dSoundMode[SoundType::SE_TANK_ENGINESTART]  = true;		// 戦車：エンジン始動
	isLoad3dSoundMode[SoundType::SE_TANK_ENGINEIDLING] = true;		// 戦車：エンジンアイドリング
	isLoad3dSoundMode[SoundType::SE_TANK_ENGINEACCEL]  = true;		// 戦車：エンジン加速
	isLoad3dSoundMode[SoundType::SE_TANK_ENGINEMOVING] = true;		// 戦車：エンジン移動中
	isLoad3dSoundMode[SoundType::SE_TANK_ENGINEDECEL]  = true;		// 戦車：エンジン減速
	isLoad3dSoundMode[SoundType::SE_TANK_CRAWLER]      = true;		// 戦車：履帯音
	isLoad3dSoundMode[SoundType::SE_TANK_SHOTFIRING]   = true;		// 戦車：砲撃音
	isLoad3dSoundMode[SoundType::SE_TANK_SHOTRELOAD]   = true;		// 戦車：装填音
	isLoad3dSoundMode[SoundType::SE_TANK_SHOTHIT]      = true;		// 戦車：被弾音
	isLoad3dSoundMode[SoundType::SE_TANK_SHOTIMPACT]   = true;		// 戦車：着弾音
	isLoad3dSoundMode[SoundType::SE_BATTLEFINISH]      = false;		// バトルフィニッシュ
}

/*-------------------------------------------*/
/* 全てのサウンドを読み込む
/*-------------------------------------------*/
void ResourceSoundManager::LoadAllSound(ImageFileParam* imageFileParam)
{
	for (int i = 0; i < SoundType::TYPE_NUM; i++)
	{
		// フラグによって、3Dサウンド用に読み込むかどうかを決定する
		SetCreate3DSoundFlag(isLoad3dSoundMode[i]);

		// メモリ内のイメージファイルからサウンドを展開する
		soundHandle[i] = LoadSoundMemByMemImage(imageFileParam[i].fileImage,
												imageFileParam[i].fileSize);

		// イメージファイルからの展開に失敗した場合
		if (soundHandle[i] == -1)
		{
			std::ostringstream tmpMessage;
			tmpMessage << "イメージファイルからサウンドの展開に失敗しました。,SoundType:" << i;

			ErrorData errorData;
			errorData.errorMessage = tmpMessage.str();
			errorData.detectedErrorClassName = "ResourceSoundManager";

			// エラーデータをリストに入れる
			ERROR_MESSAGE.SetErrorData(errorData);
		}
	}
}

/*-------------------------------------------*/
/* 全てのサウンドを削除
/*-------------------------------------------*/
void ResourceSoundManager::DeleteAllSound()
{
	for (int i = 0; i < SoundType::TYPE_NUM; i++)
	{
		// サウンドを全て削除する
		if (soundHandle[i] != NULL)
		{
			SafeDeleteSound(soundHandle[i]);
		}
	}

	// サウンドのデータが残っていれば、全て削除する
	InitSoundMem();
}

/*-------------------------------------------*/
/* 指定のサウンドハンドルを取得
/*-------------------------------------------*/
const int& ResourceSoundManager::GetHandle(ResourceSoundManager::SoundType type) const
{
	// サウンドハンドルがNULLであれば例外を投げる
	assert(soundHandle != NULL);
	// サウンドハンドル以外の定数を指定していれば例外を投げる
	assert(type != SoundType::TYPE_NUM);

	// 指定のモデルハンドルを返す
	return soundHandle[type];
}

/*-------------------------------------------*/
/* 指定のサウンドハンドルのファイルパスを取得
/*-------------------------------------------*/
const std::string ResourceSoundManager::GetFilePath(ResourceSoundManager::SoundType type) const
{
	// サウンドハンドル以外の定数を指定していれば例外を投げる
	assert(type != SoundType::TYPE_NUM);

	// 指定のサウンドハンドルのファイルパスを返す
	return SOUND_FOLDER_RELATIVE_PATH + SOUND_FILE_RELATIVE_PATH[type];
}
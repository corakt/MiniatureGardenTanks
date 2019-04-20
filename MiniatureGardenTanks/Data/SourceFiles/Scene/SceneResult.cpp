#include "../Scene/SceneResult.h"
#include "../Stage/StageManager.h"
#include "../ResourcesManager/OtherResources.h"
#include "../Character/CharacterManager.h"
#include "../BaseObject/ObjectIdManager.h"
#include "../Others/Input.h"
#include "../Camera/CameraBase.h"
#include "../ResourcesManager/ResourceSpriteManager.h"
#include "../ResourcesManager/ResourceSoundManager.h"
#include "../Scene/SceneManager.h"
#include "../Others/HelperFunction.h"
#include "../Collision/CollisionManager.h"

const CharacterBase::CharacterType SceneResult::CHARACTER_TYPE   = CharacterBase::CharacterType::PLAYER;	// キャラクターの種類
const VECTOR SceneResult::CHARACTER_POSITION[]                   = { VGet(15000,100,15000) };				// キャラクターの位置
const float SceneResult::CHARACTER_BODYANGLE[]                   = {DX_PI_F};								// キャラクターの車体角度
const COLOR_F SceneResult::CHARACTER_MODELCOLOR[]                = { GetColorF(1,0.2f,0,1) };				// キャラクターのモデルの色

const VECTOR SceneResult::CAMERA_POSITION = VGet(14100,500,14100);		// カメラの位置

const VECTOR SceneResult::LOGO_SPRITE_POS = VGet(400, 300, 0);					// ロゴの表示位置
const VECTOR SceneResult::BACKTITLE_BUTTON_SPRITE_POS = VGet(400, 850, 0);		// 「タイトルへ戻る」の表示位置
const int    SceneResult::SCENECHANGE_WAIT = 180;								// シーンを変更する際のウェイトタイム

// バトルの結果
SceneResult::ResultType SceneResult::result;

/*-------------------------------------------*/
/* コンストラクタ
/*-------------------------------------------*/
SceneResult::SceneResult()
{
	// NULLで初期化
	camera    = NULL;	// カメラ
	character = NULL;	// キャラクター
	bgm       = NULL;	// タイトルBGM
}

/*-------------------------------------------*/
/* デストラクタ
/*-------------------------------------------*/
SceneResult::~SceneResult()
{

}

/*-------------------------------------------*/
/* インスタンスの生成
/*-------------------------------------------*/
void SceneResult::CreateObject()
{
	// キャラクターを生成
	character = CHARACTER_MANAGER.CreateCharacter(CHARACTER_TYPE);

	// ステージの壁の配置データをファイルから読み込む
	STAGE_MANAGER.ReadWallMapData(STAGEWALL_MAPDATA_RESULT.c_str());
	// ステージを生成
	STAGE_MANAGER.CreateStage();

	// カメラを生成
	camera = new CameraBase;

	// バトルの結果によって、生成するBGMを変える
	if (result == ResultType::WINNER)
	{
		// リザルトの勝利BGMのサウンドオブジェクトを生成
		bgm = new SoundObject(SOUND_MANAGER.GetHandle(ResourceSoundManager::SoundType::BGM_RESULTWIN));
	}
	else
	{
		// リザルトの敗北BGMのサウンドオブジェクトを生成
		bgm = new SoundObject(SOUND_MANAGER.GetHandle(ResourceSoundManager::SoundType::BGM_RESULTLOSE));
	}
}

/*-------------------------------------------*/
/* インスタンスの削除
/*-------------------------------------------*/
void SceneResult::DestroyObject()
{
	// キャラクターに使用したインスタンスを削除
	CHARACTER_MANAGER.DestroyCharacter();

	// ステージに使用したインスタンスを削除
	STAGE_MANAGER.DestroyStage();

	// 衝突判定の管理クラスの終了処理
	COLLISION_MANAGER.Finalize();

	// カメラのメモリを解放
	SafeDelete(camera);
	
	// キャラクター
	character = NULL;

	// BGM
	SafeDelete(bgm);
}

/*-------------------------------------------*/
/* 初期化
/*-------------------------------------------*/
void SceneResult::Initialize()
{
	// トランジションカウントを初期化
	transitionCount = 0;
	// シーンを変更するかどうかのフラグを初期化
	isSceneChange = false;

	// インプットの初期化処理
	INPUT.Initialize();

	// 衝突判定の管理クラスの初期化処理
	COLLISION_MANAGER.Initialize();

	// オブジェクトIDの管理クラスの初期化
	OBJECTID_MANAGER.Initialize();

	// キャラクターの初期化処理
	CHARACTER_MANAGER.Initialize(CHARACTER_POSITION, CHARACTER_BODYANGLE, CHARACTER_MODELCOLOR);
	// キャラクターで再生されているエンジンサウンドは停止する
	character->StopEngineSound();
	// リザルトではエンジンサウンドの再生は行わない
	character->SetEngineSoundPlayingFlag(false);

	// ステージの初期化処理
	STAGE_MANAGER.Initialize();
	// ステージの地形の配置位置を計算
	STAGE_MANAGER.SetTerrainPosition();

	// カメラ
	if (camera != NULL)
	{
		// カメラの初期化処理
		camera->Initialize(ZERO_VECTOR,ZERO_VECTOR);
		// カメラのパラメータをセット
		camera->SettingCameraParameter();
	}

	// 既に再生されているBGMを止める
	StopSound();
	// BGMの再生音量を設定
	bgm->SetPlayVolume(150);
	// BGMを再生
	bgm->Playing(DX_PLAYTYPE_LOOP);
}

/*-------------------------------------------*/
/* 更新
/*-------------------------------------------*/
void SceneResult::Update()
{
	// 衝突判定の管理クラスの更新処理
	COLLISION_MANAGER.Update();

	// キャラクターの更新処理
	CHARACTER_MANAGER.Update();

	// ステージの更新処理
	STAGE_MANAGER.Update();

	// インプットの更新処理
	INPUT.Update();

	// バトルの結果によって、処理を分ける
	switch (result)
	{
	// バトル：勝利
	case SceneResult::WINNER:
		updateForWinner();
		break;

	// バトル：敗北
	case SceneResult::LOSE:
		updateForLose();
		break;

	default:
		break;
	}

	

	// カメラ
	if (camera != NULL)
	{
		// キャラクターの位置を取得
		VECTOR characterPos = character->GetParts(CharacterBase::PartsType::TURRET)->GetTransform().position;
		// カメラのトランスフォームを取得
		Transform cameraTrans = camera->GetTransform();

		// カメラの位置をセット
		cameraTrans.position = CAMERA_POSITION;

		// カメラの注視点をセット
		camera->SetLookAtPosition(characterPos);

		// カメラのトランスフォームをセット
		camera->SetTransform(cameraTrans);

		// カメラの更新処理
		camera->Update();
	}

	// トランジションエフェクトが再生中の場合は、ボタンとキーの入力を受け付けない
	if (IsTransitionEffectPlaying() == false)
	{
		// Aボタンかスペースキーが押されたら、
		// トランジションを開始して、タイトルへシーンを変更する
		if (INPUT.GetButtonDown(XINPUT_BUTTON_A) ||
			INPUT.GetKeyDown(KEY_INPUT_SPACE))
		{
			// トランジションエフェクトを再生
			PlayTransitionEffectForFade(0.5f);
			// フラグを立てる
			isSceneChange = true;
		}
	}

	if (isSceneChange)
	{
		// カウントが指定の時間を超えたら、シーンを変更
		transitionCount++;
		if (transitionCount >= SCENECHANGE_WAIT)
		{
			// タイトルへシーンを変更
			SceneManager::SetNextScene(SceneManager::SceneType::TITLE);
			// カウントを０にリセット
			transitionCount = 0;
			// フラグをリセット
			isSceneChange = false;
		}
	}
}

/*-------------------------------------------*/
/* シーンの状態ごとの更新処理：勝利
/*-------------------------------------------*/
void SceneResult::updateForWinner()
{

}

/*-------------------------------------------*/
/* シーンの状態ごとの更新処理：敗北
/*-------------------------------------------*/
void SceneResult::updateForLose()
{
	// プレイヤーを取得
	CharacterBase* character = CHARACTER_MANAGER.GetCharacter(0);
	// キャラクターをHPを０にして破壊された状態に変更する
	character->SetHitPoint(0);
}

/*-------------------------------------------*/
/* 描画
/*-------------------------------------------*/
void SceneResult::Draw()
{
	// キャラクターの描画
	CHARACTER_MANAGER.Draw();

	// ステージの描画
	STAGE_MANAGER.Draw();
	// スカイドームの描画
	STAGE_MANAGER.DrawSkydome();

	// バトルの結果によって、描画処理を分ける
	switch (result)
	{
		// バトル：勝利
	case SceneResult::WINNER:
		drawForWinner();
		break;

		// バトル：敗北
	case SceneResult::LOSE:
		drawForLose();
		break;

	default:
		break;
	}

	// 「タイトルへ戻る」画像を取得
	int backTitleButtonSprite = SPRITE_MANAGER.GetHandle(ResourceSpriteManager::SpriteType::BACKTITLE_BUTTON);
	// 画像を描画
	DrawRotaGraphF(BACKTITLE_BUTTON_SPRITE_POS.x, BACKTITLE_BUTTON_SPRITE_POS.y, 0.5f, 0, backTitleButtonSprite, TRUE);
}

/*-------------------------------------------*/
/* シーンの状態ごとの描画処理：勝利
/*-------------------------------------------*/
void SceneResult::drawForWinner()
{
	// 勝利ロゴ画像を取得
	int winLogoSprite = SPRITE_MANAGER.GetHandle(ResourceSpriteManager::SpriteType::WIN_LOGO);
	// ロゴ画像を描画
	DrawRotaGraphF(LOGO_SPRITE_POS.x, LOGO_SPRITE_POS.y, 0.7f, 0, winLogoSprite, TRUE);
}

/*-------------------------------------------*/
/* シーンの状態ごとの描画処理：敗北
/*-------------------------------------------*/
void SceneResult::drawForLose()
{
	// 敗北ロゴ画像を取得
	int loseLogoSprite = SPRITE_MANAGER.GetHandle(ResourceSpriteManager::SpriteType::LOSE_LOGO);
	// ロゴ画像を描画
	DrawRotaGraphF(LOGO_SPRITE_POS.x, LOGO_SPRITE_POS.y, 0.6f, 0, loseLogoSprite, TRUE);
}
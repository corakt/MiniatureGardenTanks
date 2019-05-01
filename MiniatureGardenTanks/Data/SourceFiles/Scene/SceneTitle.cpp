#include "../Scene/SceneTitle.h"
#include "../Scene/SceneManager.h"
#include "../Character/CharacterManager.h"
#include "../Stage/StageManager.h"
#include "../Collision/CollisionManager.h"
#include "../Camera/CameraBase.h"
#include "../ResourcesManager/ResourceSpriteManager.h"
#include "../ResourcesManager/ResourceSoundManager.h"
#include "../ResourcesManager/OtherResources.h"
#include "../BaseObject/SoundObject.h"
#include "../Others/Input.h"
#include "../Others/HelperFunction.h"
#include "../Others/Define.h"
#include <EffekseerForDXLib.h>

const CharacterBase::CharacterType SceneTitle::CHARACTER_TYPE = CharacterBase::CharacterType::ENEMY;	// キャラクターの種類
const VECTOR  SceneTitle::CHARACTER_POSITION[]                = { VGet(15000,100,15000) };				// キャラクターの位置
const float   SceneTitle::CHARACTER_BODYANGLE[]               = {DX_PI_F};								// キャラクターの車体角度
const COLOR_F SceneTitle::CHARACTER_MODELCOLOR[]              = { GetColorF(1,0.2f,0,1) };				// キャラクターのモデルの色
const VECTOR  SceneTitle::CAMERA_LOOKATPOS_OFFSET             = VGet(0,500,0);							// カメラ注視点位置のオフセット値
const float   SceneTitle::CAMERA_LOOKATPOS_DISTANCE           = 2500;									// 注視点との距離
const float   SceneTitle::CAMERA_HEIGHT                       = 500;									// カメラの高さ
const float   SceneTitle::CAMERA_ROTATION_SPEED               = 0.002f;									// カメラの回転スピード
const VECTOR  SceneTitle::TITLELOGO_SPRITE_POS                = VGet(RESOLUTION_WIDTH / 2, 350, 0);		// タイトルロゴの描画位置
const VECTOR  SceneTitle::START_BUTTON_SPRITE_POS             = VGet(RESOLUTION_WIDTH / 2, 950, 0);		// ゲームスタートボタン描画位置
const int     SceneTitle::SCENECHANGE_WAIT                    = 180;									// シーンを変更する際のウェイトタイム

/*-------------------------------------------*/
/* コンストラクタ
/*-------------------------------------------*/
SceneTitle::SceneTitle()
{
	// NULLで初期化
	character = NULL;	// キャラクター
	camera    = NULL;	// カメラ
	bgm       = NULL;	// タイトルBGM
}

/*-------------------------------------------*/
/* デストラクタ
/*-------------------------------------------*/
SceneTitle::~SceneTitle()
{
	
}

/*-------------------------------------------*/
/* オブジェクトを生成
/*-------------------------------------------*/
void SceneTitle::CreateObject()
{
	// キャラクターを生成
	character = CHARACTER_MANAGER.CreateCharacter(CHARACTER_TYPE);

	// ステージの壁の配置データをファイルから読み込む
	STAGE_MANAGER.ReadWallMapData(STAGEWALL_MAPDATA_TITLE.c_str());
	// ステージを生成
	STAGE_MANAGER.CreateStage();

	// カメラを生成
	camera = new CameraBase();

	// タイトルBGMのサウンドオブジェクトを生成
	bgm = new SoundObject(SOUND_MANAGER.GetHandle(ResourceSoundManager::SoundType::BGM_SCENETITLE));
}

/*-------------------------------------------*/
/* オブジェクトを削除
/*-------------------------------------------*/
void SceneTitle::DestroyObject()
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
void SceneTitle::Initialize()
{
	// トランジションカウントを初期化
	transitionCount = 0;
	// シーンを変更するかどうかのフラグを初期化
	isSceneChange = false;

	// 衝突判定の管理クラスの初期化処理
	COLLISION_MANAGER.Initialize();

	// キャラクターの初期化処理
	CHARACTER_MANAGER.Initialize(CHARACTER_POSITION, CHARACTER_BODYANGLE, CHARACTER_MODELCOLOR);
	// キャラクターを制御可能にする
	CHARACTER_MANAGER.SetAllCharacterControlingFlag(true);
	// キャラクターで再生されているエンジンサウンドは停止する
	character->StopEngineSound();
	// タイトルではエンジンサウンドの再生は行わない
	character->SetEngineSoundPlayingFlag(false);

	// ステージの初期化処理
	STAGE_MANAGER.Initialize();
	// ステージの地形の配置位置を計算
	STAGE_MANAGER.SetTerrainPosition();

	// インプットの初期化処理
	INPUT.Initialize();

	// カメラ
	if (camera != NULL)
	{
		// カメラの初期化処理
		camera->Initialize(ZERO_VECTOR, ZERO_VECTOR);
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
void SceneTitle::Update()
{
	// キャラクターの更新処理
	CHARACTER_MANAGER.Update();

	// ステージの更新処理
	STAGE_MANAGER.Update();

	// 衝突判定の管理クラスの更新処理
	COLLISION_MANAGER.Update();

	// インプットの更新処理
	INPUT.Update();

	// カメラ
	if (camera != NULL)
	{
		// キャラクターの位置を取得
		VECTOR characterPos = character->GetParts(CharacterBase::PartsType::TURRET)->GetTransform().position;

		// カメラの高さをセット
		camera->SetCameraHeight(CAMERA_HEIGHT);

		// キャラクターの位置にオフセット値を加えたものを注視点の位置にする
		VECTOR lookAtPos = characterPos + CAMERA_LOOKATPOS_OFFSET;
		// カメラの注視点をセット
		camera->SetLookAtPosition(lookAtPos);
		// カメラの注視点との距離をセット
		camera->SetLookAtPosToDistance(CAMERA_LOOKATPOS_DISTANCE);

		// 現在の回転角を取得
		float currentAngle = camera->GetHorizontalAngle();
		// カメラを回転させる
		camera->SetHorizontalAngle((currentAngle + CAMERA_ROTATION_SPEED));

		// カメラの回転制御
		camera->RotationCamera();

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
			SceneManager::SetNextScene(SceneManager::SceneType::MAIN);
			// カウントを０にリセット
			transitionCount = 0;
			// フラグをリセット
			isSceneChange = false;
		}
	}
}

/*-------------------------------------------*/
/* 描画
/*-------------------------------------------*/
void SceneTitle::Draw()
{
	// キャラクターの描画処理
	CHARACTER_MANAGER.Draw();

	// ステージの描画処理
	STAGE_MANAGER.Draw();
	// スカイドームの描画
	STAGE_MANAGER.DrawSkydome();

	// タイトルロゴの画像を取得
	static int titleLogoSprite = SPRITE_MANAGER.GetHandle(ResourceSpriteManager::SpriteType::TITLE_LOGO);
	// タイトルロゴを描画
	DrawRotaGraphF(TITLELOGO_SPRITE_POS.x, TITLELOGO_SPRITE_POS.y, 0.5f, 0, titleLogoSprite, TRUE);

	// スタートボタンの画像を取得
	static int startButtonSprite = SPRITE_MANAGER.GetHandle(ResourceSpriteManager::SpriteType::START_BUTTON);
	// スタートボタンを描画
	DrawRotaGraphF(START_BUTTON_SPRITE_POS.x, START_BUTTON_SPRITE_POS.y, 0.5f, 0, startButtonSprite, TRUE);
}
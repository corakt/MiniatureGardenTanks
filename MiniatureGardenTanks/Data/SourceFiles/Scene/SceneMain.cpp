#include "../Scene/SceneMain.h"
#include "../Scene/SceneResult.h"
#include "../Scene/SceneManager.h"
#include "../Stage/StageManager.h"
#include "../Character/CharacterManager.h"
#include "../Camera/CameraBase.h"
#include "../Camera/PlayerCamera.h"
#include "../CharacterShot/ShotManager.h"
#include "../Raycast/RaycastManager.h"
#include "../Collision/CollisionManager.h"
#include "../BaseObject/IdGenerator.h"
#include "../ResourcesManager/ResourceModelManager.h"
#include "../ResourcesManager/ResourceSpriteManager.h"
#include "../ResourcesManager/ResourceSoundManager.h"
#include "../ResourcesManager/OtherResources.h"
#include "../Others/HelperFunction.h"
#include "../Others/Input.h"
#include "../Others/Define.h"
#include <EffekseerForDXLib.h>
#include <DxLib.h>

// キャラクターの数
const UINT SceneMain::CHARACTER_NUM = 4;
// キャラクターの種類
const CharacterBase::CharacterType SceneMain::CHARACTER_TYPE[] =
{
	CharacterBase::CharacterType::PLAYER,	// プレイヤー
	CharacterBase::CharacterType::ENEMY,	// エネミー１
	CharacterBase::CharacterType::ENEMY,	// エネミー２
	CharacterBase::CharacterType::ENEMY		// エネミー３
};
// キャラクターの位置
const VECTOR SceneMain::CHARACTER_POSITION[] =
{
	VGet(3000,100,3000),	// プレイヤー
	VGet(3000,100,27000),	// エネミー１
	VGet(27000,100,3000),	// エネミー２
	VGet(27000,100,27000)	// エネミー３
};
// キャラクターの車体角度
const float SceneMain::CHARACTER_BODYANGLE[] = 
{
	0,						// プレイヤー
	0,						// エネミー１
	0,						// エネミー２
	0,						// エネミー３
};
// キャラクターのモデルの色
const COLOR_F SceneMain::CHARACTER_MODELCOLOR[] =
{
	GetColorF(1,0.2f,0,1),	// プレイヤー
	GetColorF(1,1,1,1),		// エネミー１
	GetColorF(1,1,1,1),		// エネミー２
	GetColorF(1,1,1,1),		// エネミー３
};

// カメラ注視点位置のオフセット値
const VECTOR SceneMain::CAMERA_LOOKATPOS_OFFSET = VGet(0, 500, 0);

// カウントダウンで扱うスプライトの位置
const VECTOR SceneMain::COUNTDOWN_SPRITE_POS[] =
{
	VGet(3000,300,2200),	// スタート
	VGet(2600,300,2400),	// １
	VGet(2400,300,3000),	// ２
	VGet(3600,300,3100),	// ３
};

// 敗北時のカメラの回転スピード
const float SceneMain::LOSE_CAMERA_ROTATION_SPEED = 0.003f;
// 敗北時のカメラの高さ
const float SceneMain::LOSE_CAMERA_HEIGHT = 100;
// 敗北時のカメラの注視点位置からの距離
const float SceneMain::LOSE_CAMERA_LOOKATPOS_DISTANCE = 1500;

// トランジションエフェクトを再生する際のウェイトタイム
const int SceneMain::TRANSITION_EFFECT_PLAY_WAIT = 240;
// リザルトシーンに変更する際のウェイトタイム
const int SceneMain::SCENECHANGE_WAIT_FOR_RESULT = 550;

/*-------------------------------------------*/
/* コンストラクタ
/*-------------------------------------------*/
SceneMain::SceneMain()
{
	// NULLで初期化
	character       = NULL;		// キャラクター
	playerCamera    = NULL;		// プレイヤーカメラ
	countDownCamera = NULL;		// カウントダウン中のカメラ
	loseCamera      = NULL;		// 敗北した際のカメラ
	bgm              = NULL;	// タイトルBGM

	// キャラクターの生成分メモリを確保
	character = new CharacterBase*[CHARACTER_NUM];
	// 確保したメモリをNULLで初期化
	for (int i = 0; i < CHARACTER_NUM; i++)
	{
		character[i] = NULL;
	}
}

/*-------------------------------------------*/
/* デストラクタ
/*-------------------------------------------*/
SceneMain::~SceneMain()
{
	
}

/*-------------------------------------------*/
/* オブジェクトを生成
/*-------------------------------------------*/
void SceneMain::CreateObject()
{
	// プレイヤーカメラのインスタンスを生成
	playerCamera = new PlayerCamera();

	// カウントダウンで使用するカメラのインスタンスを生成
	countDownCamera = new CameraBase();

	// 敗北した際のカメラのインスタンスを生成
	loseCamera = new CameraBase();
	
	// プレイヤーカメラを取得
	CHARACTER_MANAGER.GetPlayerCamera(playerCamera);

	// 指定の数だけキャラクターを生成する
	for (int i = 0; i < CHARACTER_NUM; i++)
	{
		// キャラクターを生成
		character[i] = CHARACTER_MANAGER.CreateCharacter(CHARACTER_TYPE[i]);
	}

	// ステージの壁の配置データをファイルから読み込む
	STAGE_MANAGER.ReadWallMapData(STAGEWALL_MAPDATA_MAIN.c_str());
	// ステージを生成
	STAGE_MANAGER.CreateStage();

	// ショットを生成
	SHOT_MANAGER.CreateShot();

	// メインBGMのサウンドオブジェクトを生成
	bgm = new SoundObject(SOUND_MANAGER.GetHandle(ResourceSoundManager::SoundType::BGM_SCENEMAIN));
}

/*-------------------------------------------*/
/* オブジェクトを削除
/*-------------------------------------------*/
void SceneMain::DestroyObject()
{
	// キャラクターに使用したインスタンスを削除
	CHARACTER_MANAGER.DestroyCharacter();

	// ステージに使用したインスタンスを削除
	STAGE_MANAGER.DestroyStage();

	// ショットに使用したインスタンスを削除
	SHOT_MANAGER.DestroyShot();

	// レイキャストの管理クラスの終了処理
	RAYCAST_MANAGER.Finalize();

	// 衝突判定の管理クラスの終了処理
	COLLISION_MANAGER.Finalize();

	// キャラクター
	for (int i = 0; i < CHARACTER_NUM; i++)
	{
		character[i] = NULL;
	}
	SafeDelete(character);

	// プレイヤーカメラのメモリを解放
	SafeDelete(playerCamera);
	// カウントダウンカメラのメモリを解放
	SafeDelete(countDownCamera);
	// 敗北した際のカメラのメモリを開放
	SafeDelete(loseCamera);

	// BGM
	SafeDelete(bgm);
}

/*-------------------------------------------*/
/* 初期化
/*-------------------------------------------*/
void SceneMain::Initialize()
{	
	// シーンの状態を初期化（カウントダウン）
	state = State::COUNTDOWN;
	// 現在のカウントダウンの数
	currentCountDownNum = 0;
	// シーンや状態の変更の際に扱うカウント
	transitionCount = 0;

	// インプットの初期化処理
	INPUT.Initialize();

	// 衝突判定の管理クラスの初期化処理
	COLLISION_MANAGER.Initialize();

	// キャラクターの初期化処理
	CHARACTER_MANAGER.Initialize(CHARACTER_POSITION, CHARACTER_BODYANGLE, CHARACTER_MODELCOLOR);
	for (int i = 0; i < CHARACTER_NUM; i++)
	{
		// メインではキャラクターのエンジンサウンドの再生を行う
		character[i]->SetEngineSoundPlayingFlag(true);
	}

	// ステージの初期化処理
	STAGE_MANAGER.Initialize();
	// ステージの地形の配置位置を計算
	STAGE_MANAGER.SetTerrainPosition();

	// ショットの初期化処理
	SHOT_MANAGER.Initialize();

	// レイキャストの管理クラスの初期化処理
	RAYCAST_MANAGER.Initialize();

	if (playerCamera != NULL)
	{
		// カメラの初期化処理
		playerCamera->Initialize(ZERO_VECTOR, ZERO_VECTOR);
		// カメラのパラメータをセット
		playerCamera->SettingCameraParameter();
	}

	if (countDownCamera != NULL)
	{
		// カメラの初期化処理
		countDownCamera->Initialize(ZERO_VECTOR, ZERO_VECTOR);
		// カメラのパラメータをセット
		countDownCamera->SettingCameraParameter();

		// カメラの移動制御パラメータをファイルから読み込む
		countDownCamera->ReadMoveData("Data/ResourceFiles/Others/Camera/CameraMove_Countdown.csv");
		// カメラの移動制御での位置をリセット
		countDownCamera->ResetMovePoint();
	}

	if (loseCamera != NULL)
	{
		// カメラの初期化処理
		loseCamera->Initialize(ZERO_VECTOR, ZERO_VECTOR);
		// カメラのパラメータをセット
		loseCamera->SettingCameraParameter();
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
void SceneMain::Update()
{
	// 衝突判定の管理クラスの更新処理
	COLLISION_MANAGER.Update();

	// キャラクターの更新処理
	CHARACTER_MANAGER.Update();

	// ステージの更新処理
	STAGE_MANAGER.Update();

	// シーンの状態ごとに更新処理を分ける
	switch (state)
	{
	// シーンの状態：カウントダウン
	case State::COUNTDOWN:
		updateForCountDown();
		break;

	// シーンの状態：バトル
	case State::BATTLE:
		updateForBattle();
		break;

	// シーンの状態：勝利
	case State::WINNER:
		updateForWinner();
		break;

	// シーンの状態：敗北
	case State::LOSE:
		updateForLose();
		break;

	default:
		break;
	}
}

/*-------------------------------------------*/
/* シーンの状態ごとの更新処理：カウントダウン
/*-------------------------------------------*/
void SceneMain::updateForCountDown()
{
	if (countDownCamera != NULL)
	{
		// プレイヤーの位置を取得
		VECTOR playerPos = character[0]->GetParts(CharacterBase::PartsType::TURRET)->GetTransform().position;

		// カメラの注視点位置をセット
		countDownCamera->SetLookAtPosition(playerPos);

		// 移動パラメータによってカメラを制御する
		int moveId = countDownCamera->MoveCamera();
		// 移動パラメータから返ってくるIDを現在のカウントダウンの数として登録
		currentCountDownNum = moveId;
		
		// カメラの更新処理
		countDownCamera->Update();

		// カメラの移動処理が終わったら、状態を"BATTLE"に変更
		if (moveId == -1)
		{
			// "バトル"
			state = State::BATTLE;
			// 全てのキャラクターを制御可能にする
			CHARACTER_MANAGER.SetAllCharacterControlingFlag(true);
		}
	}
}

/*-------------------------------------------*/
/* シーンの状態ごとの更新処理：バトル
/*-------------------------------------------*/
void SceneMain::updateForBattle()
{
	// ショットの更新処理
	SHOT_MANAGER.Update();
	// ショットの未使用リストと使用リストの要素を入れ替える
	SHOT_MANAGER.ReplaceShotList();

	// レイキャストの管理クラスの更新処理
	RAYCAST_MANAGER.Update();

	// インプットの更新処理
	INPUT.Update();

	// カメラ処理
	if (playerCamera != NULL)
	{
		// プレイヤーの位置を取得
		VECTOR playerPos = character[0]->GetParts(CharacterBase::PartsType::TURRET)->GetTransform().position;

		// プレイヤーの位置にオフセット値を加えたものを注視点位置にする
		VECTOR lookAtPos = playerPos + CAMERA_LOOKATPOS_OFFSET;

		// カメラの注視点位置をセット
		playerCamera->SetLookAtPosition(lookAtPos);

		// カメラの更新処理
		playerCamera->Update();

		// 生きている敵戦車が０になったら、シーンの状態を勝利に変更する
		if (CHARACTER_MANAGER.GetAliveEnemyTankNum() == 0)
		{
			// 勝利
			state = State::WINNER;
			// 全てのキャラクターを制御不能にする
			CHARACTER_MANAGER.SetAllCharacterControlingFlag(false);

			// 結果を勝利として登録
			SceneResult::SetResult(SceneResult::ResultType::WINNER);
		}

		// プレイヤーがやられたら、シーンの状態を敗北に変更
		if (CHARACTER_MANAGER.isAlivePlayerTank() == false)
		{
			// 敗北
			state = State::LOSE;
			// 全てのキャラクターを制御不能にする
			CHARACTER_MANAGER.SetAllCharacterControlingFlag(false);
			// 遷移カウントを０にリセット
			transitionCount = 0;

			// 結果を敗北として登録
			SceneResult::SetResult(SceneResult::ResultType::LOSE);

			// カメラの現在の回転角を取得
			float currentAngle = playerCamera->GetHorizontalAngle();
			// 敗北画面で扱うカメラに回転角をセット
			loseCamera->SetHorizontalAngle(currentAngle);
			// カメラの高さをセット
			loseCamera->SetCameraHeight(LOSE_CAMERA_HEIGHT);
			// 注視点からの距離をセット
			loseCamera->SetLookAtPosToDistance(LOSE_CAMERA_LOOKATPOS_DISTANCE);
		}
	}
}

/*-------------------------------------------*/
/* シーンの状態ごとの更新処理：勝利
/*-------------------------------------------*/
void SceneMain::updateForWinner()
{
	// 遷移カウントを加算
	transitionCount++;
	// 指定の時間経過したら、トランジションエフェクトを再生する
	if (transitionCount >= TRANSITION_EFFECT_PLAY_WAIT)
	{
		// トランジションエフェクトを再生
		PlayTransitionEffectForTankMove(1);
	}

	// 指定の時間経過したら、シーンをリザルトに変更する
	if (transitionCount >= SCENECHANGE_WAIT_FOR_RESULT)
	{
		SceneManager::SetNextScene(SceneManager::SceneType::RESULT);
		// 遷移カウントを０にリセット
		transitionCount = 0;
	}
}

/*-------------------------------------------*/
/* シーンの状態ごとの更新処理：敗北
/*-------------------------------------------*/
void SceneMain::updateForLose()
{
	if (loseCamera != NULL)
	{
		// プレイヤーの位置を取得
		VECTOR playerPos = character[0]->GetParts(CharacterBase::PartsType::TURRET)->GetTransform().position;

		// プレイヤーの位置にオフセット値を加えたものを注視点位置にする
		VECTOR lookAtPos = playerPos + CAMERA_LOOKATPOS_OFFSET;

		// カメラの注視点位置をセット
		loseCamera->SetLookAtPosition(lookAtPos);

		// 現在の回転角を取得
		float currentAngle = loseCamera->GetHorizontalAngle();
		// カメラを回転させる
		loseCamera->SetHorizontalAngle((currentAngle + LOSE_CAMERA_ROTATION_SPEED));

		// カメラの回転制御
		loseCamera->RotationCamera();
		// カメラの更新処理
		loseCamera->Update();
	}

	// 遷移カウントを加算
	transitionCount++;
	// 指定の時間経過したら、トランジションエフェクトを再生する
	if (transitionCount >= TRANSITION_EFFECT_PLAY_WAIT)
	{
		// トランジションエフェクトを再生
		PlayTransitionEffectForTankMove(1);
	}

	// 指定の時間経過したら、シーンをリザルトに変更する
	if (transitionCount >= SCENECHANGE_WAIT_FOR_RESULT)
	{
		SceneManager::SetNextScene(SceneManager::SceneType::RESULT);
		// 遷移カウントを０にリセット
		transitionCount = 0;
	}
}

/*-------------------------------------------*/
/* 描画
/*-------------------------------------------*/
void SceneMain::Draw()
{
	// ステージの描画処理
	STAGE_MANAGER.Draw();
	// スカイドームの描画
	STAGE_MANAGER.DrawSkydome();

	// ショットの描画処理
	SHOT_MANAGER.Draw();

	// キャラクターの描画処理
	CHARACTER_MANAGER.Draw();

	// シーンの状態ごとに描画処理を分ける
	switch (state)
	{
		// シーンの状態：カウントダウン
	case State::COUNTDOWN:
		drawForCountDown();
		break;

		// シーンの状態：バトル
	case State::BATTLE:
		drawForBattle();
		break;

		// シーンの状態：勝利
	case State::WINNER:
		drawForWinner();
		break;

		// シーンの状態：敗北
	case State::LOSE:
		drawForLose();
		break;

	default:
		break;
	}
}

/*-------------------------------------------*/
/* シーンの状態ごとの描画処理：カウントダウン
/*-------------------------------------------*/
void SceneMain::drawForCountDown()
{
	// カウントダウンで扱うスプライトハンドルを取得
	int countDownSprites[] =
	{
		SPRITE_MANAGER.GetHandle(ResourceSpriteManager::SpriteType::COUNTDOWN_START),		// スタート
		SPRITE_MANAGER.GetHandle(ResourceSpriteManager::SpriteType::COUNTDOWN_NUMBER_1),	// １
		SPRITE_MANAGER.GetHandle(ResourceSpriteManager::SpriteType::COUNTDOWN_NUMBER_2),	// ２
		SPRITE_MANAGER.GetHandle(ResourceSpriteManager::SpriteType::COUNTDOWN_NUMBER_3),	// ３
	};

	// カウントダウンで扱うスプライトをビルボードで描画
	DrawBillboard3D(COUNTDOWN_SPRITE_POS[currentCountDownNum],		// 位置
					0.5f,0.5f,800,0,								// 基準X、基準Y、スケール、回転角
					countDownSprites[currentCountDownNum],TRUE);	// スプライトハンドル
}

/*-------------------------------------------*/
/* シーンの状態ごとの描画処理：バトル
/*-------------------------------------------*/
void SceneMain::drawForBattle()
{
	// キャラクターのUIの描画処理
	CHARACTER_MANAGER.DrawUi();
}

/*-------------------------------------------*/
/* シーンの状態ごとの描画処理：勝利
/*-------------------------------------------*/
void SceneMain::drawForWinner()
{

}

/*-------------------------------------------*/
/* シーンの状態ごとの描画処理：敗北
/*-------------------------------------------*/
void SceneMain::drawForLose()
{

}


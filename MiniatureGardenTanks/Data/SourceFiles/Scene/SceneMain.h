#pragma once
#include "../Scene/SceneBase.h"
#include "../Character/CharacterBase.h"
#include "../Camera/CameraBase.h"

class CameraBase;
class SoundObject;

/*-------------------------------------------*/
/* シーン：メインシーン / 派生クラス
/*-------------------------------------------*/
class SceneMain final : public SceneBase
{
public:
			 SceneMain();					// コンストラクタ
	virtual ~SceneMain();					// デストラクタ

	void	 CreateObject()     override;	// オブジェクトの生成
	void	 DestroyObject()    override;	// オブジェクトの削除

	void     Initialize()		override;	// 初期化
	void     Update()			override;	// 更新
	void     Draw()				override;	// 描画

	// シーンの状態
	enum State
	{
		COUNTDOWN,	// カウントダウン
		BATTLE,		// バトル中
		WINNER,		// 勝利
		LOSE,		// 敗北
	};

private:
	// シーンの状態ごとの更新処理
	void UpdateForCountDown();		// カウントダウン
	void UpdateForBattle();			// バトル
	void UpdateForWinner();			// 勝利
	void UpdateForLose();			// 敗北

	// シーンの状態ごとの描画処理
	void DrawForCountDown();		// カウントダウン
	void DrawForBattle();			// バトル
	void DrawForWinner();			// 勝利
	void DrawForLose();				// 敗北

	CharacterBase** character;			// キャラクター
	CameraBase*     playerCamera;		// プレイヤーカメラ
	CameraBase*     countDownCamera;	// カウントダウン中のカメラ
	CameraBase*     loseCamera;			// 敗北した際のカメラ
	SoundObject*    bgm;				// メインBGM
	State           state;				// シーンの状態
	int    currentCountDownNum;			// 現在のカウントダウンの数
	int    transitionCount;				// シーンや状態の変更の際に扱うカウント
	
	static const UINT CHARACTER_NUM;								// キャラクターの数
	static const CharacterBase::CharacterType CHARACTER_TYPE[];		// キャラクターの種類
	static const VECTOR	 CHARACTER_POSITION[];						// キャラクターの位置
	static const float   CHARACTER_BODYANGLE[];						// キャラクターの車体角度
	static const COLOR_F CHARACTER_MODELCOLOR[];					// キャラクターのモデルの色
	static const VECTOR  CAMERA_LOOKATPOS_OFFSET;					// カメラ注視点位置のオフセット値
	static const VECTOR  COUNTDOWN_SPRITE_POS[];					// カウントダウンで扱うスプライトの位置
	
	static const float   LOSE_CAMERA_ROTATION_SPEED;				// 敗北時のカメラの回転速スピード
	static const float   LOSE_CAMERA_HEIGHT;						// 敗北時のカメラの高さ
	static const float   LOSE_CAMERA_LOOKATPOS_DISTANCE;			// 敗北時のカメラの注視点位置からの距離

	static const int     TRANSITION_EFFECT_PLAY_WAIT;				// トランジションエフェクトを再生する際のウェイトタイム
	static const int     SCENECHANGE_WAIT_FOR_RESULT;				// リザルトシーンに変更する際のウェイトタイム
};
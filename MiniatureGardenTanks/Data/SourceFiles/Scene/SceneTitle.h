#pragma once
#include "../Scene/SceneBase.h"
#include "../Character/CharacterBase.h"
#include <DxLib.h>

class CameraBase;
class SoundObject;

/*-------------------------------------------*/
/* シーン：メインシーン / 派生クラス
/*-------------------------------------------*/
class SceneTitle final : public SceneBase
{
public:
			 SceneTitle();					// コンストラクタ
	virtual ~SceneTitle();					// デストラクタ

	void	 CreateObject()     override;		// オブジェクトの生成
	void	 DestroyObject()    override;		// オブジェクトの削除

	void     Initialize()		override;		// 初期化
	void     Update()			override;		// 更新
	void     Draw()				override;		// 描画

private:
	CharacterBase* character;			// キャラクター
	SoundObject*   bgm;					// タイトルBGM
	CameraBase*    camera;				// カメラ
	int            transitionCount;		// トランジションで扱うカウント
	bool           isSceneChange;		// シーンを変更するかどうかのフラグ

	static const CharacterBase::CharacterType CHARACTER_TYPE;	// キャラクターの種類
	static const VECTOR	 CHARACTER_POSITION[];					// キャラクターの位置
	static const float   CHARACTER_BODYANGLE[];					// キャラクターの車体角度
	static const COLOR_F CHARACTER_MODELCOLOR[];				// キャラクターのモデルの色
	static const VECTOR  CAMERA_LOOKATPOS_OFFSET;				// カメラ注視点位置のオフセット値
	static const float   CAMERA_LOOKATPOS_DISTANCE;				// 注視点との距離
	static const float   CAMERA_HEIGHT;							// カメラの高さ
	static const float   CAMERA_ROTATION_SPEED;					// カメラの回転スピード
	static const VECTOR  TITLELOGO_SPRITE_POS;					// タイトルロゴの描画位置
	static const VECTOR  START_BUTTON_SPRITE_POS;				// ゲームスタートボタン描画位置
	static const int     SCENECHANGE_WAIT;						// シーン変更時の際のウェイトタイム
};
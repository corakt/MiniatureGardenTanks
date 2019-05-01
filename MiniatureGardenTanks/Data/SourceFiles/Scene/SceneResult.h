#pragma once
#include "../Scene/SceneBase.h"
#include "../Character/CharacterBase.h"

class CameraBase;
class SoundObject;

/*-------------------------------------------*/
/* シーン：リザルトシーン / 派生クラス
/*-------------------------------------------*/
class SceneResult final : public SceneBase
{
public:
			 SceneResult();					// コンストラクタ
	virtual ~SceneResult();					// デストラクタ

	void	 CreateObject()     override;	// オブジェクトの生成
	void	 DestroyObject()    override;	// オブジェクトの削除

	void     Initialize()		override;	// 初期化
	void     Update()			override;	// 更新
	void     Draw()				override;	// 描画

	// 結果の種類
	enum ResultType
	{
		WINNER,		// 勝利
		LOSE,		// 敗北
	};

	// バトルの結果をセットする
	static const ResultType& GetResult() { return result; }
	static void SetResult(const ResultType set) { result = set; }

private:
	// バトルの結果ごとの更新処理
	void UpdateForWinner();			// 勝利
	void UpdateForLose();			// 敗北

	// バトルの結果ごとの描画処理
	void DrawForWinner();			// 勝利
	void DrawForLose();				// 敗北

	CharacterBase*    character;			// キャラクター
	CameraBase*       camera;				// カメラ
	SoundObject*      bgm;					// リザルトBGM
	int               transitionCount;		// トランジションで扱うカウント
	bool              isSceneChange;		// シーンを変更するかどうかのフラグ
	static ResultType result;				// バトルの結果

	static const CharacterBase::CharacterType CHARACTER_TYPE;	// キャラクターの種類
	static const VECTOR	 CHARACTER_POSITION[];					// キャラクターの位置
	static const float   CHARACTER_BODYANGLE[];					// キャラクターの車体角度
	static const COLOR_F CHARACTER_MODELCOLOR[];				// キャラクターのモデルの色
	static const VECTOR  CAMERA_POSITION;						// カメラの位置
	static const VECTOR  LOGO_SPRITE_POS;						// ロゴ画像の表示位置
	static const VECTOR  BACKTITLE_BUTTON_SPRITE_POS;			// 「タイトルへ戻る」の表示位置
	static const int     SCENECHANGE_WAIT;						// シーン変更時の際のウェイトタイム
};
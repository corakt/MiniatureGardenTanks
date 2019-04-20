#pragma once
#include "../Scene/SceneBase.h"
#include "../Others/AsyncLoadData.h"
#include "../ResourcesManager/ResourceModelManager.h"
#include "../ResourcesManager/ResourceSpriteManager.h"
#include "../ResourcesManager/ResourceSoundManager.h"
#include <DxLib.h>

struct AnimationData
{
	int   *sprite;			// 画像
	VECTOR position;		// 表示座標
	float  size;			// 描画サイズ
	float  angle;			// 描画角度
	float  speed;			// 描画スピード
	int    animPatternNum;	// アニメパターン数
	float  animCount;		// アニメーションカウント
};

// １文字分のデータ
struct CharData
{
	VECTOR      position;			// 座標
	UINT        color;				// 色
	const char *character;			// 文字
	bool        isJump;				// ジャンプ中かどうか
	float       currentJumpPower;	// 現在のジャンプ力
};

// 非同期データ用構造体
struct AsyncLoadData
{
	ImageFileParam modelFileParam[ModelType::TYPE_NUM];
	ImageFileParam spriteFileParam[SpriteType::TYPE_NUM];
	ImageFileParam soundFileParam[SoundType::TYPE_NUM];
};

/*-------------------------------------------*/
/* シーン：ロード画面 / 派生クラス
/*-------------------------------------------*/
class SceneLoading final : public SceneBase
{
public:
			 SceneLoading();					// コンストラクタ
	virtual ~SceneLoading();					// デストラクタ

	void	 CreateObject()     override;		// オブジェクトの生成
	void	 DestroyObject()    override;		// オブジェクトの削除

	void     Initialize()		override;		// 初期化
	void     Update()			override;		// 更新
	void     Draw()				override;		// 描画

private:
	int fontHandle;							// フォントハンドル

	CharData *nowLoadingString;				// NowLoading文字列
	float     jumpMomentPosY;				// ジャンプした瞬間のY軸位置
	int       charJumpIntervalCount;		// ジャンプ間隔カウント

	AnimationData tankAnimation;			// 戦車アニメーション

	static const float  CHAR_JUMP_POWER;	// 文字のジャンプ力
	static const int    CHAR_JUMP_INTERVAL;	// 文字のジャンプ間隔
	static const float  CHAR_GRAVITY;		// 文字の重力
	static const int    STRING_LENGTH;		// 文字列の長さ
	static const VECTOR STRING_CENTER_POS;	// 文字列の基準位置
};

// 非同期読み込み用関数
void AsyncLoadResourceFromImageFile(AsyncLoadData *asyncLoadData);
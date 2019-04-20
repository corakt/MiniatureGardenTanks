#pragma once
#include <DxLib.h>

class StageManager;
class Input;

/*-------------------------------------------*/
/* シーン / 基底クラス
/*-------------------------------------------*/
class SceneBase
{
public:
				 SceneBase();				// デストラクタ
	virtual		~SceneBase();				// コンストラクタ

	virtual void CreateObject()  = 0;		// オブジェクトを生成
	virtual void DestroyObject() = 0;		// オブジェクトを削除

	virtual void Initialize()	 = 0;		// 初期化
	virtual void Update()		 = 0;		// 更新
	virtual void Draw()			 = 0;		// 描画
};
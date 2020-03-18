#pragma once
#include <DxLib.h>

/*-------------------------------------------*/
/* トランスフォームクラス
/*-------------------------------------------*/
class Transform final
{
public:
	 Transform();		// コンストラクタ
	~Transform();		// デストラクタ

	// トランスフォームのコピーを行う
	void Copy(Transform& transform);

	VECTOR position;	// 現在の座標
	VECTOR rotation;	// 現在の回転角
	VECTOR scale;		// 現在のスケール
	VECTOR direction;	// 現在の方向
	VECTOR velocity;	// 移動速度
};
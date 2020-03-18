#include "../Collision/SphereCollider.h"
#include "../Others/Define.h"

/*-------------------------------------------*/
/* コンストラクタ
/*-------------------------------------------*/
SphereCollider::SphereCollider()
{
	// 各変数の初期化
	center        = ZERO_VECTOR;	// 衝突判定範囲の中心位置
	radius        = 0;				// 衝突判定範囲の半径
	attachedModel = NULL;			// コライダーにアタッチするモデル
	isCollCheck   = false;			// 衝突判定を行うかどうか
}

/*-------------------------------------------*/
/* デストラクタ
/*-------------------------------------------*/
SphereCollider::~SphereCollider()
{
	// vectorを削除
	collModelInfo.clear();
}
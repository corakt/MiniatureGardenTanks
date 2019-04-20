#include "../Collision/BoxCollider.h"
#include "../Others/Define.h"

/*-------------------------------------------*/
/* コンストラクタ
/*-------------------------------------------*/
BoxCollider::BoxCollider()
{
	// 各変数の初期化
	center        = ZERO_VECTOR;	// 衝突判定範囲内の中心座標
	size          = ZERO_VECTOR;	// 衝突判定範囲のサイズ
	attachedModel = NULL;			// コライダーに関連付けるモデル
	isCollCheck   = false;			// 衝突判定を行うかどうか
}

/*-------------------------------------------*/
/* デストラクタ
/*-------------------------------------------*/
BoxCollider::~BoxCollider()
{
	// vectorを削除
	collModelInfo.clear();
}
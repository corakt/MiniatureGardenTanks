#include "../Collision/CircularSectorCollider.h"
#include "../Others/Define.h"

/*-------------------------------------------*/
/* コンストラクタ
/*-------------------------------------------*/
CircularSectorCollider::CircularSectorCollider()
{
	// 各変数の初期化
	center        = ZERO_VECTOR;	// 衝突判定範囲の中心位置
	direction     = VGet(0, 0, 1);	// 衝突範囲の向き
	width         = 0;				// 衝突範囲の幅
	distance      = 0;				// 衝突範囲の距離
	attachedModel = NULL;			// コライダーにアタッチするモデル
	isCollCheck   = false;			// 衝突判定を行うかどうか
}

/*-------------------------------------------*/
/* デストラクタ
/*-------------------------------------------*/
CircularSectorCollider::~CircularSectorCollider()
{
	// vectorを削除
	collModelInfo.clear();
}
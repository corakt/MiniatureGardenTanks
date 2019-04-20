#include "../BaseObject/Transform.h"
#include "../Others/Define.h"

/*-------------------------------------------*/
/* コンストラクタ
/*-------------------------------------------*/
Transform::Transform()
{
	// 処理なし
}

/*-------------------------------------------*/
/* デストラクタ
/*-------------------------------------------*/
Transform::~Transform()
{
	// 処理なし
}

/*-------------------------------------------*/
/* トランスフォームのコピーを行う
/*-------------------------------------------*/
void Transform::Copy(Transform& transform)
{
	transform.position  = position;		// 座標
	transform.direction = direction;	// 向き
	transform.rotation  = rotation;		// 回転角
	transform.scale     = scale;		// 大きさ
}
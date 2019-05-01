#include "../BaseObject/ModelObject.h"
#include "../Others/ErrorMessage.h"
#include "../Others/Define.h"
#include "../Others/Math3D.h"

/*-------------------------------------------*/
/* コンストラクタ
/*-------------------------------------------*/
ModelObject::ModelObject(const GameObject* parentObject)
{
	// モデルハンドルを初期化
	this->modelHandle = 0;

	// コンポーネントが追加されている親オブジェクトを取得
	this->parentObject = parentObject;
}

/*-------------------------------------------*/
/* デストラクタ
/*-------------------------------------------*/
ModelObject::~ModelObject()
{
	// モデルの削除
	SafeDeleteModel(modelHandle);

	// コンポーネントもとの親オブジェクト
	parentObject = NULL;
}

/*-------------------------------------------*/
/* 初期化
/*-------------------------------------------*/
void ModelObject::Initialize()
{
	transform.position  = ZERO_VECTOR;		// 位置
	transform.rotation  = ZERO_VECTOR;		// 回転角
	transform.scale     = VGet(1, 1, 1);	// スケール
	transform.direction = VGet(0, 0, 1);	// 向き
	transform.velocity  = ZERO_VECTOR;		// 移動速度
	isDraw   = false;						// 描画フラグ
	isActive = false;						// アクティブフラグ
}

/*-------------------------------------------*/
/* 更新
/*-------------------------------------------*/
void ModelObject::Update()
{

}

/*-------------------------------------------*/
/* モデルの描画
/*-------------------------------------------*/
void ModelObject::DrawModel()
{
	// 拡大行列を取得
	MATRIX scaleMatrix = MGetScale(transform.scale);
	// 平行行列を取得
	MATRIX translateMatrix = MGetTranslate(transform.position);
	// 回転行列を取得
	MATRIX rotationMatrix = GetRotMatrixFromRot(transform.rotation);

	// 行列を合成する
	MATRIX transformMatrix = scaleMatrix * rotationMatrix * translateMatrix;
	// 行列をセットする
	MV1SetMatrix(modelHandle, transformMatrix);

	// 描画フラグが立っていたら
	if (isDraw)
	{
		// モデルを描画
		MV1DrawModel(modelHandle);
	}
}
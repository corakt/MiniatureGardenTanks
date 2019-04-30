#include "../Collision/CollisionManager.h"
#include "../Collision/BoxCollider.h"
#include "../Collision/SphereCollider.h"
#include "../Collision/CircularSectorCollider.h"
#include "../Collision/CollisionCheckFunction.h"
#include "../BaseObject/ModelObject.h"
#include "../Others/Define.h"
#include "../Others/Math3D.h"
#include <vector>
#include <algorithm>

const int CollisionManager::BOX_COLLIDER_MAX            = 1000;		// ボックスコライダーの最大数
const int CollisionManager::SPHERE_COLLIDER_MAX         = 30;		// スフィアコライダーの最大数
const int CollisionManager::CIRCULARSECTOR_COLLIDER_MAX = 10;		// 扇形のコライダーの最大数

/*-------------------------------------------*/
/* コンストラクタ
/*-------------------------------------------*/
CollisionManager::CollisionManager()
{
	// コライダーの数だけメモリを確保
	boxCollider            = new BoxCollider*[BOX_COLLIDER_MAX];									// ボックス
	sphereCollider         = new SphereCollider*[SPHERE_COLLIDER_MAX];								// スフィア
	circularSectorCollider = new CircularSectorCollider*[CIRCULARSECTOR_COLLIDER_MAX];				// 扇形
	// 確保したメモリをNULLで初期化
	for (int i = 0; i < BOX_COLLIDER_MAX; i++)            { boxCollider[i]            = NULL; }		// ボックス
	for (int i = 0; i < SPHERE_COLLIDER_MAX; i++)         { sphereCollider[i]         = NULL; }		// スフィア
	for (int i = 0; i < CIRCULARSECTOR_COLLIDER_MAX; i++) { circularSectorCollider[i] = NULL; }		// 扇形
}

/*-------------------------------------------*/
/* デストラクタ
/*-------------------------------------------*/
CollisionManager::~CollisionManager()
{
	// コライダーを削除
	SafeDeleteArray(boxCollider);				// ボックス
	SafeDeleteArray(sphereCollider);			// スフィア
	SafeDeleteArray(circularSectorCollider);	// 扇形
}

/*-------------------------------------------*/
/* ボックスコライダーを追加
/*-------------------------------------------*/
BoxCollider* CollisionManager::AddBoxCollider()
{
	// ループしながらメモリを確保できる変数を探す
	for (int i = 0; i < BOX_COLLIDER_MAX; i++)
	{
		// 既に確保されていれば、次の変数に移る
		if (boxCollider[i] != NULL) { continue; }
		// 確保されていない変数を見つけた場合、その変数にメモリを確保する
		else
		{
			// ボックスコライダーを生成
			boxCollider[i] = new BoxCollider();

			// ボックスコライダーをリストに追加
			activeBoxCollider.push_back(boxCollider[i]);

			// ボックスコライダーのポインタを返す
			return boxCollider[i];
		}
	}

	// 確保できる変数が見つからなければNULLを返す
	return NULL;
}

/*-------------------------------------------*/
/* スフィアコライダーの追加
/*-------------------------------------------*/
SphereCollider* CollisionManager::AddSphereCollider()
{
	// ループしながらメモリを確保できる変数を探す
	for (int i = 0; i < SPHERE_COLLIDER_MAX; i++)
	{
		// 既に確保されていれば、次の変数に移る
		if (sphereCollider[i] != NULL) { continue; }
		// 確保されていない変数を見つけた場合、その変数にメモリを確保する
		else
		{
			// スフィアコライダーを生成
			sphereCollider[i] = new SphereCollider();

			// スフィアコライダーをリストに追加
			activeSphereCollider.push_back(sphereCollider[i]);

			// スフィアコライダーをのポインタを返す
			return sphereCollider[i];
		}
	}

	// 確保できる変数が見つからなければNULLを返す
	return NULL;
}

/*-------------------------------------------*/
/* 扇形のコライダーを追加
/*-------------------------------------------*/
CircularSectorCollider* CollisionManager::AddCircularSectorCollider()
{
	// ループしながらメモリを確保できる変数を探す
	for (int i = 0; i < CIRCULARSECTOR_COLLIDER_MAX; i++)
	{
		// 既に確保されていれば、次の変数に移る
		if (circularSectorCollider[i] != NULL) { continue; }
		// 確保されていない変数を見つけた場合、その変数にメモリを確保する
		else
		{
			// 扇形のコライダーを生成
			circularSectorCollider[i] = new CircularSectorCollider();

			// 扇形のコライダーをリストに追加
			activeCircularSectorCollider.push_back(circularSectorCollider[i]);

			// 扇形のコライダーをのポインタを返す
			return circularSectorCollider[i];
		}
	}

	// 確保できる変数が見つからなければNULLを返す
	return NULL;
}

/*-------------------------------------------*/
/* 初期化
/*-------------------------------------------*/
void CollisionManager::Initialize()
{

}

/*-------------------------------------------*/
/* 更新
/*-------------------------------------------*/
void CollisionManager::Update()
{
	// それぞれのコライダーの前フレームで保存されていた衝突情報を全て削除する

	// ボックスコライダーイテレータ
	if (activeBoxCollider.empty() == false)
	{
		for (BoxCollider*& boxColliderElem : activeBoxCollider)
		{
			// 要素がNULLであれば、次の要素に移る
			if (boxColliderElem == NULL) { continue; }

			// 前フレームで保存されていた衝突情報のvectorを全削除する
			boxColliderElem->ClearCollModelVector();
		}
	}

	// スフィアコライダーイテレータ
	if (activeSphereCollider.empty() == false)
	{
		for (SphereCollider*& sphereColliderElem : activeSphereCollider)
		{
			// 要素がNULLであれば、次の要素に移る
			if (sphereColliderElem == NULL) { continue; }

			// 前フレームで保存されていた衝突情報のvectorを全削除する
			sphereColliderElem->ClearCollModelVector();
		}
	}

	// サーキュラーセクターコライダーイテレータ（扇形）
	if (activeCircularSectorCollider.empty() == false)
	{
		for (CircularSectorCollider*& circularSectorColliderElem : activeCircularSectorCollider)
		{
			// 要素がNULLであれば、次の要素に移る
			if (circularSectorColliderElem == NULL) { continue; }

			// 前フレームで保存されていた衝突情報のvectorを全削除する
			circularSectorColliderElem->ClearCollModelVector();
		}
	}

	// 衝突判定：ボックスとボックス
	CollisionCheckBoxAndBox();
	// 衝突判定：スフィアとスフィア
	CollisionCheckSphereAndSphere();
	// 衝突判定：ボックスとスフィア
	CollisionCheckBoxAndSphere();
	// 衝突判定：扇形とボックス
	CollisionCheckCircularSectorAndBox();
}

/*-------------------------------------------*/
/* 終了処理
/*-------------------------------------------*/
void CollisionManager::Finalize()
{
	// vectorを削除
	activeBoxCollider.clear();
	activeSphereCollider.clear();
	activeCircularSectorCollider.clear();

	// ボックスコライダーを削除
	for (int i = 0; i < BOX_COLLIDER_MAX; i++)
	{
		SafeDelete(boxCollider[i]);
	}
	// スフィアコライダーを削除
	for (int i = 0; i < SPHERE_COLLIDER_MAX; i++)
	{
		SafeDelete(sphereCollider[i]);
	}
	// サーキュラーコライダーを削除
	for (int i = 0; i < CIRCULARSECTOR_COLLIDER_MAX; i++)
	{
		SafeDelete(circularSectorCollider[i]);
	}
}

/*-------------------------------------------*/
/* 衝突判定：ボックスとボックス
/*-------------------------------------------*/
void CollisionManager::CollisionCheckBoxAndBox()
{
	// リスト内が空であれば、その時点で終了
	if (activeBoxCollider.empty()) { return; }

	// 衝突判定を行うコライダー
	for (BoxCollider*& boxColliderElem1 : activeBoxCollider)
	{
		// 要素がNULLであれば、次の要素に移る
		if (boxColliderElem1 == NULL) { continue; }
		// アタッチされているモデルがNULLであればスキップ
		if (boxColliderElem1->attachedModel == NULL) { continue; }

		// 稼働していないモデルは判定を行わない
		if (boxColliderElem1->attachedModel->GetActiveFlag() == false) { continue; }

		// 衝突判定を行わないコライダーはスキップして、次の要素に移る
		if (boxColliderElem1->isCollCheck == false) { continue; }

		// 対象のコライダーに衝突している別のコライダーを調べる
		for (BoxCollider*& boxColliderElem2 : activeBoxCollider)
		{
			// 要素がNULLであれば、次の要素に移る
			if (boxColliderElem2 == NULL) { continue; }
			// アタッチされているモデルがNULLであればスキップ
			if (boxColliderElem2->attachedModel == NULL) { continue; }

			// 稼働していないモデルは判定を行わない
			if (boxColliderElem2->attachedModel->GetActiveFlag() == false) { continue; }

			// 同じコライダー同士で判定を行おうとしていた場合は、スキップして次の要素に移る
			if (boxColliderElem1 == boxColliderElem2) { continue; }

			// コライダー同士の衝突判定を行う
			bool isCollision = IsCollisionBoxAndBox(boxColliderElem1->center, boxColliderElem1->size,
									                boxColliderElem2->center, boxColliderElem2->size);

			// 衝突していた場合は、そのコライダーにアタッチされているモデルのポインタを専用のリストに渡す
			if (isCollision)
			{
				// 一時衝突情報
				CollModelInfo tmpCollModelInfo;

				// 衝突したモデルのポインタをセット
				tmpCollModelInfo.collModel = boxColliderElem2->attachedModel;
				// コライダーの種類をセット
				tmpCollModelInfo.colliderType = ColliderType::BOX;
				// 衝突情報をセット
				boxColliderElem1->SetCollModelInfo(tmpCollModelInfo);

				// 衝突したモデルのポインタをセット
				tmpCollModelInfo.collModel = boxColliderElem1->attachedModel;
				// コライダーの種類をセット
				tmpCollModelInfo.colliderType = ColliderType::BOX;
				// 衝突情報をセット
				boxColliderElem2->SetCollModelInfo(tmpCollModelInfo);

				// ボックス同士が衝突した際の処理にうつる
				OnCollisionBoxAndBox(boxColliderElem1, boxColliderElem2);
			}
		}
	}
}

/*-------------------------------------------*/
/* 衝突判定：スフィアとスフィア
/*-------------------------------------------*/
void CollisionManager::CollisionCheckSphereAndSphere()
{
	// リスト内が空であれば、その時点で終了
	if (activeSphereCollider.empty()) { return; }

	// 衝突判定を行うコライダー
	for (SphereCollider*& sphereColliderElem1 : activeSphereCollider)
	{
		// 要素がNULLであれば、次の要素に移る
		if (sphereColliderElem1 == NULL) { continue; }
		// アタッチされているモデルがNULLであればスキップ
		if (sphereColliderElem1->attachedModel == NULL) { continue; }

		// 衝突判定を行わないコライダーはスキップして、次の要素に移る
		if (sphereColliderElem1->isCollCheck == false) { continue; }

		// 対象のコライダーに衝突している別のコライダーを調べる
		for (SphereCollider*& sphereColliderElem2 : activeSphereCollider)
		{
			// 要素がNULLであれば、次の要素に移る
			if (sphereColliderElem2 == NULL) { continue; }
			// アタッチされているモデルがNULLであればスキップ
			if (sphereColliderElem2->attachedModel == NULL) { continue; }

			// 同じコライダー同士で判定を行おうとしていた場合は、スキップして次の要素に移る
			if (sphereColliderElem1 == sphereColliderElem2) { continue; }

			// コライダー同士の衝突判定を行う
			bool isCollision = IsCollisionSphereAndSphere(sphereColliderElem1->center, sphereColliderElem1->radius,
									                      sphereColliderElem2->center, sphereColliderElem2->radius);

			// 衝突していた場合は、そのコライダーにアタッチされているモデルのポインタを専用のリストに渡す
			if (isCollision)
			{
				// 一時衝突情報
				CollModelInfo tmpCollModelInfo;

				// 衝突したモデルのポインタをセット
				tmpCollModelInfo.collModel = sphereColliderElem2->attachedModel;
				// コライダーの種類をセット
				tmpCollModelInfo.colliderType = ColliderType::SPHERE;
				// 衝突情報をセット
				sphereColliderElem1->SetCollModelInfo(tmpCollModelInfo);

				// 衝突したモデルのポインタをセット
				tmpCollModelInfo.collModel = sphereColliderElem1->attachedModel;
				// コライダーの種類をセット
				tmpCollModelInfo.colliderType = ColliderType::SPHERE;
				// 衝突情報をセット
				sphereColliderElem2->SetCollModelInfo(tmpCollModelInfo);
			}
		}
	}
}

/*-------------------------------------------*/
/* 衝突判定：ボックスとスフィア
/*-------------------------------------------*/
void CollisionManager::CollisionCheckBoxAndSphere()
{
	// リスト内が空であれば、その時点で終了
	if (activeBoxCollider.empty())    { return; }		// ボックス
	if (activeSphereCollider.empty()) { return; }		// スフィア

	// 衝突判定を行うコライダー
	for (BoxCollider*& boxColliderElem : activeBoxCollider)
	{
		// 要素がNULLであれば、次の要素に移る
		if (boxColliderElem == NULL) { continue; }
		// アタッチされているモデルがNULLであればスキップ
		if (boxColliderElem->attachedModel == NULL) { continue; }

		// 対象のコライダーに衝突している別のコライダーを調べる
		for (SphereCollider*& sphereColliderElem : activeSphereCollider)
		{
			// 要素がNULLであれば、次の要素に移る
			if (sphereColliderElem == NULL) { continue; }
			// アタッチされているモデルがNULLであればスキップ
			if (sphereColliderElem->attachedModel == NULL) { continue; }

			// お互いのコライダーの衝突判定フラグが立っていなければ、
			// 判定は行わずにスキップする
			if (boxColliderElem->isCollCheck == false && sphereColliderElem->isCollCheck == false)
			{
				continue;
			}

			// コライダー同士の衝突判定を行う
			bool isCollision = IsCollisionBoxAndSphere(boxColliderElem->center, boxColliderElem->size,
				                                      sphereColliderElem->center, sphereColliderElem->radius);

			// 衝突していた場合は、そのコライダーにアタッチされているモデルのポインタを専用のリストに渡す
			if (isCollision)
			{
				// 一時衝突情報
				CollModelInfo tmpCollModelInfo;

				// 衝突したモデルのポインタをセット
				tmpCollModelInfo.collModel = sphereColliderElem->attachedModel;
				// コライダーの種類をセット
				tmpCollModelInfo.colliderType = ColliderType::SPHERE;
				// 衝突情報をセット
				boxColliderElem->SetCollModelInfo(tmpCollModelInfo);

				// 衝突したモデルのポインタをセット
				tmpCollModelInfo.collModel = boxColliderElem->attachedModel;
				// コライダーの種類をセット
				tmpCollModelInfo.colliderType = ColliderType::BOX;
				// 衝突情報をセット
				sphereColliderElem->SetCollModelInfo(tmpCollModelInfo);
			}
		}
	}
}

/*-------------------------------------------*/
/* 衝突判定：扇形とボックス
/*-------------------------------------------*/
void CollisionManager::CollisionCheckCircularSectorAndBox()
{
	// リスト内が空であれば、その時点で終了
	if (activeBoxCollider.empty())            { return; }
	if (activeCircularSectorCollider.empty()) { return; }

	// 衝突判定を行うコライダー
	for (CircularSectorCollider*& circularSectorColliderElem : activeCircularSectorCollider)
	{
		// 要素がNULLであれば、次の要素に移る
		if (circularSectorColliderElem == NULL) { continue; }
		// アタッチされているモデルがNULLであればスキップ
		if (circularSectorColliderElem->attachedModel == NULL) { continue; }

		// 対象のコライダーに衝突している別のコライダーを調べる
		for (BoxCollider*& boxColliderElem : activeBoxCollider)
		{
			// 要素がNULLであれば、次の要素に移る
			if (boxColliderElem == NULL) { continue; }
			// アタッチされているモデルがNULLであればスキップ
			if (boxColliderElem->attachedModel == NULL) { continue; }

			// 自分自身のコライダーは無視する
			if (circularSectorColliderElem->attachedModel == boxColliderElem->attachedModel) { continue; }

			// コライダー同士の衝突判定を行う
			bool isCollision = IsCollisionCircularSectorAndBox(circularSectorColliderElem->center,					// 扇形：基準位置
				                                               circularSectorColliderElem->direction,				// 扇形：向き
				                                               circularSectorColliderElem->width,					// 扇形：幅
				                                               circularSectorColliderElem->distance,				// 扇形：距離
				                                               boxColliderElem->center, boxColliderElem->size);		// ボックス：基準位置 / サイズ

			// 衝突していた場合は、そのコライダーにアタッチされているモデルのポインタを専用のリストに渡す
			if (isCollision)
			{
				// 一時衝突情報
				CollModelInfo tmpCollModelInfo;

				// 衝突したモデルのポインタをセット
				tmpCollModelInfo.collModel = boxColliderElem->attachedModel;
				// コライダーの種類をセット
				tmpCollModelInfo.colliderType = ColliderType::BOX;
				// 衝突情報をセット
				circularSectorColliderElem->SetCollModelInfo(tmpCollModelInfo);

				// 衝突したモデルのポインタをセット
				tmpCollModelInfo.collModel = circularSectorColliderElem->attachedModel;
				// コライダーの種類をセット
				tmpCollModelInfo.colliderType = ColliderType::CIRCULAR_SECTOR;
				// 衝突情報をセット
				boxColliderElem->SetCollModelInfo(tmpCollModelInfo);
			}
		}
	}
}

/*-------------------------------------------*/
/* ボックス同士が衝突した際の処理
/*-------------------------------------------*/
void CollisionManager::OnCollisionBoxAndBox(BoxCollider* collider1, BoxCollider* collider2)
{
	// ボックス同士の衝突判定のみ、オブジェクト同士がめり込まないようにする。
	// それが以下の処理である。

	// ボックスの面パラメータクラス
	//（ここでしか使わないクラスなのでローカルクラス）
	class BoxSurfaceParam
	{
	public:
		VECTOR normalVec;		// 面の法線ベクトル
		float  inner;			// 別のベクトルとの内積
		bool   isCollCheck;		// 面に対して衝突判定を行うか

		// 内積で比較する（小さい順）
		bool operator<(BoxSurfaceParam& right)
		{
			return inner < right.inner;
		}
	};

	// それぞれのコライダーの面パラメータ
	static std::vector<BoxSurfaceParam> objectSurface1;
	static std::vector<BoxSurfaceParam> objectSurface2;

	// vectorをリサイズする
	// 前面、背面、左面、右面のそれぞれ４つ
	objectSurface1.resize(4);
	objectSurface2.resize(4);

	// それぞれの法線ベクトルを格納しておく
	// （ボックスは回転しないので、XZ軸がそのまま法線ベクトルになる）
	objectSurface1[0].normalVec = VGet( 0, 0, +1);	// 前面
	objectSurface1[1].normalVec = VGet( 0, 0, -1);	// 背面
	objectSurface1[2].normalVec = VGet(-1, 0,  0);	// 左面
	objectSurface1[3].normalVec = VGet(+1, 0,  0);	// 右面

	objectSurface2[0].normalVec = VGet( 0, 0, +1);	// 前面
	objectSurface2[1].normalVec = VGet( 0, 0, -1);	// 背面
	objectSurface2[2].normalVec = VGet(-1, 0,  0);	// 左面
	objectSurface2[3].normalVec = VGet(+1, 0,  0);	// 右面

	// 衝突したコライダーにアタッチされているオブジェクトを取得
	ModelObject* object1 = collider1->attachedModel;
	ModelObject* object2 = collider2->attachedModel;

	// それぞれのオブジェクトのトランスフォームを取得
	Transform objTrans1 = object1->GetTransform();
	Transform objTrans2 = object2->GetTransform();

	// オブジェクト同士のベクトルを算出
	VECTOR objectPairVec = VSub(objTrans2.position, objTrans1.position);
	// 正規化する
	objectPairVec = SafeNormlizeVector(objectPairVec);

	// それぞれの法線ベクトルとの内積を算出して、vectorに格納する
	for (int i = 0; i < 4; i++)
	{
		// オブジェクト１
		// 内積を算出
		float tmpInner = VDot(objectPairVec, objectSurface1[i].normalVec);
		// vectorに格納
		objectSurface1[i].inner = tmpInner;

		// オブジェクト２
		// 内積を算出
		tmpInner = VDot(objectPairVec, objectSurface2[i].normalVec);
		// vectorに格納
		objectSurface2[i].inner = tmpInner;
	}

	// それぞれのオブジェクトの内積でvectorをソートする
	// （内積が一番小さい法線ベクトルの面に衝突したことになる）
	std::sort(objectSurface1.begin(), objectSurface1.end());
	std::sort(objectSurface2.begin(), objectSurface2.end());

	// 決定した面のパラメータを抜き出す
	static BoxSurfaceParam targetSurface1 = objectSurface1.front();
	static BoxSurfaceParam targetSurface2 = objectSurface2.front();
		
	// 互いのコライダーの合計サイズの半分のサイズを算出
	VECTOR colliderPairSizeHalf = (collider1->size + collider2->size) * 0.5f;

	/*-------------------------------------------*/
	/* オブジェクト１
	/*-------------------------------------------*/

	// 衝突後の調整位置の計算式
	// 新たな位置 = 相手の現在位置 * ((互いのコライダーの合計サイズ / 2) + (衝突面の法線ベクトル * (自信の移動量-相手の移動量)))

	// オブジェクトが移動中の場合のみ、位置を調整する
	if (VSquareSize(objTrans1.velocity) != 0)
	{
		// 上下
		if (targetSurface1.normalVec.z != 0)
		{
			// 衝突した際のオブジェクトのZ軸の調整置を算出
			float objectPosOffsetZ = targetSurface1.normalVec.z * (colliderPairSizeHalf.z + (targetSurface1.normalVec.z * (objTrans1.velocity.z-objTrans2.velocity.z)));

			// 算出した調整値をもとにオブジェクトの位置を決定する
			objTrans1.position.z = objTrans2.position.z + objectPosOffsetZ;
		}
		// 左右
		if (targetSurface1.normalVec.x != 0)
		{
			// 衝突した際のオブジェクトのX軸の調整置を算出
			float objectPosOffsetX = targetSurface1.normalVec.x * (colliderPairSizeHalf.x + (targetSurface1.normalVec.x * (objTrans1.velocity.x-objTrans2.velocity.x)));

			// 算出した調整値をもとにオブジェクトの位置を決定する
			objTrans1.position.x = objTrans2.position.x + objectPosOffsetX;
		}
	}

	/*-------------------------------------------*/
	/* オブジェクト２
	/*-------------------------------------------*/

	// オブジェクトが移動中の場合のみ、位置を調整する
	if (VSquareSize(objTrans2.velocity) != 0)
	{
		// 上下
		if (targetSurface2.normalVec.z != 0)
		{
			// 衝突した際のオブジェクトのZ軸の調整置を算出
			float objectPosOffsetZ = -targetSurface2.normalVec.z * (colliderPairSizeHalf.z + (-targetSurface2.normalVec.z * (objTrans2.velocity.z-objTrans1.velocity.z)));

			// 算出した調整値をもとにオブジェクトの位置を決定する
			objTrans2.position.z = objTrans1.position.z + objectPosOffsetZ;
		}
		// 左右
		if (targetSurface2.normalVec.x != 0)
		{
			// 衝突した際のオブジェクトのX軸の調整置を算出
			float objectPosOffsetX = -targetSurface2.normalVec.x * (colliderPairSizeHalf.x + (-targetSurface2.normalVec.x * (objTrans2.velocity.x - objTrans1.velocity.x)));

			// 算出した調整値をもとにオブジェクトの位置を決定する
			objTrans2.position.x = objTrans1.position.x + objectPosOffsetX;
		}
	}

	// それぞれのオブジェクトのトランスフォームをセットする
	object1->SetTransform(objTrans1);
	object2->SetTransform(objTrans2);
}
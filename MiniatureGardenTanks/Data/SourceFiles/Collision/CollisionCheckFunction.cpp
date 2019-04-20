#include "../Collision/CollisionCheckFunction.h"
#include "../Collision/SphereCollider.h"
#include "../Others/HelperFunction.h"
#include "../Others/Math3D.h"
#include "../Others/Define.h"

/*-------------------------------------------*/
/* ボックス同士の衝突判定
/*-------------------------------------------*/
bool IsCollisionBoxAndBox(const VECTOR& center1, const VECTOR& size1, const VECTOR& center2, const VECTOR& size2)
{
	// 座標の起点を左上にずらす
	VECTOR vertexPos1 = center1 - (size1 * 0.5f);
	VECTOR vertexPos2 = center2 - (size2 * 0.5f);

	if (((vertexPos1.x >= vertexPos2.x && vertexPos1.x <= vertexPos2.x + size2.x)  ||
		 (vertexPos2.x >= vertexPos1.x && vertexPos2.x <= vertexPos1.x + size1.x)) &&
		((vertexPos1.y >= vertexPos2.y && vertexPos1.y <= vertexPos2.y + size2.y)  ||
		 (vertexPos2.y >= vertexPos1.y && vertexPos2.y <= vertexPos1.y + size1.y)) &&
		((vertexPos1.z >= vertexPos2.z && vertexPos1.z <= vertexPos2.z + size2.z)  ||
		 (vertexPos2.z >= vertexPos1.z && vertexPos2.z <= vertexPos1.z + size1.z)))
	{
		return true;
	}
	return false;
}

/*-------------------------------------------*/
/* スフィア同士の衝突判定
/*-------------------------------------------*/
bool IsCollisionSphereAndSphere(const VECTOR& center1,const float& radius1,const VECTOR& center2,const float& radius2)
{
	// 衝突判定のモデル同士のベクトルを算出
	VECTOR modelEachVec = VSub(center2, center1);

	// お互いの衝突判定の円が衝突しているか調べる
	if (VSquareSize(modelEachVec) < (radius1 + radius2) * (radius1 + radius2))
	{
		return true;
	}
	return false;
}

/*-------------------------------------------*/
/* スフィアとボックスの頂点との衝突判定
/*-------------------------------------------*/
bool IsCollisionSphereAndBoxVertices(const VECTOR& boxCenter,const VECTOR& boxSize,const VECTOR& sphereCenter,const float& sphereRadius)
{
	// ボックスの８つの頂点位置を算出
	VECTOR boxVerticesPos[8];
	GetBoxVerticesPosition(boxCenter, boxSize, boxVerticesPos);

	for (int i = 0; i < 8; i++)
	{
		// ボックスの８つの頂点が球体の中に存在しているか調べる
		if (IsCollisionSphereAndSphere(sphereCenter, sphereRadius, boxVerticesPos[i], 1))
		{
			return true;
		}
	}
	return false;
}

/*-------------------------------------------*/
/* スフィアがボックスの中に入っているか
/*-------------------------------------------*/
bool InSphereToBox(const VECTOR& boxCenter, const VECTOR& boxSize, const VECTOR& sphereCenter)
{
	// ボックスの両端の頂点の位置を算出
	VECTOR vertaxPos1 = boxCenter - (boxSize * 0.5);
	VECTOR vertaxPos2 = boxCenter + (boxSize * 0.5);

	// 両端の頂点と球体とのベクトルを算出
	VECTOR vertexToSphereVec1 = sphereCenter - vertaxPos1;
	VECTOR vertexToSphereVec2 = sphereCenter - vertaxPos2;

	// ベクトルを正規化する
	vertexToSphereVec1 = VNorm(vertexToSphereVec1);
	vertexToSphereVec2 = VNorm(vertexToSphereVec2);

	// 算出したベクトルとX軸ベクトルとの内積を算出
	float vecEachDotX1 = acosf(VDot(vertexToSphereVec1, VGet( 1, 0, 0)));
	float vecEachDotX2 = acosf(VDot(vertexToSphereVec2, VGet(-1, 0, 0)));
	// 算出したベクトルとY軸ベクトルとの内積を算出
	float vecEachDotY1 = acosf(VDot(vertexToSphereVec1, VGet(0,  1, 0)));
	float vecEachDotY2 = acosf(VDot(vertexToSphereVec2, VGet(0, -1, 0)));
	// 算出したベクトルとZ軸ベクトルとの内積を算出
	float vecEachDotZ1 = acosf(VDot(vertexToSphereVec1, VGet(0, 0,  1)));
	float vecEachDotZ2 = acosf(VDot(vertexToSphereVec2, VGet(0, 0, -1)));

	// 算出した全ての内積が９０度以内か調べる
	if (vecEachDotX1 < DX_PI_F / 2 && vecEachDotX2 < DX_PI_F / 2 &&
		vecEachDotY1 < DX_PI_F / 2 && vecEachDotY2 < DX_PI_F / 2 &&
		vecEachDotZ1 < DX_PI_F / 2 && vecEachDotZ2 < DX_PI_F / 2)
	{
		return true;
	}
	return false;
}

/*-------------------------------------------*/
/* ボックスとスフィアの衝突判定
/*-------------------------------------------*/
bool IsCollisionBoxAndSphere(const VECTOR& boxCenter, const VECTOR& boxSize, const VECTOR& sphereCenter, const float& sphereRadius)
{
	// ボックスのいずれかの頂点に衝突しているか調べる
	bool isCollBoxVertices = IsCollisionSphereAndBoxVertices(boxCenter, boxSize, sphereCenter, sphereRadius);
	// 衝突している
	if (isCollBoxVertices)
	{
		return true;
	}

	// スフィアがボックスの中に入っているか調べる
	bool inBox = InSphereToBox(boxCenter, boxSize, sphereCenter);
	// 入っている
	if (inBox)
	{
		return true;
	}
	return false;
}

/*-------------------------------------------*/
/* 扇型とボックスの衝突判定
/*-------------------------------------------*/
bool IsCollisionCircularSectorAndBox(const VECTOR& csCenter, const VECTOR& csDirection, const float& csWidth, const float& csDistance, const VECTOR& boxCenter, const VECTOR& boxSize)
{
	// 扇型の左側の境界ベクトルを算出
	VECTOR csBorderVecLeft = RotationVector(csDirection, VGet(0, 1, 0), -csWidth * 0.5f);
	// 扇型の右側の境界ベクトルを算出
	VECTOR csBorderVecRight = RotationVector(csDirection, VGet(0, 1, 0), csWidth * 0.5f);

	// ボックスの両端の頂点の位置を算出
	VECTOR vertaxPos1 = boxCenter - (boxSize * 0.5f);
	VECTOR vertaxPos2 = boxCenter + (boxSize * 0.5f);

	// 扇型の基準位置からボックスの頂点まで伸びるベクトルを算出
	VECTOR csCenterToVertex1 = VSub(vertaxPos1, csCenter);
	VECTOR csCenterToVertex2 = VSub(vertaxPos2, csCenter);
	// ベクトルの長さを保存
	float vectorLength1 = VSquareSize(csCenterToVertex1);
	float vectorLength2 = VSquareSize(csCenterToVertex2);
	// ベクトルを正規化する
	csCenterToVertex1 = SafeNormlizeVector(csCenterToVertex1);
	csCenterToVertex2 = SafeNormlizeVector(csCenterToVertex2);

	// 算出したベクトルと扇形の境界ベクトルとで内積を算出
	float vectorDot1 = acosf(VDot(csDirection, csCenterToVertex1));
	float vectorDot2 = acosf(VDot(csDirection, csCenterToVertex2));

	// ボックスが扇形の範囲内に入っているか調べる
	if ((vectorDot1 < csWidth *0.5f || vectorDot2 < csWidth *0.5f) &&
		(vectorLength1 <= csDistance * csDistance || vectorLength2 <= csDistance * csDistance))
	{
		return true;
	}

	return false;
}
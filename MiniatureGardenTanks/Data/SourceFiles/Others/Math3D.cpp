#include "../Others/Define.h"
#include "../Others/Math3D.h"
#include <math.h>

/*-------------------------------------------*/
/* Lerp
/*-------------------------------------------*/
float Lerp(float a, float b, float s)
{
	float result = ((1.f - s) * a) + (s * b);
	return result;
}

/*-------------------------------------------*/
/* LerpRagian
/*-------------------------------------------*/
float  LerpRadian(float a, float b, float s)
{
	float result;
	float diff = b - a;

	if (diff < -PI)
	{
		b += TWOPI;
		result = Lerp(a, b, s);
		if (result >= TWOPI)
		{
			result -= TWOPI;
		}
	}
	else if (diff > PI)
	{
		b -= TWOPI;
		result = Lerp(a, b, s);
		if (result < 0.f)
		{
			result += TWOPI;
		}
	}
	else
	{
		result = Lerp(a, b, s);
	}
	return result;
}

/*-------------------------------------------*/
/* LerpVector
/*-------------------------------------------*/
VECTOR LerpVector(VECTOR a, VECTOR b, float s)
{
	return VAdd(a,(VScale((VSub(b,a)),s)));
}

/*-------------------------------------------*/
/* Easing : CubicEaseInOut
/*-------------------------------------------*/
float CubicEaseInOut(float t, float s, float e)
{
	float c = e - s;
	if (t < 0.5f)
	{
		t *= 2.0f;
		return c / 2.0f*t*t*t + s;
	}
	t = t * 2 - 2.0f;
	return c / 2.0f * (t*t*t + 2) + s;
}

/*-------------------------------------------*/
/* Easing : QuadraticEaseOut
/*-------------------------------------------*/
float QuadraticEaseOut(float t, float s, float e)
{
	float c = e - s;
	if (t < 0.5f)
	{
		t *= 2.0f;
		return c / 2.0f*t*t + s;
	}
	t = t * 2 - 1;
	return -c / 2.0f * (t*(t - 2) - 1) + s;
}

/*-------------------------------------------*/
/* 回転角から回転行列を取得
/*-------------------------------------------*/
MATRIX GetRotMatrixFromRot(VECTOR rotation)
{
	// 処理結果を保存する行列
	MATRIX result = MGetIdent();

	// X軸の回転行列を取得
	MATRIX rotX = MGetRotX(rotation.x);
	// Y軸の回転行列を取得
	MATRIX rotY = MGetRotY(rotation.y);
	// Z軸の回転行列を取得
	MATRIX rotZ = MGetRotZ(rotation.z);

	// ３つの回転行列を合成する
	result = rotZ * rotX * rotY;

	return result;
}

/*-------------------------------------------*/
/* 指定の軸を基準にベクトルを回転
/*-------------------------------------------*/
VECTOR RotationVector(VECTOR rotVec, VECTOR rotAxis, float angle)
{
	// 指定の軸を基準に指定の角度分回転した行列を取得
	MATRIX rotMatrix = MGetRotAxis(rotAxis, angle);

	// 回転行列をもとに回転したベクトルを返す
	return VTransform(rotVec, rotMatrix);
}

/*-------------------------------------------*/
/* ベクトルの正規化
/*-------------------------------------------*/
VECTOR SafeNormlizeVector(VECTOR vec)
{
	if (VSquareSize(vec) != 0)
	{
		// 正規化されたベクトルを返す
		return VNorm(vec);
	}
	return ZERO_VECTOR;
}

/*-------------------------------------------*/
/* カウントの値が指定された範囲内か
/*-------------------------------------------*/
bool IsCountBetween(int counter, int min, int max)
{
	if (counter >= min && counter <= max)
	{
		return true;
	}
	return false;
}
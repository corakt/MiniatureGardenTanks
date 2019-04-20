#pragma once
#include <DxLib.h>

const float PI = 3.14159265359f;
const float TWOPI = 6.2818530718f;

float  Lerp(float a, float b, float s);								// Lerp
float  LerpRadian(float a, float b, float s);						// LerpRagian
VECTOR LerpVector(VECTOR a, VECTOR b, float s);						// LerpVector

float  CubicEaseInOut(float t, float s, float e);					// Easing : CubicEaseInOut
float  QuadraticEaseOut(float t, float s, float e);					// Easing : QuadraticEaseOut

MATRIX GetRotMatrixFromRot(VECTOR rotation);						// 回転角から回転行列を取得
VECTOR RotationVector(VECTOR rotVec,VECTOR rotAxis,float angle);	// 指定の軸を基準にベクトルを回転

VECTOR SafeNormlizeVector(VECTOR vec);								// ベクトルの正規化

bool   IsCountBetween(int counter, int min, int max);				// カウントの値が指定された範囲内か
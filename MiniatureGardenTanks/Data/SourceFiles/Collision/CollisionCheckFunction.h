#pragma once
#include "../Collision/BoxCollider.h"
#include <DxLib.h>

// ボックス同士の衝突判定
bool IsCollisionBoxAndBox(const VECTOR& center1, const VECTOR& size1, const VECTOR& center2, const VECTOR& size2);

// スフィア同士の衝突判定
bool IsCollisionSphereAndSphere(const VECTOR& center1, const float& radius1, const VECTOR& center2, const float& radius2);

// ボックスとスフィアの衝突判定（ボックスは回転しないため回転を考慮した計算は行わない）
bool IsCollisionBoxAndSphere(const VECTOR& boxCenter,const VECTOR& boxSize,const VECTOR& sphereCenter,const float& sphereRadius);

// スフィアとボックスの頂点との衝突判定
bool IsCollisionSphereAndBoxVertices(const VECTOR& boxCenter, const VECTOR& boxSize, const VECTOR& sphereCenter, const float& sphereRadius);

// スフィアがボックスの中に入っているか
bool InSphereToBox(const VECTOR& boxCenter,const VECTOR& boxSize,const VECTOR& sphereCenter);

// 扇形とボックスの衝突判定
bool IsCollisionCircularSectorAndBox(const VECTOR& csCenter,const VECTOR& csDirection,const float& csWidth,const float& csDistance,const VECTOR& boxCenter,const VECTOR& boxSize);
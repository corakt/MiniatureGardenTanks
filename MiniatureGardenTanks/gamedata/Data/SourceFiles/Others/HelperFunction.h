#pragma once
#include <DxLib.h>

// 指定したボックスの頂点位置を取得
void GetBoxVerticesPosition(VECTOR center, VECTOR size, VECTOR *verticesPosBuffer);

// トランジションエフェクト「フェード」を再生
void PlayTransitionEffectForFade(float playSpeed);

// トランジションエフェクト「戦車移動」を再生
void PlayTransitionEffectForTankMove(float playSpeed);

// トランジションエフェクトが再生中かどうかを判定
bool IsTransitionEffectPlaying();
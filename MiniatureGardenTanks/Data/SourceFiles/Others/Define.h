#pragma once
#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include <algorithm>
#include <string>
#include <functional>

/*-------------------------------------------*/
/* プログラム全体で扱う定数を管理
/*-------------------------------------------*/

static const UINT RESOLUTION_WIDTH       = 1920;		// 横の解像度
static const UINT RESOLUTION_HEIGHT      = 1080;		// 縦の解像度

static const UINT WINDOW_WIDTH           = 1920;		// 横のウィンドウサイズ
static const UINT WINDOW_HEIGHT          = 1080;		// 縦のウィンドウサイズ

static const VECTOR ZERO_VECTOR = VGet(0, 0, 0);		// ゼロベクトル

static const UINT EFFEKSEER_EFFECT_MAXNUM = 10000;		// Effekseerのエフェクトの最大描画数

/*-------------------------------------------*/
/* インライン関数群
/*-------------------------------------------*/
// インスタンスをdeleteしてNULLを入れる
template <typename T>
inline void SafeDelete(T*& p)   // ポインタの参照渡し
{
	delete (p);
	(p) = NULL;
}

// インスタンス配列をdeleteしてNULLを入れる
template <typename T>
inline void SafeDeleteArray(T*& p)
{
	delete[](p);
	(p) = NULL;
}

// モデルハンドルの削除
inline void SafeDeleteModel(int& modelHandle)
{
	MV1DeleteModel(modelHandle);
	modelHandle = NULL;
}

// グラフィックハンドルの削除
inline void SafeDeleteGraph(int& graphHandle)
{
	DeleteGraph(graphHandle);
	graphHandle = NULL;
}

// サウンドハンドルの削除
inline void SafeDeleteSound(int& soundHandle)
{
	DeleteSoundMem(soundHandle);
	soundHandle = NULL;
}

// エフェクトハンドルの削除
inline void SafeDeleteEffect(int& effectHandle)
{
	DeleteEffekseerEffect(effectHandle);
	effectHandle = NULL;
}

// フォントハンドルの削除
inline void SafeDeleteFont(int& fontHandle)
{
	DeleteFontToHandle(fontHandle);
	fontHandle = NULL;
}

// string型からハッシュ値を生成
inline const int CreateHashToString(std::string string)
{
	std::hash<std::string> hash;
	return hash(string);
}

/*-------------------------------------------*/
/* 演算子オーバーロード
/*-------------------------------------------*/
// ベクトル同士の足し算
inline VECTOR operator+(const VECTOR& left, const VECTOR& right)
{
	return VAdd(left, right);
}

// ベクトル同士の引き算
inline VECTOR operator-(const VECTOR& left, const VECTOR& right)
{
	return VSub(left, right);
}

// ベクトル同士の掛け算
inline VECTOR operator*(const VECTOR& left, const float& right)
{
	return VScale(left, right);
}

// 行列同士の掛け算
inline MATRIX operator*(const MATRIX& left, const MATRIX& right)
{

	return MMult(left, right);
}

// int型pairの比較
inline bool operator==(const std::pair<int, int>& left, const std::pair<int, int>& right)
{
	// それぞれの要素で比較する
	if (left.first == right.first && left.second == right.second)
	{
		return true;
	}
	return false;
}

inline bool operator!=(const std::pair<int, int>& left, const std::pair<int, int>& right)
{
	// それぞれの要素で比較する
	if (left.first != right.first && left.second != right.second)
	{
		return true;
	}
	return false;
}
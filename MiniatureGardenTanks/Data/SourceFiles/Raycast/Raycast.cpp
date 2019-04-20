#include "../Raycast/Raycast.h"

/*-------------------------------------------*/
/* コンストラクタ
/*-------------------------------------------*/
Raycast::Raycast(VECTOR origin, VECTOR direction, float distance)
{
	// 引数からの情報を取得
	this->origin    = origin;		// レイの原点
	this->direction = direction;	// レイの向き
	this->distance  = distance;		// レイの距離
}

/*-------------------------------------------*/
/* デストラクタ
/*-------------------------------------------*/
Raycast::~Raycast()
{
	// ヒットしたモデルの情報を格納しているリストを削除
	hitModelInfo.clear();
}

/*-------------------------------------------*/
/* レイにヒットしたモデルの情報をセットする
/*-------------------------------------------*/
void Raycast::SetHitModelInfo(HitModelInfo hitModelInfo)
{
	// ヒットしたモデルの情報をセット
	this->hitModelInfo.push_back(hitModelInfo);
}

/*-------------------------------------------*/
/* モデルの情報を格納しているリストを削除
/*-------------------------------------------*/
void Raycast::ClearHitModelInfo()
{
	hitModelInfo.clear();
}
#pragma once
#include "../BaseObject/GameObject.h"
#include "../BaseObject/IdGenerator.h"

/*-------------------------------------------*/
/* コンストラクタ
/*-------------------------------------------*/
GameObject::GameObject()
{
	// 初期化
	isActive = false;
	objectId = 0;

	// オブジェクトIDを生成
	objectId = ID_GENERATOR.Generate();
}

/*-------------------------------------------*/
/* デストラクタ
/*-------------------------------------------*/
GameObject::~GameObject()
{
	// オブジェクトIDを削除する
	ID_GENERATOR.Delete(objectId);
}
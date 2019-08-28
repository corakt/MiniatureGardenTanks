#pragma once
#include "../BaseObject/Object.h"
#include "../BaseObject/ObjectIdManager.h"

/*-------------------------------------------*/
/* コンストラクタ
/*-------------------------------------------*/
Object::Object()
{
	// 初期化
	isActive = false;
	objectId = 0;

	// オブジェクトIDを生成
	objectId = OBJECTID_MANAGER.CreateId();
}

/*-------------------------------------------*/
/* デストラクタ
/*-------------------------------------------*/
Object::~Object()
{
	// オブジェクトIDを削除する
	OBJECTID_MANAGER.DestroyId(objectId);
}
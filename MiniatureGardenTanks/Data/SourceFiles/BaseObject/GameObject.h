#pragma once
#include "../BaseObject/Transform.h"
#include "../Others/Objecttype.h"
#include <DxLib.h>
#include <string>

/*-------------------------------------------*/
/* ゲームオブジェクトクラス
/* （ハンドルを持たない基本のオブジェクト）
/*-------------------------------------------*/
class GameObject
{
public:
			 GameObject();		// コンストラクタ
	virtual ~GameObject();		// デストラクタ

	// コンポーネントの追加
	template <class T>
	T* AddComponent()
	{
		// コンポーネントを生成して、ポインタを返す
		// 生成元の親オブジェクトを渡す
		return new T(this);
	}

	// タグ
	const ObjectType& GetObjectType() const                 { return objectType; }
	void              SetObjectType(  const ObjectType set) { objectType = set;  }

	// オブジェクトID
	const std::uint32_t& GetObjectId() const { return objectId; }

	// トランスフォーム
	const Transform& GetTransform() const                { return transform; }
	void			 SetTransform(  const Transform set) { transform = set;  }

	// オブジェクトのアクティブフラグ
	const bool& GetActiveFlag () const           { return isActive;  }
	void	    SetActiveFlag (  const bool set) { isActive = set;   }

protected:
	Transform     transform;	// トランスフォーム
	ObjectType    objectType;	// オブジェクトの種類
	std::uint32_t objectId;		// オブジェクトを識別するID
	bool          isActive;		// 機能しているかどうか
};
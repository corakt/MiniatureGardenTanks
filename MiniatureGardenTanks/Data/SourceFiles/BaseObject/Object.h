#pragma once
#include "../BaseObject/Transform.h"
#include <DxLib.h>
#include <string>

/*-------------------------------------------*/
/* オブジェクトクラス
/* （ハンドルを持たない基本のオブジェクト）
/*-------------------------------------------*/
class Object
{
public:
			 Object();		// コンストラクタ
	virtual ~Object();		// デストラクタ

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
	std::uint32_t objectId;		// オブジェクトを識別するID
	bool          isActive;		// 機能しているかどうか
};
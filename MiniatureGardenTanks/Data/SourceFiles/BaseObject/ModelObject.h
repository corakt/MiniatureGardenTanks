#pragma once
#include "../BaseObject/Object.h"
#include "../BaseObject/Transform.h"
#include "../ResourcesManager/ResourceModelManager.h"
#include "../Others/ErrorMessage.h"
#include "../Others/Define.h"

/*-------------------------------------------*/
/* モデルオブジェクトクラス
/* （モデルのベースとなるクラス）
/*-------------------------------------------*/
class ModelObject : public Object
{
public:
			 ModelObject(int modelHandle,ModelType type);	// コンストラクタ
	virtual ~ModelObject();									// デストラクタ

	// モデルのハンドル
	const int& GetHandle() { return modelHandle; }

	// オブジェクトの種類
	const ModelType& GetModelType() const { return modelType; }

	// オブジェクトのメッシュの数
	const int& GetModelMeshNum () const { return modelMeshNum; }
	
	// オブジェクトを描画中か否かのフラグ
	const bool& GetDrawFlag     () const           { return isDraw; }
	void		SetDrawFlag     (  const bool set) { isDraw = set;  }

	void Initialize();			// 初期化
	void Update();				// 更新
	void DrawModel();			// モデルの描画

protected:
	int       modelHandle;		// オブジェクトのハンドル
	ModelType modelType;		// モデルの種類
	int       modelMeshNum;		// オブジェクトに含まれるメッシュの数
	bool      isDraw;			// 描画中かどうか
};
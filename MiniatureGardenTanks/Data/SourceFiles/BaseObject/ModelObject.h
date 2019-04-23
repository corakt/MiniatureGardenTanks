#pragma once
#include "../BaseObject/GameObject.h"
#include "../BaseObject/Transform.h"
#include "../ResourcesManager/ResourceModelManager.h"
#include "../Others/ErrorMessage.h"
#include "../Others/Define.h"

/*-------------------------------------------*/
/* モデルオブジェクトクラス
/* （モデルのベースとなるクラス）
/*-------------------------------------------*/
class ModelObject final : public GameObject
{
public:
			 ModelObject(const GameObject* parentObject);		// コンストラクタ
	virtual ~ModelObject();										// デストラクタ

	// モデルのハンドルを取得
	const int& GetHandle() { return modelHandle; }
	// モデルハンドルをセット
	void SetHandle(const int set)
	{
		// 既にハンドルが格納されていたら削除する
		// 引数にNULLが渡されたら、関数を抜ける
		if (modelHandle != NULL) { SafeDeleteModel(modelHandle); }

		// モデルハンドルのコピーを行う
		modelHandle = MV1DuplicateModel(set);

		// モデルのメッシュの数を取得
		modelMeshNum = MV1GetMeshNum(this->modelHandle);

		// コピーに失敗したら、エラー文を表示
		if (this->modelHandle == -1)
		{
			ErrorData errorData;
			errorData.errorMessage = "モデルのコピーに失敗しました。";
			errorData.detectedErrorClassName = "ModelObject";

			// エラーデータをリストに挿入する
			ERROR_MESSAGE.SetErrorData(errorData);
		}
	}

	// オブジェクトのメッシュの数
	const int& GetModelMeshNum () const { return modelMeshNum; }
	
	// オブジェクトを描画中か否かのフラグ
	const bool& GetDrawFlag     () const           { return isDraw; }
	void		SetDrawFlag     (  const bool set) { isDraw = set;  }

	void Initialize();				// 初期化
	void Update();					// 更新
	void DrawModel();				// モデルの描画

private:
	int               modelHandle;		// オブジェクトのハンドル
	int               modelMeshNum;		// オブジェクトに含まれるメッシュの数
	const GameObject* parentObject;		// コンポーネント元の親オブジェクト
	bool              isDraw;			// 描画中かどうか
};
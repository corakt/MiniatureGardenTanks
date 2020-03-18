#include "../Raycast/RaycastManager.h"
#include "../Raycast/Raycast.h"
#include "../BaseObject/ModelObject.h"
#include "../Others/Define.h"

const int RaycastManager::RAYCAST_MAX = 10;		// レイキャストの最大数

/*-------------------------------------------*/
/* コンストラクタ
/*-------------------------------------------*/
RaycastManager::RaycastManager()
{
	// レイキャストの最大数分メモリを確保
	raycast = new Raycast*[RAYCAST_MAX];
	// メモリをNULLで初期化
	for (int i = 0; i < RAYCAST_MAX; i++)
	{
		raycast[i] = NULL;
	}
}

/*-------------------------------------------*/
/* デストラクタ
/*-------------------------------------------*/
RaycastManager::~RaycastManager()
{
	// レイキャストを削除
	SafeDeleteArray(raycast);
}

/*-------------------------------------------*/
/* レイキャストを追加
/*-------------------------------------------*/
Raycast* RaycastManager::AddRaycast(VECTOR origin, VECTOR direction, float distance)
{
	// ループしながらメモリを確保できる変数を探す
	for (int i = 0; i < RAYCAST_MAX; i++)
	{
		// 既に確保されていれば、次の変数に移る
		if (raycast[i] != NULL) { continue; }
		// 確保されていない変数を見つけた場合、その変数にメモリを確保する
		else
		{
			raycast[i] = new Raycast(origin,direction,distance);

			// レイキャストをリストに追加
			activeRaycast.push_back(raycast[i]);

			// レイキャストのポインタを返す
			return raycast[i];
		}
	}

	// 確保できる変数が見つからなければNULLを返す
	return NULL;
}

/*-------------------------------------------*/
/* レイキャスト対象のモデルをセット
/*-------------------------------------------*/
void RaycastManager::SetTargetModel(ModelObject* model)
{
	// 対象のモデルをセット
	targetModel.push_back(model);
}

/*-------------------------------------------*/
/* 初期化
/*-------------------------------------------*/
void RaycastManager::Initialize()
{
	if (targetModel.empty() == false)
	{
		for (ModelObject*& targetModelElem : targetModel)
		{
			if (targetModelElem != NULL)
			{
				// DXライブラリの衝突判定の準備を行う
				MV1SetupCollInfo(targetModelElem->GetHandle(), -1, 1, 1, 1);
			}
		}
	}
}

/*-------------------------------------------*/
/* 更新
/*-------------------------------------------*/
void RaycastManager::Update()
{
	// 各リストのいずれかが空であれば、処理を終了して関数を抜ける
	if (activeRaycast.empty()) { return; }		// 生成したレイキャスト
	if (targetModel.empty())   { return; }		// 衝突判定の調べる対象のモデル

	for (Raycast*& raycastElem : activeRaycast)
	{
		// NULLだった場合は、スキップして次の要素へ
		if (raycastElem == NULL) { continue; }
		
		// リストに登録されているモデルの情報を削除
		raycastElem->ClearHitModelInfo();

		for (ModelObject*& targetModelElem : targetModel)
		{
			// NULLだった場合は、スキップして次の要素へ
			if (targetModelElem == NULL) { continue; }
			
			// DXライブラリの衝突判定の情報を更新する
			MV1RefreshCollInfo(targetModelElem->GetHandle(), -1);

			// レイキャストにアタッチしているモデルと衝突判定を行おうとしていたら
			// スキップして次の要素へ
			if (raycastElem->attachedModel == targetModelElem) { continue; }

			// モデルとレイがヒットしているか調べる
			MV1_COLL_RESULT_POLY result = MV1CollCheck_Line(targetModelElem->GetHandle(),-1, raycastElem->origin,
				                                            VAdd(raycastElem->origin,VScale(raycastElem->direction, raycastElem->distance)));
			
			// ヒットしたモデルが存在した場合
			if (result.HitFlag)
			{
				// ヒットしたモデルのトランスフォームを取得
				Transform hitModelTrans = targetModelElem->GetTransform();
			
				HitModelInfo hitModelInfo;
				// ヒットしたモデルの情報をセット
				hitModelInfo.hitModel    = targetModelElem;												// ヒットしたモデル
				hitModelInfo.hitPosition = result.HitPosition;											// ヒットした位置
				hitModelInfo.distance    = VSize(VSub(hitModelTrans.position, raycastElem->origin));	// ヒットしたモデルと原点との距離
				
				// モデルの情報をリストにセット
				raycastElem->SetHitModelInfo(hitModelInfo);
			}
		}
	}
}

/*-------------------------------------------*/
/* 終了処理
/*-------------------------------------------*/
void RaycastManager::Finalize()
{
	// リストを削除
	activeRaycast.clear();	// 稼働中のレイキャスト
	targetModel.clear();	// 対象のモデル

	for (int i = 0; i < RAYCAST_MAX; i++)
	{
		// レイキャストを削除
		SafeDelete(raycast[i]);
	}

	if (targetModel.empty() == false)
	{
		for (ModelObject*& targetModelElem : targetModel)
		{
			if (targetModelElem != NULL)
			{
				// DXライブラリの衝突判定の後始末を行う
				MV1TerminateCollInfo(targetModelElem->GetHandle(), -1);
			}
		}
	}
}
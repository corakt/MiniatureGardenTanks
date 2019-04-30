#include "../Stage/TerrainBase.h"
#include "../Others/Define.h"

/*-------------------------------------------*/
/* コンストラクタ
/*-------------------------------------------*/
TerrainBase::TerrainBase()
{
	// 地形のタグを設定
	
}

/*-------------------------------------------*/
/* デストラクタ
/*-------------------------------------------*/
TerrainBase::~TerrainBase()
{
	// 処理なし
}

/*-------------------------------------------*/
/* 共通の初期化パラメータ
/*-------------------------------------------*/
void TerrainBase::InitializeCommonParameter()
{
	// 各変数の初期化
	transform.scale = VGet(3, 3, 3);	// モデルのスケール
	isActive        = true;				// 稼働中かどうか
	terrainModel->SetDrawFlag(true);	// 描画フラグをセット
}

/*-------------------------------------------*/
/* 描画
/*-------------------------------------------*/
void TerrainBase::Draw()
{
	if (isActive)
	{
		// カメラの視界に入っているか調べるボックスのサイズを設定
		static const VECTOR VIEW_CLIP_BOX_SIZE = VGet(2300,2300,2300);

		// 関数の"CheckCameraViewClip_Box"は、正確には2点の座標で表される
		// 透明のボックスがカメラの視界に入っているかどうかで判定している。
		// つまり、地形と同じ位置にその透明なボックスを置くことで
		// 地形がカメラに入っているかどうかを調べることが出来る。

		// ちなみに、ボックスのサイズを地形と同じにしてしまうと
		// 地形が表示されたり、されなかったりで画面端がちらついてしまうので
		// ボックスのサイズは地形よりも大きめに設定している。
		// そのサイズを表している変数が上記の"VIEW_CLIP_BOX_SIZE"である。
		// ちなみに、地形の実際のサイズは1500*1500*1500の正方形である。

		// 地形の上端の座標を算出
		VECTOR rectPos1 = transform.position - (VGet(VIEW_CLIP_BOX_SIZE.x, VIEW_CLIP_BOX_SIZE.y, VIEW_CLIP_BOX_SIZE.z * 0.5f));
		// 地形の下端の座標を算出
		VECTOR rectPos2 = transform.position + (VGet(VIEW_CLIP_BOX_SIZE.x, VIEW_CLIP_BOX_SIZE.y, VIEW_CLIP_BOX_SIZE.z * 0.5f));

		// 地形がカメラの視界の中に存在しているか調べる。
		// 存在している場合にfalseを返す。
		if (CheckCameraViewClip_Box(rectPos1, rectPos2) == false)
		{
			// 視界に存在している地形のみ描画する
			terrainModel->DrawModel();
		}
	}
}
#include "../Others/ErrorMessage.h"
#include <DxLib.h>

/*-------------------------------------------*/
/* コンストラクタ
/*-------------------------------------------*/
ErrorMessage::ErrorMessage()
{
	// 処理なし
}

/*-------------------------------------------*/
/* デストラクタ
/*-------------------------------------------*/
ErrorMessage::~ErrorMessage()
{
	// 全てのエラーデータをクリアする
	allErrorData.clear();
}

/*-------------------------------------------*/
/* 描画
/*-------------------------------------------*/
void ErrorMessage::Draw()
{
	if (allErrorData.empty() == false)
	{
		// 受け取ったエラーメッセージを全て表示する
		for (ErrorData& errorDataElem : allErrorData)
		{
			printfDx("Error:%s,ClassName:%s\n", errorDataElem.errorMessage.c_str(), errorDataElem.detectedErrorClassName.c_str());
		}
	}
}
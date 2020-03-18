#pragma once
#include "../BaseObject/SoundObject.h"
#include "../Others/ErrorMessage.h"
#include "../Others/Define.h"

/*-------------------------------------------*/
/* コンストラクタ
/*-------------------------------------------*/
SoundObject::SoundObject(int soundHandle)
{
	// 引数にNULLが渡されたら、関数を抜ける
	if (soundHandle == NULL) { return; }
	
	// サウンドハンドルのコピーを行う
	this->soundHandle = DuplicateSoundMem(soundHandle);

	// コピーに失敗したら、エラー文を表示
	if (this->soundHandle == -1)
	{
		ErrorData errorData;
		errorData.errorMessage           = "サウンドのコピーに失敗しました。";
		errorData.detectedErrorClassName = "SoundObject";

		// エラーデータをリストに挿入する
		ERROR_MESSAGE.SetErrorData(errorData);
	}
}

/*-------------------------------------------*/
/* デストラクタ
/*-------------------------------------------*/
SoundObject::~SoundObject()
{
	// サウンドの削除
	SafeDeleteSound(soundHandle);
}
#pragma once
#include "../Others/Singleton.h"
#include <list>
#include <string>

struct ErrorData
{
	std::string errorMessage;				// エラーメッセージ
	std::string detectedErrorClassName;		// エラーが検出されたクラスの名前
};

/*-------------------------------------------*/
/* エラーメッセージを表示するクラス
/*（シングルトン）
/*-------------------------------------------*/
class ErrorMessage final : public Singleton<ErrorMessage>
{
	friend Singleton<ErrorMessage>;

public:
	         ErrorMessage();		// コンストラクタ
	virtual ~ErrorMessage();		// デストラクタ

	void Draw();					// 描画

	// エラーデータをセットする
	void SetErrorData(const ErrorData set) { allErrorData.push_back(set); }

private:
	// プログラム内の全てのエラーメッセージ
	std::list<ErrorData> allErrorData;
};

#define ERROR_MESSAGE ErrorMessage::GetInstance()
#pragma once

template <class T>
class Singleton
{
public:
	// インスタンスの取得
	static T& GetInstance()
	{
		static T instance;
		return instance;
	}

protected:
	Singleton()
	{
		// 処理なし
	}
	virtual ~Singleton()
	{
		// 処理なし
	}
};
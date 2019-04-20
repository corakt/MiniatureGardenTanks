#include "../Others/AsyncLoadData.h"
#include <iostream>
#include <fstream>

/*-------------------------------------------*/
/* イメージファイルを読み込む
/*-------------------------------------------*/
int LoadImageFile(const TCHAR *filePath, void **fileImageAddr, int *fileSize)
{
	// ファイルを開く
	std::ifstream fin(filePath, std::ios::in | std::ios::binary);

	// 失敗したら -1 を返す
	if (!fin) return -1;

	// ファイルの末尾を探す
	fin.seekg(0, std::fstream::end);
	// ファイル末尾のインデックス取得
	std::streampos eofPos = fin.tellg();
	// 先頭に戻るためにclearする
	fin.clear();
	// ファイル先頭に戻る
	fin.seekg(0, std::fstream::beg);
	// ファイル先頭のインデックス取得
	std::streampos begPos = fin.tellg();
	// 末尾 - 先頭でファイルサイズを取得
	*fileSize = (int)(eofPos - begPos);

	// メモリの確保
	*fileImageAddr = new char[*fileSize]();

	// ファイルの読み込み
	fin.read((char*)*fileImageAddr, *fileSize);

	// ファイルを閉じる
	fin.close();

	// 成功なら 0 を返す
	return 0;
}

/*-------------------------------------------*/
/* イメージファイル読み込み用コールバック関数
/*-------------------------------------------*/
int LoadImageFileFunc(const TCHAR *filePath, void **fileImageAddr, int *fileSize, void *fileReadFuncData)
{
	return LoadImageFile(filePath, fileImageAddr, fileSize);
}

/*-------------------------------------------*/
/* イメージファイル解放用コールバック関数
/*-------------------------------------------*/
int ReleaseFileFunc(void *memoryAddr, void *fileReadFuncData)
{
	delete(memoryAddr);

	return 0;
}
#pragma once
#include <Windows.h>

// イメージファイルのパラメータ
struct ImageFileParam
{
	void* fileImage;	// ファイルのイメージデータ
	int   fileSize;		// ファイルサイズ
};

// イメージファイルを読み込む
int LoadImageFile(const TCHAR *filePath, void **fileImageAddr, int *fileSize);

// イメージファイル読み込み用コールバック関数
int LoadImageFileFunc(const TCHAR *filePath, void **fileImageAddr, int *fileSize, void *fileReadFuncData);

// イメージファイル解放用コールバック関数
int ReleaseFileFunc(void *memoryAddr, void *fileReadFuncData);
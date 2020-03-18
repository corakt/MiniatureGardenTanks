#pragma once
#include "../BaseObject/Object.h"
#include <DxLib.h>

/*-------------------------------------------*/
/* サウンドオブジェクトクラス
/* （サウンドのベースとなるクラス）
/*-------------------------------------------*/
class SoundObject : public Object
{
public:
			 SoundObject(int soundHandle);	// コンストラクタ
	virtual ~SoundObject();					// デストラクタ

	// サウンドハンドルを取得
	const int&  GetHandle () { return soundHandle; }

	// サウンドを再生
	void Playing(int playType) 
	{
		// 既に再生中の場合は無視
		if (CheckSoundMem(soundHandle)) { return; }

		// 再生
		PlaySoundMem(soundHandle, playType); 
	}

	// サウンドを停止
	void Stop() { StopSoundMem(soundHandle); }

	// サウンドの再生位置を設定
	void SetPlayingPosition(VECTOR position) { Set3DPositionSoundMem(position, soundHandle); }
	
	// サウンドの可聴範囲を設定
	void SetPlayingRadius(float radius) { Set3DRadiusSoundMem(radius, soundHandle); }

	// サウンド再生時の音量
	const int GetPlayVolume()  const           { return GetVolumeSoundMem2(soundHandle); }
	void       SetPlayVolume ( const int set ) { ChangeVolumeSoundMem(set, soundHandle); }

	// 再生中かどうかのフラグを取得
	const bool IsPlaying() { return (bool)CheckSoundMem(soundHandle); }

protected:
	int soundHandle;	// サウンドのハンドル
};
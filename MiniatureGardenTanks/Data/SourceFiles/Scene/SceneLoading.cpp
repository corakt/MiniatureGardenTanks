#include "../Scene/SceneLoading.h"
#include "../Scene/SceneManager.h"
#include "../ResourcesManager/ResourceEffectManager.h"
#include "../Others/Define.h"

const float  SceneLoading::CHAR_JUMP_POWER    = 10;					// 文字のジャンプ力
const int    SceneLoading::CHAR_JUMP_INTERVAL = 5;					// 文字のジャンプ間隔
const float  SceneLoading::CHAR_GRAVITY       = 1;					// 文字の重力
const int    SceneLoading::STRING_LENGTH      = 10;					// 文字列の長さ
const VECTOR SceneLoading::STRING_CENTER_POS  = VGet(1000,900,0);	// 文字列の基準位置

/*-------------------------------------------*/
/* 非同期読み込み用関数
/*-------------------------------------------*/
void AsyncLoadResourceFromImageFile(AsyncLoadData *asyncLoadData)
{
	// モデルのイメージファイルをメモリに読み込む
	for (int i = 0; i < (int)ModelType::TYPE_NUM; i++)
	{
		LoadImageFile(MODEL_MANAGER.GetFilePath((ModelType)i).c_str(),		// ファイルのパス
					  &asyncLoadData->modelFileParam[i].fileImage,			// イメージファイル格納先アドレス
					  &asyncLoadData->modelFileParam[i].fileSize);			// ファイルサイズ格納先アドレス
	}

	// スプライトのイメージファイルをメモリに読み込む
	for (int i = 0; i < (int)SpriteType::TYPE_NUM; i++)
	{
		LoadImageFile(SPRITE_MANAGER.GetFilePath((SpriteType)i).c_str(),	// ファイルのパス
					  &asyncLoadData->spriteFileParam[i].fileImage,			// イメージファイル格納先アドレス
					  &asyncLoadData->spriteFileParam[i].fileSize);			// ファイルサイズ格納先アドレス
	}

	// サウンドのイメージファイルをメモリに読み込む
	for (int i = 0; i < (int)SoundType::TYPE_NUM; i++)
	{
		LoadImageFile(SOUND_MANAGER.GetFilePath((SoundType)i).c_str(),		// ファイルのパス
					  &asyncLoadData->soundFileParam[i].fileImage,			// イメージファイル格納先アドレス
					  &asyncLoadData->soundFileParam[i].fileSize);			// ファイルサイズ格納先アドレス
	}

	// スレッドを終了する
	ExitThread(0);
}

/*-------------------------------------------*/
/* コンストラクタ
/*-------------------------------------------*/
SceneLoading::SceneLoading()
{
	
}

/*-------------------------------------------*/
/* デストラクタ
/*-------------------------------------------*/
SceneLoading::~SceneLoading()
{
	
}

/*-------------------------------------------*/
/* インスタンスの生成
/*-------------------------------------------*/
void SceneLoading::CreateObject()
{
	// スレッド用パラメータ
	HANDLE threadHandle;
	DWORD  id;
	DWORD  exitCode;
	AsyncLoadData *asyncLoadData;

	asyncLoadData = NULL;
	asyncLoadData = new AsyncLoadData();

	// スレッドを作成
	threadHandle = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)AsyncLoadResourceFromImageFile, asyncLoadData, 0, &id);

	Initialize();
	do {
		// ロード画面の描画
		ClearDrawScreen();
		{
			Update();
			Draw();
		}
		ScreenFlip();
		clsDx();

		// スレッドの状態を調べる
		GetExitCodeThread(threadHandle, &exitCode);
	}
	// 読み込が終わったら進む
	while (ProcessMessage() == 0 && exitCode != 0);

	// メモリ内のイメージファイルからモデルを展開する
	MODEL_MANAGER.LoadAllModel(asyncLoadData->modelFileParam);
	
	// メモリ内のイメージファイルからスプライトを展開する
	SPRITE_MANAGER.LoadAllSprite(asyncLoadData->spriteFileParam);

	// サウンドの初期化処理
	SOUND_MANAGER.Initialize();
	// メモリ内のイメージファイルからサウンドを展開する
	SOUND_MANAGER.LoadAllSound(asyncLoadData->soundFileParam);

	// エフェクトを読み込む
	EFFECT_MANAGER.LoadAllEffect();

	// メモリ内のモデルのイメージファイルを削除する
	for (int i = 0; i < (int)ModelType::TYPE_NUM; i++)
	{
		SafeDelete(asyncLoadData->modelFileParam[i].fileImage);
	}

	// メモリ内のスプライトのイメージファイルを削除する
	for (int i = 0; i < (int)SpriteType::TYPE_NUM; i++)
	{
		SafeDelete(asyncLoadData->spriteFileParam[i].fileImage);
	}

	// メモリ内のサウンドのイメージファイルを削除する
	for (int i = 0; i < (int)SoundType::TYPE_NUM; i++)
	{
		SafeDelete(asyncLoadData->soundFileParam[i].fileImage);
	}

	// イメージファイルを削除してメモリを解放する
	SafeDelete(asyncLoadData);

	// 次のシーンをタイトルにセットする
	SceneManager::SetNextScene(SceneManager::SceneType::TITLE);
}

/*-------------------------------------------*/
/* インスタンスの削除
/*-------------------------------------------*/
void SceneLoading::DestroyObject()
{
	for (int i = 0; i < STRING_LENGTH; i++)
	{
		// グラフィックを削除する
		SafeDeleteGraph(tankAnimation.sprite[i]);
	}
	// メモリを解放
	SafeDelete(tankAnimation.sprite);
	SafeDeleteArray(nowLoadingString);

	// フォントを削除
	SafeDeleteFont(fontHandle);
	// システム内のフォントを削除
	RemoveFontResource("genkai-mincho.ttf");
}

/*-------------------------------------------*/
/* 初期化
/*-------------------------------------------*/
void SceneLoading::Initialize()
{
	// 戦車のアニメーションのパラメータを設定する
	tankAnimation.position       = VGet(1600, 900, 0);		// 表示座標
	tankAnimation.size           = 1;						// 描画サイズ
	tankAnimation.angle          = 0;						// 描画角度
	tankAnimation.speed          = 0.8f;					// 描画スピード
	tankAnimation.animPatternNum = 20;						// アニメパターン数

	// アニメーションのパターン数だけメモリを確保
	tankAnimation.sprite = new int[tankAnimation.animPatternNum];

	// カウントを初期化
	tankAnimation.animCount = 0;

	// 画像分割用パラメータ
	const int DIV_NUM_X  = 5;		// 横分割数
	const int DIV_NUM_Y  = 4;		// 縦分割数
	const int DIV_SIZE_X = 300;		// 横分割サイズ
	const int DIV_SIZE_Y = 200;		// 縦分割サイズ

	// アニメーション用の画像を読み込む
	LoadDivGraph("Data/ResourceFiles/Sprites/Loading/Tank_Anim.png",	// ファイルへのパス
				 DIV_NUM_X*DIV_NUM_Y,									// 画像の総分割数
				 DIV_NUM_X,												// 横分割数
				 DIV_NUM_Y,												// 縦分割数
				 DIV_SIZE_X,											// 横分割サイズ
				 DIV_SIZE_Y,											// 縦分割サイズ
				 tankAnimation.sprite);									// 画像ハンドル

	// システム内にフォントを追加
	AddFontResourceEx("genkai-mincho.ttf", FR_PRIVATE, NULL);
	// DXライブラリでフォントを生成
	fontHandle = CreateFontToHandle("源界明朝", 36, 4, DX_FONTTYPE_ANTIALIASING);

	// 文字列の長さだけメモリを確保
	nowLoadingString = new CharData[STRING_LENGTH];

	// 文字を代入していく
	nowLoadingString[0].character = "N";	// N
	nowLoadingString[1].character = "o";	// o
	nowLoadingString[2].character = "w";	// w
	nowLoadingString[3].character = "L";	// L
	nowLoadingString[4].character = "o";	// o
	nowLoadingString[5].character = "a";	// a
	nowLoadingString[6].character = "d";	// d
	nowLoadingString[7].character = "i";	// i
	nowLoadingString[8].character = "n";	// n
	nowLoadingString[9].character = "g";	// g

	// 文字のパラメータを初期化
	for (int i = 0; i < STRING_LENGTH; i++)
	{
		nowLoadingString[i].color            = GetColor(0, 0, 0);									// 文字の色
		nowLoadingString[i].position         = STRING_CENTER_POS + VGet(40 * (float)i,0, 0);		// 文字の座標
		nowLoadingString[i].isJump           = false;												// ジャンプ中かどうか
		nowLoadingString[i].currentJumpPower = 0;													// 現在のジャンプ力
	}
	charJumpIntervalCount = 0;		// ジャンプした瞬間のY軸位置
	jumpMomentPosY        = 0;		// ジャンプ間隔カウント
}

/*-------------------------------------------*/
/* 更新
/*-------------------------------------------*/
void SceneLoading::Update()
{
	// 文字のジャンプ処理
	charJumpIntervalCount++;

	for (int i = 0; i < STRING_LENGTH; i++)
	{
		if ((charJumpIntervalCount - (i * 5)) % 60 == 1)
		{
			// ジャンプフラグを立てる
			nowLoadingString[i].isJump = true;
			// Y軸位置を保存
			jumpMomentPosY = nowLoadingString[i].position.y;
			// ジャンプ力をセット
			nowLoadingString[i].currentJumpPower = CHAR_JUMP_POWER;

		}
		if (nowLoadingString[i].isJump)
		{
			// ジャンプ力を減衰させていく
			nowLoadingString[i].currentJumpPower -= CHAR_GRAVITY;
			// 文字をジャンプさせる
			nowLoadingString[i].position.y -= nowLoadingString[i].currentJumpPower;

			if (nowLoadingString[i].position.y > jumpMomentPosY)
			{
				// ジャンプフラグを倒す
				nowLoadingString[i].isJump = false;
				// 各パラメータをリセット
				nowLoadingString[i].currentJumpPower = 0;
				nowLoadingString[i].position.y = STRING_CENTER_POS.y;
			}
		}
	}
}

/*-------------------------------------------*/
/* 描画
/*-------------------------------------------*/
void SceneLoading::Draw()
{
	// 白い背景を描画
	DrawBox(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, GetColor(255, 255, 255), TRUE);

	// 戦車のアニメーションを再生
	tankAnimation.animCount += tankAnimation.speed;

	// 現在のカウント数がパターンの枚数を上回ったら、０にリセット
	if (tankAnimation.animCount > tankAnimation.animPatternNum)
	{
		tankAnimation.animCount = 0;
	}
	
	// 戦車のイラストを描画
	DrawRotaGraphF(tankAnimation.position.x,								// X軸の位置をセット
				   tankAnimation.position.y,								// Y軸の位置をセット
				   tankAnimation.size,										// スケールをセット
				   tankAnimation.angle,										// 回転角をセット
				   tankAnimation.sprite[(int)tankAnimation.animCount],		// 描画する画像のハンドルをセット
				   TRUE);													// 透過部分を透過する


	// 文字列の描画
	for (int i = 0; i < STRING_LENGTH; i++)
	{
		DrawStringToHandle((int)nowLoadingString[i].position.x,				// X軸の位置をセット
						   (int)nowLoadingString[i].position.y, 			// Y軸の位置をセット
						   nowLoadingString[i].character,					// 描画する文字をセット
						   nowLoadingString[i].color,						// 文字色をセット
						   fontHandle);										// 描画する文字のハンドルをセット
	}
}
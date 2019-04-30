#include "../Character/CharacterPlayer.h"
#include "../ResourcesManager/ResourceModelManager.h"
#include "../ResourcesManager/ResourceSpriteManager.h"
#include "../ResourcesManager/ResourceEffectManager.h"
#include "../Raycast/Raycast.h"
#include "../Others/Math3D.h"
#include "../Others/Define.h"
#include "../Collision/BoxCollider.h"

/*-------------------------------------------*/
/* コンストラクタ
/*-------------------------------------------*/
CharacterPlayer::CharacterPlayer(UINT id, CharacterType type)
	:CharacterBase::CharacterBase(id,type)
{
	// NULLで初期化
	laser = NULL;

	// キャラクターのオブジェクトの種類を設定
	objectType = ObjectType::CHARACTER_PLAYER;

	// レーザーのモデルオブジェクトを生成
	laser = AddComponent<ModelObject>();
	// オブジェクトの種類を設定
	laser->SetObjectType(ObjectType::CHARACTER_LASER);
	// モデルの種類を
	//laser = new ModelObject(MODEL_MANAGER.GetHandle(ResourceModelManager::ModelType::TANK_LASER),ModelType::TANK_LASER);
}

/*-------------------------------------------*/
/* デストラクタ
/*-------------------------------------------*/
CharacterPlayer::~CharacterPlayer()
{
	// モデル：レーザーで使ったメモリを解放
	SafeDelete(laser);
}

/*-------------------------------------------*/
/* 初期化
/*-------------------------------------------*/
void CharacterPlayer::Initialize()
{
	// 共通のパラメータを初期化
	InitializeCommonParameter();

	// エフェクトの初期化
	effects.reloadGauge = -1;		// リロードゲージ

	// モデル：レーザーの初期化処理
	laser->Initialize();			// トランスフォーム
	laser->SetActiveFlag(true);		// 稼働フラグ
	laser->SetDrawFlag(true);		// 描画フラグ
}

/*-------------------------------------------*/
/* 更新
/*-------------------------------------------*/
void CharacterPlayer::Update()
{
	// 共通のパラメータを更新
	UpdateCommonParameter();
}

/*-------------------------------------------*/
/* 描画
/*-------------------------------------------*/
void CharacterPlayer::Draw()
{
	// 共通のモデルを描画
	DrawCommonModel();
}

/*-------------------------------------------*/
/* コールバック関数：砲弾を発射したとき
/*-------------------------------------------*/
void CharacterPlayer::OnFiringShot()
{
	// 砲塔のトランスフォームを取得
	Transform turretTrans = tank[PartsType::TURRET]->GetTransform();

	// 再生位置をセット
	sounds.shotFiring->SetPlayingPosition(turretTrans.position);
	// 砲弾の発射音を再生
	sounds.shotFiring->Playing(DX_PLAYTYPE_BACK);

	// エフェクトを再生
	PlayEffectForFiringExplosion();		// ショット発射時の爆発
}

/*-------------------------------------------*/
/* コールバック関数：砲弾がキャラクターにヒットしたとき
/*-------------------------------------------*/
void CharacterPlayer::OnCharacterHitShot()
{
	// ショットがヒットした際の”HIT”ロゴを描画
	isDrawHitLogo = true;
}

/*-------------------------------------------*/
/* UIの描画
/*-------------------------------------------*/
void CharacterPlayer::DrawUi()
{
	// 各UIの表示
	DrawHpGauge();			// HPゲージ
	DrawActiveGauge();		// アクティブゲージ
	DrawHitLogo();			// 攻撃ヒット時のロゴ
	DrawReloadGauge();		// リロード中のゲージ
	DrawSkillCutin();		// スキル発動時のカットイン
	DrawLaser();			// レーザーを描画
	DrawSightUi();			// 砲弾着弾地点の照準器
}

/*-------------------------------------------*/
/* スキルを発動
/*-------------------------------------------*/
void CharacterPlayer::ActivateSkill()
{
	// カットイン表示フラグを立てる
	isDrawCutin = true;
	//キャラクターの状態を表すステートを変更する
	state = State::SKILL_ACTIVE;
}

/*-------------------------------------------*/
/* HPゲージの表示
/*-------------------------------------------*/
void CharacterPlayer::DrawHpGauge()
{
	// スプライトハンドルを取得
	static int spriteHpGauge      = SPRITE_MANAGER.GetHandle(ResourceSpriteManager::SpriteType::HPGAUGE);			// HPゲージ
	static int spriteHpGaugeFrame = SPRITE_MANAGER.GetHandle(ResourceSpriteManager::SpriteType::HPGAUGE_FRAME);		// HPゲージのフレーム

	// HPゲージ用ローカル変数
	static const float  GAUGE_STARTPERCENT = 12.5f;				// ゲージの開始地点のパーセント
	static const float  GAUGE_ENDPERCENT   = 37.5f;				// ゲージの終了地点のパーセント
	static const VECTOR GAUGEPOS_OFFSET    = VGet(190,190,0);	// ゲージ表示位置のオフセット

	// ゲージの最大長さを算出
	const float GAUGE_LENGTH_MAX = GAUGE_ENDPERCENT - GAUGE_STARTPERCENT;

	// HPの残量から現在のゲージの長さを算出
	const float GAUGE_CURRENT_LENGTH = GAUGE_LENGTH_MAX * (float)hitPoint / HITPOINT_MAX;

	// ゲージが減った分の長さを算出
	const float GAUGE_DECREASED_LENGTH = GAUGE_LENGTH_MAX - GAUGE_CURRENT_LENGTH;

	// ゲージの中身の部分を描画
	DrawCircleGauge((RESOLUTION_WIDTH  / 2) + (int)GAUGEPOS_OFFSET.x,
					(RESOLUTION_HEIGHT / 2) + (int)GAUGEPOS_OFFSET.y,
					(double)GAUGE_ENDPERCENT,
					spriteHpGauge,
					(double)(GAUGE_STARTPERCENT + GAUGE_DECREASED_LENGTH));

	// ゲージのフレーム部分を描画
	DrawRotaGraph((RESOLUTION_WIDTH  / 2) + (int)GAUGEPOS_OFFSET.x,
				  (RESOLUTION_HEIGHT / 2) + (int)GAUGEPOS_OFFSET.y,1,0,
				  spriteHpGaugeFrame,TRUE);
}

/*-------------------------------------------*/
/* アクティブゲージの表示
/*-------------------------------------------*/
void CharacterPlayer::DrawActiveGauge()
{
	// スプライトハンドルを取得
	static int spriteApGauge      = SPRITE_MANAGER.GetHandle(ResourceSpriteManager::SpriteType::APGAUGE);			// APゲージ
	static int spriteApGaugeFrame = SPRITE_MANAGER.GetHandle(ResourceSpriteManager::SpriteType::APGAUGE_FRAME);		// APゲージのフレーム

	// HPゲージ用ローカル変数
	static const int    FILTERPARAM_ADDSUB = 4;					// フィルターパラメータの増減量
	static const int    FLASH_TIME         = 120;				// 点滅時間
	static const float  GAUGE_STARTPERCENT = 62.5f;				// ゲージの開始地点のパーセント
	static const float  GAUGE_ENDPERCENT   = 87.5f;				// ゲージの終了地点のパーセント
	static const VECTOR GAUGEPOS_OFFSET    = VGet(190,190,0);	// ゲージ表示位置のオフセット

	// ゲージの最大長さを算出
	const float GAUGE_LENGTH_MAX = GAUGE_ENDPERCENT - GAUGE_STARTPERCENT;

	// アクティブポイントの量から現在のゲージの長さを算出
	const float GAUGE_CURRENT_LENGTH = GAUGE_LENGTH_MAX * (float)activePoint / ACTIVEPOINT_MAX;

	// アクティブゲージが満タンのときは、ゲージを点滅させる
	if (state == State::ACTIVEGAUGE_FULL)
	{
		counters.apGauge++;
		if (counters.apGauge <= FLASH_TIME / 2)
		{
			counters.apGaugeFilterParam += FILTERPARAM_ADDSUB;
		}
		if (counters.apGauge >= FLASH_TIME / 2)
		{
			counters.apGaugeFilterParam -= FILTERPARAM_ADDSUB;
		}
		if (counters.apGauge == FLASH_TIME + 1)
		{
			counters.apGauge            = 0;
			counters.apGaugeFilterParam = 0;
		}
	}
	// スキル未使用時はカウンターを初期化
	else
	{
		counters.apGauge = 0;
		counters.apGaugeFilterParam = 0;
	}
	

	// ゲージの中身の部分を描画
	DrawCircleGauge((RESOLUTION_WIDTH  / 2) - (int)GAUGEPOS_OFFSET.x,
					(RESOLUTION_HEIGHT / 2) + (int)GAUGEPOS_OFFSET.y,
					(double)(GAUGE_STARTPERCENT + GAUGE_CURRENT_LENGTH),
					spriteApGauge,
					(double)GAUGE_STARTPERCENT);
	
	SetDrawBlendMode(DX_BLENDMODE_ADD_X4, counters.apGaugeFilterParam);
	// フィルター用のゲージを描画させる
	DrawCircleGauge((RESOLUTION_WIDTH  / 2) - (int)GAUGEPOS_OFFSET.x,
					(RESOLUTION_HEIGHT / 2) + (int)GAUGEPOS_OFFSET.y,
					(double)(GAUGE_STARTPERCENT + GAUGE_CURRENT_LENGTH),
					spriteApGauge,
					(double)GAUGE_STARTPERCENT);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);

	// ゲージのフレームの部分を描画
	DrawRotaGraph((RESOLUTION_WIDTH  / 2) - (int)GAUGEPOS_OFFSET.x,
				  (RESOLUTION_HEIGHT / 2) + (int)GAUGEPOS_OFFSET.y,1,0,
				  spriteApGaugeFrame,TRUE);
}

/*-------------------------------------------*/
/* 攻撃ヒット時のロゴを描画
/*-------------------------------------------*/
void CharacterPlayer::DrawHitLogo()
{
	// スプライトハンドルを取得
	static int spriteHitLogo = SPRITE_MANAGER.GetHandle(ResourceSpriteManager::SpriteType::HITLOGO);	// 攻撃ヒット時のロゴ

	// ヒットロゴ描画用ローカル変数
	static const int BLENDPARAM_ADDSUB  = 15;		// ブレンドパラメータの増減量
	static const int LOGO_FADEIN_TIME   = 20;		// ロゴのフェードイン時間
	static const int LOGO_FADEOUT_TIME  = 20;		// ロゴのフェードアウト時間
	static const int LOGO_DRAW_TIME     = 20;		// ロゴの描画時間

	if (isDrawHitLogo)
	{
		counters.hitLogo++;
		// フェードインで描画を始める
		if (counters.hitLogo <= LOGO_FADEIN_TIME)
		{
			counters.hitLogoBlendParam += BLENDPARAM_ADDSUB;
		}
		// 指定の時間描画し続ける
		if (counters.hitLogo >= (LOGO_FADEIN_TIME+ LOGO_FADEOUT_TIME))
		{
			counters.hitLogoBlendParam -= BLENDPARAM_ADDSUB;
		}
		// フェードアウトでロゴを消していく
		if (counters.hitLogo == (LOGO_FADEIN_TIME + LOGO_FADEOUT_TIME + LOGO_DRAW_TIME))
		{
			// カウンターとフラグを初期化する
			counters.hitLogoBlendParam = 0;
			counters.hitLogo = 0;
			isDrawHitLogo = false;
		}

		// アルファブレンドを適用して、ロゴを描画する
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, counters.hitLogoBlendParam);
		DrawRotaGraph(960,670,0.3f,0, spriteHitLogo,TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}

/*-------------------------------------------*/
/* 照準UIの表示
/*-------------------------------------------*/
void CharacterPlayer::DrawSightUi()
{
	// スプライトハンドルを取得
	static int spriteTargetSight = SPRITE_MANAGER.GetHandle(ResourceSpriteManager::SpriteType::TARGETSIGHT);	// 照準器

	// 着弾地点の座標をスクリーン座標に変換
	VECTOR screenImpactPos = ConvWorldPosToScreenPos(impactPosition);

	// 砲塔のトランスフォームを取得
	Transform turretTrans = tank[PartsType::TURRET]->GetTransform();
	// 着弾地点の距離を取得
	float impactPosDist = VSize(impactPosition - turretTrans.position);

	// 着弾地点の距離を０～１に丸め込む
	impactPosDist = impactPosDist / 30000.0f;

	// 照準の枠を描画
	SetDrawBright(200,50,50);
	DrawRotaGraphF(screenImpactPos.x, screenImpactPos.y, 1.0f-impactPosDist, 0, spriteTargetSight, TRUE);
	SetDrawBright(255, 255, 255);
}

/*-------------------------------------------*/
/* レーザー描画
/*-------------------------------------------*/
void CharacterPlayer::DrawLaser()
{
	// パラメータ
	static VECTOR POS_OFFSET   = VGet(0, -100, 0);		// 描画位置のオフセット値
	static int    BLEND_PARAM  = 255;					// ブレンド値
	static float  OPACITY_RATE = 0.5f;					// 透過値

	// モデル：レーザーのトランスフォームを取得
	Transform laserTrans = laser->GetTransform();

	// 描画位置を砲塔の先端部分＋オフセット値に設定
	laserTrans.position = turretTipPosition + POS_OFFSET;
	// モデルの回転角を砲塔の回転角に合わせる
	laserTrans.rotation = turretTransform.rotation;
	// モデルの向きをレイキャストの向きに合わせる
	laserTrans.direction = turretDirRaycast->direction;
	// モデルの伸びるサイズをショットの着弾位置までの距離に設定
	laserTrans.scale.z = VSize(impactPosition - turretTransform.position);
	// 描画する透過値を設定
	MV1SetOpacityRate(laser->GetHandle(), OPACITY_RATE);

	// モデルを加算ブレンドで描画
	MV1SetMaterialDrawBlendMode(laser->GetHandle(), 0, DX_BLENDMODE_ADD);
	MV1SetMaterialDrawBlendParam(laser->GetHandle(), 0, BLEND_PARAM);

	// モデル：レーザーのトランスフォームをセット
	laser->SetTransform(laserTrans);

	// モデルのレーザーを描画
	laser->DrawModel();
}

/*------------------------------------------ - */
/* リロードゲージの表示
/*-------------------------------------------*/
void CharacterPlayer::DrawReloadGauge()
{
	// リロードゲージ再生用ローカル変数
	static const VECTOR EFFECT_SCALE = VGet(0.3f, 0.3f, 0.3f);		// エフェクトのスケール

	if (isReload)
	{
		// エフェクトが既に再生中じゃなければ
		if (IsEffekseer2DEffectPlaying(effects.reloadGauge) == -1)
		{
			// Effekseerに再生するエフェクトをセット
			effects.reloadGauge = PlayEffekseer2DEffect(EFFECT_MANAGER.GetHandle(ResourceEffectManager::EffectType::SHOT_RELOADGAUGE));
			// エフェクトのスケールを調整する
			SetScalePlayingEffekseer2DEffect(effects.reloadGauge, EFFECT_SCALE.x, EFFECT_SCALE.y, EFFECT_SCALE.z);;
		}


		// 着弾地点の座標をスクリーン座標に変換
		VECTOR screenImpactPos = ConvWorldPosToScreenPos(impactPosition);

		// エフェクトの再生位置を照準UIの周りにくるように調整
		SetPosPlayingEffekseer2DEffect(effects.reloadGauge, screenImpactPos.x, screenImpactPos.y, 0);
	}
}

/*-------------------------------------------*/
/* スキル使用時のカットイン描画
/*-------------------------------------------*/
void CharacterPlayer::DrawSkillCutin()
{
	// スプライトハンドルを取得
	static int spriteSlillCutin = SPRITE_MANAGER.GetHandle(ResourceSpriteManager::SpriteType::SKILLCUTIN);	// 照準器

	if (isDrawCutin)
	{
		// 真ん中手前まで横スクロール
		if (IsCountBetween((int)cutinSpritePos.x, 960 + 100, 2300))
		{
			cutinSpritePos.x -= 120;
		}
		// スクロールのスピードを遅くする
		if (IsCountBetween((int)cutinSpritePos.x, 960 - 100, 960 + 99))
		{
			cutinSpritePos.x -= 2;
		}
		// スクロールスピードをもとに戻す
		if (IsCountBetween((int)cutinSpritePos.x, -600, 960 - 99))
		{
			cutinSpritePos.x -= 120;
		}

		// カットイン描画
		DrawRotaGraph((int)cutinSpritePos.x, (int)cutinSpritePos.y, 1, 0, spriteSlillCutin, TRUE);

		if (cutinSpritePos.x < -600)
		{
			isDrawCutin = false;
		}
	}
	else
	{
		cutinSpritePos = VGet(2300, 540, 0);
	}
}
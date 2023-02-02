#include "stdafx.h"
#include "GravityGauge.h"
#include "Player.h"

namespace
{
	const float POS_X = -740.0f;					// X座標
	const float POS_Y = 330.0f;						// Y座標
	const float SCALE = 0.59f;						// 拡大率
	const float WIDTH = 512.0f;						// 横幅
	const float HEIGHT = 512.0f;					// 縦幅
	const float DECREASE_PER_FRAME = 2.0f;			// フレームごとの減少量
	const float INCREASE_PER_FRAME = 5.0f;			// フレームごとの増加量
	const float MAX_ANGLE_DEG_TO_DISPLAY = 360.0f;	// 表示する最大角度(Degree)
	const float MIN_ANGLE_DEG_TO_DISPLAY = 0.0f;	// 表示する最小角度(Degree)
}

bool GravityGauge::Start()
{
	// スプライトの初期化データ
	SpriteInitData spriteInitData;
	spriteInitData.m_ddsFilePath[0] = "Assets/sprite/gravityGauge/gauge2.DDS";
	spriteInitData.m_fxFilePath = "Assets/shader/gravityGauge.fx";
	spriteInitData.m_width = WIDTH;
	spriteInitData.m_height = HEIGHT;
	spriteInitData.m_expandConstantBuffer = &m_displayAreaRad;
	spriteInitData.m_expandConstantBufferSize = sizeof(m_displayAreaRad);
	spriteInitData.m_alphaBlendMode = AlphaBlendMode_Trans;
	// 初期化データをもとにスプライトを初期化する。
	m_gaugeSprite.InitSpecialImage(spriteInitData);
	m_gaugeSprite.SetPosition(Vector3{ POS_X, POS_Y, 0.0f });
	m_gaugeSprite.SetScale(Vector3{ SCALE,SCALE,1.0f });

	m_gaugeFrameSprite.Init(
		"Assets/sprite/gravityGauge/frame.DDS",
		512.0f,
		512.0f
	);
	m_gaugeFrameSprite.SetPosition(Vector3{ POS_X, POS_Y, 0.0f });
	m_gaugeFrameSprite.SetScale(Vector3{ SCALE,SCALE,1.0f });

	m_player = FindGO<Player>("player");

	return true;
}

void GravityGauge::Update()
{
	CalcDisplayArea(m_player->IsFloating());

	m_gaugeSprite.Update();
	m_gaugeFrameSprite.Update();
}

void GravityGauge::CalcDisplayArea(bool isFloating)
{
	// 
	float displayAreaDeg = Math::RadToDeg(m_displayAreaRad);
	if (isFloating) {
		displayAreaDeg -= DECREASE_PER_FRAME;
		if (displayAreaDeg < MIN_ANGLE_DEG_TO_DISPLAY) {
			displayAreaDeg = MIN_ANGLE_DEG_TO_DISPLAY;
		}
	}
	else {
		displayAreaDeg += INCREASE_PER_FRAME;
		if (displayAreaDeg >= MAX_ANGLE_DEG_TO_DISPLAY) {
			displayAreaDeg = MAX_ANGLE_DEG_TO_DISPLAY;
		}
	}
	m_displayAreaRad = Math::DegToRad(displayAreaDeg);
}

void GravityGauge::Render(RenderContext& rc)
{
	m_gaugeFrameSprite.Draw(rc);
	m_gaugeSprite.Draw(rc);
}

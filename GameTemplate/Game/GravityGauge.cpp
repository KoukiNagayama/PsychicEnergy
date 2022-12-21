#include "stdafx.h"
#include "GravityGauge.h"
#include "Player.h"

namespace
{
	const float POS_X = -820.0f;
	const float POS_Y = 410.0f;
	const float SCALE = 0.59f;
}

bool GravityGauge::Start()
{
	// スプライトの初期化データ
	SpriteInitData spriteInitData;
	spriteInitData.m_ddsFilePath[0] = "Assets/sprite/gravityGauge/gauge.DDS";
	spriteInitData.m_fxFilePath = "Assets/shader/gravityGauge.fx";
	spriteInitData.m_width = 512.0f;
	spriteInitData.m_height = 512.0f;
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
	float displayAreaDeg = Math::RadToDeg(m_displayAreaRad);
	if (isFloating) {
		displayAreaDeg -= 0.2f;
		if (displayAreaDeg < 0.0f) {
			displayAreaDeg = 0.0f;
		}
	}
	else {
		displayAreaDeg += 5.0f;
		if (displayAreaDeg >= 360.0f) {
			displayAreaDeg = 360.0f;
		}
	}
	m_displayAreaRad = Math::DegToRad(displayAreaDeg);
}

void GravityGauge::Render(RenderContext& rc)
{
	m_gaugeFrameSprite.Draw(rc);
	m_gaugeSprite.Draw(rc);
}

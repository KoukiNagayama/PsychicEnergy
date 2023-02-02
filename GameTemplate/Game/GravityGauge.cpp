#include "stdafx.h"
#include "GravityGauge.h"
#include "Player.h"

namespace
{
	const float POS_X = -740.0f;					// X���W
	const float POS_Y = 330.0f;						// Y���W
	const float SCALE = 0.59f;						// �g�嗦
	const float WIDTH = 512.0f;						// ����
	const float HEIGHT = 512.0f;					// �c��
	const float DECREASE_PER_FRAME = 2.0f;			// �t���[�����Ƃ̌�����
	const float INCREASE_PER_FRAME = 5.0f;			// �t���[�����Ƃ̑�����
	const float MAX_ANGLE_DEG_TO_DISPLAY = 360.0f;	// �\������ő�p�x(Degree)
	const float MIN_ANGLE_DEG_TO_DISPLAY = 0.0f;	// �\������ŏ��p�x(Degree)
}

bool GravityGauge::Start()
{
	// �X�v���C�g�̏������f�[�^
	SpriteInitData spriteInitData;
	spriteInitData.m_ddsFilePath[0] = "Assets/sprite/gravityGauge/gauge2.DDS";
	spriteInitData.m_fxFilePath = "Assets/shader/gravityGauge.fx";
	spriteInitData.m_width = WIDTH;
	spriteInitData.m_height = HEIGHT;
	spriteInitData.m_expandConstantBuffer = &m_displayAreaRad;
	spriteInitData.m_expandConstantBufferSize = sizeof(m_displayAreaRad);
	spriteInitData.m_alphaBlendMode = AlphaBlendMode_Trans;
	// �������f�[�^�����ƂɃX�v���C�g������������B
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

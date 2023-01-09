#include "stdafx.h"
#include "Sight.h"
#include "Player.h"

namespace {
	const float SCALE = 0.3f;
	const float MAX_MUL_ALPHA = 1.0f;
	const float MIN_MUL_ALPHA = 0.0f;
	const float CHANGE_ALPHA_PER_FRAME = 1.0f / 5.0f;
}

bool Sight::Start()
{
	m_spriteRender.Init(
		"Assets/sprite/sight/sight.DDS",
		512.0f,
		512.0f
	);

	m_spriteRender.SetScale(Vector3(SCALE, SCALE, 1.0f));
	m_spriteRender.Update();

	return true;
}

void Sight::Update()
{
	if (m_isDrawSight) {
		m_currentAlpha += CHANGE_ALPHA_PER_FRAME;
		if (m_currentAlpha >= MAX_MUL_ALPHA) {
			m_currentAlpha = MAX_MUL_ALPHA;
		}
	}
	else {
		m_currentAlpha -= CHANGE_ALPHA_PER_FRAME;
		if (m_currentAlpha <= MIN_MUL_ALPHA) {
			m_currentAlpha = MIN_MUL_ALPHA;
		}
	}


}

void Sight::Render(RenderContext& rc)
{
	m_spriteRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, m_currentAlpha));
	m_spriteRender.Draw(rc);
}

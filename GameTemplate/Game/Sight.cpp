#include "stdafx.h"
#include "Sight.h"
#include "Player.h"

namespace {
	const Vector3 SPRITE_SCALE = { 0.3f, 0.3f, 1.0f };		// �X�v���C�g�̊g�嗦
	const float MAX_MUL_ALPHA = 1.0f;						// ��Z���郿�l�̍ő�l
	const float MIN_MUL_ALPHA = 0.0f;						// ��Z���郿�l�̍ŏ��l
	const float CHANGE_ALPHA_PER_FRAME = 1.0f / 5.0f;		// 1�t���[�����Ƃ̃��l�̕ω���
	const Vector2 SPRITE_SIZE = { 512.0f, 512.0f };			// �X�v���C�g�̃T�C�Y
	const Vector3 SPRITE_MUL_RGB = { 1.0f, 1.0f, 1.0f };	// ��Z����RGB
}

bool Sight::Start()
{
	// �X�v���C�g��������
	m_spriteRender.Init(
		"Assets/sprite/sight/sight.DDS",
		SPRITE_SIZE.x,
		SPRITE_SIZE.y
	);

	m_spriteRender.SetScale(SPRITE_SCALE);
	m_spriteRender.Update();

	return true;
}

void Sight::Update()
{
	// ���l��ω�������
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
	if (m_currentAlpha <= MIN_MUL_ALPHA) {
		// ���l��0.0�ȉ��Ȃ�Ε`�悵�Ȃ�
		return;
	}
	m_spriteRender.SetMulColor({ SPRITE_MUL_RGB.x, SPRITE_MUL_RGB.y, SPRITE_MUL_RGB.z, m_currentAlpha });
	m_spriteRender.Draw(rc);
}

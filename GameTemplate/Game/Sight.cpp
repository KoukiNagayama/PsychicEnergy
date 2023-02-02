#include "stdafx.h"
#include "Sight.h"
#include "Player.h"

namespace {
	const Vector3 SPRITE_SCALE = { 0.3f, 0.3f, 1.0f };		// スプライトの拡大率
	const float MAX_MUL_ALPHA = 1.0f;						// 乗算するα値の最大値
	const float MIN_MUL_ALPHA = 0.0f;						// 乗算するα値の最小値
	const float CHANGE_ALPHA_PER_FRAME = 1.0f / 5.0f;		// 1フレームごとのα値の変化量
	const Vector2 SPRITE_SIZE = { 512.0f, 512.0f };			// スプライトのサイズ
	const Vector3 SPRITE_MUL_RGB = { 1.0f, 1.0f, 1.0f };	// 乗算するRGB
}

bool Sight::Start()
{
	// スプライトを初期化
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
	// α値を変化させる
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
		// α値が0.0以下ならば描画しない
		return;
	}
	m_spriteRender.SetMulColor({ SPRITE_MUL_RGB.x, SPRITE_MUL_RGB.y, SPRITE_MUL_RGB.z, m_currentAlpha });
	m_spriteRender.Draw(rc);
}

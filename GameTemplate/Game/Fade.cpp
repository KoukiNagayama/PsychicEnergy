#include "stdafx.h"
#include "Fade.h"

namespace
{
	const Vector3	SCALE = Vector3(1.436f, 1.436f, 1.0f);
	const Vector3	POSITION = Vector3(-90.0f, 7.75, 0.0f);
	const Vector2	SPRITE_SIZE = { 1920.0f, 1080.0f };		// スプライトのサイズ
	const float		MIN_ALPHA_VALUE = 0.0f;					// α値の最小値
	const float		MAX_ALPHA_VALUE = 1.0f;					// α値の最大値
	const float		MUL_COLOR_RGB = 1.0f;					// 乗算するカラーのRGB成分
}

namespace nsPsychicEnergy
{
	namespace nsFade
	{
		Fade::Fade()
		{
		}
		Fade::~Fade()
		{
		}

		bool Fade::Start()
		{
			// スプライトの初期化。
			m_spriteRender.Init("Assets/sprite/loading/nowLoading.DDS", SPRITE_SIZE.x, SPRITE_SIZE.y);
			m_spriteRender.Update();
			return true;
		}

		void Fade::Update()
		{
			switch (m_state) {

			// フェードイン中
			case enFadeState_FadeIn:
				// α値を減算する。
				m_currentAlpha -= g_gameTime->GetFrameDeltaTime();
				if (m_currentAlpha <= MIN_ALPHA_VALUE) {
					m_currentAlpha = MIN_ALPHA_VALUE;
					m_state = enFadeState_Idle;
				}
				break;
			// フェードアウト中
			case enFadeState_FadeOut:
				// α値を加算する。
				m_currentAlpha += g_gameTime->GetFrameDeltaTime();
				if (m_currentAlpha >= MAX_ALPHA_VALUE) {
					m_currentAlpha = MAX_ALPHA_VALUE;
					m_state = enFadeState_Idle;
				}
				break;
			// 待機中
			case enFadeState_Idle:
				// 何もしない。
				break;
			}
		}

		void Fade::Render(RenderContext& rc)
		{
			// α値が最低値でなければ描画する
			if (m_currentAlpha > MIN_ALPHA_VALUE) {
				// スプライトのカラーに乗算してα値を変更する。
				m_spriteRender.SetMulColor({ MUL_COLOR_RGB, MUL_COLOR_RGB, MUL_COLOR_RGB, m_currentAlpha });
				m_spriteRender.Draw(rc);
			}
		}
	}
}


#include "stdafx.h"
#include "Fade.h"

namespace
{
	const Vector3	SCALE = Vector3(1.436f, 1.436f, 1.0f);
	const Vector3	POSITION = Vector3(-90.0f, 7.75, 0.0f);
	const Vector2	SPRITE_SIZE = { 1920.0f, 1080.0f };		// �X�v���C�g�̃T�C�Y
	const float		MIN_ALPHA_VALUE = 0.0f;					// ���l�̍ŏ��l
	const float		MAX_ALPHA_VALUE = 1.0f;					// ���l�̍ő�l
	const float		MUL_COLOR_RGB = 1.0f;					// ��Z����J���[��RGB����
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
			// �X�v���C�g�̏������B
			m_spriteRender.Init("Assets/sprite/loading/nowLoading.DDS", SPRITE_SIZE.x, SPRITE_SIZE.y);
			m_spriteRender.Update();
			return true;
		}

		void Fade::Update()
		{
			switch (m_state) {

			// �t�F�[�h�C����
			case enFadeState_FadeIn:
				// ���l�����Z����B
				m_currentAlpha -= g_gameTime->GetFrameDeltaTime();
				if (m_currentAlpha <= MIN_ALPHA_VALUE) {
					m_currentAlpha = MIN_ALPHA_VALUE;
					m_state = enFadeState_Idle;
				}
				break;
			// �t�F�[�h�A�E�g��
			case enFadeState_FadeOut:
				// ���l�����Z����B
				m_currentAlpha += g_gameTime->GetFrameDeltaTime();
				if (m_currentAlpha >= MAX_ALPHA_VALUE) {
					m_currentAlpha = MAX_ALPHA_VALUE;
					m_state = enFadeState_Idle;
				}
				break;
			// �ҋ@��
			case enFadeState_Idle:
				// �������Ȃ��B
				break;
			}
		}

		void Fade::Render(RenderContext& rc)
		{
			// ���l���Œ�l�łȂ���Ε`�悷��
			if (m_currentAlpha > MIN_ALPHA_VALUE) {
				// �X�v���C�g�̃J���[�ɏ�Z���ă��l��ύX����B
				m_spriteRender.SetMulColor({ MUL_COLOR_RGB, MUL_COLOR_RGB, MUL_COLOR_RGB, m_currentAlpha });
				m_spriteRender.Draw(rc);
			}
		}
	}
}


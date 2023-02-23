#include "stdafx.h"
#include "Result.h"
#include "DisplayGameTimer.h"
#include "CommonDataForTimer.h"
#include "Fade.h"
#include "Title.h"
#include "CommonDataForSound.h"
#include "ResultBGM.h"
#include "ResultSprite.h"
#include "CommonDataForPriority.h"
namespace
{
	const Vector2 RESULT_BG_SPRITE_SIZE = { 1920.0f, 1080.0f };

}

namespace nsPsychicEnergy
{

	namespace nsResult
	{
		Result::~Result()
		{
			DeleteGO(m_displayGameTimer);
			DeleteGO(m_fade);
			DeleteGO(m_resultSprite);

			m_resultBGM->StartFadeOut();
		}

		bool Result::Start()
		{
			m_displayGameTimer = FindGO<nsTimer::DisplayGameTimer>("gameTimer");
			// �^�C���A�^�b�N�I�����̎��Ԃ��擾����B
			m_recordedTime = m_displayGameTimer->GetTime();

			// ���U���g�̃X�e�[�g�����肷��
			if (m_recordedTime >= nsTimer::MAX_VALUE_OF_TIMER) {
				m_resultState = enResultState_TimeUp;
			}
			else {
				m_resultState = enResultState_GameClear;
			}

			// �t�F�[�h
			m_fade = NewGO<nsFade::Fade>(nsPriority::enPrioritySecond, "fade");
			m_fade->StartFadeIn();

			// ���U���g�̔w�i�Ɏg�p����X�v���C�g������������B
			m_resultBGSprite.Init("Assets/sprite/result/result.DDS",
				RESULT_BG_SPRITE_SIZE.x,
				RESULT_BG_SPRITE_SIZE.y
			);
			m_resultBGSprite.Update();

			// ���U���g�Ɋւ���I�u�W�F�N�g�𐶐�����B
			m_resultBGM = NewGO<ResultBGM>(nsPriority::enPriorityFirst, "resultBGM");
			m_resultSprite = NewGO<ResultSprite>(nsPriority::enPriorityFirst, "resultSprite");

			return true;
		}

		void Result::Update()
		{
			// �t�F�[�h�A�E�g�B
			FadeOut();
		}

		void Result::FadeOut()
		{
			if (m_isWaitFadeout) {
				if (!m_fade->IsFade()) {
					// �t�F�[�h�A�E�g���I����Ă���΁A�^�C�g���ɑJ�ڂ���B
					NewGO<nsTitle::Title>(nsPriority::enPriorityFirst, "title");
					DeleteGO(this);
				}
			}
			else {
				if (g_pad[0]->IsTrigger(enButtonB)) {
					// �t�F�[�h�A�E�g���J�n����B
					m_isWaitFadeout = true;
					m_fade->StartFadeOut();
					
					// ���艹��炷�B
					m_decisionSound = NewGO<SoundSource>(nsPriority::enPriorityFirst);
					m_decisionSound->Init(nsSound::enSoundNumber_Decision);
					m_decisionSound->SetVolume(nsSound::DECISION_VOLUME);
					m_decisionSound->Play(false);
				}
			}
		}

		void Result::Render(RenderContext& rc)
		{
			// �`�悷��B
			m_resultBGSprite.Draw(rc);
		}

	}
}


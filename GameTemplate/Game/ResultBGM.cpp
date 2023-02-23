#include "stdafx.h"
#include "ResultBGM.h"
#include "CommonDataForSound.h"
#include "sound/SoundEngine.h"
#include "CommonDataForPriority.h"

namespace nsPsychicEnergy
{

	namespace nsResult
	{
		bool ResultBGM::Start()
		{
			// ���ʂ�����������B
			m_volume = nsSound::INITIAL_VOLUME;

			// ������o�^����B
			g_soundEngine->ResistWaveFileBank(nsSound::enSoundNumber_ResultBGM, "Assets/sound/bgm/resultBGM.wav");
			
			// �����I�u�W�F�N�g������������B
			m_resultBGM = NewGO<SoundSource>(nsPriority::enPriorityFirst);
			m_resultBGM->Init(nsSound::enSoundNumber_ResultBGM);
			m_resultBGM->SetVolume(m_volume);
			m_resultBGM->Play(true);
			return true;
		}

		void ResultBGM::Update()
		{
			// �t�F�[�h�A�E�g
			if (m_isStartFadeOut) {
				// ���ʂ����Z����B
				m_volume -= g_gameTime->GetFrameDeltaTime();
				if (m_volume <= nsSound::FINAL_VOLUME) {
					DeleteGO(m_resultBGM);
					DeleteGO(this);
				}
			}
			// �t�F�[�h�C��
			else {
				// ���ʂ����Z����B
				m_volume += g_gameTime->GetFrameDeltaTime();
				if (m_volume >= nsSound::BGM_MAX_VOLUME) {
					m_volume = nsSound::BGM_MAX_VOLUME;
				}
			}
			// ���ʂ�ݒ�B
			m_resultBGM->SetVolume(m_volume);
		}

	}
}


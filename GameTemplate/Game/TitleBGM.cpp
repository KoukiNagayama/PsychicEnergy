#include "stdafx.h"
#include "TitleBGM.h"
#include "CommonDataForSound.h"
#include "sound/SoundEngine.h"
#include "CommonDataForPriority.h"

namespace nsPsychicEnergy
{
	namespace nsTitle
	{
		bool TitleBGM::Start()
		{
			// ���ʂ��������B
			m_volume = nsSound::INITIAL_VOLUME;

			// ������o�^����B
			g_soundEngine->ResistWaveFileBank(nsSound::enSoundNumber_TitleBGM, "Assets/sound/bgm/titleBGM.wav");
			
			// BGM�p�����I�u�W�F�N�g�𐶐�����B
			m_titleBGM = NewGO<SoundSource>(nsPriority::enPriorityFirst);
			m_titleBGM->Init(nsSound::enSoundNumber_TitleBGM);
			m_titleBGM->SetVolume(m_volume);
			m_titleBGM->Play(true);
			return true;
		}

		void TitleBGM::Update()
		{
			// �t�F�[�h�A�E�g
			if (m_isStartFadeOut) {
				// ���ʂ����Z����B
				m_volume -= g_gameTime->GetFrameDeltaTime();

				// ���ʂ��Œ�l�������΃I�u�W�F�N�g���폜����B
				if (m_volume <= nsSound::FINAL_VOLUME) {
					DeleteGO(m_titleBGM);
					DeleteGO(this);
				}
			}
			// �t�F�[�h�C��
			else {
				// ���ʂ����Z����B
				m_volume += g_gameTime->GetFrameDeltaTime();

				// ���ʂ��ő�l������Ή��ʂ��Œ肷��B
				if (m_volume >= nsSound::BGM_MAX_VOLUME) {
					m_volume = nsSound::BGM_MAX_VOLUME;
				}
			}

			// �ύX�������ʂ������ɐݒ肷��B
			m_titleBGM->SetVolume(m_volume);
		}


	}
}



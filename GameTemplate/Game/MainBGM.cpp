#include "stdafx.h"
#include "MainBGM.h"
#include "CommonDataForSound.h"
#include "sound/SoundEngine.h"
#include "CommonDataForPriority.h"


namespace nsPsychicEnergy
{

	namespace nsSound
	{

		bool MainBGM::Start()
		{
			// ���ʂ�ݒ�B
			m_volume = nsSound::INITIAL_VOLUME;

			// wave�t�@�C����o�^����B
			g_soundEngine->ResistWaveFileBank(nsSound::enSoundNumber_MainBGM, "Assets/sound/bgm/MainBGM.wav");

			// ���C��BGM�𐶐�����B
			m_mainBGM = NewGO<SoundSource>(nsPriority::enPriorityFirst);
			m_mainBGM->Init(nsSound::enSoundNumber_MainBGM);
			m_mainBGM->SetVolume(m_volume);
			m_mainBGM->Play(true);
			return true;
		}

		void MainBGM::Update()
		{

			// �t�F�[�h�A�E�g
			if (m_isStartFadeOut) {
				m_volume -= g_gameTime->GetFrameDeltaTime();
				if (m_volume <= nsSound::FINAL_VOLUME) {
					DeleteGO(m_mainBGM);
					DeleteGO(this);
				}
			}
			// �t�F�[�h�C��
			else {
				m_volume += g_gameTime->GetFrameDeltaTime();
				if (m_volume >= nsSound::BGM_MAX_VOLUME) {
					m_volume = nsSound::BGM_MAX_VOLUME;
				}
			}
			// ���ʂ��X�V�B
			m_mainBGM->SetVolume(m_volume);
		}
	}
}



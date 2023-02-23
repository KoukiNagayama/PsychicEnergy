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
			// 音量を設定。
			m_volume = nsSound::INITIAL_VOLUME;

			// waveファイルを登録する。
			g_soundEngine->ResistWaveFileBank(nsSound::enSoundNumber_MainBGM, "Assets/sound/bgm/MainBGM.wav");

			// メインBGMを生成する。
			m_mainBGM = NewGO<SoundSource>(nsPriority::enPriorityFirst);
			m_mainBGM->Init(nsSound::enSoundNumber_MainBGM);
			m_mainBGM->SetVolume(m_volume);
			m_mainBGM->Play(true);
			return true;
		}

		void MainBGM::Update()
		{

			// フェードアウト
			if (m_isStartFadeOut) {
				m_volume -= g_gameTime->GetFrameDeltaTime();
				if (m_volume <= nsSound::FINAL_VOLUME) {
					DeleteGO(m_mainBGM);
					DeleteGO(this);
				}
			}
			// フェードイン
			else {
				m_volume += g_gameTime->GetFrameDeltaTime();
				if (m_volume >= nsSound::BGM_MAX_VOLUME) {
					m_volume = nsSound::BGM_MAX_VOLUME;
				}
			}
			// 音量を更新。
			m_mainBGM->SetVolume(m_volume);
		}
	}
}



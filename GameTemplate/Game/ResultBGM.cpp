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
			// 音量を初期化する。
			m_volume = nsSound::INITIAL_VOLUME;

			// 音源を登録する。
			g_soundEngine->ResistWaveFileBank(nsSound::enSoundNumber_ResultBGM, "Assets/sound/bgm/resultBGM.wav");
			
			// 音源オブジェクトを初期化する。
			m_resultBGM = NewGO<SoundSource>(nsPriority::enPriorityFirst);
			m_resultBGM->Init(nsSound::enSoundNumber_ResultBGM);
			m_resultBGM->SetVolume(m_volume);
			m_resultBGM->Play(true);
			return true;
		}

		void ResultBGM::Update()
		{
			// フェードアウト
			if (m_isStartFadeOut) {
				// 音量を減算する。
				m_volume -= g_gameTime->GetFrameDeltaTime();
				if (m_volume <= nsSound::FINAL_VOLUME) {
					DeleteGO(m_resultBGM);
					DeleteGO(this);
				}
			}
			// フェードイン
			else {
				// 音量を加算する。
				m_volume += g_gameTime->GetFrameDeltaTime();
				if (m_volume >= nsSound::BGM_MAX_VOLUME) {
					m_volume = nsSound::BGM_MAX_VOLUME;
				}
			}
			// 音量を設定。
			m_resultBGM->SetVolume(m_volume);
		}

	}
}


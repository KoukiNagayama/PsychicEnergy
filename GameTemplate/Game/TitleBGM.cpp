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
			// 音量を初期化。
			m_volume = nsSound::INITIAL_VOLUME;

			// 音源を登録する。
			g_soundEngine->ResistWaveFileBank(nsSound::enSoundNumber_TitleBGM, "Assets/sound/bgm/titleBGM.wav");
			
			// BGM用音源オブジェクトを生成する。
			m_titleBGM = NewGO<SoundSource>(nsPriority::enPriorityFirst);
			m_titleBGM->Init(nsSound::enSoundNumber_TitleBGM);
			m_titleBGM->SetVolume(m_volume);
			m_titleBGM->Play(true);
			return true;
		}

		void TitleBGM::Update()
		{
			// フェードアウト
			if (m_isStartFadeOut) {
				// 音量を減算する。
				m_volume -= g_gameTime->GetFrameDeltaTime();

				// 音量が最低値を下回ればオブジェクトを削除する。
				if (m_volume <= nsSound::FINAL_VOLUME) {
					DeleteGO(m_titleBGM);
					DeleteGO(this);
				}
			}
			// フェードイン
			else {
				// 音量を加算する。
				m_volume += g_gameTime->GetFrameDeltaTime();

				// 音量が最大値を上回れば音量を固定する。
				if (m_volume >= nsSound::BGM_MAX_VOLUME) {
					m_volume = nsSound::BGM_MAX_VOLUME;
				}
			}

			// 変更した音量を音源に設定する。
			m_titleBGM->SetVolume(m_volume);
		}


	}
}



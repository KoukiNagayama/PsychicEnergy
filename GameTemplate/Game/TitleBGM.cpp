#include "stdafx.h"
#include "TitleBGM.h"
#include "CommonDataForSound.h"
#include "sound/SoundEngine.h"

bool TitleBGM::Start()
{
	m_volume = nsSound::INITIAL_VOLUME;

	g_soundEngine->ResistWaveFileBank(nsSound::enSoundNumber_TitleBGM, "Assets/sound/bgm/titleBGM.wav");
	m_titleBGM = NewGO<SoundSource>(0);
	m_titleBGM->Init(nsSound::enSoundNumber_TitleBGM);
	m_titleBGM->SetVolume(m_volume);
	m_titleBGM->Play(true);
	return true;
}

void TitleBGM::Update()
{
	// フェードアウト
	if (m_isStartFadeOut) {
		m_volume -= g_gameTime->GetFrameDeltaTime();
		if (m_volume <= nsSound::FINAL_VOLUME) {
			DeleteGO(m_titleBGM);
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

	m_titleBGM->SetVolume(m_volume);
}

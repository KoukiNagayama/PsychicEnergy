#include "stdafx.h"
#include "ResultBGM.h"
#include "CommonDataForSound.h"
#include "sound/SoundEngine.h"

bool ResultBGM::Start()
{
	m_volume = nsSound::INITIAL_VOLUME;

	g_soundEngine->ResistWaveFileBank(nsSound::enSoundNumber_ResultBGM, "Assets/sound/bgm/resultBGM.wav");
	m_resultBGM = NewGO<SoundSource>(0);
	m_resultBGM->Init(nsSound::enSoundNumber_ResultBGM);
	m_resultBGM->SetVolume(m_volume);
	m_resultBGM->Play(true);
	return true;
}

void ResultBGM::Update()
{
	// フェードアウト
	if (m_isStartFadeOut) {
		m_volume -= g_gameTime->GetFrameDeltaTime();
		if (m_volume <= nsSound::FINAL_VOLUME) {
			DeleteGO(m_resultBGM);
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

	m_resultBGM->SetVolume(m_volume);
}

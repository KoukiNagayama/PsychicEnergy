#include "stdafx.h"
#include "Result.h"
#include "DisplayGameTimer.h"
#include "CommonDataForTimer.h"
#include "Fade.h"
#include "Title.h"
#include "CommonDataForSound.h"
#include "ResultBGM.h"
namespace
{
	const Vector2 RESULT_BG_SPRITE_SIZE = { 1920.0f, 1080.0f };

}

Result::~Result()
{
	DeleteGO(m_displayGameTimer);
	DeleteGO(m_fade);

	m_resultBGM->StartFadeOut();
}

bool Result::Start()
{
	m_displayGameTimer = FindGO<DisplayGameTimer>("gameTimer");
	// タイマーを止める
	m_displayGameTimer->Disable();
	// タイムアタック終了時の時間を取得する。
	m_recordedTime = m_displayGameTimer->GetTime();

	// リザルトのステートを決定する
	if (m_recordedTime >= nsTimer::MAX_VALUE_OF_TIMER) {
		m_resultState = enResultState_TimeUp;
	}
	else {
		m_resultState = enResultState_GameClear;
	}

	// フェード
	m_fade = NewGO<Fade>(4, "fade");
	m_fade->StartFadeIn();

	m_resultBGSprite.Init("Assets/sprite/result/result.DDS",
		RESULT_BG_SPRITE_SIZE.x,
		RESULT_BG_SPRITE_SIZE.y
	);
	m_resultBGSprite.Update();

	m_resultBGM = NewGO<ResultBGM>(0, "resultBGM");

	return true;
}

void Result::Update()
{
	FadeOut();
}

void Result::FadeOut()
{
	if (m_isWaitFadeout) {
		if (!m_fade->IsFade()) {
			NewGO<Title>(0, "title");
			DeleteGO(this);
		}
	}
	else {
		if (g_pad[0]->IsTrigger(enButtonB)){
			m_isWaitFadeout = true;
			m_fade->StartFadeOut();
			// 決定音を鳴らす。
			m_decisionSound = NewGO<SoundSource>(0);
			m_decisionSound->Init(nsSound::enSoundNumber_Decision);
			m_decisionSound->SetVolume(nsSound::DECISION_VOLUME);
			m_decisionSound->Play(false);
		}
	}
}

void Result::Render(RenderContext& rc)
{
	m_resultBGSprite.Draw(rc);
}

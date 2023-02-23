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
			m_fade = NewGO<nsFade::Fade>(nsPriority::enPrioritySecond, "fade");
			m_fade->StartFadeIn();

			// リザルトの背景に使用するスプライトを初期化する。
			m_resultBGSprite.Init("Assets/sprite/result/result.DDS",
				RESULT_BG_SPRITE_SIZE.x,
				RESULT_BG_SPRITE_SIZE.y
			);
			m_resultBGSprite.Update();

			// リザルトに関するオブジェクトを生成する。
			m_resultBGM = NewGO<ResultBGM>(nsPriority::enPriorityFirst, "resultBGM");
			m_resultSprite = NewGO<ResultSprite>(nsPriority::enPriorityFirst, "resultSprite");

			return true;
		}

		void Result::Update()
		{
			// フェードアウト。
			FadeOut();
		}

		void Result::FadeOut()
		{
			if (m_isWaitFadeout) {
				if (!m_fade->IsFade()) {
					// フェードアウトが終わっていれば、タイトルに遷移する。
					NewGO<nsTitle::Title>(nsPriority::enPriorityFirst, "title");
					DeleteGO(this);
				}
			}
			else {
				if (g_pad[0]->IsTrigger(enButtonB)) {
					// フェードアウトを開始する。
					m_isWaitFadeout = true;
					m_fade->StartFadeOut();
					
					// 決定音を鳴らす。
					m_decisionSound = NewGO<SoundSource>(nsPriority::enPriorityFirst);
					m_decisionSound->Init(nsSound::enSoundNumber_Decision);
					m_decisionSound->SetVolume(nsSound::DECISION_VOLUME);
					m_decisionSound->Play(false);
				}
			}
		}

		void Result::Render(RenderContext& rc)
		{
			// 描画する。
			m_resultBGSprite.Draw(rc);
		}

	}
}


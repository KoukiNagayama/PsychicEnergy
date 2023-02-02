#include "stdafx.h"
#include "Result.h"
#include "DisplayGameTimer.h"
#include "CommonDataForTimer.h"

bool Result::Start()
{
	auto displayGameTimer = FindGO<DisplayGameTimer>("gameTimer");
	// タイマーを止める
	displayGameTimer->Disable();
	// タイムアタック終了時の時間を取得する。
	m_recordedTime = displayGameTimer->GetTime();

	// リザルトのステートを決定する
	if (m_recordedTime >= MAX_VALUE_OF_TIMER) {
		m_resultState = enResultState_TimeUp;
	}
	else {
		m_resultState = enResultState_GameClear;
	}

	return true;
}

void Result::Update()
{

}

void Result::Render(RenderContext& rc)
{

}

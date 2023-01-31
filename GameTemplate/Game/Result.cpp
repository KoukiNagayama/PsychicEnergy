#include "stdafx.h"
#include "Result.h"
#include "DisplayGameTimer.h"

bool Result::Start()
{
	auto displayGameTimer = FindGO<DisplayGameTimer>("gameTimer");
	// タイムアタック終了時の時間を取得する。
	m_recordedTime = displayGameTimer->GetTime();


	return true;
}

void Result::Update()
{

}

void Result::Render(RenderContext& rc)
{

}

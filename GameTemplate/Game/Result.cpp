#include "stdafx.h"
#include "Result.h"
#include "DisplayGameTimer.h"

bool Result::Start()
{
	auto displayGameTimer = FindGO<DisplayGameTimer>("gameTimer");
	// �^�C���A�^�b�N�I�����̎��Ԃ��擾����B
	m_recordedTime = displayGameTimer->GetTime();


	return true;
}

void Result::Update()
{

}

void Result::Render(RenderContext& rc)
{

}

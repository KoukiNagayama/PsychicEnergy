#include "stdafx.h"
#include "Result.h"
#include "DisplayGameTimer.h"
#include "CommonDataForTimer.h"

bool Result::Start()
{
	auto displayGameTimer = FindGO<DisplayGameTimer>("gameTimer");
	// �^�C�}�[���~�߂�
	displayGameTimer->Disable();
	// �^�C���A�^�b�N�I�����̎��Ԃ��擾����B
	m_recordedTime = displayGameTimer->GetTime();

	// ���U���g�̃X�e�[�g�����肷��
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

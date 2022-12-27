#include "stdafx.h"
#include "PlayerWalkState.h"
#include "PlayerIdleState.h"
#include "PlayerIdleInAirState.h"
#include "PlayerSlideState.h"
#include "PlayerJumpState.h"
#include "PlayerFallState.h"


void PlayerWalkState::Enter()
{
	// �t���O���I�u�W�F�N�g�ɐG��Ă���Ɛݒ肷��B
	m_player->SetIsTouchObject(true);

	m_player->ResetSlideParam();

}

IPlayerState* PlayerWalkState::StateChange()
{
	if (fabsf(g_pad[0]->GetLStickXF()) < 0.001f && fabsf(g_pad[0]->GetLStickYF()) < 0.001f) {
		// �ҋ@�X�e�[�g�ɑJ�ڂ���B
		return new PlayerIdleState(m_player);
	}
	if (g_pad[0]->IsTrigger(enButtonRB1)) {
		// �󒆂ł̑ҋ@�X�e�[�g�ɑJ�ڂ���B
		return new PlayerIdleInAirState(m_player);
	}
	if (g_pad[0]->IsTrigger(enButtonLB2)) {
		// �X���C�f�B���O�X�e�[�g�ɑJ�ڂ���B
		return new PlayerSlideState(m_player);
	}
	if (g_pad[0]->IsTrigger(enButtonA)) {
		// �W�����v�X�e�[�g�ɑJ�ڂ���B
		return new PlayerJumpState(m_player);
	}
	if (m_player->IsOnGround() == false) {
		return new PlayerFallState(m_player);
	}
	// �����܂ŗ�����X�e�[�g��J�ڂ��Ȃ��B
	return nullptr;
}

void PlayerWalkState::Update()
{
	m_player->MoveOnGround();

	m_player->SelectAnimationOnGround();
}
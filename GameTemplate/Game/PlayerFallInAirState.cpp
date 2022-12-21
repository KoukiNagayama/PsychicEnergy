#include "stdafx.h"
#include "PlayerFallInAirState.h"
#include "PlayerIdleInAirState.h"
#include "PlayerIdleState.h"

void PlayerFallInAirState::Enter()
{
	// �A�j���[�V������ݒ肷��B
	//player->SetAnimation(Player::enAnimationClip_Idle);

	// �ړ����������肷��B
	m_player->DecideMoveDirection();
}

IPlayerState* PlayerFallInAirState::StateChange()
{
	if (g_pad[0]->IsTrigger(enButtonRB1)) {
		// �󒆂ł̑ҋ@�X�e�[�g�ɑJ�ڂ���B
		return new PlayerIdleInAirState(m_player);
	}
	if (g_pad[0]->IsTrigger(enButtonLB1)) {
		// �ʏ�̑ҋ@�X�e�[�g�ɑJ�ڂ���B
		m_player->FloatModeChange(false);
		return new PlayerIdleState(m_player);
	}
	// �����܂ŗ�����J�ڂ��Ȃ��B
	return nullptr;
}

void PlayerFallInAirState::Update()
{
	// �󒆂ł̈ړ������B
	m_player->MoveOnAirspace();
}
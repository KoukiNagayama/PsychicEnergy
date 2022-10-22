#include "stdafx.h"
#include "PlayerFallInAirState.h"
#include "PlayerIdleInAirState.h"
#include "PlayerIdleState.h"

void PlayerFallInAirState::Enter(Player* player)
{
	// �A�j���[�V������ݒ肷��B
	player->SetAnimation(Player::enAnimationClip_Idle);

	// �ړ����������肷��B
	player->DecideMoveDirection();
}

PlayerState* PlayerFallInAirState::HandleInput(Player* player)
{
	if (g_pad[0]->IsTrigger(enButtonRB1)) {
		// �󒆂ł̑ҋ@�X�e�[�g�ɑJ�ڂ���B
		return new PlayerIdleInAirState();
	}
	if (g_pad[0]->IsTrigger(enButtonLB1)) {
		// �ʏ�̑ҋ@�X�e�[�g�ɑJ�ڂ���B
		return new PlayerIdleState();
	}
	// �����܂ŗ�����J�ڂ��Ȃ��B
	return nullptr;
}

void PlayerFallInAirState::Update(Player* player)
{
	// �󒆂ł̈ړ������B
	player->MoveOnAirspace();
}
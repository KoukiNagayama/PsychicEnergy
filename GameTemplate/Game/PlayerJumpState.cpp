#include "stdafx.h"
#include "PlayerJumpState.h"
#include "PlayerWalkState.h"
#include "PlayerIdleState.h"

PlayerJumpState::~PlayerJumpState()
{

}

void PlayerJumpState::Enter(Player* player)
{
	player->SetAnimation(Player::enAnimationClip_NormalJump);
}

PlayerState* PlayerJumpState::StateChange(Player* player)
{
	// �A�j���[�V�������Đ�����Ă���Ԃ̓X�e�[�g��J�ڂ��Ȃ��B
	if (player->IsPlayingAnimation()) {
		return nullptr;
	}
	else if (fabsf(g_pad[0]->GetLStickXF()) >= 0.001f || fabsf(g_pad[0]->GetLStickYF()) >= 0.001f) {
		return new PlayerWalkState();
	}
	else {
		return new PlayerIdleState();
	}
}

void PlayerJumpState::Update(Player* player)
{
	player->Jump();
}

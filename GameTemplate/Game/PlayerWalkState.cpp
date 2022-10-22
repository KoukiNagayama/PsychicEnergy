#include "stdafx.h"
#include "PlayerWalkState.h"
#include "PlayerIdleState.h"
#include "PlayerIdleInAirState.h"


void PlayerWalkState::Enter(Player* player)
{
	// �Đ�����A�j���[�V������ݒ肷��B
	player->SetAnimation(Player::enAnimationClip_Walk);

	// �t���O���I�u�W�F�N�g�ɐG��Ă���Ɛݒ肷��B
	player->SetIsTouchObject(true);
}

PlayerState* PlayerWalkState::HandleInput(Player* player)
{
	if (fabsf(g_pad[0]->GetLStickXF()) < 0.1f && fabsf(g_pad[0]->GetLStickYF()) < 0.1f) {
		// �ҋ@�X�e�[�g�ɑJ�ڂ���B
		return new PlayerIdleState();
	}
	if (g_pad[0]->IsTrigger(enButtonRB1)) {
		// �󒆂ł̑ҋ@�X�e�[�g�ɑJ�ڂ���B
		return new PlayerIdleInAirState();
	}
	// �����܂ŗ�����X�e�[�g��J�ڂ��Ȃ��B
	return nullptr;
}

void PlayerWalkState::Update(Player* player)
{
	player->WalkOnGround();
}
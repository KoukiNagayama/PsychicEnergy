#include "stdafx.h"
#include "PlayerIdleState.h"
#include "PlayerWalkState.h"
#include "PlayerIdleInAirState.h"

PlayerIdleState::~PlayerIdleState()
{

}

void PlayerIdleState::Enter(Player* player) 
{
	// �Đ�����A�j���[�V������ݒ�B
	player->SetAnimation(Player::enAnimationClip_Idle);

	// �t���O���I�u�W�F�N�g�ɐG��Ă���悤�ɐݒ肷��B
	player->SetIsTouchObject(true);
}

PlayerState* PlayerIdleState::HandleInput(Player* player)
{
	if (g_pad[0]->IsTrigger(enButtonRB1)) {
		// �󒆂ł̃X�e�[�g�ɑJ�ڂ���B
		return new PlayerIdleInAirState();
	}
	if (fabsf(g_pad[0]->GetLStickXF()) !=  0.0f || fabsf(g_pad[0]->GetLStickYF()) !=  0.0f) {
		// �����X�e�[�g�ɑJ�ڂ���B
		return new PlayerWalkState();
	}
	// �����܂ŗ�����X�e�[�g��J�ڂ��Ȃ��B
	return nullptr;

}

void PlayerIdleState::Update(Player* player)
{

}
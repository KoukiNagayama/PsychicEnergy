#include "stdafx.h"
#include "PlayerIdleInAirState.h"
#include "PlayerIdleState.h"
#include "PlayerFallInAirState.h"

void PlayerIdleInAirState::Enter(Player* player)
{
	// �Đ�����A�j���[�V������ݒ肷��B
	player->SetAnimation(Player::enAnimationClip_Idle);
	// �t���O���I�u�W�F�N�g�ɐG��Ă��Ȃ��悤�ɐݒ肷��B
	player->SetIsTouchObject(false);
}

PlayerState* PlayerIdleInAirState::HandleInput(Player* player)
{
	if (g_pad[0]->IsTrigger(enButtonRB1)) {
		// �ʏ�̑ҋ@��ԂɑJ�ڂ���B
		return new PlayerFallInAirState();
	}
	if (g_pad[0]->IsTrigger(enButtonLB1)) {
		// 
		return nullptr;
	}
	// �����܂ŗ�����X�e�[�g��J�ڂ��Ȃ��B
	return nullptr;
}

void PlayerIdleInAirState::Update(Player* player)
{

}

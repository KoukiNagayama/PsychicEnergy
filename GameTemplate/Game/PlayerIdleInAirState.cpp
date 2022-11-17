#include "stdafx.h"
#include "PlayerIdleInAirState.h"
#include "PlayerIdleState.h"
#include "PlayerFallInAirState.h"

void PlayerIdleInAirState::Enter(Player* player)
{
	// �Đ�����A�j���[�V������ݒ肷��B
	//player->SetAnimation(Player::enAnimationClip_Idle);
	// �t���O���I�u�W�F�N�g�ɐG��Ă��Ȃ��悤�ɐݒ肷��B
	player->SetIsTouchObject(false);

	player->ResetSlide();
}

PlayerState* PlayerIdleInAirState::StateChange(Player* player)
{
	if (g_pad[0]->IsTrigger(enButtonRB1)) {
		// �󒆂ł̗����X�e�[�g�ɑJ�ڂ���B
		return new PlayerFallInAirState();
	}
	if (g_pad[0]->IsTrigger(enButtonLB1)) {
		// �ʏ�̑ҋ@�X�e�[�g�ɑJ�ڂ���B
		return new PlayerIdleState();
	}
	// �����܂ŗ�����X�e�[�g��J�ڂ��Ȃ��B
	return nullptr;
}

void PlayerIdleInAirState::Update(Player* player)
{

}

#include "stdafx.h"
#include "PlayerWalkState.h"
#include "PlayerIdleState.h"
#include "PlayerIdleInAirState.h"
#include "PlayerSlideState.h"


void PlayerWalkState::Enter(Player* player)
{
	// �t���O���I�u�W�F�N�g�ɐG��Ă���Ɛݒ肷��B
	player->SetIsTouchObject(true);

	player->ResetSlide();

}

PlayerState* PlayerWalkState::StateChange(Player* player)
{
	if (fabsf(g_pad[0]->GetLStickXF()) < 0.001f && fabsf(g_pad[0]->GetLStickYF()) < 0.001f) {
		// �ҋ@�X�e�[�g�ɑJ�ڂ���B
		return new PlayerIdleState();
	}
	if (g_pad[0]->IsTrigger(enButtonRB1)) {
		// �󒆂ł̑ҋ@�X�e�[�g�ɑJ�ڂ���B
		return new PlayerIdleInAirState();
	}
	if (g_pad[0]->IsTrigger(enButtonLB2)) {
		// �X���C�f�B���O�X�e�[�g�ɑJ�ڂ���B
		return new PlayerSlideState();
	}
	// �����܂ŗ�����X�e�[�g��J�ڂ��Ȃ��B
	return nullptr;
}

void PlayerWalkState::Update(Player* player)
{
	player->MoveOnGround();
}
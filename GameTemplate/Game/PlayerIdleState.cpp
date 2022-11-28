#include "stdafx.h"
#include "PlayerIdleState.h"
#include "PlayerWalkState.h"
#include "PlayerJumpState.h"
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

	// �X���C�h�����Z�b�g����B
	player->ResetSlideParam();

	player->ResetJump();
}

PlayerState* PlayerIdleState::StateChange(Player* player)
{
	if (g_pad[0]->IsTrigger(enButtonRB1)) {
		// �󒆂ł̑ҋ@�X�e�[�g�ɑJ�ڂ���B
		return new PlayerIdleInAirState();
	}
	else if (player->IsOnGround()) {
		if (fabsf(g_pad[0]->GetLStickXF()) >= 0.001f || fabsf(g_pad[0]->GetLStickYF()) >= 0.001f) {
			// �����X�e�[�g�ɑJ�ڂ���B
			return new PlayerWalkState();
		}
		else if (g_pad[0]->IsTrigger(enButtonA)) {
			// �W�����v�X�e�[�g�ɑJ�ڂ���B
			return new PlayerJumpState();
		}
	}
	// �����܂ŗ�����X�e�[�g��J�ڂ��Ȃ��B
	return nullptr;

}

void PlayerIdleState::Update(Player* player)
{
	//player->WalkOnGround();
	// todo �ς���
	player->m_moveSpeed.x = 0.0f;
	player->m_moveSpeed.z = 0.0f;
	player->m_moveSpeed.y -= 14.0f;
	player->m_position = player->m_charaCon.Execute(player->m_moveSpeed, g_gameTime->GetFrameDeltaTime());
	player->m_modelRender.SetPosition(player->m_position);
}
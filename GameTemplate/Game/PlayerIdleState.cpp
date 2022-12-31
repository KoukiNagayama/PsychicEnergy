#include "stdafx.h"
#include "PlayerIdleState.h"
#include "PlayerWalkState.h"
#include "PlayerJumpState.h"
#include "PlayerIdleInAirState.h"
#include "PlayerFallState.h"

PlayerIdleState::~PlayerIdleState()
{

}

void PlayerIdleState::Enter() 
{
	// �Đ�����A�j���[�V������ݒ�B
	m_player->SetAnimation(Player::enAnimationClip_Idle);

	// �t���O���I�u�W�F�N�g�ɐG��Ă���悤�ɐݒ肷��B
	m_player->SetIsTouchObject(true);

	// �X���C�h�����Z�b�g����B
	m_player->ResetSlideParam();

	// �W�����v�����Z�b�g����B
	m_player->ResetJump();



}

IPlayerState* PlayerIdleState::StateChange()
{
	if (g_pad[0]->IsTrigger(enButtonRB1)) {
		// �󒆂ł̑ҋ@�X�e�[�g�ɑJ�ڂ���B
		return new PlayerIdleInAirState(m_player);
	}
	else if (m_player->IsOnGround()) {
		if (fabsf(g_pad[0]->GetLStickXF()) >= 0.001f || fabsf(g_pad[0]->GetLStickYF()) >= 0.001f) {
			// �����X�e�[�g�ɑJ�ڂ���B
			return new PlayerWalkState(m_player);
		}
		else if (g_pad[0]->IsTrigger(enButtonA)) {
			// �W�����v�X�e�[�g�ɑJ�ڂ���B
			return new PlayerJumpState(m_player);
		}
	}
	if (m_player->IsOnGround() == false) {
		return new PlayerFallState(m_player);
	}
	// �����܂ŗ�����X�e�[�g��J�ڂ��Ȃ��B
	return nullptr;

}

void PlayerIdleState::Update()
{
	//player->WalkOnGround();
	// todo �ς���
	m_player->m_moveSpeed.x = 0.0f;
	m_player->m_moveSpeed.z = 0.0f;
	m_player->m_moveSpeed.y -= 14.0f;
	m_player->m_position = m_player->m_charaCon.Execute(m_player->m_moveSpeed, g_gameTime->GetFrameDeltaTime());
	m_player->m_modelRender.SetPosition(m_player->m_position);

	m_player->Rotation();
}
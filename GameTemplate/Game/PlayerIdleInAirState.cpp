#include "stdafx.h"
#include "PlayerIdleInAirState.h"
#include "PlayerIdleState.h"
#include "PlayerFallInAirState.h"

void PlayerIdleInAirState::Enter()
{
	// �Đ�����A�j���[�V������ݒ肷��B
	m_player->SetAnimation(Player::enAnimationClip_IdleAir);
	// �t���O���I�u�W�F�N�g�ɐG��Ă��Ȃ��悤�ɐݒ肷��B
	m_player->SetIsTouchObject(false);
	// �X���C�h���̃p�����[�^�����Z�b�g����B
	m_player->ResetSlideParam();
	// ���V��Ԃł���Ɛݒ肷��B
	m_player->FloatModeChange(true);

	//player->m_moveSpeed = Vector3::Zero;
	m_player->m_moveSpeed.y = 100.0f;
}

IPlayerState* PlayerIdleInAirState::StateChange()
{
	if (g_pad[0]->IsTrigger(enButtonRB1)) {
		// �󒆂ł̗����X�e�[�g�ɑJ�ڂ���B
		return new PlayerFallInAirState(m_player);
	}
	if (g_pad[0]->IsTrigger(enButtonLB1)) {
		// �ʏ�̑ҋ@�X�e�[�g�ɑJ�ڂ���B
		m_player->m_modelRender.SetIsFloating(false);
		m_player->FloatModeChange(false);
		return new PlayerIdleState(m_player);
	}
	// �����܂ŗ�����X�e�[�g��J�ڂ��Ȃ��B
	return nullptr;
}

void PlayerIdleInAirState::Update()
{
	if (m_player->m_moveSpeed.y > 0.0f) {
		m_player->m_moveSpeed *= 0.97f;
		m_player->m_position = m_player->m_charaCon.Execute(m_player->m_moveSpeed, g_gameTime->GetFrameDeltaTime());
		m_player->m_modelRender.SetPosition(m_player->m_position);
	}
}

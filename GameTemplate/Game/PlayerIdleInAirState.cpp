#include "stdafx.h"
#include "PlayerIdleInAirState.h"
#include "PlayerIdleState.h"
#include "PlayerFallInAirState.h"
#include "Sight.h"
#include "GravityGauge.h"

PlayerIdleInAirState::~PlayerIdleInAirState()
{
	m_sight->SetIsDrawSight(false);
}

void PlayerIdleInAirState::Enter()
{
	m_sight = FindGO<Sight>("sight");
	m_gravityGauge = FindGO<GravityGauge>("gravityGauge");
	m_sight->SetIsDrawSight(true);
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
	if (g_pad[0]->IsTrigger(enButtonLB1)
		|| m_gravityGauge->GetDisplayAreaAngleDeg() <= 0.0f
		) {
		// �ʏ�̑ҋ@�X�e�[�g�ɑJ�ڂ���B
		m_player->FloatModeChange(false);
		g_worldRotation->SetIsReseting(true);
		m_player->RotationToCorrectForward();
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

	//m_player->Rotation();

}

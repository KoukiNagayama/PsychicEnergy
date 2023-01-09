#include "stdafx.h"
#include "PlayerFallInAirState.h"
#include "PlayerIdleInAirState.h"
#include "PlayerIdleState.h"

PlayerFallInAirState::~PlayerFallInAirState()
{
	DeleteGO(m_player->m_effectEmitterWind);
}

void PlayerFallInAirState::Enter()
{

	// �A�j���[�V������ݒ肷��B
	m_player->SetAnimation(Player::enAnimationClip_FallAir);

	// �ړ����������肷��B
	m_player->DecideMoveDirection();

	m_player->RotationFallAir();

	g_worldRotation->SetIsGetNormal(true);

	GenerateWindEffect();

}

IPlayerState* PlayerFallInAirState::StateChange()
{
	if (g_pad[0]->IsTrigger(enButtonRB1)) {
		// �󒆂ł̑ҋ@�X�e�[�g�ɑJ�ڂ���B
		return new PlayerIdleInAirState(m_player);
	}
	if (g_pad[0]->IsTrigger(enButtonLB1)) {
		// �ʏ�̑ҋ@�X�e�[�g�ɑJ�ڂ���B
		m_player->FloatModeChange(false);
		return new PlayerIdleState(m_player);
	}
	if (m_player->IsPlayerTouchObject()) {
		m_player->FloatModeChange(false);
		return new PlayerIdleState(m_player);
	}
	// �����܂ŗ�����J�ڂ��Ȃ��B
	return nullptr;
}

void PlayerFallInAirState::Update()
{
	// �󒆂ł̈ړ������B
	m_player->MoveOnAirspace();

	m_player->m_effectEmitterWind->SetPosition(m_player->m_position);

	m_secToRegenerateWindEffect += g_gameTime->GetFrameDeltaTime();

	if (m_secToRegenerateWindEffect >= 0.8f) {
		DeleteGO(m_player->m_effectEmitterWind);
		GenerateWindEffect();
		m_secToRegenerateWindEffect = 0.0f;
	}
}

void PlayerFallInAirState::GenerateWindEffect()
{
	m_player->m_effectEmitterWind = NewGO<EffectEmitter>(0, "effectWind");
	m_player->m_effectEmitterWind->Init(0);
	m_player->m_effectEmitterWind->SetPosition(m_player->m_position);
	m_player->m_effectEmitterWind->SetRotation(m_player->m_rotation);

	m_player->m_effectEmitterWind->SetScale(Vector3::One * 100.0f);
	m_player->m_effectEmitterWind->Play();
}
#include "stdafx.h"
#include "PlayerSlideState.h"
#include "PlayerIdleState.h"
#include "PlayerJumpState.h"
#include "PlayerFallState.h"
#include "GravityGauge.h"

PlayerSlideState::~PlayerSlideState()
{
	DeleteGO(m_player->m_effectEmitterWind);
}

void PlayerSlideState::Enter()
{
	m_player->SetAnimation(Player::enAnimationClip_Slide);

	m_player->InitSlideParam();

	// 風のエフェクトを生成。
	m_player->GenerateWindEffect();
}

IPlayerState* PlayerSlideState::StateChange()
{
	if (g_pad[0]->IsPress(enButtonLB2) == false) {
		return new PlayerIdleState(m_player);
	}
	else if (g_pad[0]->IsTrigger(enButtonA)) {
		//return new PlayerJumpState();
	}
	else if (m_player->IsOnGround() == false) {
		return new PlayerFallState(m_player);
	}
	return nullptr;
}

void PlayerSlideState::Update()
{
	// スライディングの移動
	m_player->Slide();
	// 回転
	m_player->Rotation();

	// 風のエフェクト
	m_player->m_effectEmitterWind->SetPosition(m_player->m_position);
	m_secToRegenerateWindEffect += g_gameTime->GetFrameDeltaTime();
	if (m_secToRegenerateWindEffect >= 0.8f) {
		DeleteGO(m_player->m_effectEmitterWind);
		m_player->GenerateWindEffect();
		m_secToRegenerateWindEffect = 0.0f;
	}

}



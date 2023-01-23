#include "stdafx.h"
#include "PlayerJumpState.h"
#include "PlayerWalkState.h"
#include "PlayerIdleState.h"
#include "PlayerIdleInAirState.h"
#include "PlayerFallState.h"


PlayerJumpState::~PlayerJumpState()
{
	m_player->SetAnimationSpeed(1.0f);
}

void PlayerJumpState::Enter()
{
	m_player->m_moveSpeed.y += 450.0f;

	m_player->SetAnimationSpeed(0.9f);

	m_player->SelectJumpAnimation();
}

IPlayerState* PlayerJumpState::StateChange()
{
	if (m_player->IsOnGround()){
		return new PlayerIdleState(m_player);
	}
	else if (g_pad[0]->IsTrigger(enButtonRB1)) {
		return new PlayerIdleInAirState(m_player);
	}
	if (m_count >= 0.9f) {
		return new PlayerFallState(m_player);
	}
	return nullptr;
}

void PlayerJumpState::Update()
{
	m_player->Jump();

	m_count += g_gameTime->GetFrameDeltaTime();

	m_player->Rotation();

}
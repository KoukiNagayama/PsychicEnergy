#include "stdafx.h"
#include "PlayerJumpState.h"
#include "PlayerWalkState.h"
#include "PlayerIdleState.h"
#include "PlayerIdleInAirState.h"


PlayerJumpState::~PlayerJumpState()
{
	
}

void PlayerJumpState::Enter(Player* player)
{
	player->m_moveSpeed.y += 450.0f;

	player->SetAnimationSpeed(0.9f);

	player->SelectJumpAnimation();
}

PlayerState* PlayerJumpState::StateChange(Player* player)
{
	if (player->IsOnGround()){
		return new PlayerIdleState();
	}
	else if (g_pad[0]->IsTrigger(enButtonRB1)) {
		return new PlayerIdleInAirState();
	}
	return nullptr;
}

void PlayerJumpState::Update(Player* player)
{
	player->Jump();
}
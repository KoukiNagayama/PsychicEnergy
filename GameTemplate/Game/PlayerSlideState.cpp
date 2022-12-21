#include "stdafx.h"
#include "PlayerSlideState.h"
#include "PlayerIdleState.h"
#include "PlayerJumpState.h"

PlayerSlideState::~PlayerSlideState()
{

}

void PlayerSlideState::Enter()
{
	m_player->SetAnimation(Player::enAnimationClip_Slide);

	m_player->InitSlideParam();
}

IPlayerState* PlayerSlideState::StateChange()
{
	if (g_pad[0]->IsPress(enButtonLB2) == false) {
		return new PlayerIdleState(m_player);
	}
	else if (g_pad[0]->IsTrigger(enButtonA)) {
		//return new PlayerJumpState();
	}
	return nullptr;
}

void PlayerSlideState::Update()
{
	m_player->Slide();
}



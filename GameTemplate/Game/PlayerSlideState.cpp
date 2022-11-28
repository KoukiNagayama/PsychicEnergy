#include "stdafx.h"
#include "PlayerSlideState.h"
#include "PlayerIdleState.h"
#include "PlayerJumpState.h"

PlayerSlideState::~PlayerSlideState()
{

}

void PlayerSlideState::Enter(Player* player)
{
	player->SetAnimation(Player::enAnimationClip_Slide);

	player->InitSlideParam();
}

PlayerState* PlayerSlideState::StateChange(Player* player)
{
	if (g_pad[0]->IsPress(enButtonLB2) == false) {
		return new PlayerIdleState();
	}
	else if (g_pad[0]->IsTrigger(enButtonB)) {
		//return new PlayerJumpState();
	}
	return nullptr;
}

void PlayerSlideState::Update(Player* player)
{
	player->Slide();
}



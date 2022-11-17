#include "stdafx.h"
#include "PlayerSlideState.h"
#include "PlayerIdleState.h"

PlayerSlideState::~PlayerSlideState()
{

}

void PlayerSlideState::Enter(Player* player)
{
	player->SetAnimation(Player::enAnimationClip_Slide);

	player->InitSlide();
}

PlayerState* PlayerSlideState::StateChange(Player* player)
{
	if (g_pad[0]->IsPress(enButtonLB2) == false) {
		return new PlayerIdleState();
	}
	return nullptr;
}

void PlayerSlideState::Update(Player* player)
{
	player->Slide();
}



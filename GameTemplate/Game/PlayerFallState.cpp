#include "stdafx.h"
#include "PlayerFallState.h"
#include "PlayerIdleState.h"
#include "PlayerWalkState.h"


void PlayerFallState::Enter()
{
	m_player->SetAnimation(Player::enAnimationClip_NormalJump);
}

IPlayerState* PlayerFallState::StateChange()
{
	if (m_player->IsOnGround()){
		if (fabsf(g_pad[0]->GetLStickXF()) >= 0.001f || fabsf(g_pad[0]->GetLStickYF()) >= 0.001f) {
			return new PlayerWalkState(m_player);
		}
		else {
			return new PlayerIdleState(m_player);
		}
	}
	// ‚±‚±‚Ü‚Å—ˆ‚½‚ç‘JˆÚ‚µ‚È‚¢B
	return nullptr;
}

void PlayerFallState::Update()
{
	m_player->MoveOnGround();
}
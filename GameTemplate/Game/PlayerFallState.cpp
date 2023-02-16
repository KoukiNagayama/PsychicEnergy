#include "stdafx.h"
#include "PlayerFallState.h"
#include "PlayerIdleState.h"
#include "PlayerWalkState.h"
#include "PlayerIdleInAirState.h"
#include "CommonDataForSound.h"


PlayerFallState::~PlayerFallState()
{

}

void PlayerFallState::Enter()
{
	m_player->SetAnimation(Player::enAnimationClip_NormalJump);
}

IPlayerState* PlayerFallState::StateChange()
{
	if (m_player->IsOnGround()){
		if (m_player->IsOnGround()) {
			m_landingSound = NewGO<SoundSource>(0);
			m_landingSound->Init(nsSound::enSoundNumber_PlayerLanding);
			m_landingSound->Play(false);
		}
		return new PlayerIdleState(m_player);
	}
	else if (g_pad[0]->IsTrigger(enButtonRB1)) {
		return new PlayerIdleInAirState(m_player);
	}
	// ‚±‚±‚Ü‚Å—ˆ‚½‚ç‘JˆÚ‚µ‚È‚¢B
	return nullptr;
}

void PlayerFallState::Update()
{
	m_player->MoveOnGround();

	m_player->Rotation();

}
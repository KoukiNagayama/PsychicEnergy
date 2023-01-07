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
	// アニメーションを設定する。
	m_player->SetAnimation(Player::enAnimationClip_FallAir);

	// 移動方向を決定する。
	m_player->DecideMoveDirection();

	m_player->RotationFallAir();

	g_worldRotation->SetIsGetNormal(true);

	GenerateWindEffect();

}

IPlayerState* PlayerFallInAirState::StateChange()
{
	if (g_pad[0]->IsTrigger(enButtonRB1)) {
		// 空中での待機ステートに遷移する。
		return new PlayerIdleInAirState(m_player);
	}
	if (g_pad[0]->IsTrigger(enButtonLB1)) {
		// 通常の待機ステートに遷移する。
		m_player->FloatModeChange(false);
		return new PlayerIdleState(m_player);
	}
	// ここまで来たら遷移しない。
	return nullptr;
}

void PlayerFallInAirState::Update()
{
	// 空中での移動処理。
	m_player->MoveOnAirspace();

	m_player->m_effectEmitterWind->SetPosition(m_player->m_position);

	if (m_player->m_effectEmitterWind->IsPlay() == false) {
		DeleteGO(m_player->m_effectEmitterWind);
		GenerateWindEffect();
	}
}

void PlayerFallInAirState::GenerateWindEffect()
{
	m_player->m_effectEmitterWind = NewGO<EffectEmitter>(0, "effectWind");
	m_player->m_effectEmitterWind->Init(0);
	m_player->m_effectEmitterWind->SetPosition(m_player->m_position);
	m_player->m_effectEmitterWind->SetRotation(m_player->m_rotation);
	
	Quaternion rot;
	rot;
	m_player->m_effectEmitterWind->SetScale(Vector3::One * 50.0f);
	m_player->m_effectEmitterWind->Play();
}
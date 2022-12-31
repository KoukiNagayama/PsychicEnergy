#include "stdafx.h"
#include "PlayerFallInAirState.h"
#include "PlayerIdleInAirState.h"
#include "PlayerIdleState.h"

void PlayerFallInAirState::Enter()
{
	// アニメーションを設定する。
	m_player->SetAnimation(Player::enAnimationClip_FallAir);

	// 移動方向を決定する。
	m_player->DecideMoveDirection();

	m_player->RotationFallAir();

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

}
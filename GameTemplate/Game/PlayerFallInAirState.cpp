#include "stdafx.h"
#include "PlayerFallInAirState.h"
#include "PlayerIdleInAirState.h"
#include "PlayerIdleState.h"

void PlayerFallInAirState::Enter(Player* player)
{
	// アニメーションを設定する。
	player->SetAnimation(Player::enAnimationClip_Idle);

	// 移動方向を決定する。
	player->DecideMoveDirection();
}

PlayerState* PlayerFallInAirState::HandleInput(Player* player)
{
	if (g_pad[0]->IsTrigger(enButtonRB1)) {
		// 空中での待機ステートに遷移する。
		return new PlayerIdleInAirState();
	}
	if (g_pad[0]->IsTrigger(enButtonLB1)) {
		// 通常の待機ステートに遷移する。
		return new PlayerIdleState();
	}
	// ここまで来たら遷移しない。
	return nullptr;
}

void PlayerFallInAirState::Update(Player* player)
{
	// 空中での移動処理。
	player->MoveOnAirspace();
}
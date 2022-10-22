#include "stdafx.h"
#include "PlayerIdleInAirState.h"
#include "PlayerIdleState.h"
#include "PlayerFallInAirState.h"

void PlayerIdleInAirState::Enter(Player* player)
{
	// 再生するアニメーションを設定する。
	player->SetAnimation(Player::enAnimationClip_Idle);
	// フラグをオブジェクトに触れていないように設定する。
	player->SetIsTouchObject(false);
}

PlayerState* PlayerIdleInAirState::HandleInput(Player* player)
{
	if (g_pad[0]->IsTrigger(enButtonRB1)) {
		// 通常の待機状態に遷移する。
		return new PlayerFallInAirState();
	}
	if (g_pad[0]->IsTrigger(enButtonLB1)) {
		// 
		return nullptr;
	}
	// ここまで来たらステートを遷移しない。
	return nullptr;
}

void PlayerIdleInAirState::Update(Player* player)
{

}

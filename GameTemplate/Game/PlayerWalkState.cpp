#include "stdafx.h"
#include "PlayerWalkState.h"
#include "PlayerIdleState.h"
#include "PlayerIdleInAirState.h"


void PlayerWalkState::Enter(Player* player)
{
	// 再生するアニメーションを設定する。
	player->SetAnimation(Player::enAnimationClip_Walk);

	// フラグをオブジェクトに触れていると設定する。
	player->SetIsTouchObject(true);
}

PlayerState* PlayerWalkState::HandleInput(Player* player)
{
	if (fabsf(g_pad[0]->GetLStickXF()) < 0.1f && fabsf(g_pad[0]->GetLStickYF()) < 0.1f) {
		// 待機ステートに遷移する。
		return new PlayerIdleState();
	}
	if (g_pad[0]->IsTrigger(enButtonRB1)) {
		// 空中での待機ステートに遷移する。
		return new PlayerIdleInAirState();
	}
	// ここまで来たらステートを遷移しない。
	return nullptr;
}

void PlayerWalkState::Update(Player* player)
{
	player->WalkOnGround();
}
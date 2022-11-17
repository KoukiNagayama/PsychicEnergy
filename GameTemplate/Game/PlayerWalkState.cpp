#include "stdafx.h"
#include "PlayerWalkState.h"
#include "PlayerIdleState.h"
#include "PlayerIdleInAirState.h"
#include "PlayerSlideState.h"


void PlayerWalkState::Enter(Player* player)
{
	// フラグをオブジェクトに触れていると設定する。
	player->SetIsTouchObject(true);

	player->ResetSlide();

}

PlayerState* PlayerWalkState::StateChange(Player* player)
{
	if (fabsf(g_pad[0]->GetLStickXF()) < 0.001f && fabsf(g_pad[0]->GetLStickYF()) < 0.001f) {
		// 待機ステートに遷移する。
		return new PlayerIdleState();
	}
	if (g_pad[0]->IsTrigger(enButtonRB1)) {
		// 空中での待機ステートに遷移する。
		return new PlayerIdleInAirState();
	}
	if (g_pad[0]->IsTrigger(enButtonLB2)) {
		// スライディングステートに遷移する。
		return new PlayerSlideState();
	}
	// ここまで来たらステートを遷移しない。
	return nullptr;
}

void PlayerWalkState::Update(Player* player)
{
	player->MoveOnGround();
}
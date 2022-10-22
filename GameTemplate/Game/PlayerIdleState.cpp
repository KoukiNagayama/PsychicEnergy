#include "stdafx.h"
#include "PlayerIdleState.h"
#include "PlayerWalkState.h"
#include "PlayerIdleInAirState.h"

PlayerIdleState::~PlayerIdleState()
{

}

void PlayerIdleState::Enter(Player* player) 
{
	// 再生するアニメーションを設定。
	player->SetAnimation(Player::enAnimationClip_Idle);

	// フラグをオブジェクトに触れているように設定する。
	player->SetIsTouchObject(true);
}

PlayerState* PlayerIdleState::HandleInput(Player* player)
{
	if (g_pad[0]->IsTrigger(enButtonRB1)) {
		// 空中でのステートに遷移する。
		return new PlayerIdleInAirState();
	}
	if (fabsf(g_pad[0]->GetLStickXF()) !=  0.0f || fabsf(g_pad[0]->GetLStickYF()) !=  0.0f) {
		// 歩きステートに遷移する。
		return new PlayerWalkState();
	}
	// ここまで来たらステートを遷移しない。
	return nullptr;

}

void PlayerIdleState::Update(Player* player)
{

}
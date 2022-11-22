#include "stdafx.h"
#include "PlayerIdleState.h"
#include "PlayerWalkState.h"
#include "PlayerJumpState.h"
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

	// スライドをリセットする。
	player->ResetSlideParam();
}

PlayerState* PlayerIdleState::StateChange(Player* player)
{
	if (g_pad[0]->IsTrigger(enButtonRB1)) {
		// 空中での待機ステートに遷移する。
		return new PlayerIdleInAirState();
	}
	if (fabsf(g_pad[0]->GetLStickXF()) >= 0.001f || fabsf(g_pad[0]->GetLStickYF()) >= 0.001f) {
		// 歩きステートに遷移する。
		return new PlayerWalkState();
	}
	if (g_pad[0]->IsTrigger(enButtonA)) {
		// ジャンプステートに遷移する。
		return new PlayerJumpState();
	}
	// ここまで来たらステートを遷移しない。
	return nullptr;

}

void PlayerIdleState::Update(Player* player)
{
	//player->WalkOnGround();
}
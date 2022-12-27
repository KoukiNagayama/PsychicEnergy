#include "stdafx.h"
#include "PlayerWalkState.h"
#include "PlayerIdleState.h"
#include "PlayerIdleInAirState.h"
#include "PlayerSlideState.h"
#include "PlayerJumpState.h"
#include "PlayerFallState.h"


void PlayerWalkState::Enter()
{
	// フラグをオブジェクトに触れていると設定する。
	m_player->SetIsTouchObject(true);

	m_player->ResetSlideParam();

}

IPlayerState* PlayerWalkState::StateChange()
{
	if (fabsf(g_pad[0]->GetLStickXF()) < 0.001f && fabsf(g_pad[0]->GetLStickYF()) < 0.001f) {
		// 待機ステートに遷移する。
		return new PlayerIdleState(m_player);
	}
	if (g_pad[0]->IsTrigger(enButtonRB1)) {
		// 空中での待機ステートに遷移する。
		return new PlayerIdleInAirState(m_player);
	}
	if (g_pad[0]->IsTrigger(enButtonLB2)) {
		// スライディングステートに遷移する。
		return new PlayerSlideState(m_player);
	}
	if (g_pad[0]->IsTrigger(enButtonA)) {
		// ジャンプステートに遷移する。
		return new PlayerJumpState(m_player);
	}
	if (m_player->IsOnGround() == false) {
		return new PlayerFallState(m_player);
	}
	// ここまで来たらステートを遷移しない。
	return nullptr;
}

void PlayerWalkState::Update()
{
	m_player->MoveOnGround();

	m_player->SelectAnimationOnGround();
}
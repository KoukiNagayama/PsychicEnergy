#include "stdafx.h"
#include "PlayerIdleState.h"
#include "PlayerWalkState.h"
#include "PlayerJumpState.h"
#include "PlayerIdleInAirState.h"
#include "PlayerFallState.h"

PlayerIdleState::~PlayerIdleState()
{

}

void PlayerIdleState::Enter() 
{
	// 再生するアニメーションを設定。
	m_player->SetAnimation(Player::enAnimationClip_Idle);

	// フラグをオブジェクトに触れているように設定する。
	m_player->SetIsTouchObject(true);

	// スライドをリセットする。
	m_player->ResetSlideParam();

	// ジャンプをリセットする。
	m_player->ResetJump();



}

IPlayerState* PlayerIdleState::StateChange()
{
	if (g_pad[0]->IsTrigger(enButtonRB1)) {
		// 空中での待機ステートに遷移する。
		return new PlayerIdleInAirState(m_player);
	}
	else if (m_player->IsOnGround()) {
		if (fabsf(g_pad[0]->GetLStickXF()) >= 0.001f || fabsf(g_pad[0]->GetLStickYF()) >= 0.001f) {
			// 歩きステートに遷移する。
			return new PlayerWalkState(m_player);
		}
		else if (g_pad[0]->IsTrigger(enButtonA)) {
			// ジャンプステートに遷移する。
			return new PlayerJumpState(m_player);
		}
	}
	if (m_player->IsOnGround() == false) {
		return new PlayerFallState(m_player);
	}
	// ここまで来たらステートを遷移しない。
	return nullptr;

}

void PlayerIdleState::Update()
{
	//player->WalkOnGround();
	// todo 変える
	m_player->m_moveSpeed.x = 0.0f;
	m_player->m_moveSpeed.z = 0.0f;
	m_player->m_moveSpeed.y -= 14.0f;
	m_player->m_position = m_player->m_charaCon.Execute(m_player->m_moveSpeed, g_gameTime->GetFrameDeltaTime());
	m_player->m_modelRender.SetPosition(m_player->m_position);

	m_player->Rotation();
}
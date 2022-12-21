#include "stdafx.h"
#include "PlayerIdleInAirState.h"
#include "PlayerIdleState.h"
#include "PlayerFallInAirState.h"

void PlayerIdleInAirState::Enter()
{
	// 再生するアニメーションを設定する。
	m_player->SetAnimation(Player::enAnimationClip_IdleAir);
	// フラグをオブジェクトに触れていないように設定する。
	m_player->SetIsTouchObject(false);
	// スライド時のパラメータをリセットする。
	m_player->ResetSlideParam();
	// 浮遊状態であると設定する。
	m_player->FloatModeChange(true);

	//player->m_moveSpeed = Vector3::Zero;
	m_player->m_moveSpeed.y = 100.0f;
}

IPlayerState* PlayerIdleInAirState::StateChange()
{
	if (g_pad[0]->IsTrigger(enButtonRB1)) {
		// 空中での落下ステートに遷移する。
		return new PlayerFallInAirState(m_player);
	}
	if (g_pad[0]->IsTrigger(enButtonLB1)) {
		// 通常の待機ステートに遷移する。
		m_player->m_modelRender.SetIsFloating(false);
		m_player->FloatModeChange(false);
		return new PlayerIdleState(m_player);
	}
	// ここまで来たらステートを遷移しない。
	return nullptr;
}

void PlayerIdleInAirState::Update()
{
	if (m_player->m_moveSpeed.y > 0.0f) {
		m_player->m_moveSpeed *= 0.97f;
		m_player->m_position = m_player->m_charaCon.Execute(m_player->m_moveSpeed, g_gameTime->GetFrameDeltaTime());
		m_player->m_modelRender.SetPosition(m_player->m_position);
	}
}

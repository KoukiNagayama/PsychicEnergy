#include "stdafx.h"
#include "PlayerIdleInAirState.h"
#include "PlayerIdleState.h"
#include "PlayerFallInAirState.h"

void PlayerIdleInAirState::Enter(Player* player)
{
	// 再生するアニメーションを設定する。
	player->SetAnimation(Player::enAnimationClip_IdleAir);
	// フラグをオブジェクトに触れていないように設定する。
	player->SetIsTouchObject(false);
	// スライド時のパラメータをリセットする。
	player->ResetSlideParam();
	// 浮遊状態であると設定する。
	player->FloatModeChange(true);

	//player->m_moveSpeed = Vector3::Zero;
	player->m_moveSpeed.y = 100.0f;
}

PlayerState* PlayerIdleInAirState::StateChange(Player* player)
{
	if (g_pad[0]->IsTrigger(enButtonRB1)) {
		// 空中での落下ステートに遷移する。
		return new PlayerFallInAirState();
	}
	if (g_pad[0]->IsTrigger(enButtonLB1)) {
		// 通常の待機ステートに遷移する。
		player->m_modelRender.SetIsFloating(false);
		return new PlayerIdleState();
	}
	// ここまで来たらステートを遷移しない。
	return nullptr;
}

void PlayerIdleInAirState::Update(Player* player)
{
	if (player->m_moveSpeed.y > 0.0f) {
		player->m_moveSpeed *= 0.97f;
		player->m_position = player->m_charaCon.Execute(player->m_moveSpeed, g_gameTime->GetFrameDeltaTime());
		player->m_modelRender.SetPosition(player->m_position);
	}
}

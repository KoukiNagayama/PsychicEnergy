#include "stdafx.h"
#include "PlayerIdleState.h"
#include "PlayerWalkState.h"
#include "PlayerJumpState.h"
#include "PlayerIdleInAirState.h"
#include "PlayerFallState.h"
#include "CommonDataForPlayer.h"

namespace
{

	const Vector3 MOVE_SPEED_TO_GRAVITY = { 0.0f, -14.0f, 0.0f };		// 移動速度。
}

/// <summary>
/// PsychicEnergyの名前空間。
/// </summary>
namespace nsPsychicEnergy
{
	/// <summary>
	/// プレイヤーの名前空間。
	/// </summary>
	namespace nsPlayer
	{

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
				if (fabsf(g_pad[0]->GetLStickXF()) >= MIN_INPUT_QUANTITY || fabsf(g_pad[0]->GetLStickYF()) >= MIN_INPUT_QUANTITY) {
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
			// 移動速度を設定し、移動させる。
			m_player->m_moveSpeed.x = MOVE_SPEED_TO_GRAVITY.x;
			m_player->m_moveSpeed.z = MOVE_SPEED_TO_GRAVITY.z;
			m_player->m_moveSpeed.y += MOVE_SPEED_TO_GRAVITY.y;
			m_player->m_position = m_player->m_charaCon.Execute(m_player->m_moveSpeed, g_gameTime->GetFrameDeltaTime());
			m_player->m_modelRender.SetPosition(m_player->m_position);

			// 回転処理。
			m_player->Rotation();

	}
}

}
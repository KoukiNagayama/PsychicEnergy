#include "stdafx.h"
#include "PlayerJumpState.h"
#include "PlayerWalkState.h"
#include "PlayerIdleState.h"
#include "PlayerIdleInAirState.h"
#include "PlayerFallState.h"
#include "CommonDataForPlayer.h"

namespace
{
	const float		ADD_MOVE_SPEED_Y = 450.0f;			// y方向への移動速度の加算値
	const int		TIME_TO_CHANGE_STATE = 0.9f;		// ステートが変化する時間
}


namespace nsPsychicEnergy
{

	namespace nsPlayer
	{
		PlayerJumpState::~PlayerJumpState()
		{
			// アニメーションスピードを修正する。
			m_player->SetAnimationSpeed(NORMAL_ANIMATION_SPEED);
		}

		void PlayerJumpState::Enter()
		{
			// y方向への移動速度を加算する。
			m_player->m_moveSpeed.y += ADD_MOVE_SPEED_Y;

			// アニメーションスピードを遅くする。
			m_player->SetAnimationSpeed(0.9f);

			// ジャンプに使用するアニメーションを選択する。
			m_player->SelectJumpAnimation();
		}

		IPlayerState* PlayerJumpState::StateChange()
		{
			// 待機ステートに遷移する。
			if (m_player->IsOnGround()) {
				return new PlayerIdleState(m_player);
			}
			// 空中での待機ステートに遷移する。
			else if (g_pad[0]->IsTrigger(enButtonRB1)) {
				return new PlayerIdleInAirState(m_player);
			}

			// 落下ステートに遷移する。
			if (m_timerToStateChange >= TIME_TO_CHANGE_STATE) {
				return new PlayerFallState(m_player);
			}
			return nullptr;
		}

		void PlayerJumpState::Update()
		{
			// ジャンプ処理。
			m_player->Jump();

			// カウントを加算する。
			m_timerToStateChange += g_gameTime->GetFrameDeltaTime();

			// 回転処理。
			m_player->Rotation();

		}
	}
}

#include "stdafx.h"
#include "PlayerIdleInAirState.h"
#include "PlayerIdleState.h"
#include "PlayerFallInAirState.h"
#include "Sight.h"
#include "GravityGauge.h"

namespace
{

	const float POWER_TO_FLOAT = 100.0f;				// 空中に浮かせる力
	const float FORCE_ATTENUATION_MULTIPLIER = 0.97f;	// 力の減衰の倍率
	const float MIN_MOVE_SPEED = 0.0f;					// 移動速度の最低値
}

namespace nsPsychicEnergy
{

	namespace nsPlayer
	{

		PlayerIdleInAirState::~PlayerIdleInAirState()
		{
			m_sight->SetIsDrawSight(false);
		}

		void PlayerIdleInAirState::Enter()
		{
			// インスタンスを検索。
			m_sight = FindGO<nsSight::Sight>("sight");
			m_gravityGauge = FindGO<nsGravityGauge::GravityGauge>("gravityGauge");

			// 照準を表示する。
			m_sight->SetIsDrawSight(true);
			// 再生するアニメーションを設定する。
			m_player->SetAnimation(Player::enAnimationClip_IdleAir);
			// フラグをオブジェクトに触れていないように設定する。
			m_player->SetIsTouchObject(false);
			// スライド時のパラメータをリセットする。
			m_player->ResetSlideParam();
			// 浮遊状態であると設定する。
			m_player->FloatModeChange(true);

			m_player->m_moveSpeed.y = POWER_TO_FLOAT;
		}

		IPlayerState* PlayerIdleInAirState::StateChange()
		{
			if (g_pad[0]->IsTrigger(enButtonRB1)) {
				// 空中での落下ステートに遷移する。
				return new PlayerFallInAirState(m_player);
			}
			if (g_pad[0]->IsTrigger(enButtonLB1)
				|| m_gravityGauge->GetDisplayAreaAngleDeg() <= 0.0f
				) {
				// 通常の待機ステートに遷移する。
				m_player->FloatModeChange(false);
				
				// 世界をリセットする。
				nsK2EngineLow::nsWorldRotation::g_worldRotation->SetIsReseting(true);
				
				// 向きを修正する。
				m_player->RotationToCorrectForward();
				return new PlayerIdleState(m_player);
			}
			// ここまで来たらステートを遷移しない。
			return nullptr;
		}

		void PlayerIdleInAirState::Update()
		{
			if (m_player->m_moveSpeed.y > MIN_MOVE_SPEED) {
				m_player->m_moveSpeed *= FORCE_ATTENUATION_MULTIPLIER;
				m_player->m_position = m_player->m_charaCon.Execute(m_player->m_moveSpeed, g_gameTime->GetFrameDeltaTime());
				m_player->m_modelRender.SetPosition(m_player->m_position);
			}
		}

	}
}


#include "stdafx.h"
#include "PlayerFallInAirState.h"
#include "PlayerIdleInAirState.h"
#include "PlayerIdleState.h"
#include "GravityGauge.h"
#include "PlayerJumpState.h"
#include "GameCamera.h"
#include "CommonDataForPlayer.h"

namespace nsPsychicEnergy
{

	namespace nsPlayer
	{


		PlayerFallInAirState::~PlayerFallInAirState()
		{
			// 風のエフェクトを削除。
			DeleteGO(m_player->m_effectEmitterWind);

			// 法線を取得しないように設定。
			nsK2EngineLow::nsWorldRotation::g_worldRotation->SetIsGetNormal(false);
		}

		void PlayerFallInAirState::Enter()
		{
			// インスタンスを検索。
			m_gravityGauge = FindGO<nsGravityGauge::GravityGauge>("gravityGauge");
			m_gameCamera = FindGO<nsGameCamera::GameCamera>("gameCamera");

			// アニメーションを設定する。
			m_player->SetAnimation(Player::enAnimationClip_FallAir);
			
			// 移動方向を決定する。
			m_player->DecideMoveDirection();
			
			// プレイヤーのモデルの向きを決める。
			m_player->RotationFallAir();
			
			// 衝突した場合に法線を取得するように設定。
			nsK2EngineLow::nsWorldRotation::g_worldRotation->SetIsGetNormal(true);
			
			// 風のエフェクトを生成。
			m_player->GenerateWindEffect();
		}

		IPlayerState* PlayerFallInAirState::StateChange()
		{
			if (g_pad[0]->IsTrigger(enButtonRB1)) {
				// 空中での待機ステートに遷移する。
				return new PlayerIdleInAirState(m_player);
			}
			if (g_pad[0]->IsTrigger(enButtonLB1)
				|| m_gravityGauge->GetDisplayAreaAngleDeg() <= MIN_REMAINING_GAUGE
				) {
				// 通常の待機ステートに遷移する。
				m_player->FloatModeChange(false);
				nsK2EngineLow::nsWorldRotation::g_worldRotation->SetIsReseting(true);
				return new PlayerIdleState(m_player);
			}
			if (m_player->IsPlayerTouchObject()) {
				// モードを変更する。
				m_player->FloatModeChange(false);
				// カメラを修正する。
				m_gameCamera->FixFront();
				return new PlayerIdleState(m_player);
			}
			// ここまで来たら遷移しない。
			return nullptr;
		}

		void PlayerFallInAirState::Update()
		{
			// 空中での移動処理。
			m_player->MoveOnAirspace();

			// 風のエフェクトの座標を更新する。
			m_player->m_effectEmitterWind->SetPosition(m_player->m_position);

			m_secToRegenerateWindEffect += g_gameTime->GetFrameDeltaTime();

			if (m_secToRegenerateWindEffect >= TIME_TO_REGENERATE_EFFECT) {
				DeleteGO(m_player->m_effectEmitterWind);
				m_player->GenerateWindEffect();
				m_secToRegenerateWindEffect = RESET_REGENERATE_EFFECT_TIMER;
			}
		}
	}
}

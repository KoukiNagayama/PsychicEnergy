#include "stdafx.h"
#include "PlayerSlideState.h"
#include "PlayerIdleState.h"
#include "PlayerJumpState.h"
#include "PlayerFallState.h"
#include "GravityGauge.h"
#include "CommonDataForPlayer.h"

namespace nsPsychicEnergy
{

	namespace nsPlayer
	{
		PlayerSlideState::~PlayerSlideState()
		{
			// 風のエフェクトを削除する。
			DeleteGO(m_player->m_effectEmitterWind);
		}

		void PlayerSlideState::Enter()
		{
			// アニメーションを設定。
			m_player->SetAnimation(Player::enAnimationClip_Slide);

			// スライディングのパラメータを初期化する。
			m_player->InitSlideParam();

			// 風のエフェクトを生成。
			m_player->GenerateWindEffect();
		}

		IPlayerState* PlayerSlideState::StateChange()
		{
			if (g_pad[0]->IsPress(enButtonLB2) == false) {
				return new PlayerIdleState(m_player);
			}
			else if (m_player->IsOnGround() == false) {
				return new PlayerFallState(m_player);
			}
			return nullptr;
		}

		void PlayerSlideState::Update()
		{
			// スライディングの移動
			m_player->Slide();
			// 回転
			m_player->Rotation();

			// 風のエフェクトの座標をプレイヤーに合わせる。
			m_player->m_effectEmitterWind->SetPosition(m_player->m_position);
			
			// 風のエフェクトを再生成するまでのタイマーを加算する。
			m_secToRegenerateWindEffect += g_gameTime->GetFrameDeltaTime();

			// タイマーが一定値を超えた場合にエフェクトを再生成する。
			if (m_secToRegenerateWindEffect >= TIME_TO_REGENERATE_EFFECT) {
				DeleteGO(m_player->m_effectEmitterWind);
				m_player->GenerateWindEffect();
				m_secToRegenerateWindEffect = RESET_REGENERATE_EFFECT_TIMER;
			}

		}



	}
}


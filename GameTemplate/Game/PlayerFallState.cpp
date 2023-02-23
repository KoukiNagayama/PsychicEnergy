#include "stdafx.h"
#include "PlayerFallState.h"
#include "PlayerIdleState.h"
#include "PlayerWalkState.h"
#include "PlayerIdleInAirState.h"
#include "CommonDataForSound.h"
#include "CommonDataForPriority.h"


namespace nsPsychicEnergy
{

	namespace nsPlayer
	{

		PlayerFallState::~PlayerFallState()
		{

		}

		void PlayerFallState::Enter()
		{
			// アニメーションを選択。
			m_player->SetAnimation(Player::enAnimationClip_NormalJump);
		}

		IPlayerState* PlayerFallState::StateChange()
		{
			// 通常の待機ステートに遷移。
			if (m_player->IsOnGround()) {
				// 着地音を生成。
				m_landingSound = NewGO<SoundSource>(nsPriority::enPriorityFirst);
				m_landingSound->Init(nsSound::enSoundNumber_PlayerLanding);
				m_landingSound->Play(false);
				return new PlayerIdleState(m_player);
			}
			// 空中での待機ステートに遷移。
			else if (g_pad[0]->IsTrigger(enButtonRB1)) {
				return new PlayerIdleInAirState(m_player);
			}
			// ここまで来たら遷移しない。
			return nullptr;
		}

		void PlayerFallState::Update()
		{
			m_player->MoveOnGround();

			m_player->Rotation();
		}
	}
}


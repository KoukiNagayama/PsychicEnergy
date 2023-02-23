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
			// �A�j���[�V������I���B
			m_player->SetAnimation(Player::enAnimationClip_NormalJump);
		}

		IPlayerState* PlayerFallState::StateChange()
		{
			// �ʏ�̑ҋ@�X�e�[�g�ɑJ�ځB
			if (m_player->IsOnGround()) {
				// ���n���𐶐��B
				m_landingSound = NewGO<SoundSource>(nsPriority::enPriorityFirst);
				m_landingSound->Init(nsSound::enSoundNumber_PlayerLanding);
				m_landingSound->Play(false);
				return new PlayerIdleState(m_player);
			}
			// �󒆂ł̑ҋ@�X�e�[�g�ɑJ�ځB
			else if (g_pad[0]->IsTrigger(enButtonRB1)) {
				return new PlayerIdleInAirState(m_player);
			}
			// �����܂ŗ�����J�ڂ��Ȃ��B
			return nullptr;
		}

		void PlayerFallState::Update()
		{
			m_player->MoveOnGround();

			m_player->Rotation();
		}
	}
}


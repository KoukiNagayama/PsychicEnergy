#include "stdafx.h"
#include "PlayerJumpState.h"
#include "PlayerWalkState.h"
#include "PlayerIdleState.h"
#include "PlayerIdleInAirState.h"
#include "PlayerFallState.h"
#include "CommonDataForPlayer.h"

namespace
{
	const float		ADD_MOVE_SPEED_Y = 450.0f;			// y�����ւ̈ړ����x�̉��Z�l
	const int		TIME_TO_CHANGE_STATE = 0.9f;		// �X�e�[�g���ω����鎞��
}


namespace nsPsychicEnergy
{

	namespace nsPlayer
	{
		PlayerJumpState::~PlayerJumpState()
		{
			// �A�j���[�V�����X�s�[�h���C������B
			m_player->SetAnimationSpeed(NORMAL_ANIMATION_SPEED);
		}

		void PlayerJumpState::Enter()
		{
			// y�����ւ̈ړ����x�����Z����B
			m_player->m_moveSpeed.y += ADD_MOVE_SPEED_Y;

			// �A�j���[�V�����X�s�[�h��x������B
			m_player->SetAnimationSpeed(0.9f);

			// �W�����v�Ɏg�p����A�j���[�V������I������B
			m_player->SelectJumpAnimation();
		}

		IPlayerState* PlayerJumpState::StateChange()
		{
			// �ҋ@�X�e�[�g�ɑJ�ڂ���B
			if (m_player->IsOnGround()) {
				return new PlayerIdleState(m_player);
			}
			// �󒆂ł̑ҋ@�X�e�[�g�ɑJ�ڂ���B
			else if (g_pad[0]->IsTrigger(enButtonRB1)) {
				return new PlayerIdleInAirState(m_player);
			}

			// �����X�e�[�g�ɑJ�ڂ���B
			if (m_timerToStateChange >= TIME_TO_CHANGE_STATE) {
				return new PlayerFallState(m_player);
			}
			return nullptr;
		}

		void PlayerJumpState::Update()
		{
			// �W�����v�����B
			m_player->Jump();

			// �J�E���g�����Z����B
			m_timerToStateChange += g_gameTime->GetFrameDeltaTime();

			// ��]�����B
			m_player->Rotation();

		}
	}
}

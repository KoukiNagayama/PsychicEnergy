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
			// ���̃G�t�F�N�g���폜����B
			DeleteGO(m_player->m_effectEmitterWind);
		}

		void PlayerSlideState::Enter()
		{
			// �A�j���[�V������ݒ�B
			m_player->SetAnimation(Player::enAnimationClip_Slide);

			// �X���C�f�B���O�̃p�����[�^������������B
			m_player->InitSlideParam();

			// ���̃G�t�F�N�g�𐶐��B
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
			// �X���C�f�B���O�̈ړ�
			m_player->Slide();
			// ��]
			m_player->Rotation();

			// ���̃G�t�F�N�g�̍��W���v���C���[�ɍ��킹��B
			m_player->m_effectEmitterWind->SetPosition(m_player->m_position);
			
			// ���̃G�t�F�N�g���Đ�������܂ł̃^�C�}�[�����Z����B
			m_secToRegenerateWindEffect += g_gameTime->GetFrameDeltaTime();

			// �^�C�}�[�����l�𒴂����ꍇ�ɃG�t�F�N�g���Đ�������B
			if (m_secToRegenerateWindEffect >= TIME_TO_REGENERATE_EFFECT) {
				DeleteGO(m_player->m_effectEmitterWind);
				m_player->GenerateWindEffect();
				m_secToRegenerateWindEffect = RESET_REGENERATE_EFFECT_TIMER;
			}

		}



	}
}


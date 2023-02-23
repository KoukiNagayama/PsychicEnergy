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
			// ���̃G�t�F�N�g���폜�B
			DeleteGO(m_player->m_effectEmitterWind);

			// �@�����擾���Ȃ��悤�ɐݒ�B
			nsK2EngineLow::nsWorldRotation::g_worldRotation->SetIsGetNormal(false);
		}

		void PlayerFallInAirState::Enter()
		{
			// �C���X�^���X�������B
			m_gravityGauge = FindGO<nsGravityGauge::GravityGauge>("gravityGauge");
			m_gameCamera = FindGO<nsGameCamera::GameCamera>("gameCamera");

			// �A�j���[�V������ݒ肷��B
			m_player->SetAnimation(Player::enAnimationClip_FallAir);
			
			// �ړ����������肷��B
			m_player->DecideMoveDirection();
			
			// �v���C���[�̃��f���̌��������߂�B
			m_player->RotationFallAir();
			
			// �Փ˂����ꍇ�ɖ@�����擾����悤�ɐݒ�B
			nsK2EngineLow::nsWorldRotation::g_worldRotation->SetIsGetNormal(true);
			
			// ���̃G�t�F�N�g�𐶐��B
			m_player->GenerateWindEffect();
		}

		IPlayerState* PlayerFallInAirState::StateChange()
		{
			if (g_pad[0]->IsTrigger(enButtonRB1)) {
				// �󒆂ł̑ҋ@�X�e�[�g�ɑJ�ڂ���B
				return new PlayerIdleInAirState(m_player);
			}
			if (g_pad[0]->IsTrigger(enButtonLB1)
				|| m_gravityGauge->GetDisplayAreaAngleDeg() <= MIN_REMAINING_GAUGE
				) {
				// �ʏ�̑ҋ@�X�e�[�g�ɑJ�ڂ���B
				m_player->FloatModeChange(false);
				nsK2EngineLow::nsWorldRotation::g_worldRotation->SetIsReseting(true);
				return new PlayerIdleState(m_player);
			}
			if (m_player->IsPlayerTouchObject()) {
				// ���[�h��ύX����B
				m_player->FloatModeChange(false);
				// �J�������C������B
				m_gameCamera->FixFront();
				return new PlayerIdleState(m_player);
			}
			// �����܂ŗ�����J�ڂ��Ȃ��B
			return nullptr;
		}

		void PlayerFallInAirState::Update()
		{
			// �󒆂ł̈ړ������B
			m_player->MoveOnAirspace();

			// ���̃G�t�F�N�g�̍��W���X�V����B
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

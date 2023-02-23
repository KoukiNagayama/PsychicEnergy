#pragma once
#include "IPlayerState.h"
/// <summary>
/// PsychicEnergy�̖��O��ԁB
/// </summary>
namespace nsPsychicEnergy
{
	// �O���錾�B
	namespace nsGameCamera { class GameCamera; }
	namespace nsGravityGauge { class GravityGauge; }

	/// <summary>
	/// �v���C���[�̖��O��ԁB
	/// </summary>
	namespace nsPlayer
	{

		/// <summary>
		/// �v���C���[��������ł����Ԃł̗����X�e�[�g
		/// </summary>
		class PlayerFallInAirState : public IPlayerState
		{
		public:
			/// <summary>
			/// �R���X�g���N�^�B
			/// </summary>
			/// <param name="player">�v���C���[�̃C���X�^���X�B</param>
			PlayerFallInAirState(Player* player) :
				IPlayerState(player) {}
			/// <summary>
			/// �f�X�g���N�^�B
			/// </summary>
			~PlayerFallInAirState() override;
			/// <summary>
			/// �X�e�[�g�J�n���̏����B
			/// </summary>
			void Enter() override;
			/// <summary>
			/// �X�e�[�g�̑J�ڏ����B
			/// </summary>
			/// <returns>�J�ڂ���X�e�[�g</returns>
			IPlayerState* StateChange() override;
			/// <summary>
			/// �X�e�[�g�ɂ�����X�V�����B
			/// </summary>
			void Update() override;
		private:
			float							m_secToRegenerateWindEffect = 0.0f;		// ���̃G�t�F�N�g���Đ�������܂ł̎��ԁB
			nsGravityGauge::GravityGauge*	m_gravityGauge = nullptr;				// �d�̓Q�[�W
			nsGameCamera::GameCamera*		m_gameCamera = nullptr;					// �Q�[���J����
		};

	}
}


#pragma once
#include "IPlayerState.h"
/// <summary>
/// PsychicEnergy�̖��O��ԁB
/// </summary>
namespace nsPsychicEnergy
{
	// �O���錾�B
	namespace nsSight { class Sight; }
	namespace nsGravityGauge { class GravityGauge; }

	/// <summary>
	/// �v���C���[�̖��O��ԁB
	/// </summary>
	namespace nsPlayer
	{
		/// <summary>
		/// �v���C���[�̋󒆂ł̑ҋ@�X�e�[�g�N���X
		/// </summary>
		class PlayerIdleInAirState : public IPlayerState
		{
		public:
			/// <summary>
			/// �R���X�g���N�^�B
			/// </summary>
			/// <param name="player">�v���C���[�̃C���X�^���X�B</param>
			PlayerIdleInAirState(Player* player) :
				IPlayerState(player) {}
			/// <summary>
			/// �f�X�g���N�^�B
			/// </summary>
			~PlayerIdleInAirState();
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
			nsSight::Sight*					m_sight = nullptr;				// �Ə�
			nsGravityGauge::GravityGauge*	m_gravityGauge = nullptr;		// �d�̓Q�[�W
		};
	}
}



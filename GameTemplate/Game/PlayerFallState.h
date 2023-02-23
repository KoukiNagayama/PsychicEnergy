#pragma once
#include "IPlayerState.h"
#include "sound/SoundSource.h"
/// <summary>
/// PsychicEnergy�̖��O��ԁB
/// </summary>
namespace nsPsychicEnergy
{
	// �O���錾�B
	namespace nsGravityGauge { class GravityGauge; }

	/// <summary>
	/// �v���C���[�̖��O��ԁB
	/// </summary>
	namespace nsPlayer
	{
		// �O���錾�B
		class Player;

		/// <summary>
		/// �v���C���[�̗����X�e�[�g�N���X
		/// </summary>
		class PlayerFallState : public IPlayerState
		{
		public:
			/// <summary>
			/// �R���X�g���N�^�B
			/// </summary>
			/// <param name="player">�v���C���[�̃C���X�^���X�B</param>
			PlayerFallState(Player* player) :
				IPlayerState(player) {}
			/// <summary>
			/// �f�X�g���N�^�B
			/// </summary>
			~PlayerFallState() override;
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
			SoundSource* m_landingSound = nullptr;		// ���n���̉����B
		};
	}
}



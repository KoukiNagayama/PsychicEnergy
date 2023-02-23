#pragma once
#include "IPlayerState.h"
/// <summary>
/// PsychicEnergy�̖��O��ԁB
/// </summary>
namespace nsPsychicEnergy
{
	/// <summary>
	/// �v���C���[�̖��O��ԁB
	/// </summary>
	namespace nsPlayer
	{
		/// <summary>
		/// �v���C���[�̒n��ł̕��s�X�e�[�g�N���X�B
		/// </summary>
		class PlayerWalkState : public IPlayerState
		{
		public:
			/// <summary>
			/// �R���X�g���N�^�B
			/// </summary>
			/// <param name="player">�v���C���[�̃C���X�^���X�B</param>
			PlayerWalkState(Player* player) :
				IPlayerState(player) {}
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
		};
	}
}



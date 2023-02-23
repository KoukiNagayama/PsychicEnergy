#pragma once
class Title;
class TitleCharacter;

/// <summary>
/// PsychicEnergy�̖��O��ԁB
/// </summary>
namespace nsPsychicEnergy
{

	/// <summary>
	/// �^�C�g���Ɋւ��閼�O��ԁB
	/// </summary>
	namespace nsTitle
	{
		// �O���錾�B
		class Title;
		class TitleCharacter;


		/// <summary>
		/// �^�C�g���J�����N���X
		/// </summary>
		class TitleCamera : public IGameObject
		{
		public:
			/// <summary>
			/// �R���X�g���N�^�B
			/// </summary>
			TitleCamera() {}
			/// <summary>
			/// �f�X�g���N�^�B
			/// </summary>
			~TitleCamera() {}
			/// <summary>
			/// �J�n�����B
			/// �C���X�^���X�������ɏ��������s���Btrue��Ԃ����Ƃ��ɌĂ΂�Ȃ��Ȃ�B
			/// </summary>
			bool Start();
			/// <summary>
			/// �X�V�����B
			/// </summary>
			void Update();
			/// <summary>
			/// �t�F�[�h�A�E�g���n�߂邩�H
			/// </summary>
			bool IsStartFadeOut()
			{
				return m_isStartFadeOut;
			}
		private:
			Title*				m_title = nullptr;								// �^�C�g���N���X
			TitleCharacter*		m_titleCharacter = nullptr;						// �^�C�g���L�����N�^�[�N���X
			Vector3				m_position = Vector3::Zero;						// �J�����̍��W
			Quaternion			m_rotation = Quaternion::Identity;				// �J�����̉�]
			Vector3				m_toCameraPos = Vector3::Zero;					// �����_���王�_�ւ̃x�N�g��
			Vector3				m_charaPos = Vector3::Zero;						// �L�����N�^�[�̍��W
			bool				m_isStartFadeOut = false;						// �t�F�[�h�A�E�g���n�߂邩
		};

	}
}


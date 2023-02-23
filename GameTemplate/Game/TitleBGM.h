#pragma once
#include "sound/SoundSource.h"

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
		/// <summary>
		/// �^�C�g���ŗ����BGM�N���X�B
		/// </summary>
		class TitleBGM : public IGameObject
		{
		public:
			/// <summary>
			/// �R���X�g���N�^�B
			/// </summary>
			TitleBGM() {}
			/// <summary>
			/// �f�X�g���N�^�B
			/// </summary>
			~TitleBGM() {}
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
			/// �t�F�[�h�A�E�g���J�n����B
			/// </summary>
			void StartFadeOut()
			{
				m_isStartFadeOut = true;
			}
		private:
			SoundSource*		m_titleBGM = nullptr;			// �^�C�g���ŗ����BGM�̉���
			float				m_volume = 0.0f;				// ����
			bool				m_isStartFadeOut = false;		// �t�F�[�h�A�E�g�����邩�H
		};
	}
}



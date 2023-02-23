#pragma once
#include "sound/SoundSource.h"

/// <summary>
/// PsychicEnergy�̖��O��ԁB
/// </summary>
namespace nsPsychicEnergy
{

	/// <summary>
	/// �T�E���h�Ɋւ��閼�O��ԁB
	/// </summary>
	namespace nsSound
	{
		/// <summary>
		/// ���C��BGM�N���X�B
		/// </summary>
		class MainBGM : public IGameObject
		{
		public:
			/// <summary>
			/// �R���X�g���N�^�B
			/// </summary>
			MainBGM() {}
			/// <summary>
			/// �f�X�g���N�^�B
			/// </summary>
			~MainBGM() {}
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
			SoundSource*	m_mainBGM = nullptr;			// �����N���X�B
			float			m_volume = 0.0f;				// ���ʁB
			bool			m_isStartFadeOut = false;		// �t�F�[�h�A�E�g���J�n���邩�H
		};

	}
}


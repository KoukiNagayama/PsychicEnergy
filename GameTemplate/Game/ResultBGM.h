#pragma once
#include "sound/SoundSource.h"

/// <summary>
/// PsychicEnergy�̖��O��ԁB
/// </summary>
namespace nsPsychicEnergy
{
	/// <summary>
	/// ���U���g�Ɋւ��閼�O��ԁB
	/// </summary>
	namespace nsResult
	{
		/// <summary>
		/// ���U���g���ɗ����BGM
		/// </summary>
		class ResultBGM : public IGameObject
		{
		public:
			/// <summary>
			/// �R���X�g���N�^�B
			/// </summary>
			ResultBGM() {}
			/// <summary>
			/// �f�X�g���N�^�B
			/// </summary>
			~ResultBGM() {}
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
			SoundSource*	m_resultBGM = nullptr;			// ���U���g���ɗ����BGM�̉���
			float			m_volume = 0.0f;				// ���ʁB
			bool			m_isStartFadeOut = false;		// �t�F�[�h�A�E�g���J�n���邩
		};
	}
}



#pragma once

namespace nsPsychicEnergy
{
	/// <summary>
	/// �t�F�[�h�̖��O���
	/// </summary>
	namespace nsFade
	{

		/// <summary>
		/// �t�F�[�h�N���X
		/// </summary>
		class Fade : public IGameObject
		{
		public:
			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			Fade();
			/// <summary>
			/// �f�X�g���N�^
			/// </summary>
			~Fade();
			/// <summary>
			/// �J�n�����B
			/// �C���X�^���X�������Ɉ�x�������s�B
			/// </summary>
			bool Start() override;
			/// <summary>
			/// �X�V�����B
			/// </summary>
			void Update() override;
			/// <summary>
			/// �`�揈���B
			/// </summary>
			/// <param name="rc">�����_�����O�R���e�L�X�g�B</param>
			void Render(RenderContext& rc) override;
			/// <summary>
			/// �t�F�[�h�C���B
			/// </summary>
			void StartFadeIn()
			{
				m_state = enFadeState_FadeIn;
			}
			/// <summary>
			/// �t�F�[�h�A�E�g�B
			/// </summary>
			void StartFadeOut()
			{
				m_state = enFadeState_FadeOut;
			}
			/// <summary>
			/// �t�F�[�h�����H
			/// </summary>
			/// <returns>�t�F�[�h���Ȃ�true�B</returns>
			const bool IsFade() const
			{
				return m_state != enFadeState_Idle;
			}
			/// <summary>
			/// ���l���擾�B
			/// </summary>
			/// <returns>���l�B</returns>
			const float GetCurrentAlpha() const
			{
				return m_currentAlpha;
			}
		private:
			/// <summary>
			/// �X�e�[�g
			/// </summary>
			enum EnFadeState {
				enFadeState_FadeIn,			//�t�F�[�h�C�����B
				enFadeState_FadeOut,		//�t�F�[�h�A�E�g���B
				enFadeState_Idle,			//�A�C�h�����B
			};

			SpriteRender			m_spriteRender;					//�X�v���C�g�����_�\�B
			EnFadeState				m_state = enFadeState_Idle;		//��ԁB
			float					m_currentAlpha = 1.0f;			//���݂̃��l�B������ύX����Ɖ摜�������ɂȂ�B
		};
	}
}




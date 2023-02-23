#pragma once
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
		/// �^�C�g���Ŏg�p����X�v���C�g�N���X�B
		/// </summary>
		class TitleSprite : public IGameObject
		{
		public:
			/// <summary>
			/// �R���X�g���N�^�B
			/// </summary>
			TitleSprite() {}
			/// <summary>
			/// �f�X�g���N�^�B
			/// </summary>
			~TitleSprite() {}
			/// <summary>
			/// �J�n�����B
			/// �C���X�^���X�������ɏ��������s���Btrue��Ԃ����Ƃ��ɌĂ΂�Ȃ��Ȃ�B
			/// </summary>
			bool Start();
			/// <summary>
			/// �`�揈���B
			/// </summary>
			/// <param name="rc"></param>
			void Render(RenderContext& rc);
		private:
			SpriteRender			m_titleLogoSprite;			// �^�C�g�����S�̃X�v���C�g
			SpriteRender			m_textSprite;				// �e�L�X�g�̃X�v���C�g
		};
	}
}



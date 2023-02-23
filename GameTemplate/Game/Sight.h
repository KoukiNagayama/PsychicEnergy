#pragma once
/// <summary>
/// PsychicEnergy�̖��O���
/// </summary>
namespace nsPsychicEnergy
{

	/// <summary>
	/// �Ə��̖��O���
	/// </summary>
	namespace nsSight
	{
		/// <summary>
		/// �Ə��N���X
		/// </summary>
		class Sight : public IGameObject
		{
		public:
			/// <summary>
			/// �R���X�g���N�^�B
			/// </summary>
			Sight() {}
			/// <summary>
			/// �f�X�g���N�^�B
			/// </summary>
			~Sight() {}
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
			/// �`�揈���B
			/// </summary>
			/// <param name="rc"></param>
			void Render(RenderContext& rc);
			/// <summary>
			/// �Ə���\�����邩�ǂ����ݒ肷��B
			/// </summary>
			/// <param name="isDrawSight">�Ə���\�����邩</param>
			void SetIsDrawSight(const bool& isDrawSight)
			{
				m_isDrawSight = isDrawSight;
			}
		private:
			SpriteRender		m_spriteRender;			// �X�v���C�g�����_�[
			float				m_currentAlpha = 0.0f;	// ���݂̃��l
			bool				m_isDrawSight = false;	// �Ə���\�����邩
		};

	}
}



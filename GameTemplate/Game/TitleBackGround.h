#pragma once
#include "CommonDataForBackGround.h"

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
		/// �^�C�g���Ŏg�p����w�i�I�u�W�F�N�g�N���X�B
		/// </summary>
		class TitleBackGround : public IGameObject
		{
		public:
			/// <summary>
			/// �R���X�g���N�^�B
			/// </summary>
			TitleBackGround() {}
			/// <summary>
			/// �f�X�g���N�^�B
			/// </summary>
			~TitleBackGround() {}
			/// <summary>
			/// �J�n�����B
			/// �C���X�^���X�������ɏ��������s���Btrue��Ԃ����Ƃ��ɌĂ΂�Ȃ��Ȃ�B
			/// </summary>
			bool Start();
			/// <summary>
			/// �`�揈���B
			/// </summary>
			/// <param name="rc">�����_�����O�R���e�L�X�g�B</param>
			void Render(RenderContext& rc);
			/// <summary>
			/// ���W��ݒ�B
			/// </summary>
			/// <param name="position">�ݒ肷����W�B</param>
			void SetPosition(Vector3& position)
			{
				m_position = position;
			}
			/// <summary>
			/// ��]��ݒ�B
			/// </summary>
			/// <param name="rotation">�ݒ肷���]�l�B</param>
			void SetRotation(Quaternion& rotation)
			{
				m_rotation = rotation;
			}
			/// <summary>
			/// �g�嗦��ݒ�B
			/// </summary>
			/// <param name="scale">�ݒ肷��g�嗦�B</param>
			void SetScale(Vector3& scale)
			{
				m_scale = scale;
			}
			/// <summary>
			/// ���f���̃^�C�v��ݒ�B
			/// </summary>
			/// <param name="num">�ݒ肷��^�C�v�̔ԍ��B</param>
			void SetTypeNum(int num)
			{
				m_typeNum = num;
			}
		private:
			ModelRender			m_modelRender;						// ���f�������_�[
			Vector3				m_position = Vector3::Zero;			// ���W
			Vector3				m_scale = Vector3::One;				// �g�嗦
			Quaternion			m_rotation = Quaternion::Identity;	// ��]
			int					m_typeNum = -1;						// ���f���̃^�C�v�ԍ�
		};
	}
}

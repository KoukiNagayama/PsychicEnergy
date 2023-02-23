#pragma once


/// <summary>
/// PsychicEnergy�̖��O���
/// </summary>
namespace nsPsychicEnergy
{
	// �O���錾
	namespace nsPlayer { class Player; }	// �v���C���[�B


	/// <summary>
	/// �w�i�I�u�W�F�N�g�̖��O���
	/// </summary>
	namespace nsBackGround
	{
		/// <summary>
		/// �w�i���f���N���X
		/// </summary>
		class BackGround : public IGameObject
		{
		public:
			/// <summary>
			/// �R���X�g���N�^�B
			/// </summary>
			BackGround() {}
			/// <summary>
			///	�f�X�g���N�^�B
			/// </summary>
			~BackGround() {}
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
			/// <summary>
			/// ���f�������_�[���擾�B
			/// </summary>
			/// <returns></returns>
			ModelRender& GetModelRender()
			{
				return m_modelRender;
			}
			/// <summary>
			/// ���[���h�s���ݒ肵�A��Ԃ��邽�ߏ����������B
			/// </summary>
			/// <param name="worldMat">��]��̃��[���h�s��B</param>
			inline void SetWorldMatrixWithLerp(const Matrix& worldMat)
			{
				m_prevMatrix = m_modelRender.GetWorldMatrix();
				m_nextMatrix = worldMat;
				m_rotateTimer = 0.0f;
			}
		private:
			ModelRender				m_modelRender;								// ���f�������_�[
			Vector3					m_position = Vector3::Zero;					// ���W
			Vector3					m_scale = Vector3::One;						// �g�嗦
			Quaternion				m_rotation = Quaternion::Identity;			// ��]
			int						m_typeNum = 10;								// ���f���̃^�C�v
			PhysicsStaticObject		m_physicsStaticObject;						// �ÓI�����I�u�W�F�N�g
			Matrix					m_worldMatrix = Matrix::Identity;			// ���[���h�s��
			nsPlayer::Player*		m_player = nullptr;							// �v���C���[�N���X
			Matrix					m_prevMatrix = Matrix::Identity;			// ���O�̃��[���h�s��
			Matrix					m_nextMatrix = Matrix::Identity;			// ���̃��[���h�s��
			float					m_rotateTimer = 1.0f;						// ��]���鎞��
		};


	}
}

#pragma once
#include "sound/SoundSource.h"
/// <summary>
/// PsychicEnergy�̖��O��ԁB
/// </summary>
namespace nsPsychicEnergy
{
	// �O���錾�B
	namespace nsGame { class Game; }
	namespace nsPlayer { class Player; }

	/// <summary>
	/// �����O�̖��O��ԁB
	/// </summary>
	namespace nsRing
	{
		/// <summary>
		/// �����O�N���X�B
		/// </summary>
		class Ring : public IGameObject
		{
		public:
			/// <summary>
			/// �R���X�g���N�^�B
			/// </summary>
			Ring() {}
			/// <summary>
			/// �f�X�g���N�^�B
			/// </summary>
			~Ring();
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
			/// ���W���擾�B
			/// </summary>
			/// <returns>���W�B</returns>
			const Vector3& GetPosition() const
			{
				return m_position;
			}
			/// <summary>
			/// ���f�������_�[���擾�B
			/// </summary>
			/// <returns>�g�p���Ă��郂�f�������_�[�B</returns>
			ModelRender& GetModelRender()
			{
				return m_modelRender;
			}
			/// <summary>
			/// ���`��Ԃ��s�����߂̖ڕW�ƂȂ郏�[���h�s���ݒ�B
			/// </summary>
			/// <param name="worldMat">�ڕW�ƂȂ郏�[���h�s��B</param>
			inline void SetWorldMatrixWithLerp(const Matrix& worldMat)
			{
				m_prevMatrix = m_modelRender.GetWorldMatrix();
				m_nextMatrix = worldMat;
				m_rotateTimer = 0.0f;
			}
			/// <summary>
			/// �����O���擾�������Ƃ�ʒm
			/// </summary>
			void NotifyGet();
		private:
			/// <summary>
			/// ��]
			/// </summary>
			void Rotation();
			/// <summary>
			/// �����蔻��
			/// </summary>
			void Collision();
			/// <summary>
			/// �v���C���[�Ƃ̋������v�Z
			/// </summary>
			float CalcDistanceToPlayer();
		private:
			nsGame::Game*			m_game = nullptr;					// �Q�[���N���X
			ModelRender				m_modelRender;						// ���f�������_�[
			Vector3					m_position;							// ���W
			Vector3					m_scale;							// �g�嗦
			Quaternion				m_rotation;							// ��]
			nsPlayer::Player*		m_player = nullptr;					// �v���C���[
			Matrix					m_prevMatrix = Matrix::Identity;	// ���O�̍s��
			Matrix					m_nextMatrix = Matrix::Identity;	// ��]��̖ڕW�ƂȂ�s��
			float					m_rotateTimer = 1.0f;				// ��]����������܂ł̃^�C�}�[
			SoundSource*			m_acquisitionSound = nullptr;		// �擾��
		};

	}
}


#pragma once
class BackGround;
class Ring;
namespace nsK2EngineLow {
	class WorldRotation
	{
	public:
		WorldRotation() {}
		~WorldRotation() {}
		/// <summary>
		/// �w�i�I�u�W�F�N�g����]����Ώۂɒǉ�
		/// </summary>
		/// <param name="modelRender"></param>
		void AddBackGround(BackGround* modelRender)
		{
			m_backGroundArray.push_back(modelRender);
		}
		/// <summary>
		/// �����O����]����Ώۂɒǉ�
		/// </summary>
		/// <param name="modelRender"></param>
		void AddRing(Ring* modelRender)
		{
			m_ringArray.push_back(modelRender);
		}
		/// <summary>
		/// �v���C���[�̃��[���h�s���o�^
		/// </summary>
		/// <param name="matrix"></param>
		void InitPlayerWorldMatrix(const Matrix& matrix)
		{
			m_playerWorldMatrix.push_back(&matrix);
		}
		/// <summary>
		/// 
		/// </summary>
		/// <param name="matrix"></param>
		void SetGroundWorldMatrix(const Matrix& matrix)
		{
			m_groundWorldMatrix = &matrix;
		}
		/// <summary>
		/// �Փ˂����@����ݒ�B
		/// </summary>
		/// <param name="hitNormal"></param>
		void SetHitNormal(const Vector3& hitNormal)
		{
			if (m_isGetNormal) {
				m_hitNormal = hitNormal;
				m_isGetNormal = false;
			}
		}
		/// <summary>
		/// �@�����擾���邩�ǂ�����ݒ�B
		/// </summary>
		/// <param name="isGetNormal"></param>
		void SetIsGetNormal(const bool& isGetNormal)
		{
			m_isGetNormal = isGetNormal;
		}
		/// <summary>
		/// ���[���h�s��̉�]
		/// </summary>
		void RotationWorldMatrix();
		/// <summary>
		/// ���[���h�s��̃��Z�b�g
		/// </summary>
		void ResetWorldMatrix();

	private:
		std::vector<const Matrix*>	m_playerWorldMatrix;					// �v���C���[�̃��[���h�s��
		Matrix		m_rotationMatrix = Matrix::Identity;					// ���E�̉�]�s��
		std::vector<BackGround*> m_backGroundArray;							// �w�i�I�u�W�F�N�g�̔z��
		std::vector<Ring*> m_ringArray;										// �����O�̔z��
		Quaternion  m_matrixRot = Quaternion::Identity;						// 
		Vector3		m_hitNormal = Vector3::Zero;							// �v���C���[���G�ꂽ�I�u�W�F�N�g�̖ʂ̖@��
		Vector3		m_lastHitNormal = Vector3::Zero;						// �Ō�ɋL�^���ꂽ�v���C���[���G�ꂽ�I�u�W�F�N�g�̖ʂ̖@��
		bool		m_isGetNormal = false;									// �Փ˂����@�����擾���邩
		const Matrix*				m_groundWorldMatrix;
		Vector3		m_groundNormal = Vector3::Up;		
	};

	extern WorldRotation* g_worldRotation;
}



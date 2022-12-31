#pragma once
namespace nsK2EngineLow {
	class WorldRotation
	{
	public:
		WorldRotation() {}
		~WorldRotation() {}
		void AddMapModelData(ModelRender& modelRender)
		{
			m_modelArray.push_back(&modelRender);
		}
		void InitPlayerModelData(const Matrix& matrix)
		{
			m_playerWorldMatrix.push_back(&matrix);
		}
		void SetHitNormal(const Vector3& hitNormal)
		{
			m_hitNormal = hitNormal;
		}

		void RotationWorldMatrix();
	private:
		std::vector<const Matrix*>	m_playerWorldMatrix;					// �v���C���[�̃��[���h�s��
		Matrix		m_rotationMatrix = Matrix::Identity;					// ���E�̉�]�s��
		std::vector<ModelRender*> m_modelArray;								// ��]����I�u�W�F�N�g�̔z��
		Quaternion  m_testRot = Quaternion::Identity;						// 
		Vector3		m_hitNormal = Vector3::Zero;							// �v���C���[���G�ꂽ�I�u�W�F�N�g�̖ʂ̖@��
		Vector3		m_lastHitNormal = Vector3::Zero;						// �Ō�ɋL�^���ꂽ�v���C���[���G�ꂽ�I�u�W�F�N�g�̖ʂ̖@��
		int			m_count = 0;
		//Quaternion  m_testRot2 = Quaternion::Identity;
	};

	extern WorldRotation* g_worldRotation;
}



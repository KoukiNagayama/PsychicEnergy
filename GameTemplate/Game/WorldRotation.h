#pragma once
class WorldRotation
{
public:
	WorldRotation() {}
	~WorldRotation() {}
	void AddMapModelData(ModelRender& modelRender)
	{
		m_modelArray.push_back(&modelRender);
	}
	void InitPlayerModelData(Matrix& matrix)
	{
		m_playerWorldMatrix.push_back(&matrix);
	}

	void RotationWorldMatrix();
private:
	std::vector<Matrix*>	m_playerWorldMatrix;					// �v���C���[�̃��[���h�s��
	Matrix		m_rotationMatrix;						// ���E�̉�]�s��
	bool		m_isGetPlayerWorldMatrix = false;		// �v���C���[�̃��[���h�s��͎擾�ς݂��H
	std::vector<ModelRender*> m_modelArray;
	Quaternion  m_testRot = Quaternion::Identity;
};

extern WorldRotation* g_worldRotation;


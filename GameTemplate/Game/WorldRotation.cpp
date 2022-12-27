#include "stdafx.h"
#include "WorldRotation.h"

void WorldRotation::RotationWorldMatrix()
{
	m_testRot.AddRotationDegY(5.0f);
	m_rotationMatrix.MakeRotationFromQuaternion(m_testRot);
	// �v���C���[�̃��[���h�s��̋t�s��
	Matrix playerWorldMatInv;
	// �v���C���[�̃��[���h�s������ƂɃ��[���h�s��̋t�s����v�Z
	playerWorldMatInv.Inverse(*m_playerWorldMatrix[0]);

	for (auto& modelMat : m_modelArray) {
		// ��]��̃��[���h���W�ϐ�
		Matrix afterRotMatrix;
		// ���[���h���W�̉�]��
		// �}�b�v�`�b�v�̃��[���h�s��(��)
		// �~�v���C���[�̃��[���h�s��̋t�s��
		// �~���E�̉�]�s��
		// �~�v���C���[�̃��[���h�s��ŋ��߂�
		afterRotMatrix.Multiply(modelMat->GetWorldMatrix(), playerWorldMatInv);
		afterRotMatrix.Multiply(afterRotMatrix, m_rotationMatrix);
		afterRotMatrix.Multiply(afterRotMatrix, *m_playerWorldMatrix[0]);

		modelMat->SetWorldMatrix(afterRotMatrix);
	}
}

WorldRotation* g_worldRotation = nullptr;

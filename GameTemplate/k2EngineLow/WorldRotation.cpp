#include "k2EngineLowPreCompile.h"
#include "WorldRotation.h"
#include "../Game/BackGround.h"
#include "../Game/Ring.h"

namespace nsK2EngineLow
{
	void WorldRotation::RotationWorldMatrix()
	{
		//m_testRot.AddRotationDegZ(1.0f);
		if (m_hitNormal.x == m_lastHitNormal.x
			&& m_hitNormal.y == m_lastHitNormal.y
			&& m_hitNormal.z == m_lastHitNormal.z
			) {
			m_ringArray.clear();
			return;
		}

		// ���ݎ擾���Ă���@�����L�^����
		m_lastHitNormal = m_hitNormal;

		// �N�H�[�^�j�I�������]�s������
		m_matrixRot.SetRotation(m_hitNormal, Vector3::Up);
		m_rotationMatrix.MakeRotationFromQuaternion(m_matrixRot);
	

		// �v���C���[�̃��[���h�s��̋t�s��
		Matrix playerWorldMatInv;
		// �v���C���[�̃��[���h�s������ƂɃ��[���h�s��̋t�s����v�Z
		//playerWorldMatInv.Inverse(*m_playerWorldMatrix[0]);
		Matrix playerTranslationMat = Matrix::Identity;
		//playerTranslationMat.v[0] = m_playerWorldMatrix[0]->v[0];
		//playerTranslationMat.v[1] = m_playerWorldMatrix[0]->v[1];
		//playerTranslationMat.v[2] = m_playerWorldMatrix[0]->v[2];
		playerTranslationMat.v[3] = m_playerWorldMatrix[0]->v[3];
		playerWorldMatInv.Inverse(playerTranslationMat);
		
		for (auto& backGround : m_backGroundArray) {
			// ��]��̃��[���h���W�ϐ�
			Matrix afterRotMatrix;
			// ���[���h���W�̉�]��
			// �}�b�v�`�b�v�̃��[���h�s��
			// �~�v���C���[�̃��[���h�s��̋t�s��
			// �~���E�̉�]�s��
			// �~�v���C���[�̃��[���h�s��ŋ��߂�
			ModelRender* modelMat = &backGround->GetModelRender();
			afterRotMatrix.Multiply(modelMat->GetWorldMatrix(), playerWorldMatInv);
			afterRotMatrix.Multiply(afterRotMatrix, m_rotationMatrix);
			afterRotMatrix.Multiply(afterRotMatrix, playerTranslationMat);
			backGround->SetWorldMatrixWithLerp(afterRotMatrix);
		}
		// ��Ɠ��l�Ƀ����O����]������
		for (auto& ring : m_ringArray) {
			Matrix afterRotMatrix;
			ModelRender* modelMat = &ring->GetModelRender();
			afterRotMatrix.Multiply(modelMat->GetWorldMatrix(), playerWorldMatInv);
			afterRotMatrix.Multiply(afterRotMatrix, m_rotationMatrix);
			afterRotMatrix.Multiply(afterRotMatrix, playerTranslationMat);
			ring->SetWorldMatrixWithLerp(afterRotMatrix);
		}
		m_ringArray.clear();
	}

	void WorldRotation::ResetWorldMatrix()
	{
		// �L�^����Ă���@�������Z�b�g����B
		m_lastHitNormal = Vector3::Zero;
		m_groundNormal.Normalize();
		m_matrixRot.SetRotation(m_groundNormal, Vector3::Up);
		m_rotationMatrix.MakeRotationFromQuaternion(m_matrixRot);

		// �v���C���[�̃��[���h�s��̋t�s��
		Matrix playerWorldMatInv;
		// �v���C���[�̃��[���h�s������ƂɃ��[���h�s��̋t�s����v�Z
		//playerWorldMatInv.Inverse(*m_playerWorldMatrix[0]);
		Matrix playerTranslationMat = Matrix::Identity;
		//playerTranslationMat.v[0] = m_playerWorldMatrix[0]->v[0];
		//playerTranslationMat.v[1] = m_playerWorldMatrix[0]->v[1];
		//playerTranslationMat.v[2] = m_playerWorldMatrix[0]->v[2];
		playerTranslationMat.v[3] = m_playerWorldMatrix[0]->v[3];
		playerWorldMatInv.Inverse(playerTranslationMat);

		for (auto& backGround : m_backGroundArray) {
			// ��]��̃��[���h���W�ϐ�
			Matrix afterRotMatrix;
			// ���[���h���W�̉�]��
			// �}�b�v�`�b�v�̃��[���h�s��
			// �~�v���C���[�̃��[���h�s��̋t�s��
			// �~���E�̉�]�s��
			// �~�v���C���[�̃��[���h�s��ŋ��߂�
			ModelRender* modelMat = &backGround->GetModelRender();
			afterRotMatrix.Multiply(modelMat->GetWorldMatrix(), playerWorldMatInv);
			afterRotMatrix.Multiply(afterRotMatrix, m_rotationMatrix);
			afterRotMatrix.Multiply(afterRotMatrix, playerTranslationMat);

			modelMat->SetWorldMatrix(afterRotMatrix);
		}

		m_groundNormal = Vector3::Up;

	}

	WorldRotation* g_worldRotation = nullptr;
}


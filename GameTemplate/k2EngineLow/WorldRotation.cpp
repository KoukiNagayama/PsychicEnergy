#include "k2EngineLowPreCompile.h"
#include "WorldRotation.h"
#include "../Game/BackGround.h"
#include "../Game/Ring.h"

namespace nsK2EngineLow
{
	void WorldRotation::Update()
	{
		m_ringArray.clear();
	}

	void WorldRotation::RotationWorldMatrix()
	{
		if (m_isReseting){
			return;
		}

		if (m_hitNormal.x == m_lastHitNormal.x
			&& m_hitNormal.y == m_lastHitNormal.y
			&& m_hitNormal.z == m_lastHitNormal.z
			) {
			//m_ringArray.clear();
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
		Matrix playerTranslationMat = Matrix::Identity;
		// �s��̉�]�����Ɗg�听�����������A���s�ړ����������𒊏o����B
		playerTranslationMat.v[3] = m_playerWorldMatrix[0]->v[3];
		playerWorldMatInv.Inverse(playerTranslationMat);
		
		for (auto& backGround : m_backGroundArray) {
			// ��]��̃��[���h���W�ϐ�
			Matrix worldMatAfterRotation;
			// ���[���h���W�̉�]��
			// �}�b�v�`�b�v�̃��[���h�s��
			// �~�v���C���[�̃��[���h�s��̋t�s��
			// �~���E�̉�]�s��
			// �~�v���C���[�̃��[���h�s��ŋ��߂�
			ModelRender* modelMat = &backGround->GetModelRender();
			worldMatAfterRotation.Multiply(modelMat->GetWorldMatrix(), playerWorldMatInv);
			worldMatAfterRotation.Multiply(worldMatAfterRotation, m_rotationMatrix);
			worldMatAfterRotation.Multiply(worldMatAfterRotation, playerTranslationMat);
			backGround->SetWorldMatrixWithLerp(worldMatAfterRotation);
		}
		// ��Ɠ��l�Ƀ����O����]������
		for (auto& ring : m_ringArray) {
			Matrix worldMatAfterRotation;
			ModelRender* modelMat = &ring->GetModelRender();
			worldMatAfterRotation.Multiply(modelMat->GetWorldMatrix(), playerWorldMatInv);
			worldMatAfterRotation.Multiply(worldMatAfterRotation, m_rotationMatrix);
			worldMatAfterRotation.Multiply(worldMatAfterRotation, playerTranslationMat);
			ring->SetWorldMatrixWithLerp(worldMatAfterRotation);
		}
		m_ringArray.clear();
		m_matrixRot.Apply(m_directionOfCurrentReference);
	}

	void WorldRotation::ResetWorldMatrix()
	{
		if (m_isReseting == false) {
			return;
		}

		// �L�^����Ă���@���������Z�b�g����B
		m_hitNormal = Vector3::Zero;
		m_lastHitNormal = Vector3::Zero;

		// ���������]
		// �N�H�[�^�j�I�������]�s������
		m_matrixRot.SetRotation(m_directionOfCurrentReference, Vector3::Up);
		m_rotationMatrix.MakeRotationFromQuaternion(m_matrixRot);

		// �v���C���[�̃��[���h�s��̋t�s��
		Matrix playerWorldMatInv;
		// �v���C���[�̃��[���h�s������ƂɃ��[���h�s��̋t�s����v�Z
		Matrix playerTranslationMat = Matrix::Identity;
		// �s��̉�]�����Ɗg�听�����������A���s�ړ����������𒊏o����B
		playerTranslationMat.v[3] = m_playerWorldMatrix[0]->v[3];
		playerWorldMatInv.Inverse(playerTranslationMat);

		for (auto& backGround : m_backGroundArray) {
			// ��]��̃��[���h���W�ϐ�
			Matrix worldMatAfterRotation;
			// ���[���h���W�̉�]��
			// �}�b�v�`�b�v�̃��[���h�s��
			// �~�v���C���[�̕��s�ړ��s��̋t�s��
			// �~���E�̉�]�s��
			// �~�v���C���[�̕��s�ړ��s��ŋ��߂�
			ModelRender* modelMat = &backGround->GetModelRender();
			worldMatAfterRotation.Multiply(modelMat->GetWorldMatrix(), playerWorldMatInv);
			worldMatAfterRotation.Multiply(worldMatAfterRotation, m_rotationMatrix);
			worldMatAfterRotation.Multiply(worldMatAfterRotation, playerTranslationMat);
			backGround->SetWorldMatrixWithLerp(worldMatAfterRotation);
		}
		// ��Ɠ��l�Ƀ����O����]������
		for (auto& ring : m_ringArray) {
			Matrix worldMatAfterRotation;
			ModelRender* modelMat = &ring->GetModelRender();
			worldMatAfterRotation.Multiply(modelMat->GetWorldMatrix(), playerWorldMatInv);
			worldMatAfterRotation.Multiply(worldMatAfterRotation, m_rotationMatrix);
			worldMatAfterRotation.Multiply(worldMatAfterRotation, playerTranslationMat);
			ring->SetWorldMatrixWithLerp(worldMatAfterRotation);
		}
		m_ringArray.clear();
		// ���Z�b�g���ł���ƃt���O��ݒ�B
		m_isReseting = false;
		m_directionOfCurrentReference = Vector3::Up;

	}

	WorldRotation* g_worldRotation = nullptr;
}


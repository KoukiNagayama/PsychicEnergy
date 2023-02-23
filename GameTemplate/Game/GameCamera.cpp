#include "stdafx.h"
#include "GameCamera.h"
#include "Player.h"

namespace
{
	const float TO_CAMERA_POS_X_FROM_TARGET = 0.0f;			// �J�����̒����_���王�_�ւ̃x�N�g����X�l
	const float TO_CAMERA_POS_Y_FROM_TARGET = 70.0f;		// �J�����̒����_���王�_�ւ̃x�N�g����Y�l
	const float TO_CAMERA_POS_Z_FROM_TARGET = 150.0f;		// �J�����̒����_���王�_�ւ̃x�N�g����Z�l
	const float FAR_CLIP = 20000.0f;						// �����ʂ܂ł̋���
	const float NEAR_CLIP = 2.0f;							// �ߕ��ʂ܂ł̋���
	const float CAMERA_MAX_MOVE_SPEED = 10000.0f;			// �J�����̈ړ��̍ō����x
	const float CAMERA_COLLISION_RADIUS = 1.5f;				// �J�����Ɏg�p����R���W�����̔��a�̒l
	const float CAMERA_UP_ROTATION_LIMIT = 70.0f;			// �J�����̏��]�̏��
	const float CAMERA_DOWN_ROTATION_LIMIT = -55.0f;		// �J�����̉���]�̏��
	const float COMPENSATION_OF_ROTATION_RANGE = 1.5f;		// �J�����̉�]�͈͂̕␳
	const float TARGET_Y = 70.0f;							// �����_�̍������グ��l
	const float TO_CAMERA_POS_CHANGE_SCALE = 1.208f;		// �����_���王�_�ւ̃x�N�g���̔{���̕ύX�l 
	const float TO_SLIDING_CAMERA_POS = 400.0f;				// �����_����X���C�f�B���O�p�J�����ւ̃x�N�g��
	const float ROTATION_LIMIT_VALUE = 0.9999f;				// ��]�l�̃��~�b�g
	const float REVERSE_VECTOR_VALUE = -1.0f;				// �x�N�g���𔽓]������l
	const float MULTIPLIER_OF_INPUT_AMOUNT_IN_X_DIRECTION = 2.5f;	// X�����̓��͗ʂ̔{��
	const float MULTIPLIER_OF_INPUT_AMOUNT_IN_Y_DIRECTION = 1.3f;	// Y�����̓��͗ʂ̔{��
	const float DISTANCE_TO_CAMERA_POS = -400.0f;			// �J�����̍��W�ւ̋���
	const float DAMPING_RATE = 0.8f;						// �΂˂̌�����
}

namespace nsPsychicEnergy
{

	namespace nsGameCamera
	{
		bool GameCamera::Start()
		{
			// �v���C���[�̏����擾
			m_player = FindGO<nsPlayer::Player>("player");

			Init();

			return true;
		}

		void GameCamera::Init()
		{
			// �J�����̒����_���王�_�ւ̃x�N�g����ݒ�B
			m_toCameraPos.Set(
				TO_CAMERA_POS_X_FROM_TARGET,
				TO_CAMERA_POS_Y_FROM_TARGET,
				TO_CAMERA_POS_Z_FROM_TARGET
			);

			//�΂˃J�������������B
			m_springCamera.Init(
				*g_camera3D,
				CAMERA_MAX_MOVE_SPEED,
				true,
				CAMERA_COLLISION_RADIUS
			);

			// �΂˃J�����̂΂˂̌�������ݒ�B
			m_springCamera.SetDampingRate(DAMPING_RATE);

			// �ߕ��ʂƉ����ʂ܂ł̋�����ݒ�B
			m_springCamera.SetFar(FAR_CLIP);
			m_springCamera.SetNear(NEAR_CLIP);


		}

		void GameCamera::Update()
		{
			// �J���������؂�ւ�
			if (m_player->IsSlide()) {
				UpdateOnSlide();
			}
			else if (m_player->IsPlayerTouchObject()) {
				UpdateTouchingObject();
			}
			else {
				UpdateOnAirspace();
			}

			// �J�������X�V�B
			m_springCamera.SetPosition(m_position);
			m_springCamera.SetTarget(m_target);
			m_springCamera.Update();
		}

		void GameCamera::UpdateTouchingObject()
		{
			if (m_isResetToCameraPosScale == false) {
				// �����_���王�_�ւ̃x�N�g�������Z�b�g����B
				m_toCameraPos /= TO_SLIDING_CAMERA_POS / sqrtf(TO_CAMERA_POS_Y_FROM_TARGET * TO_CAMERA_POS_Y_FROM_TARGET + TO_CAMERA_POS_Z_FROM_TARGET * TO_CAMERA_POS_Z_FROM_TARGET);
				m_isResetToCameraPosScale = true;
			}

			// ���݂̏����
			Vector3 currentModelUpAxis = m_player->GetCurrentModelUpAxis();
			// ��]
			//Quaternion rotation = m_player->GetRotation();
			// �J�����̏�������X�V�B
			g_camera3D->SetUp(currentModelUpAxis);

			// �����_
			m_target = m_player->GetPosition();

			// �����_���グ��@���v�����@�ǂɒ���t�����ۂɃY����\��
			m_target.y += TARGET_Y;

			// �X�V�O�̒����_���王�_�ւ̃x�N�g��
			Vector3 toCameraPosOld = m_toCameraPos;

			// �E�X�e�B�b�N��X���̓��͗�
			float x = g_pad[0]->GetRStickXF();
			// �E�X�e�B�b�N��Y���̓��͗�
			float y = g_pad[0]->GetRStickYF();

			// ���͗ʂ����]�ʂ��v�Z
			m_degreeX = x * MULTIPLIER_OF_INPUT_AMOUNT_IN_X_DIRECTION;
			m_degreeY = y * MULTIPLIER_OF_INPUT_AMOUNT_IN_Y_DIRECTION;

			// ������̎����̉�]
			Quaternion qRot;
			qRot.SetRotationDeg(currentModelUpAxis, m_degreeX);
			qRot.Apply(m_toCameraPos);

			// �E�����̎����̉�]
			Vector3 axisX;
			axisX.Cross(currentModelUpAxis, m_toCameraPos);
			axisX.Normalize();
			qRot.SetRotationDeg(axisX, m_degreeY);
			qRot.Apply(m_toCameraPos);

			// �J�����ւ̕���
			Vector3 toPosDir = m_toCameraPos;
			toPosDir.Normalize();


			// �J�����̉�]�͈͂̏���l�𒴂��Ȃ��悤�ɕ␳����
			if (toPosDir.y < -ROTATION_LIMIT_VALUE) {
				m_toCameraPos = toCameraPosOld;
			}
			else if (toPosDir.y > ROTATION_LIMIT_VALUE) {
				m_toCameraPos = toCameraPosOld;
			}

			// �ŏI�I�ȃJ�����̍��W���w�肷��B
			m_position = m_target + m_toCameraPos;
		}

		void GameCamera::UpdateOnAirspace()
		{
			// �J�����̐��ʕ���
			Vector3 forward = g_camera3D->GetForward();
			// �J�����̉E����
			Vector3 right = g_camera3D->GetRight();
			// �J�����̏����
			Vector3 cameraUpAxis;
			cameraUpAxis.Cross(forward, right);
			cameraUpAxis.Normalize();
			g_camera3D->SetUp(cameraUpAxis);

			// �����_
			m_target = m_player->GetPosition();

			// �����_���グ��@���v�����@�ǂɒ���t�����ۂɃY����\��
			m_target.y += TARGET_Y;

			float x = g_pad[0]->GetRStickXF();
			float y = g_pad[0]->GetRStickYF();

			// ���͗ʂ����]�ʂ��v�Z
			m_degreeX = x * MULTIPLIER_OF_INPUT_AMOUNT_IN_X_DIRECTION;
			m_degreeY = y * MULTIPLIER_OF_INPUT_AMOUNT_IN_Y_DIRECTION;

			// 
			Quaternion qRot;
			qRot.SetRotationDeg(cameraUpAxis, m_degreeX);
			qRot.Apply(m_toCameraPos);

			Vector3 axisX;
			axisX.Cross(cameraUpAxis, m_toCameraPos);
			axisX.Normalize();
			qRot.SetRotationDeg(axisX, m_degreeY);
			qRot.Apply(m_toCameraPos);

			Vector3 toPosDir = m_toCameraPos;
			toPosDir.Normalize();

			m_position = m_target + m_toCameraPos;
		}

		void GameCamera::UpdateOnSlide()
		{
			if (m_isResetToCameraPosScale) {
				m_isResetToCameraPosScale = false;
			}
			// ���݂̏����
			Vector3 currentModelUpAxis = m_player->GetCurrentModelUpAxis();
			// ��]
			Quaternion rotation = m_player->GetRotation();
			// �J�����̏�������X�V�B
			g_camera3D->SetUp(currentModelUpAxis);

			// �����_
			m_target = m_player->GetPosition();

			// �����_���グ��@
			m_target.y += TARGET_Y;

			// ���_�𒲐�
			m_toCameraPos = m_player->GetForward() * DISTANCE_TO_CAMERA_POS;

			// �X�V�O�̒����_���王�_�ւ̃x�N�g��
			Vector3 toCameraPosOld = m_toCameraPos;

			// �E�X�e�B�b�N��X���̓��͗�
			float x = g_pad[0]->GetRStickXF();
			// �E�X�e�B�b�N��Y���̓��͗�
			float y = g_pad[0]->GetRStickYF();

			// ���͗ʂ����]�ʂ��v�Z
			m_degreeX += x * MULTIPLIER_OF_INPUT_AMOUNT_IN_X_DIRECTION;
			m_degreeY += y * MULTIPLIER_OF_INPUT_AMOUNT_IN_Y_DIRECTION;

			// ������̎����̉�]
			Quaternion qRot;
			qRot.SetRotationDeg(currentModelUpAxis, m_degreeX);
			qRot.Apply(m_toCameraPos);

			// �E�����̎����̉�]
			Vector3 axisX;
			axisX.Cross(currentModelUpAxis, m_toCameraPos);
			axisX.Normalize();
			qRot.SetRotationDeg(axisX, m_degreeY);
			qRot.Apply(m_toCameraPos);

			// �J�����ւ̕���
			Vector3 toPosDir = m_toCameraPos;
			toPosDir.Normalize();

			// �J�����̉�]�͈͂̏���l�𒴂��Ȃ��悤�ɕ␳����
			if (toPosDir.y < -ROTATION_LIMIT_VALUE) {
				m_toCameraPos = toCameraPosOld;
			}
			else if (toPosDir.y > ROTATION_LIMIT_VALUE) {
				m_toCameraPos = toCameraPosOld;
			}
			// �ŏI�I�ȃJ�����̍��W���w�肷��B
			m_position = m_target + m_toCameraPos;
		}

		void GameCamera::FixFront()
		{
			// �v���C���[�̐��ʕ����B
			Vector3 playerForward = m_player->GetForward();

			// ���_���璍���_�ւ̃x�N�g��
			Vector3 toCameraPos = m_toCameraPos;
			// �x�N�g���𔽓]�����A���K������B
			toCameraPos.Normalize();

			// ��]�Ɏg�p����N�H�[�^�j�I��
			Quaternion qRot;
			// �N�H�[�^�j�I�����쐬���A�K�p����B
			qRot.SetRotation(toCameraPos, playerForward);
			qRot.Apply(m_toCameraPos);

			// ���̂܂܂ł̓v���C���[�̐��ʑ��ɃJ�������ړ����Ă��܂����ߔ��]������B
			m_toCameraPos *= REVERSE_VECTOR_VALUE;
			// ���W���X�V�B
			m_position = m_target + m_toCameraPos;
			m_springCamera.SetPosition(m_position);
		}

	}
}

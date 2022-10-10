#include "stdafx.h"
#include "GameCamera.h"
#include "Player.h"

namespace
{
	const float TO_CAMERA_POS_X_FROM_TARGET = 0.0f;			// �J�����̒����_���王�_�ւ̃x�N�g����X�l
	const float TO_CAMERA_POS_Y_FROM_TARGET = 100.0f;		// �J�����̒����_���王�_�ւ̃x�N�g����Y�l
	const float TO_CAMERA_POS_Z_FROM_TARGET = 300.0f;		// �J�����̒����_���王�_�ւ̃x�N�g����Z�l
	const float FAR_CLIP = 10000.0f;						// �����ʂ܂ł̋���
	const float NEAR_CLIP = 2.0f;							// �ߕ��ʂ܂ł̋���
	const float CAMERA_MAX_MOVE_SPEED = 1000.0f;			// �J�����̈ړ��̍ō����x
	const float CAMERA_COLLISION_RADIUS = 1.5f;				// �J�����Ɏg�p����R���W�����̔��a�̒l
}


bool GameCamera::Start()
{
	// �v���C���[�̏����擾
	m_player = FindGO<Player>("player");

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
		false,
		CAMERA_COLLISION_RADIUS
	);

	// �΂˃J�����̂΂˂̌�������ݒ�B
	m_springCamera.SetDampingRate(1.0f);

	// �ߕ��ʂƉ����ʂ܂ł̋�����ݒ�B
	m_springCamera.SetFar(FAR_CLIP);
	m_springCamera.SetNear(NEAR_CLIP);
}

void GameCamera::Update()
{
	bool isOnGround = m_player->IsPlayerTouchObject();

	if (isOnGround) {
		UpdateTouchingObject();
	}
	else {
		UpdateOnAirspace();
	}
}

void GameCamera::UpdateTouchingObject()
{
	// ���݂̏����
	Vector3 currentModelUpAxis = m_player->GetCurrentModelUpAxis();
	// ��]
	Quaternion rotation = m_player->GetRotation();
	// �J�����̏�������X�V�B
	g_camera3D->SetUp(currentModelUpAxis);

	// �����_
	Vector3 target = m_player->GetPosition();
	target.y += 7.0f;

	// �X�V�O�̒����_���王�_�ւ̃x�N�g��
	Vector3 toCameraPosOld = m_toCameraPos;

	// �E�X�e�B�b�N��X���̓��͗�
	float x = g_pad[0]->GetRStickXF();
	// �E�X�e�B�b�N��Y���̓��͗�
	float y = g_pad[0]->GetRStickYF();

	// ���͗ʂ����]�ʂ��v�Z
	m_degreeX += x * 2.5f;
	m_degreeY += y * 1.3f;

	// ���ΓI�ȃJ�����ƃv���C���[�̈ʒu�֌W������Ȃ��悤�ɕ␳����
	m_toCameraPos.Set(
		TO_CAMERA_POS_X_FROM_TARGET,
		TO_CAMERA_POS_Y_FROM_TARGET,
		TO_CAMERA_POS_Z_FROM_TARGET
	);
	rotation.Apply(m_toCameraPos);

	// Y�����̉�]
	Quaternion qRot;
	qRot.SetRotationDeg(currentModelUpAxis, m_degreeX);
	qRot.Apply(m_toCameraPos);

	// X�����̉�]
	Vector3 axisX;
	axisX.Cross(currentModelUpAxis, m_toCameraPos);
	axisX.Normalize();
	qRot.SetRotationDeg(axisX, m_degreeY);
	qRot.Apply(m_toCameraPos);

	// �J�����̉�]�͈͂̏���l�𒴂��Ȃ��悤�ɕ␳����
	if (m_degreeY < -50.0f) {
		m_degreeY += 1.0f;
		m_toCameraPos = toCameraPosOld;
	}
	else if (m_degreeY > 60.0f) {
		m_degreeY -= 1.0f;
		m_toCameraPos = toCameraPosOld;
	}

	// �ŏI�I�ȃJ�����̍��W���w�肷��B
	Vector3 position = target + m_toCameraPos;

	// �J�����̏����X�V�B
	m_springCamera.SetPosition(position);
	m_springCamera.SetTarget(target);
	m_springCamera.Update();
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

	Vector3 target = m_player->GetPosition();
	target.y += 7.0f;

	Vector3 toCameraPosOld = m_toCameraPos;

	float x = g_pad[0]->GetRStickXF();
	float y = g_pad[0]->GetRStickYF();

	// ���͗ʂ����]�ʂ��v�Z
	m_degreeX = x * 2.5f;
	m_degreeY = y * 1.3f;

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

	Vector3 position = target + m_toCameraPos;

	m_springCamera.SetPosition(position);
	m_springCamera.SetTarget(target);

	m_springCamera.Update();
}

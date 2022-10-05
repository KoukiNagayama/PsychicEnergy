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

	// �ߕ��ʂƉ����ʂ܂ł̋�����ݒ�B
	g_camera3D->SetFar(FAR_CLIP);
	g_camera3D->SetNear(NEAR_CLIP);

	// �΂˃J�������������B
	m_springCamera.Init(
		*g_camera3D,
		CAMERA_MAX_MOVE_SPEED,
		true,
		CAMERA_COLLISION_RADIUS
	);
}

void GameCamera::Update()
{
	Vector3 target = m_player->GetPosition();
	target.y += 7.0f;

	Vector3 toCameraPosOld = m_toCameraPos;

	float x = g_pad[0]->GetRStickXF();
	float y = g_pad[0]->GetRStickYF();

	Quaternion qRot;
	qRot.SetRotationDeg(Vector3::AxisY, 2.5f * x);
	qRot.Apply(m_toCameraPos);

	Vector3 axisX;
	axisX.Cross(Vector3::AxisY, m_toCameraPos);
	axisX.Normalize();
	qRot.SetRotationDeg(axisX, 1.3f * y);
	qRot.Apply(m_toCameraPos);

	Vector3 toPosDir = m_toCameraPos;
	toPosDir.Normalize();
	if (toPosDir.y < -2.0f) {
		m_toCameraPos = toCameraPosOld;
	}
	else if (toPosDir.y > 0.9f) {
		m_toCameraPos = toCameraPosOld;
	}

	Vector3 position = target + m_toCameraPos;

	m_springCamera.SetPosition(position);
	m_springCamera.SetTarget(target);

	m_springCamera.Update();
}

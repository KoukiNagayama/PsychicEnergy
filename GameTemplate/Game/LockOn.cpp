#include "stdafx.h"
#include "LockOn.h"
#include "Ring.h"
#include "Player.h"

namespace
{
	const float RESET_RATING_POINT = 1000000000.0f;				// �]���_�����Z�b�g����l
	const float TARGET_POS_Y_UP = 30.0f;					// �ڕW�ƂȂ���W�������̂��ߏ������W���グ��
}

bool LockOn::Start()
{
	// �S�Ẵ����O���������A�i�[���Ă���
	m_ringArray = FindGOs<Ring>("ring");
	m_player = FindGO<Player>("player");

	m_lockOnSprite.Init(
		"Assets/sprite/sight/sight.DDS",
		512.0f,
		512.0f
	);

	m_lockOnSprite.SetScale(Vector3(0.1f, 0.1f, 1.0f));
	m_lockOnSprite.Update();

	// �����̖ڕW�����߂�
	DecideTarget();

	return true;
}

void LockOn::Update()
{
	//if (m_targetRing->IsDead()) {
	//	DecideTarget();
	//}
	if (g_pad[0]->IsTrigger(enButtonRB2)) {
		DecideTarget();
	}
	CalculatePositionOfSprite();
	m_lockOnSprite.Update();

}

void LockOn::DecideTarget()
{
	// �����Ɗp�x����]���_���v�Z
	// �������߂��A�p�x�����������̂قǕ]���_���Ⴂ
	// �]���_����ԒႢ���̂����b�N�I������
	
	// �ŏ��̕]���_�����߂ɐݒ肵�Ă���
	m_minRatingPoint = RESET_RATING_POINT;
	// �J�����̐��ʕ���
	Vector3 camForward = g_camera3D->GetForward();

	for (auto& ring : m_ringArray) {
		// �J�������烊���O�։��т�x�N�g��
		Vector3 toRingPos = ring->GetPosition() - g_camera3D->GetPosition();
		// �p�x
		float angle = acosf(camForward.Dot(toRingPos));
		// ����
		float distance = toRingPos.Length();
		
		// �p�x�Ƌ�������]���_���v�Z
		float ratingPoint = angle * distance;
		if (ratingPoint <= m_minRatingPoint) {
			m_targetRing = ring;
			m_minRatingPoint = ratingPoint;
		}
	}
}

void LockOn::CalculatePositionOfSprite()
{
	Vector3 targetPos = m_targetRing->GetPosition();
	targetPos.y += TARGET_POS_Y_UP;
	g_camera3D->CalcScreenPositionFromWorldPosition(m_screenPos, targetPos);
	m_lockOnSprite.SetPosition(Vector3(m_screenPos.x, m_screenPos.y, 0.0f));
}

void LockOn::Render(RenderContext& rc)
{
	m_lockOnSprite.Draw(rc);
}

#include "stdafx.h"
#include "LockOn.h"
#include "Ring.h"
#include "Player.h"

namespace
{
	const float RESET_RATING_POINT = 1000000000.0f;						// �]���_�����Z�b�g����l
	const float RIDIAN_ANGLE_IDENTIFY_TARGET_IS_IN_VIEW = Math::PI / 4;	// �ڕW�����E���ɂ���Ǝ��ʂ���p�x(Degree�P��)
	const float HALF_HEIGHT_OF_FRAME_BUFFER = FRAME_BUFFER_H * 0.5f;	// �t���[���o�b�t�@�̏c���̔���
	const float HALF_WIDTH_OF_FRAME_BUFFER = FRAME_BUFFER_W * 0.5f;		// �t���[���o�b�t�@�̉����̔���
	const float SPACING_FROM_EDGE_IN_SCREEN_POSITION = 50.0f;			// �X�N���[�����W�ɂ������ʒ[�Ƃ̊Ԋu
}

bool LockOn::Start()
{

	m_player = FindGO<Player>("player");

	m_lockOnSprite.Init(
		"Assets/sprite/lockOn/lockOn/lockOn.DDS",
		512.0f,
		512.0f
	);
	m_lockOnSprite.SetScale(Vector3(0.3f, 0.3f, 1.0f));
	m_lockOnSprite.Update();

	m_arrowSprite.Init(
		"Assets/sprite/lockOn/arrow/arrow.DDS",
		512.0f,
		512.0f
	);
	m_arrowSprite.SetScale(Vector3(0.3f, 0.3f, 1.0f));
	m_arrowSprite.Update();

	m_camForward = g_camera3D->GetForward();
	// �����̖ڕW�����߂�
	DecideTarget();

	return true;
}

void LockOn::Update()
{
	m_camForward = g_camera3D->GetForward();
	if (m_targetRing->IsDead()
		|| g_pad[0]->IsTrigger(enButtonRB2)) {
		DecideTarget();
	}

	if (m_isDisable) {
		return;
	}

	IdentifyIfTargetIsInView();
	if (m_isTargetInView) {
		m_lockOnSprite.SetPosition(Vector3(m_screenPos.x, m_screenPos.y, 0.0f));
		m_lockOnSprite.Update();
	}
	else {
		CalculateScreenPositionOfSpriteForArrow();
	}


}

void LockOn::DecideTarget()
{
	// �S�Ẵ����O���������A�i�[���Ă���
	m_ringArray = FindGOs<Ring>("ring");
	// �����Ɗp�x����]���_���v�Z
	// �������߂��A�p�x�����������̂قǕ]���_���Ⴂ
	// �]���_����ԒႢ���̂����b�N�I������
	//if (sizeof(m_ringArray) < sizeof(Ring)) {
	//	m_isDisable = true;
	//	return;
	//}
	
	// �ŏ��̕]���_�����߂ɐݒ肵�Ă���
	m_minRatingPoint = RESET_RATING_POINT;

	for (auto& ring : m_ringArray) {
		// �J�������烊���O�։��т�x�N�g��
		Vector3 toRingPos = ring->GetPosition() - g_camera3D->GetPosition();
		// ����
		float distance = toRingPos.Length();

		toRingPos.Normalize();
		// �p�x
		float angle = acosf(m_camForward.Dot(toRingPos));

		// �p�x�Ƌ�������]���_���v�Z
		float ratingPoint = angle * distance;
		if (ratingPoint > m_minRatingPoint) {
			return;
		}
		// �]���_�Ƃ��̃����O���L�^����
		m_targetRing = ring;
		m_minRatingPoint = ratingPoint;

	}
}

void LockOn::IdentifyIfTargetIsInView()
{
	Vector3 targetPos = m_targetRing->GetPosition();
	g_camera3D->CalcScreenPositionFromWorldPosition(m_screenPos, targetPos);

	m_playerToTargetPos = targetPos - m_player->GetPosition();
	Vector3 camForward = g_camera3D->GetForward();
	m_playerToTargetPos.Normalize();
	if (camForward.Dot(m_playerToTargetPos) >= 0.0f) {
		// �X�N���[�����W�͒��S��(0,0)�̂��߃t���[���o�b�t�@�̂��ꂼ��̕��̔����̒l��p��
		if (HALF_HEIGHT_OF_FRAME_BUFFER >= m_screenPos.y
			&& -HALF_HEIGHT_OF_FRAME_BUFFER <= m_screenPos.y
			&& HALF_WIDTH_OF_FRAME_BUFFER >= m_screenPos.x
			&& -HALF_WIDTH_OF_FRAME_BUFFER <= m_screenPos.x) {
			m_isTargetInView = true;

		}
		else {
			m_isTargetInView = false;
		}
		m_isBehind = false;
	}
	else{
		m_isTargetInView = false;
		m_isBehind = true;

	}
}

void LockOn::CalculateScreenPositionOfSpriteForArrow()
{

	// x���W�̏C��
	// �ڕW�ɑ΂��ċt�����̏ꍇ�̏C��
	if (m_isBehind) {
		// ��]���ɊO���Ɍ����邽�߂ɋt�ɂ���
		m_screenPos.x = -m_screenPos.x;
		m_screenPos.y = -m_screenPos.y;

		SeekWhichEdgeIsClose();

		if (m_isNearRightEdge) {
			m_screenPos.x = HALF_WIDTH_OF_FRAME_BUFFER - SPACING_FROM_EDGE_IN_SCREEN_POSITION;
		}
		else {
			m_screenPos.x = -HALF_WIDTH_OF_FRAME_BUFFER + SPACING_FROM_EDGE_IN_SCREEN_POSITION;
		}
	}
	else {
		// ���ʌ����̏ꍇ�̏C��
		if (HALF_WIDTH_OF_FRAME_BUFFER - SPACING_FROM_EDGE_IN_SCREEN_POSITION < m_screenPos.x) {
			m_screenPos.x = HALF_WIDTH_OF_FRAME_BUFFER - SPACING_FROM_EDGE_IN_SCREEN_POSITION;
		}
		else if (-HALF_WIDTH_OF_FRAME_BUFFER + SPACING_FROM_EDGE_IN_SCREEN_POSITION > m_screenPos.x) {
			m_screenPos.x = -HALF_WIDTH_OF_FRAME_BUFFER + SPACING_FROM_EDGE_IN_SCREEN_POSITION;
		}
	}

	// y���W�̏C�����@�͓����̂���
	if (HALF_HEIGHT_OF_FRAME_BUFFER - SPACING_FROM_EDGE_IN_SCREEN_POSITION < m_screenPos.y) {
		m_screenPos.y = HALF_HEIGHT_OF_FRAME_BUFFER - SPACING_FROM_EDGE_IN_SCREEN_POSITION;
	}
	else if (-HALF_HEIGHT_OF_FRAME_BUFFER + SPACING_FROM_EDGE_IN_SCREEN_POSITION > m_screenPos.y) {
		m_screenPos.y = -HALF_HEIGHT_OF_FRAME_BUFFER + SPACING_FROM_EDGE_IN_SCREEN_POSITION;
	}

	m_arrowSprite.SetPosition(Vector3(m_screenPos.x, m_screenPos.y, 0.0f));

	// �I�u�W�F�N�g�ɖ���������
	Quaternion rot;
	rot.SetRotation(Vector3(0.0f, 1.0f, 0.0f), Vector3(m_screenPos.x, m_screenPos.y, 0.0f));
	m_arrowSprite.SetRotation(rot);

	m_arrowSprite.Update();
}

void LockOn::SeekWhichEdgeIsClose()
{
	// ���E�ǂ���̉�ʒ[�ɋ߂������߂�
	// �E
	//if (m_screenPos.x >= 0.0f
	//	&& HALF_WIDTH_OF_FRAME_BUFFER - m_screenPos.x <= HALF_HEIGHT_OF_FRAME_BUFFER - m_screenPos.y
	//		&& HALF_WIDTH_OF_FRAME_BUFFER - m_screenPos.x <= m_screenPos.x + HALF_WIDTH_OF_FRAME_BUFFER) {
	//	m_isNearRightEdge = true;
	//}
	//// ��
	//else if (m_screenPos.x < 0.0f
	//	&& HALF_WIDTH_OF_FRAME_BUFFER + m_screenPos.x <= HALF_HEIGHT_OF_FRAME_BUFFER - m_screenPos.y
	//		&& HALF_WIDTH_OF_FRAME_BUFFER + m_screenPos.x <= m_screenPos.x + HALF_WIDTH_OF_FRAME_BUFFER) {
	//	m_isNearRightEdge = false;
	//}

	if (m_playerToTargetPos.Dot(g_camera3D->GetRight()) >= 0.0f) {
		m_isNearRightEdge = true;
	}
	else {
		m_isNearRightEdge = false;
	}

}

void LockOn::Render(RenderContext& rc)
{
	if (m_isDisable) {
		// �`�悵�Ȃ�
		return;
	}
	if (m_isTargetInView) {
		m_lockOnSprite.Draw(rc);
	}
	else {
		m_arrowSprite.Draw(rc);
	}
}

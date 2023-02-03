#include "stdafx.h"
#include "TitleCamera.h"
#include "TitleCharacter.h"
#include "Title.h"

namespace
{
	const float FAR_CLIP = 10000.0f;
	const float NEAR_CLIP = 2.0f;
	const float SHIFT_TARGET_TO_RIGHT_OF_CHARACTER = -160.0f;
	const float SHIFT_TARGET_TO_UP_OF_CHARACTER = 30.0f;
	const float TO_CAMERA_POS_X_FROM_TARGET = 200.0f;		// �J�����̒����_���王�_�ւ̃x�N�g����X�l
	const float TO_CAMERA_POS_Y_FROM_TARGET = 100.0f;		// �J�����̒����_���王�_�ւ̃x�N�g����Y�l
	const float TO_CAMERA_POS_Z_FROM_TARGET = 280.0f;		// �J�����̒����_���王�_�ւ̃x�N�g����Z�l
	const float ADD_ROTATION_DEGREE = 3.0f;					
}

bool TitleCamera::Start()
{
	m_title = FindGO<Title>("title");
	// �����_�Ƃ���L�����N�^�[�������B
	m_titleCharacter = FindGO<TitleCharacter>("titleCharacter");
	// ���_�ƂȂ���W
	m_charaPos = m_titleCharacter->GetPosition();
	Vector3 targetPos = m_charaPos;
	targetPos.x += SHIFT_TARGET_TO_RIGHT_OF_CHARACTER;
	targetPos.y += SHIFT_TARGET_TO_UP_OF_CHARACTER;
	// �����_��ݒ�B
	g_camera3D->SetTarget(targetPos);

	// ���_��ݒ�B
	m_toCameraPos.Set(
		targetPos.x + TO_CAMERA_POS_X_FROM_TARGET,
		targetPos.y + TO_CAMERA_POS_Y_FROM_TARGET,
		targetPos.z + TO_CAMERA_POS_Z_FROM_TARGET
	);
	g_camera3D->SetPosition(m_toCameraPos);

	// �ߕ��ʂƉ����ʂ�ݒ�B
	g_camera3D->SetFar(FAR_CLIP);
	g_camera3D->SetNear(NEAR_CLIP);

	// �J�������X�V�B
	g_camera3D->Update();

	return true;
}

void TitleCamera::Update()
{
	m_charaPos = m_titleCharacter->GetPosition();
	if (m_charaPos.z >= m_position.z) {
		g_camera3D->SetTarget(m_charaPos);
		m_isStartFadeOut = true;
	}
}

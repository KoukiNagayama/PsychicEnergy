#include "stdafx.h"
#include "Player.h"

bool Player::Start()
{
	m_animationClips[enAnimationClip_Idle].Load("Assets/animData/idle.tka");
	m_animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Walk].Load("Assets/animData/walk.tka");
	m_animationClips[enAnimationClip_Walk].SetLoopFlag(true);

	m_model.Init(
		"Assets/modelData/unityChan.tkm",
		m_animationClips,
		enAnimationClip_Num,
		enModelUpAxisY,
		true,
		true
	);

	m_currentModelUpAxis = Vector3::AxisY;

	m_model.SetTRS(m_position, m_rotation, m_scale);
	return true;
}

void Player::Update()
{
	Move();

	TestRotation();

	m_model.Update();
}

void Player::Move()
{
	m_position.x -= g_pad[0]->GetLStickXF() * 5.0f;
	m_position.z -= g_pad[0]->GetLStickYF() * 5.0f;

	m_model.SetPosition(m_position);
}

void Player::TestRotation()
{
	if (g_pad[0]->IsTrigger(enButtonA)) {
		m_rotation.AddRotationDegX(50.0f);
		m_model.SetRotation(m_rotation);

		m_rotation.Apply(m_currentModelUpAxis);
		m_currentModelUpAxis.Normalize();
	}
}

void Player::Render(RenderContext& rc)
{
	m_model.Draw(rc);
}

#include "stdafx.h"
#include "EngineTest.h"

bool EngineTest::Start()
{
	m_animationClips[enAnimationClip_Idle].Load("Assets/animData/idle.tka");
	m_animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Walk].Load("Assets/animData/walk.tka");
	m_animationClips[enAnimationClip_Walk].SetLoopFlag(true);

	// ユニティちゃん
	m_testModel.Init("Assets/modelData/unityChan.tkm",
		m_animationClips,
		enAnimationClip_Num,
		enModelUpAxisY,
		true
	);
	m_testModel.SetTRS(m_modelPos, m_modelRot, m_modelSca);

	// 背景
	m_bgModel.Init("Assets/modelData/bg.tkm");
	m_bgModel.SetPosition(Vector3({ 0.0f,0.0f,0.0f }));

	

	return true;
}

void EngineTest::Update()
{
	Move();
	Rotation();
	CamUpdate();

	m_testModel.PlayAnimation(enAnimationClip_Idle);

	m_testModel.Update();
	m_bgModel.Update();
}

void EngineTest::Move()
{
	m_modelPos.x += g_pad[0]->GetLStickXF();
	m_modelPos.z += g_pad[0]->GetLStickYF();

	m_testModel.SetPosition(m_modelPos);
}

void EngineTest::Rotation()
{
	m_modelRot.AddRotationDegY(1.0f);
	m_testModel.SetRotation(m_modelRot);
}

void EngineTest::CamUpdate()
{
	if (g_pad[0]->IsTrigger(enButtonA)) {
		g_camera3D->SetPosition({ 0.0f, 100.0f, -100.0f });
	}
	else if (g_pad[0]->IsTrigger(enButtonB)) {
		g_camera3D->SetPosition({ 0.0f,100.0f,100.0f });
	}
}

void EngineTest::Render(RenderContext& rc)
{
	m_testModel.Draw(rc);
	m_bgModel.Draw(rc);
}

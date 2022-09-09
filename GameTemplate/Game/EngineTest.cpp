#include "stdafx.h"
#include "EngineTest.h"

bool EngineTest::Start()
{
	m_animationClips[enAnimationClip_Idle].Load("Assets/animData/idle.tka");
	m_animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Walk].Load("Assets/animData/walk.tka");
	m_animationClips[enAnimationClip_Walk].SetLoopFlag(true);

	m_testModel.Init("Assets/modelData/unityChan.tkm",
		m_animationClips,
		enAnimationClip_Num,
		enModelUpAxisY
	);

	m_testModel.SetTRS(m_modelPos, m_modelRot, m_modelSca);
	
	return true;
}

void EngineTest::Update()
{
	Move();
	m_testModel.PlayAnimation(enAnimationClip_Idle);
	m_testModel.Update();
}

void EngineTest::Move()
{
	m_modelPos.x += g_pad[0]->GetLStickXF();
	m_modelPos.z += g_pad[0]->GetLStickYF();

	m_testModel.SetPosition(m_modelPos);
}

void EngineTest::Render(RenderContext& rc)
{
	m_testModel.Draw(rc);
}

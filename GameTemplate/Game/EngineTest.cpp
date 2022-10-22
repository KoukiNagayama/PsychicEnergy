#include "stdafx.h"
#include "EngineTest.h"
#include "TestModel.h"
#include "Player.h"
#include "GameCamera.h"


bool EngineTest::Start()
{
	m_animationClips[enAnimationClip_Idle].Load("Assets/animData/idle.tka");
	m_animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Walk].Load("Assets/animData/walk.tka");
	m_animationClips[enAnimationClip_Walk].SetLoopFlag(true);

	//// ƒ†ƒjƒeƒB‚¿‚á‚ñ
	//m_unityChan.Init("Assets/modelData/unityChan.tkm",
	//	m_animationClips,
	//	enAnimationClip_Num,
	//	enModelUpAxisY,
	//	true,
	//	true
	//);
	//m_unityChan.SetTRS(m_modelPos, m_modelRot, m_modelSca);

	// ”wŒi
	/*m_bgModel.Init(
		"Assets/modelData/box.tkm",
		nullptr,
		0,
		enModelUpAxisZ,
		false,
		false,
		false
	);
	m_bgModel.SetPosition(Vector3({ 0.0f,0.0f,0.0f }));*/

	m_levelRender.Init("Assets/level3D/testLevel.tkl", [&](LevelObjectData& objData)
		{
			if (objData.EqualObjectName(L"ground") == true) {
				m_testModel0 = NewGO<TestModel>(0, "testModel0");
				m_testModel0->SetTypeNum(0);
				m_testModel0->SetPosition(objData.position);
				m_testModel0->SetRotation(objData.rotation);
				m_testModel0->SetScale(objData.scale);
				return true;
			}
			else if (objData.ForwardMatchName(L"testModel1") == true) {
				m_testModel1 = NewGO<TestModel>(0, "testModel1");
				m_testModel1->SetTypeNum(1);
				m_testModel1->SetPosition(objData.position);
				m_testModel1->SetRotation(objData.rotation);
				m_testModel1->SetScale(objData.scale);
				return true;
			}
			else if (objData.ForwardMatchName(L"testModel2") == true) {
				m_testModel2 = NewGO<TestModel>(0, "testModel2");
				m_testModel2->SetTypeNum(1);
				m_testModel2->SetPosition(objData.position);
				m_testModel2->SetRotation(objData.rotation);
				m_testModel2->SetScale(objData.scale);
				return true;
			}
			else if (objData.EqualObjectName(L"unityChan") == true) {
				m_player = NewGO<Player>(0, "player");
				m_player->SetPosition(objData.position);
				m_player->SetRotation(objData.rotation);
				m_player->SetScale(objData.scale);
				return true;
			}
			return false;
		});
	m_gameCamera = NewGO<GameCamera>(0, "gameCamera");

	//std::vector<nsK2EngineLow::TkmFile::SPolygon> polygons = m_bgModel.GetModel().GetTkmFile().GetPolygon();

	return true;
}

void EngineTest::Update()
{
	//Move();
	//Rotation();
	//CamUpdate();

	/*m_unityChan.PlayAnimation(enAnimationClip_Idle);

	m_unityChan.Update();*/
	//m_bgModel.Update();

}

void EngineTest::Move()
{
	m_modelPos.x -= g_pad[0]->GetLStickXF() * 5.0f;
	m_modelPos.z -= g_pad[0]->GetLStickYF() * 5.0f;

	m_unityChan.SetPosition(m_modelPos);
}

void EngineTest::Rotation()
{
	m_modelRot.AddRotationDegY(1.0f);
	m_unityChan.SetRotation(m_modelRot);
}

void EngineTest::CamUpdate()
{
	/*if (g_pad[0]->IsTrigger(enButtonA)) {
		g_camera3D->SetPosition({ 500.0f, 500.0f, -500.0f });
	}
	else if (g_pad[0]->IsTrigger(enButtonB)) {
		g_camera3D->SetPosition({ 0.0f,70.0f,200.0f });
	}*/
}

void EngineTest::Render(RenderContext& rc)
{
	//m_unityChan.Draw(rc);
	//
	//m_bgModel.Draw(rc);
}

#include "stdafx.h"
#include "BackGround.h"

bool BackGround::Start()
{
	// tkmファイルパスを指定。
	const char* filePath = nullptr;

	bool isShadowCaster = true;
	if (m_typeNum == 0) {
		filePath = "Assets/modelData/testStage/ground.tkm";
		isShadowCaster = false;
	}
	if (m_typeNum == 1) {
		filePath = "Assets/modelData/backGroundModel/backGroundModel_box1.tkm";
	}
	else if (m_typeNum == 2) {
		filePath = "Assets/modelData/backGroundModel/backGroundModel_box2.tkm";
	}
	else if (m_typeNum == 3) {
		filePath = "Assets/modelData/backGroundModel/backGroundModel_box3.tkm";
	}

	m_modelRender.Init(
		filePath,
		nullptr,
		0,
		enModelUpAxisZ,
		false,
		false,
		isShadowCaster,
		false
	);
	m_modelRender.SetTRS(m_position, m_rotation, m_scale);
	m_modelRender.Update();

	m_physicsStaticObject.CreateFromModel(
		m_modelRender.GetModel(),
		m_modelRender.GetModel().GetWorldMatrix()
	);

	// デバッグ用　当たり判定描画
	//PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();
	m_modelRender.Update();
	return true;
}

void BackGround::Update()
{

}

void BackGround::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}

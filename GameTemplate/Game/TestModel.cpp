#include "stdafx.h"
#include "TestModel.h"

bool TestModel::Start()
{
	// tkmファイルパスを指定。
	const char* filePath = nullptr;
	if (m_typeNum == 0) {
		filePath = "Assets/modelData/testStage/ground.tkm";
	}
	else if (m_typeNum == 1) {
		filePath = "Assets/modelData/testStage/testModel1.tkm";
	}
	else if (m_typeNum == 2) {
		filePath = "Assets/modelData/testStage/testModel2.tkm";
	}
	// モデルを初期化。
	m_model.Init(
		filePath,
		nullptr,
		0,
		enModelUpAxisZ,
		false,
		false,
		true,
		0
	);

	m_model.SetTRS(m_pos, m_rot, m_sca);
	m_model.Update();

	m_physicsStaticObject.CreateFromModel(
		m_model.GetModel(),
		m_model.GetModel().GetWorldMatrix()
	);

	// デバッグ用　当たり判定描画
	PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();

	return true;
}

void TestModel::Update()
{
	m_model.Update();
}

void TestModel::Render(RenderContext& rc)
{
	m_model.Draw(rc);
}

#include "stdafx.h"
#include "BackGround.h"
#include "WorldRotation.h"
#include "Player.h"

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

	// ワールド行列を記録しておく
	m_worldMatrix = m_modelRender.GetWorldMatrix();
	
	// デバッグ用　当たり判定描画
	//PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();
	m_modelRender.Update();

	// ワールド行列の回転をするオブジェクトを設定する。
	g_worldRotation->AddMapModelData(m_modelRender);
	// 地面のワールド行列は基準の行列として別でも記録しておく
	if (m_typeNum == 0) {
		g_worldRotation->SetGroundWorldMatrix(m_worldMatrix);
	}
	return true;
}

void BackGround::Update()
{
	// 現在のワールド行列
	Matrix nowWorldMatrix = m_modelRender.GetWorldMatrix();
	if (m_worldMatrix == nowWorldMatrix) {
		return;
	}

	// 背景が保持する当たり判定を新たなワールド行列に対して再生成する。
	m_physicsStaticObject.Release();
	m_physicsStaticObject.CreateFromModel(
		m_modelRender.GetModel(),
		m_modelRender.GetWorldMatrix()
	);
	m_worldMatrix = nowWorldMatrix;
}

void BackGround::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}

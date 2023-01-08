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
	g_worldRotation->AddBackGround(this);
	// 地面のワールド行列は基準の行列として別でも記録しておく
	if (m_typeNum == 0) {
		g_worldRotation->SetGroundWorldMatrix(m_worldMatrix);
	}

	m_player = FindGO<Player>("player");
	return true;
}

void BackGround::Update()
{
	{
		constexpr float kRotateTime = 0.7f;
		if (m_rotateTimer < kRotateTime)
		{
			m_rotateTimer += g_gameTime->GetFrameDeltaTime();
			m_rotateTimer = min(m_rotateTimer, kRotateTime);

			const float rate = m_rotateTimer / kRotateTime;
			Matrix mat;
			mat.v[0].Lerp(rate, m_prevMatrix.v[0], m_nextMatrix.v[0]);
			mat.v[1].Lerp(rate, m_prevMatrix.v[1], m_nextMatrix.v[1]);
			mat.v[2].Lerp(rate, m_prevMatrix.v[2], m_nextMatrix.v[2]);
			mat.v[3].Lerp(rate, m_prevMatrix.v[3], m_nextMatrix.v[3]);

			m_modelRender.SetWorldMatrix(mat);
		}
	}
	// 現在のワールド行列が記憶されたワールド行列と一致する場合動作させない
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

	m_player->SetIsTouchObject(true);
}

void BackGround::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}

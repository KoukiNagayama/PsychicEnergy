#include "stdafx.h"
#include "Ring.h"

bool Ring::Start()
{
	ModelInitData modelInitData;
	modelInitData.m_tkmFilePath = "Assets/modelData/ring/ring.tkm";
	modelInitData.m_fxFilePath = "Assets/shader/ring.fx";
	modelInitData.m_vsEntryPointFunc = "VSMain";
	modelInitData.m_psEntryPointFunc = "PSMain";
	modelInitData.m_modelUpAxis = enModelUpAxisZ;

	m_modelRender.InitDirectlyNotifyForwardRendering(modelInitData);
	//// モデルを初期化
	//m_modelRender.Init(
	//	"Assets/modelData/ring/ring.tkm",
	//	nullptr,
	//	0,
	//	enModelUpAxisZ,
	//	false,
	//	false,
	//	false,
	//	false
	//);

	m_modelRender.SetTRS(m_position, m_rotation, m_scale);
	m_modelRender.Update();

	// ワールド行列を記録しておく
	//m_worldMatrix = m_modelRender.GetWorldMatrix();

	// デバッグ用　当たり判定描画
	//PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();
	m_modelRender.Update();

	// ワールド行列の回転をするオブジェクトを設定する。
	//g_worldRotation->AddBackGround(this);
	//// 地面のワールド行列は基準の行列として別でも記録しておく
	//if (m_typeNum == 0) {
	//	g_worldRotation->SetGroundWorldMatrix(m_worldMatrix);
	//}

	//m_player = FindGO<Player>("player");
	return true;
}

void Ring::Update()
{
	
}

void Ring::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}
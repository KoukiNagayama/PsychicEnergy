#include "stdafx.h"
#include "Ring.h"
#include "Player.h"

namespace
{
	const float DIST_TO_JUDGE_TRUE = 100.0f;
}

bool Ring::Start()
{
	// モデルの初期化データ
	ModelInitData modelInitData;
	modelInitData.m_tkmFilePath = "Assets/modelData/ring/ring.tkm";
	modelInitData.m_fxFilePath = "Assets/shader/ring.fx";
	modelInitData.m_vsEntryPointFunc = "VSMain";
	modelInitData.m_psEntryPointFunc = "PSMain";
	modelInitData.m_modelUpAxis = enModelUpAxisZ;
	// 初期化データをもとにモデルを初期化
	m_modelRender.InitDirectlyNotifyForwardRendering(modelInitData);

	// モデルの座標、回転、拡大率を設定。
	m_modelRender.SetTRS(m_position, m_rotation, m_scale);
	m_modelRender.Update();

	m_player = FindGO<Player>("player");
	return true;
}

void Ring::Update()
{
	Collision();

	Rotation();
}

void Ring::Rotation()
{
	constexpr float kRotateTime = 0.1f;
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
		m_position.x = mat.m[3][0];
		m_position.y = mat.m[3][1];
		m_position.z = mat.m[3][2];
	}
}

void Ring::Collision()
{
	if (DIST_TO_JUDGE_TRUE * DIST_TO_JUDGE_TRUE < CalcDistanceToPlayer()) {
		g_worldRotation->AddRing(this);
		return;
	}

	DeleteGO(this);
}

float Ring::CalcDistanceToPlayer()
{
	// 距離
	Vector3 distance;
	distance = m_position - m_player->GetPosition();

	return distance.LengthSq();
}

void Ring::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}
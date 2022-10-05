#include "stdafx.h"
#include "GameCamera.h"
#include "Player.h"

namespace
{
	const float TO_CAMERA_POS_X_FROM_TARGET = 0.0f;			// カメラの注視点から視点へのベクトルのX値
	const float TO_CAMERA_POS_Y_FROM_TARGET = 100.0f;		// カメラの注視点から視点へのベクトルのY値
	const float TO_CAMERA_POS_Z_FROM_TARGET = 300.0f;		// カメラの注視点から視点へのベクトルのZ値
	const float FAR_CLIP = 10000.0f;						// 遠平面までの距離
	const float NEAR_CLIP = 2.0f;							// 近平面までの距離
	const float CAMERA_MAX_MOVE_SPEED = 1000.0f;			// カメラの移動の最高速度
	const float CAMERA_COLLISION_RADIUS = 1.5f;				// カメラに使用するコリジョンの半径の値
}


bool GameCamera::Start()
{
	// プレイヤーの情報を取得
	m_player = FindGO<Player>("player");

	Init();

	return true;
}

void GameCamera::Init()
{
	// カメラの注視点から視点へのベクトルを設定。
	m_toCameraPos.Set(
		TO_CAMERA_POS_X_FROM_TARGET,
		TO_CAMERA_POS_Y_FROM_TARGET,
		TO_CAMERA_POS_Z_FROM_TARGET
	);

	// 近平面と遠平面までの距離を設定。
	g_camera3D->SetFar(FAR_CLIP);
	g_camera3D->SetNear(NEAR_CLIP);

	// ばねカメラを初期化。
	m_springCamera.Init(
		*g_camera3D,
		CAMERA_MAX_MOVE_SPEED,
		true,
		CAMERA_COLLISION_RADIUS
	);
}

void GameCamera::Update()
{
	Vector3 target = m_player->GetPosition();
	target.y += 7.0f;

	Vector3 toCameraPosOld = m_toCameraPos;

	float x = g_pad[0]->GetRStickXF();
	float y = g_pad[0]->GetRStickYF();

	Quaternion qRot;
	qRot.SetRotationDeg(Vector3::AxisY, 2.5f * x);
	qRot.Apply(m_toCameraPos);

	Vector3 axisX;
	axisX.Cross(Vector3::AxisY, m_toCameraPos);
	axisX.Normalize();
	qRot.SetRotationDeg(axisX, 1.3f * y);
	qRot.Apply(m_toCameraPos);

	Vector3 toPosDir = m_toCameraPos;
	toPosDir.Normalize();
	if (toPosDir.y < -2.0f) {
		m_toCameraPos = toCameraPosOld;
	}
	else if (toPosDir.y > 0.9f) {
		m_toCameraPos = toCameraPosOld;
	}

	Vector3 position = target + m_toCameraPos;

	m_springCamera.SetPosition(position);
	m_springCamera.SetTarget(target);

	m_springCamera.Update();
}

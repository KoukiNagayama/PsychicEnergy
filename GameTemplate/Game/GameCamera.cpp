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

	//ばねカメラを初期化。
	m_springCamera.Init(
		*g_camera3D,
		CAMERA_MAX_MOVE_SPEED,
		false,
		CAMERA_COLLISION_RADIUS
	);

	// ばねカメラのばねの減衰率を設定。
	m_springCamera.SetDampingRate(1.0f);

	// 近平面と遠平面までの距離を設定。
	m_springCamera.SetFar(FAR_CLIP);
	m_springCamera.SetNear(NEAR_CLIP);
}

void GameCamera::Update()
{
	bool isOnGround = m_player->IsPlayerTouchObject();

	if (isOnGround) {
		UpdateTouchingObject();
	}
	else {
		UpdateOnAirspace();
	}
}

void GameCamera::UpdateTouchingObject()
{
	// 現在の上方向
	Vector3 currentModelUpAxis = m_player->GetCurrentModelUpAxis();
	// 回転
	Quaternion rotation = m_player->GetRotation();
	// カメラの上方向を更新。
	g_camera3D->SetUp(currentModelUpAxis);

	// 注視点
	Vector3 target = m_player->GetPosition();
	target.y += 7.0f;

	// 更新前の注視点から視点へのベクトル
	Vector3 toCameraPosOld = m_toCameraPos;

	// 右スティックのX軸の入力量
	float x = g_pad[0]->GetRStickXF();
	// 右スティックのY軸の入力量
	float y = g_pad[0]->GetRStickYF();

	// 入力量から回転量を計算
	m_degreeX += x * 2.5f;
	m_degreeY += y * 1.3f;

	// 相対的なカメラとプレイヤーの位置関係がずれないように補正する
	m_toCameraPos.Set(
		TO_CAMERA_POS_X_FROM_TARGET,
		TO_CAMERA_POS_Y_FROM_TARGET,
		TO_CAMERA_POS_Z_FROM_TARGET
	);
	rotation.Apply(m_toCameraPos);

	// Y軸回りの回転
	Quaternion qRot;
	qRot.SetRotationDeg(currentModelUpAxis, m_degreeX);
	qRot.Apply(m_toCameraPos);

	// X軸回りの回転
	Vector3 axisX;
	axisX.Cross(currentModelUpAxis, m_toCameraPos);
	axisX.Normalize();
	qRot.SetRotationDeg(axisX, m_degreeY);
	qRot.Apply(m_toCameraPos);

	// カメラの回転範囲の上限値を超えないように補正する
	if (m_degreeY < -50.0f) {
		m_degreeY += 1.0f;
		m_toCameraPos = toCameraPosOld;
	}
	else if (m_degreeY > 60.0f) {
		m_degreeY -= 1.0f;
		m_toCameraPos = toCameraPosOld;
	}

	// 最終的なカメラの座標を指定する。
	Vector3 position = target + m_toCameraPos;

	// カメラの情報を更新。
	m_springCamera.SetPosition(position);
	m_springCamera.SetTarget(target);
	m_springCamera.Update();
}

void GameCamera::UpdateOnAirspace()
{
	// カメラの正面方向
	Vector3 forward = g_camera3D->GetForward();
	// カメラの右方向
	Vector3 right = g_camera3D->GetRight();
	// カメラの上方向
	Vector3 cameraUpAxis;
	cameraUpAxis.Cross(forward, right);
	cameraUpAxis.Normalize();
	g_camera3D->SetUp(cameraUpAxis);

	Vector3 target = m_player->GetPosition();
	target.y += 7.0f;

	Vector3 toCameraPosOld = m_toCameraPos;

	float x = g_pad[0]->GetRStickXF();
	float y = g_pad[0]->GetRStickYF();

	// 入力量から回転量を計算
	m_degreeX = x * 2.5f;
	m_degreeY = y * 1.3f;

	Quaternion qRot;
	qRot.SetRotationDeg(cameraUpAxis, m_degreeX);
	qRot.Apply(m_toCameraPos);

	Vector3 axisX;
	axisX.Cross(cameraUpAxis, m_toCameraPos);
	axisX.Normalize();
	qRot.SetRotationDeg(axisX, m_degreeY);
	qRot.Apply(m_toCameraPos);

	Vector3 toPosDir = m_toCameraPos;
	toPosDir.Normalize();

	Vector3 position = target + m_toCameraPos;

	m_springCamera.SetPosition(position);
	m_springCamera.SetTarget(target);

	m_springCamera.Update();
}

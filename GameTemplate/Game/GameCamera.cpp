#include "stdafx.h"
#include "GameCamera.h"
#include "Player.h"

namespace
{
	const float TO_CAMERA_POS_X_FROM_TARGET = 0.0f;			// カメラの注視点から視点へのベクトルのX値
	const float TO_CAMERA_POS_Y_FROM_TARGET = 70.0f;		// カメラの注視点から視点へのベクトルのY値
	const float TO_CAMERA_POS_Z_FROM_TARGET = 150.0f;		// カメラの注視点から視点へのベクトルのZ値
	const float FAR_CLIP = 15000.0f;						// 遠平面までの距離
	const float NEAR_CLIP = 2.0f;							// 近平面までの距離
	const float CAMERA_MAX_MOVE_SPEED = 10000.0f;			// カメラの移動の最高速度
	const float CAMERA_COLLISION_RADIUS = 1.5f;				// カメラに使用するコリジョンの半径の値
	const float CAMERA_UP_ROTATION_LIMIT = 70.0f;			// カメラの上回転の上限
	const float CAMERA_DOWN_ROTATION_LIMIT = -55.0f;		// カメラの下回転の上限
	const float CAMERA_RIGHT_ROTATION_LIMIT = 90.0f;
	const float CAMERA_LEFT_ROTATION_LIMIT = -90.0f;
	const float COMPENSATION_OF_ROTATION_RANGE = 1.5f;		// カメラの回転範囲の補正
	const float TARGET_Y = 70.0f;							// 注視点の高さを上げる値
	const float TO_CAMERA_POS_CHANGE_SCALE = 1.208f;		// 注視点から視点へのベクトルの倍率の変更値 
	const float TO_SLIDING_CAMERA_POS = 400.0f;				// 注視点からスライディング用カメラへのベクトル
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
		true,
		CAMERA_COLLISION_RADIUS
	);

	// ばねカメラのばねの減衰率を設定。
	m_springCamera.SetDampingRate(0.8f);

	// 近平面と遠平面までの距離を設定。
	m_springCamera.SetFar(FAR_CLIP);
	m_springCamera.SetNear(NEAR_CLIP);


}

void GameCamera::Update()
{

	// カメラ操作を切り替え
	if (m_player->IsSlide()) {
		UpdateOnSlide();
	}
	else if (m_player->IsPlayerTouchObject()) {
		UpdateTouchingObject();
	}
	else {
		UpdateOnAirspace();
	}

	// カメラを更新。
	m_springCamera.SetPosition(m_position);
	m_springCamera.SetTarget(m_target);
	m_springCamera.Update();
}

void GameCamera::UpdateTouchingObject()
{
	if (m_isResetToCameraPosScale == false) {
		//m_springCamera.SetDampingRate(1.3f);
		// 注視点から視点へのベクトルをリセットする。
		m_toCameraPos /= TO_SLIDING_CAMERA_POS / sqrtf(TO_CAMERA_POS_Y_FROM_TARGET * TO_CAMERA_POS_Y_FROM_TARGET + TO_CAMERA_POS_Z_FROM_TARGET * TO_CAMERA_POS_Z_FROM_TARGET);
		m_isResetToCameraPosScale = true;
	}

	// 現在の上方向
	Vector3 currentModelUpAxis = m_player->GetCurrentModelUpAxis();
	// 回転
	//Quaternion rotation = m_player->GetRotation();
	// カメラの上方向を更新。
	g_camera3D->SetUp(currentModelUpAxis);

	// 注視点
	m_target = m_player->GetPosition();

	// 注視点を上げる　※要調整　壁に張り付いた際にズレる可能性
	m_target.y += TARGET_Y;

	// 更新前の注視点から視点へのベクトル
	Vector3 toCameraPosOld = m_toCameraPos;

	// 右スティックのX軸の入力量
	float x = g_pad[0]->GetRStickXF();
	// 右スティックのY軸の入力量
	float y = g_pad[0]->GetRStickYF();

	// 入力量から回転量を計算
	m_degreeX = x * 2.5f;
	m_degreeY = y * 1.3f;

	// 相対的にカメラとプレイヤーの位置関係がずれないように補正する
	//m_toCameraPos.Set(
	//	TO_CAMERA_POS_X_FROM_TARGET,
	//	TO_CAMERA_POS_Y_FROM_TARGET,
	//	TO_CAMERA_POS_Z_FROM_TARGET
	//);

	//rotation.Apply(m_toCameraPos);

	// 上向きの軸回りの回転
	Quaternion qRot;
	qRot.SetRotationDeg(currentModelUpAxis, m_degreeX);
	qRot.Apply(m_toCameraPos);

	// 右向きの軸回りの回転
	Vector3 axisX;
	axisX.Cross(currentModelUpAxis, m_toCameraPos);
	axisX.Normalize();
	qRot.SetRotationDeg(axisX, m_degreeY);
	qRot.Apply(m_toCameraPos);

	// カメラの回転範囲の上限値を超えないように補正する
	if (m_degreeY < CAMERA_DOWN_ROTATION_LIMIT) {
		m_degreeY += COMPENSATION_OF_ROTATION_RANGE;
		m_toCameraPos = toCameraPosOld;
	}
	else if (m_degreeY > CAMERA_UP_ROTATION_LIMIT) {
		m_degreeY -= COMPENSATION_OF_ROTATION_RANGE;
		m_toCameraPos = toCameraPosOld;
	}

	// 最終的なカメラの座標を指定する。
	m_position = m_target + m_toCameraPos;
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

	// 注視点
	m_target = m_player->GetPosition();
	
	// 注視点を上げる　※要調整　壁に張り付いた際にズレる可能性
	m_target.y += TARGET_Y;

	float x = g_pad[0]->GetRStickXF();
	float y = g_pad[0]->GetRStickYF();

	// 入力量から回転量を計算
	m_degreeX = x * 2.5f;
	m_degreeY = y * 1.3f;

	// 
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

	m_position = m_target + m_toCameraPos;
}

void GameCamera::UpdateOnSlide()
{
	if (m_isResetToCameraPosScale) {
		m_isResetToCameraPosScale = false;
	}
	// 現在の上方向
	Vector3 currentModelUpAxis = m_player->GetCurrentModelUpAxis();
	// 回転
	Quaternion rotation = m_player->GetRotation();
	// カメラの上方向を更新。
	g_camera3D->SetUp(currentModelUpAxis);

	// 注視点
	m_target = m_player->GetPosition();

	// 注視点を上げる　※要調整　壁に張り付いた際にズレる可能性
	m_target.y += TARGET_Y;

	m_toCameraPos = m_player->GetForward() * -400.0f;

	// 更新前の注視点から視点へのベクトル
	Vector3 toCameraPosOld = m_toCameraPos;

	// 右スティックのX軸の入力量
	float x = g_pad[0]->GetRStickXF();
	// 右スティックのY軸の入力量
	float y = g_pad[0]->GetRStickYF();

	// 入力量から回転量を計算
	m_degreeX += x * 2.5f;
	m_degreeY += y * 1.3f;

	// 上向きの軸回りの回転
	Quaternion qRot;
	qRot.SetRotationDeg(currentModelUpAxis, m_degreeX);
	qRot.Apply(m_toCameraPos);

	// 右向きの軸回りの回転
	Vector3 axisX;
	axisX.Cross(currentModelUpAxis, m_toCameraPos);
	axisX.Normalize();
	qRot.SetRotationDeg(axisX, m_degreeY);
	qRot.Apply(m_toCameraPos);

	// カメラの回転範囲の上限値を超えないように補正する
	if (m_degreeY < CAMERA_DOWN_ROTATION_LIMIT) {
		m_degreeY += COMPENSATION_OF_ROTATION_RANGE;
		m_toCameraPos = toCameraPosOld;
	}
	else if (m_degreeY > CAMERA_UP_ROTATION_LIMIT) {
		m_degreeY -= COMPENSATION_OF_ROTATION_RANGE;
		m_toCameraPos = toCameraPosOld;
	}

	// 最終的なカメラの座標を指定する。
	m_position = m_target + m_toCameraPos;
}

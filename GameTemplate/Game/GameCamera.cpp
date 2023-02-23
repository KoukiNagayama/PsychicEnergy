#include "stdafx.h"
#include "GameCamera.h"
#include "Player.h"

namespace
{
	const float TO_CAMERA_POS_X_FROM_TARGET = 0.0f;			// カメラの注視点から視点へのベクトルのX値
	const float TO_CAMERA_POS_Y_FROM_TARGET = 70.0f;		// カメラの注視点から視点へのベクトルのY値
	const float TO_CAMERA_POS_Z_FROM_TARGET = 150.0f;		// カメラの注視点から視点へのベクトルのZ値
	const float FAR_CLIP = 20000.0f;						// 遠平面までの距離
	const float NEAR_CLIP = 2.0f;							// 近平面までの距離
	const float CAMERA_MAX_MOVE_SPEED = 10000.0f;			// カメラの移動の最高速度
	const float CAMERA_COLLISION_RADIUS = 1.5f;				// カメラに使用するコリジョンの半径の値
	const float CAMERA_UP_ROTATION_LIMIT = 70.0f;			// カメラの上回転の上限
	const float CAMERA_DOWN_ROTATION_LIMIT = -55.0f;		// カメラの下回転の上限
	const float COMPENSATION_OF_ROTATION_RANGE = 1.5f;		// カメラの回転範囲の補正
	const float TARGET_Y = 70.0f;							// 注視点の高さを上げる値
	const float TO_CAMERA_POS_CHANGE_SCALE = 1.208f;		// 注視点から視点へのベクトルの倍率の変更値 
	const float TO_SLIDING_CAMERA_POS = 400.0f;				// 注視点からスライディング用カメラへのベクトル
	const float ROTATION_LIMIT_VALUE = 0.9999f;				// 回転値のリミット
	const float REVERSE_VECTOR_VALUE = -1.0f;				// ベクトルを反転させる値
	const float MULTIPLIER_OF_INPUT_AMOUNT_IN_X_DIRECTION = 2.5f;	// X方向の入力量の倍率
	const float MULTIPLIER_OF_INPUT_AMOUNT_IN_Y_DIRECTION = 1.3f;	// Y方向の入力量の倍率
	const float DISTANCE_TO_CAMERA_POS = -400.0f;			// カメラの座標への距離
	const float DAMPING_RATE = 0.8f;						// ばねの減衰率
}

namespace nsPsychicEnergy
{

	namespace nsGameCamera
	{
		bool GameCamera::Start()
		{
			// プレイヤーの情報を取得
			m_player = FindGO<nsPlayer::Player>("player");

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
			m_springCamera.SetDampingRate(DAMPING_RATE);

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
			m_degreeX = x * MULTIPLIER_OF_INPUT_AMOUNT_IN_X_DIRECTION;
			m_degreeY = y * MULTIPLIER_OF_INPUT_AMOUNT_IN_Y_DIRECTION;

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

			// カメラへの方向
			Vector3 toPosDir = m_toCameraPos;
			toPosDir.Normalize();


			// カメラの回転範囲の上限値を超えないように補正する
			if (toPosDir.y < -ROTATION_LIMIT_VALUE) {
				m_toCameraPos = toCameraPosOld;
			}
			else if (toPosDir.y > ROTATION_LIMIT_VALUE) {
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
			m_degreeX = x * MULTIPLIER_OF_INPUT_AMOUNT_IN_X_DIRECTION;
			m_degreeY = y * MULTIPLIER_OF_INPUT_AMOUNT_IN_Y_DIRECTION;

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

			// 注視点を上げる　
			m_target.y += TARGET_Y;

			// 視点を調整
			m_toCameraPos = m_player->GetForward() * DISTANCE_TO_CAMERA_POS;

			// 更新前の注視点から視点へのベクトル
			Vector3 toCameraPosOld = m_toCameraPos;

			// 右スティックのX軸の入力量
			float x = g_pad[0]->GetRStickXF();
			// 右スティックのY軸の入力量
			float y = g_pad[0]->GetRStickYF();

			// 入力量から回転量を計算
			m_degreeX += x * MULTIPLIER_OF_INPUT_AMOUNT_IN_X_DIRECTION;
			m_degreeY += y * MULTIPLIER_OF_INPUT_AMOUNT_IN_Y_DIRECTION;

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

			// カメラへの方向
			Vector3 toPosDir = m_toCameraPos;
			toPosDir.Normalize();

			// カメラの回転範囲の上限値を超えないように補正する
			if (toPosDir.y < -ROTATION_LIMIT_VALUE) {
				m_toCameraPos = toCameraPosOld;
			}
			else if (toPosDir.y > ROTATION_LIMIT_VALUE) {
				m_toCameraPos = toCameraPosOld;
			}
			// 最終的なカメラの座標を指定する。
			m_position = m_target + m_toCameraPos;
		}

		void GameCamera::FixFront()
		{
			// プレイヤーの正面方向。
			Vector3 playerForward = m_player->GetForward();

			// 視点から注視点へのベクトル
			Vector3 toCameraPos = m_toCameraPos;
			// ベクトルを反転させ、正規化する。
			toCameraPos.Normalize();

			// 回転に使用するクォータニオン
			Quaternion qRot;
			// クォータニオンを作成し、適用する。
			qRot.SetRotation(toCameraPos, playerForward);
			qRot.Apply(m_toCameraPos);

			// このままではプレイヤーの正面側にカメラが移動してしまうため反転させる。
			m_toCameraPos *= REVERSE_VECTOR_VALUE;
			// 座標を更新。
			m_position = m_target + m_toCameraPos;
			m_springCamera.SetPosition(m_position);
		}

	}
}

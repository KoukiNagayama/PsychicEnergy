#include "stdafx.h"
#include "TitleCamera.h"
#include "TitleCharacter.h"
#include "Title.h"

namespace
{
	const float FAR_CLIP = 10000.0f;							// カメラの遠平面
	const float NEAR_CLIP = 2.0f;								// カメラの近平面
	const float SHIFT_TARGET_TO_RIGHT_OF_CHARACTER = -160.0f;	// 注視点をキャラクターより右にずらす変化量
	const float SHIFT_TARGET_TO_UP_OF_CHARACTER = 30.0f;		// 注視点をキャラクターより上にずらす変化量
	const float TO_CAMERA_POS_X_FROM_TARGET = 200.0f;			// カメラの注視点から視点へのベクトルのX値
	const float TO_CAMERA_POS_Y_FROM_TARGET = 100.0f;			// カメラの注視点から視点へのベクトルのY値
	const float TO_CAMERA_POS_Z_FROM_TARGET = 280.0f;			// カメラの注視点から視点へのベクトルのZ値
	const float ADD_ROTATION_DEGREE = 3.0f;					
}


namespace nsPsychicEnergy
{

	namespace nsTitle
	{
		bool TitleCamera::Start()
		{
			// タイトルのインスタンスを検索。
			m_title = FindGO<Title>("title");

			// 注視点とするキャラクターを検索。
			m_titleCharacter = FindGO<TitleCharacter>("titleCharacter");

			// 注視点となる座標
			m_charaPos = m_titleCharacter->GetPosition();
			Vector3 targetPos = m_charaPos;
			targetPos.x += SHIFT_TARGET_TO_RIGHT_OF_CHARACTER;
			targetPos.y += SHIFT_TARGET_TO_UP_OF_CHARACTER;
			
			// 注視点を設定。
			g_camera3D->SetTarget(targetPos);

			// 視点を設定。
			m_toCameraPos.Set(
				targetPos.x + TO_CAMERA_POS_X_FROM_TARGET,
				targetPos.y + TO_CAMERA_POS_Y_FROM_TARGET,
				targetPos.z + TO_CAMERA_POS_Z_FROM_TARGET
			);
			g_camera3D->SetPosition(m_toCameraPos);

			// 近平面と遠平面までの距離を設定。
			g_camera3D->SetFar(FAR_CLIP);
			g_camera3D->SetNear(NEAR_CLIP);

			// カメラを更新。
			g_camera3D->Update();

			return true;
		}

		void TitleCamera::Update()
		{
			m_charaPos = m_titleCharacter->GetPosition();

			// キャラクターの座標のZ値がカメラの座標のZ値を超えたならば。
			if (m_charaPos.z >= m_position.z) {
				// 注視点をキャラクターの座標にし、フェードアウトさせる。
				g_camera3D->SetTarget(m_charaPos);
				m_isStartFadeOut = true;
			}
		}

	}
}


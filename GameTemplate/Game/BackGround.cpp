#include "stdafx.h"
#include "BackGround.h"
#include "WorldRotation.h"
#include "Player.h"
#include "CommonDataForBackGround.h"
#include "CommonDataForWorldRotation.h"
#include "WorldRotation.h"
#include "CommonDataForModel.h"

namespace
{

}


namespace nsPsychicEnergy
{

	namespace nsBackGround
	{
		bool BackGround::Start()
		{
			// tkmファイルパスを指定。
			const char* filePath = nullptr;

			// 影を落とす側のオブジェクトであるか。
			bool isShadowCaster = true;
			
			// 地面のオブジェクト。シャドウキャスターではない。
			if (m_typeNum == nsBackGround::enModelType_Ground) {
				filePath = "Assets/modelData/backGroundModel/ground.tkm";
				isShadowCaster = false;
			}

			// 箱1のオブジェクト。シャドウキャスターである。
			if (m_typeNum == nsBackGround::enModelType_Box1) {
				filePath = "Assets/modelData/backGroundModel/backGroundModel_box1.tkm";
			}

			// 箱2のオブジェクト。シャドウキャスターである。
			else if (m_typeNum == nsBackGround::enModelType_Box2) {
				filePath = "Assets/modelData/backGroundModel/backGroundModel_box2.tkm";
			}

			// 箱3のオブジェクト。シャドウキャスターである。
			else if (m_typeNum == nsBackGround::enModelType_Box3) {
				filePath = "Assets/modelData/backGroundModel/backGroundModel_box3.tkm";
			}

			// モデルの初期化。
			m_modelRender.Init(
				filePath,
				nullptr,
				nsModel::NO_ANIMATION_CLIPS,
				enModelUpAxisZ,
				false,
				false,
				isShadowCaster,
				false
			);

			// 座標、回転、拡大率の設定。
			m_modelRender.SetTRS(m_position, m_rotation, m_scale);
			m_modelRender.Update();


			// 静的物理オブジェクトを作成
			m_physicsStaticObject.CreateFromModel(
				m_modelRender.GetModel(),
				m_modelRender.GetModel().GetWorldMatrix()
			);

			// ワールド行列を記録しておく
			m_worldMatrix = m_modelRender.GetWorldMatrix();

			// ワールド行列の回転をするオブジェクトを設定する。
			nsK2EngineLow::nsWorldRotation::g_worldRotation->AddBackGround(this);
			// プレイヤーのインスタンスを検索する。
			m_player = FindGO<nsPlayer::Player>("player");
			return true;
		}

		void BackGround::Update()
		{
			// 回転が終了していなければ回転を行う。
			if (m_rotateTimer < nsWorldRotation::TIME_UNTIL_ROTATION_ENDS)
			{
				// タイマーを加算。
				m_rotateTimer += g_gameTime->GetFrameDeltaTime();
				m_rotateTimer = min(m_rotateTimer, nsWorldRotation::TIME_UNTIL_ROTATION_ENDS);

				// 線形補間のための補間率を計算。
				const float rate = m_rotateTimer / nsWorldRotation::TIME_UNTIL_ROTATION_ENDS;
				
				// 計算後のワールド行列。
				Matrix mat;

				for (int i = nsWorldRotation::NUM_OF_FIRST_COMPONENT_IN_WORLD_MATRIX;
					i <= nsWorldRotation::NUM_OF_LAST_COMPONENT_IN_WORLD_MATRIX;
					i++)
				{
					// 線形補間を行う。
					mat.v[i].Lerp(rate, m_prevMatrix.v[i], m_nextMatrix.v[i]);
				}
				// 計算したワールド行列をモデルに設定する。
				m_modelRender.SetWorldMatrix(mat);
			}

			// 回転中はプレイヤーを動作させない。
			m_player->SetDisablePlayerMove(false);

			// 現在のワールド行列が記憶されたワールド行列と一致する場合動作させない
			Matrix currentWorldMatrix = m_modelRender.GetWorldMatrix();
			if (m_worldMatrix == currentWorldMatrix) {
				return;
			}

			// 背景が保持する当たり判定を新たなワールド行列に対して再生成する。
			m_physicsStaticObject.Release();
			m_physicsStaticObject.CreateFromModel(
				m_modelRender.GetModel(),
				m_modelRender.GetWorldMatrix()
			);

			// 現在のワールド行列を記録しておく。
			m_worldMatrix = currentWorldMatrix;

			// このフレームにおける回転の挙動が全て終わったためプレイヤーを動けるように設定する。
			m_player->SetDisablePlayerMove(true);

			// リセットの場合の回転があるため、リセット中ではないと設定する。
			nsK2EngineLow::nsWorldRotation::g_worldRotation->SetIsReseting(false);

		}

		void BackGround::Render(RenderContext& rc)
		{
			// 描画。
			m_modelRender.Draw(rc);
		}

	}
}

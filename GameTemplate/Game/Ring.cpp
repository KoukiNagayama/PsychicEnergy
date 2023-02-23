#include "stdafx.h"
#include "Ring.h"
#include "Game.h"
#include "Player.h"
#include "CommonDataForWorldRotation.h"
#include "CommonDataForSound.h"
#include "CommonDataForPriority.h"

namespace
{
	const float DIST_TO_JUDGE_TRUE = 120.0f;
	const float ACQUISITION_SOUND_VOLUME = 0.7f;
	const int	STORAGE_POS_OF_TRANSLATION_MAT_IN_WORLD_MAT = 3;
	const int	TRANSLATION_MAT_X = 0;
	const int	TRANSLATION_MAT_Y = 1;
	const int	TRANSLATION_MAT_Z = 2;
}

namespace nsPsychicEnergy
{

	namespace nsRing
	{
		Ring::~Ring()
		{

		}

		bool Ring::Start()
		{
			// ゲームクラスのインスタンスを検索する。
			m_game = FindGO<nsGame::Game>("game");

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

			// プレイヤーのインスタンスを検索。
			m_player = FindGO<nsPlayer::Player>("player");

			return true;
		}
		void Ring::NotifyGet()
		{
			// 取得したリングを増やす
			m_game->GetRing();

			// 取得音を鳴らす
			m_acquisitionSound = NewGO<SoundSource>(nsPriority::enPriorityFirst);
			m_acquisitionSound->Init(nsSound::enSoundNumber_Acquisition);
			m_acquisitionSound->SetVolume(ACQUISITION_SOUND_VOLUME);
			m_acquisitionSound->Play(false);

		}
		void Ring::Update()
		{
			Collision();

			Rotation();
		}

		void Ring::Rotation()
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

				// ワールド行列から現在の座標を抽出する。
				m_position.x = mat.m[STORAGE_POS_OF_TRANSLATION_MAT_IN_WORLD_MAT][TRANSLATION_MAT_X];
				m_position.y = mat.m[STORAGE_POS_OF_TRANSLATION_MAT_IN_WORLD_MAT][TRANSLATION_MAT_Y];
				m_position.z = mat.m[STORAGE_POS_OF_TRANSLATION_MAT_IN_WORLD_MAT][TRANSLATION_MAT_Z];

				// モデルの座標を更新する。
				m_modelRender.SetPosition(m_position);
			}
		}

		void Ring::Collision()
		{
			if (DIST_TO_JUDGE_TRUE * DIST_TO_JUDGE_TRUE < CalcDistanceToPlayer()) {
				// プレイヤーとの距離が遠い場合は取得されてないとして回転対象に追加する。
				nsK2EngineLow::nsWorldRotation::g_worldRotation->AddRing(this);
				return;
			}

			// ここまで来た場合はプレイヤーが取得したとする。
			// 取得されたことを通知する。
			NotifyGet();
			// プレイヤーが取得したためオブジェクトを削除
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
			// 描画。
			m_modelRender.Draw(rc);
		}
	}
}


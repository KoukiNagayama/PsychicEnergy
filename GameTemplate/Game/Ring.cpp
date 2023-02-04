#include "stdafx.h"
#include "Ring.h"
#include "Game.h"
#include "Player.h"
#include "CommonDataForWorldRotation.h"
#include "CommonDataForSound.h"

namespace
{
	const float DIST_TO_JUDGE_TRUE = 150.0f;
	const float ACQUISITION_SOUND_VOLUME = 0.7f;
}

Ring::~Ring()
{
	
}

bool Ring::Start()
{
	m_game = FindGO<Game>("game");
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
void Ring::NotifyGet()
{
	// 取得したリングを増やす
	m_game->GetRing();

	// 取得音を鳴らす
	m_acquisitionSound = NewGO<SoundSource>(0);
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
	if (m_rotateTimer < TIME_UNTIL_ROTATION_ENDS)
	{
		m_rotateTimer += g_gameTime->GetFrameDeltaTime();
		m_rotateTimer = min(m_rotateTimer, TIME_UNTIL_ROTATION_ENDS);

		const float rate = m_rotateTimer / TIME_UNTIL_ROTATION_ENDS;
		Matrix mat;
		for (int i = NUM_OF_FIRST_COMPONENT_IN_WORLD_MATRIX;
			i <= NUM_OF_LAST_COMPONENT_IN_WORLD_MATRIX;
			i++)
		{
			mat.v[i].Lerp(rate, m_prevMatrix.v[i], m_nextMatrix.v[i]);
		}
		m_modelRender.SetWorldMatrix(mat);

		// ワールド行列から現在の座標を抽出する。
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
	m_modelRender.Draw(rc);
}
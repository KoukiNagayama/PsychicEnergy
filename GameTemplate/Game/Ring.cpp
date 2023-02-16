#include "stdafx.h"
#include "Ring.h"
#include "Game.h"
#include "Player.h"
#include "CommonDataForWorldRotation.h"
#include "CommonDataForSound.h"

namespace
{
	const float DIST_TO_JUDGE_TRUE = 120.0f;
	const float ACQUISITION_SOUND_VOLUME = 0.7f;
}

Ring::~Ring()
{
	
}

bool Ring::Start()
{
	m_game = FindGO<Game>("game");
	// ���f���̏������f�[�^
	ModelInitData modelInitData;
	modelInitData.m_tkmFilePath = "Assets/modelData/ring/ring.tkm";
	modelInitData.m_fxFilePath = "Assets/shader/ring.fx";
	modelInitData.m_vsEntryPointFunc = "VSMain";
	modelInitData.m_psEntryPointFunc = "PSMain";
	modelInitData.m_modelUpAxis = enModelUpAxisZ;
	// �������f�[�^�����ƂɃ��f����������
	m_modelRender.InitDirectlyNotifyForwardRendering(modelInitData);

	// ���f���̍��W�A��]�A�g�嗦��ݒ�B
	m_modelRender.SetTRS(m_position, m_rotation, m_scale);
	m_modelRender.Update();

	m_player = FindGO<Player>("player");
	return true;
}
void Ring::NotifyGet()
{
	// �擾���������O�𑝂₷
	m_game->GetRing();

	// �擾����炷
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

		// ���[���h�s�񂩂猻�݂̍��W�𒊏o����B
		m_position.x = mat.m[3][0];
		m_position.y = mat.m[3][1];
		m_position.z = mat.m[3][2];

		m_modelRender.SetPosition(m_position);
	}
}

void Ring::Collision()
{
	if (DIST_TO_JUDGE_TRUE * DIST_TO_JUDGE_TRUE < CalcDistanceToPlayer()) {
		g_worldRotation->AddRing(this);
		return;
	}
	NotifyGet();
	// �v���C���[���擾�������߃I�u�W�F�N�g���폜
	DeleteGO(this);
}

float Ring::CalcDistanceToPlayer()
{
	// ����
	Vector3 distance;
	distance = m_position - m_player->GetPosition();

	return distance.LengthSq();
}

void Ring::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}
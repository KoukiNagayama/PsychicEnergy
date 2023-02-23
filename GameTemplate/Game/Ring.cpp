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
			// �Q�[���N���X�̃C���X�^���X����������B
			m_game = FindGO<nsGame::Game>("game");

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

			// �v���C���[�̃C���X�^���X�������B
			m_player = FindGO<nsPlayer::Player>("player");

			return true;
		}
		void Ring::NotifyGet()
		{
			// �擾���������O�𑝂₷
			m_game->GetRing();

			// �擾����炷
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
			// ��]���I�����Ă��Ȃ���Ή�]���s���B
			if (m_rotateTimer < nsWorldRotation::TIME_UNTIL_ROTATION_ENDS)
			{
				// �^�C�}�[�����Z�B
				m_rotateTimer += g_gameTime->GetFrameDeltaTime();
				m_rotateTimer = min(m_rotateTimer, nsWorldRotation::TIME_UNTIL_ROTATION_ENDS);

				// ���`��Ԃ̂��߂̕�ԗ����v�Z�B
				const float rate = m_rotateTimer / nsWorldRotation::TIME_UNTIL_ROTATION_ENDS;

				// �v�Z��̃��[���h�s��B
				Matrix mat;

				for (int i = nsWorldRotation::NUM_OF_FIRST_COMPONENT_IN_WORLD_MATRIX;
					i <= nsWorldRotation::NUM_OF_LAST_COMPONENT_IN_WORLD_MATRIX;
					i++)
				{
					// ���`��Ԃ��s���B
					mat.v[i].Lerp(rate, m_prevMatrix.v[i], m_nextMatrix.v[i]);
				}
				// �v�Z�������[���h�s������f���ɐݒ肷��B
				m_modelRender.SetWorldMatrix(mat);

				// ���[���h�s�񂩂猻�݂̍��W�𒊏o����B
				m_position.x = mat.m[STORAGE_POS_OF_TRANSLATION_MAT_IN_WORLD_MAT][TRANSLATION_MAT_X];
				m_position.y = mat.m[STORAGE_POS_OF_TRANSLATION_MAT_IN_WORLD_MAT][TRANSLATION_MAT_Y];
				m_position.z = mat.m[STORAGE_POS_OF_TRANSLATION_MAT_IN_WORLD_MAT][TRANSLATION_MAT_Z];

				// ���f���̍��W���X�V����B
				m_modelRender.SetPosition(m_position);
			}
		}

		void Ring::Collision()
		{
			if (DIST_TO_JUDGE_TRUE * DIST_TO_JUDGE_TRUE < CalcDistanceToPlayer()) {
				// �v���C���[�Ƃ̋����������ꍇ�͎擾����ĂȂ��Ƃ��ĉ�]�Ώۂɒǉ�����B
				nsK2EngineLow::nsWorldRotation::g_worldRotation->AddRing(this);
				return;
			}

			// �����܂ŗ����ꍇ�̓v���C���[���擾�����Ƃ���B
			// �擾���ꂽ���Ƃ�ʒm����B
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
			// �`��B
			m_modelRender.Draw(rc);
		}
	}
}


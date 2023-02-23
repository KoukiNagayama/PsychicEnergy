#include "stdafx.h"
#include "TitleCharacter.h"
#include "Title.h"

namespace
{
	const float INCREASE_Y_WHEN_FLOATING = 30.0f;	// ���V����Y���W�̑�����
	const float DISTANCE_TRAVEL_PER_FRAME = 20.0f;	// �t���[�����ƂɈړ����鎞��
	const float ANIMATION_INTERPORATE_TIME = 0.3f;	// �A�j���[�V�����̕�Ԏ���
	const float TIME_TO_COMPLETE_FLOAT = 0.3f;		// ���V�����Ƃ��鎞��

}

namespace nsPsychicEnergy
{

	namespace nsTitle
	{
		TitleCharacter::~TitleCharacter()
		{
			// �����_�����O�G���W���̕��V�ݒ����������B
			g_renderingEngine->SetIsFloating(false);
		}

		bool TitleCharacter::Start()
		{
			// �^�C�g���N���X�̃C���X�^���X������
			m_title = FindGO<Title>("title");

			// �A�j���[�V�����N���b�v�����[�h����B
			// �ҋ@�A�j���[�V�����B
			m_animationClips[enAnimationClip_Idle].Load("Assets/animData/WD/idle_2.tka");
			m_animationClips[enAnimationClip_Idle].SetLoopFlag(true);
			// �󒆂ł̑ҋ@�A�j���[�V�����B
			m_animationClips[enAnimationClip_IdleAir].Load("Assets/animData/WD/idle_air_4.tka");
			m_animationClips[enAnimationClip_IdleAir].SetLoopFlag(true);
			// �󒆂ł̗���(�ړ�)�A�j���[�V�����B
			m_animationClips[enAnimationClip_FallAir].Load("Assets/animData/WD/fall_air.tka");
			m_animationClips[enAnimationClip_FallAir].SetLoopFlag(true);

			// ���f�����������B
			m_modelRender.Init(
				"Assets/modelData/WD/WD.tkm",
				m_animationClips,
				enAnimationClip_Num,
				enModelUpAxisZ,
				true,
				true,
				true,
				true
			);

			// ���f���̍��W�A��]�A�g�嗦��ݒ�B
			m_modelRender.SetTRS(m_position, m_rotation, m_scale);

			// ���f���̍X�V�B
			m_modelRender.Update();

			return true;
		}

		void TitleCharacter::Update()
		{
			// �X�e�[�g�̕ύX���������Ă��炸�AB��������Ă���Ȃ�΁B
			if (!m_isCompletedStateChange && m_title->IsPushedB()) {
				
				// ���V��Ԃł���Ɛݒ�B
				m_modelRender.SetIsFloating(true);
				g_renderingEngine->SetIsFloating(true);

				// y���W���グ�ĕ��V������B
				m_position.y += INCREASE_Y_WHEN_FLOATING;
				m_currentAnimationClip = enAnimationClip_IdleAir;
				m_isFloating = true;

				// �X�e�[�g�̕ύX�����������Ƃ���B
				m_isCompletedStateChange = true;
			}

			// ���V���Ă���Ȃ�΁B
			if (m_isFloating) {

				// ���V���Ă���̎��Ԃ����Z����B
				m_floatingCount += g_gameTime->GetFrameDeltaTime();

				// ���l�𒴂����Ȃ�΁B
				if (m_floatingCount >= TIME_TO_COMPLETE_FLOAT) {
					// ��O�ɕ��V�ړ�������B
					m_currentAnimationClip = enAnimationClip_FallAir;
					m_position.z += DISTANCE_TRAVEL_PER_FRAME;
				}
			}

			// �A�j���[�V�������Đ�����B
			PlayAnimation();

			// ���f���̍��W���X�V�B
			m_modelRender.SetPosition(m_position);
			m_modelRender.Update();
		}

		void TitleCharacter::PlayAnimation()
		{
			// �A�j���[�V�������Đ�
			m_modelRender.PlayAnimation(m_currentAnimationClip, ANIMATION_INTERPORATE_TIME);
		}

		void TitleCharacter::Render(RenderContext& rc)
		{
			// �`��
			m_modelRender.Draw(rc);
		}

	}
}


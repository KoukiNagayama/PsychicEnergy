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
			// tkm�t�@�C���p�X���w��B
			const char* filePath = nullptr;

			// �e�𗎂Ƃ����̃I�u�W�F�N�g�ł��邩�B
			bool isShadowCaster = true;
			
			// �n�ʂ̃I�u�W�F�N�g�B�V���h�E�L���X�^�[�ł͂Ȃ��B
			if (m_typeNum == nsBackGround::enModelType_Ground) {
				filePath = "Assets/modelData/backGroundModel/ground.tkm";
				isShadowCaster = false;
			}

			// ��1�̃I�u�W�F�N�g�B�V���h�E�L���X�^�[�ł���B
			if (m_typeNum == nsBackGround::enModelType_Box1) {
				filePath = "Assets/modelData/backGroundModel/backGroundModel_box1.tkm";
			}

			// ��2�̃I�u�W�F�N�g�B�V���h�E�L���X�^�[�ł���B
			else if (m_typeNum == nsBackGround::enModelType_Box2) {
				filePath = "Assets/modelData/backGroundModel/backGroundModel_box2.tkm";
			}

			// ��3�̃I�u�W�F�N�g�B�V���h�E�L���X�^�[�ł���B
			else if (m_typeNum == nsBackGround::enModelType_Box3) {
				filePath = "Assets/modelData/backGroundModel/backGroundModel_box3.tkm";
			}

			// ���f���̏������B
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

			// ���W�A��]�A�g�嗦�̐ݒ�B
			m_modelRender.SetTRS(m_position, m_rotation, m_scale);
			m_modelRender.Update();


			// �ÓI�����I�u�W�F�N�g���쐬
			m_physicsStaticObject.CreateFromModel(
				m_modelRender.GetModel(),
				m_modelRender.GetModel().GetWorldMatrix()
			);

			// ���[���h�s����L�^���Ă���
			m_worldMatrix = m_modelRender.GetWorldMatrix();

			// ���[���h�s��̉�]������I�u�W�F�N�g��ݒ肷��B
			nsK2EngineLow::nsWorldRotation::g_worldRotation->AddBackGround(this);
			// �v���C���[�̃C���X�^���X����������B
			m_player = FindGO<nsPlayer::Player>("player");
			return true;
		}

		void BackGround::Update()
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
			}

			// ��]���̓v���C���[�𓮍삳���Ȃ��B
			m_player->SetDisablePlayerMove(false);

			// ���݂̃��[���h�s�񂪋L�����ꂽ���[���h�s��ƈ�v����ꍇ���삳���Ȃ�
			Matrix currentWorldMatrix = m_modelRender.GetWorldMatrix();
			if (m_worldMatrix == currentWorldMatrix) {
				return;
			}

			// �w�i���ێ����铖���蔻���V���ȃ��[���h�s��ɑ΂��čĐ�������B
			m_physicsStaticObject.Release();
			m_physicsStaticObject.CreateFromModel(
				m_modelRender.GetModel(),
				m_modelRender.GetWorldMatrix()
			);

			// ���݂̃��[���h�s����L�^���Ă����B
			m_worldMatrix = currentWorldMatrix;

			// ���̃t���[���ɂ������]�̋������S�ďI��������߃v���C���[�𓮂���悤�ɐݒ肷��B
			m_player->SetDisablePlayerMove(true);

			// ���Z�b�g�̏ꍇ�̉�]�����邽�߁A���Z�b�g���ł͂Ȃ��Ɛݒ肷��B
			nsK2EngineLow::nsWorldRotation::g_worldRotation->SetIsReseting(false);

		}

		void BackGround::Render(RenderContext& rc)
		{
			// �`��B
			m_modelRender.Draw(rc);
		}

	}
}

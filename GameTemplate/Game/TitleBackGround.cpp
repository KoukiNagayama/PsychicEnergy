#include "stdafx.h"
#include "TitleBackGround.h"
#include "CommonDataForModel.h"

namespace nsPsychicEnergy
{

	namespace nsTitle
	{
		bool TitleBackGround::Start()
		{
			// tkm�t�@�C���p�X�̕�����B
			const char* filePath = nullptr;

			// �e���L���X�g���邩�B
			bool isShadowCaster = true;

			// tkm�t�@�C���p�X���w�肷��B
			// �n��
			if (m_typeNum == nsBackGround::enModelType_Ground) {
				filePath = "Assets/modelData/backGroundModel/ground.tkm";
				// �n�ʃI�u�W�F�N�g�����͉e���L���X�g���Ȃ��B
				isShadowCaster = false;
			}
			// ��1
			else if (m_typeNum == nsBackGround::enModelType_Box1) {
				filePath = "Assets/modelData/backGroundModel/backGroundModel_box1.tkm";
			}
			// ��2
			else if (m_typeNum == nsBackGround::enModelType_Box2) {
				filePath = "Assets/modelData/backGroundModel/backGroundModel_box2.tkm";
			}
			// ��3
			else if (m_typeNum == nsBackGround::enModelType_Box3) {
				filePath = "Assets/modelData/backGroundModel/backGroundModel_box3.tkm";
			}

			// ���f�����������B
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

			// ���f���̍��W�A��]�A�g�嗦��ݒ�B
			m_modelRender.SetTRS(m_position, m_rotation, m_scale);

			// ���f�����X�V�B
			m_modelRender.Update();
			return true;
		}

		void TitleBackGround::Render(RenderContext& rc)
		{
			// �`��B
			m_modelRender.Draw(rc);

		}
	}
}
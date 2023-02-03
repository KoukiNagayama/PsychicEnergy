#include "stdafx.h"
#include "TitleBackGround.h"

bool TitleBackGround::Start()
{
	// tkm�t�@�C���p�X���w��B
	const char* filePath = nullptr;

	// �e���L���X�g���邩�B
	bool isShadowCaster = true;

	// tkm�t�@�C���p�X���w�肷��B
	if (m_typeNum == nsBackGround::enModelType_Ground) {
		filePath = "Assets/modelData/backGroundModel/ground.tkm";
		// �n�ʃI�u�W�F�N�g�����͉e���L���X�g���Ȃ��B
		isShadowCaster = false;
	}
	if (m_typeNum == nsBackGround::enModelType_Box1) {
		filePath = "Assets/modelData/backGroundModel/backGroundModel_box1.tkm";
	}
	else if (m_typeNum == nsBackGround::enModelType_Box2) {
		filePath = "Assets/modelData/backGroundModel/backGroundModel_box2.tkm";
	}
	else if (m_typeNum == nsBackGround::enModelType_Box3) {
		filePath = "Assets/modelData/backGroundModel/backGroundModel_box3.tkm";
	}

	// ���f�����������B
	m_modelRender.Init(
		filePath,
		nullptr,
		0,
		enModelUpAxisZ,
		false,
		false,
		isShadowCaster,
		false
	);
	m_modelRender.SetTRS(m_position, m_rotation, m_scale);
	m_modelRender.Update();
	return true;
}

void TitleBackGround::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}
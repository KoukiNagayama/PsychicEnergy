#include "stdafx.h"
#include "TitleBackGround.h"

bool TitleBackGround::Start()
{
	// tkmファイルパスを指定。
	const char* filePath = nullptr;

	// 影をキャストするか。
	bool isShadowCaster = true;

	// tkmファイルパスを指定する。
	if (m_typeNum == nsBackGround::enModelType_Ground) {
		filePath = "Assets/modelData/backGroundModel/ground.tkm";
		// 地面オブジェクトだけは影をキャストしない。
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

	// モデルを初期化。
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
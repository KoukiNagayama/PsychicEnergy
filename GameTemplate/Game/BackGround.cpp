#include "stdafx.h"
#include "BackGround.h"
#include "WorldRotation.h"
#include "Player.h"
#include "CommonDataForWorldRotation.h"

namespace
{
	const int MODEL_TYPE_GROUND = 0;
	const int MODEL_TYPE_BOX1 = 1;
	const int MODEL_TYPE_BOX2 = 2;
	const int MODEL_TYPE_BOX3 = 3;


}

bool BackGround::Start()
{
	// tkm�t�@�C���p�X���w��B
	const char* filePath = nullptr;

	bool isShadowCaster = true;
	if (m_typeNum == MODEL_TYPE_GROUND) {
		filePath = "Assets/modelData/backGroundModel/ground.tkm";
		isShadowCaster = false;
	}
	if (m_typeNum == MODEL_TYPE_BOX1) {
		filePath = "Assets/modelData/backGroundModel/backGroundModel_box1.tkm";
	}
	else if (m_typeNum == MODEL_TYPE_BOX2) {
		filePath = "Assets/modelData/backGroundModel/backGroundModel_box2.tkm";
	}
	else if (m_typeNum == MODEL_TYPE_BOX3) {
		filePath ="Assets/modelData/backGroundModel/backGroundModel_box3.tkm";
	}

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
	// �ÓI�����I�u�W�F�N�g���쐬
	m_physicsStaticObject.CreateFromModel(
		m_modelRender.GetModel(),
		m_modelRender.GetModel().GetWorldMatrix()
	);

	// ���[���h�s����L�^���Ă���
	m_worldMatrix = m_modelRender.GetWorldMatrix();

	// ���[���h�s��̉�]������I�u�W�F�N�g��ݒ肷��B
	g_worldRotation->AddBackGround(this);
	// �v���C���[��
	m_player = FindGO<Player>("player");
	return true;
}

void BackGround::Update()
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
	}
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
	m_worldMatrix = currentWorldMatrix;
	m_player->SetDisablePlayerMove(true);
	g_worldRotation->SetIsReseting(false);

}

void BackGround::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}

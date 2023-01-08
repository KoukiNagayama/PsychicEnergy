#include "stdafx.h"
#include "BackGround.h"
#include "WorldRotation.h"
#include "Player.h"

bool BackGround::Start()
{
	// tkm�t�@�C���p�X���w��B
	const char* filePath = nullptr;

	bool isShadowCaster = true;
	if (m_typeNum == 0) {
		filePath = "Assets/modelData/testStage/ground.tkm";
		isShadowCaster = false;
	}
	if (m_typeNum == 1) {
		filePath = "Assets/modelData/backGroundModel/backGroundModel_box1.tkm";
	}
	else if (m_typeNum == 2) {
		filePath = "Assets/modelData/backGroundModel/backGroundModel_box2.tkm";
	}
	else if (m_typeNum == 3) {
		filePath = "Assets/modelData/backGroundModel/backGroundModel_box3.tkm";
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

	m_physicsStaticObject.CreateFromModel(
		m_modelRender.GetModel(),
		m_modelRender.GetModel().GetWorldMatrix()
	);

	// ���[���h�s����L�^���Ă���
	m_worldMatrix = m_modelRender.GetWorldMatrix();
	
	// �f�o�b�O�p�@�����蔻��`��
	//PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();
	m_modelRender.Update();

	// ���[���h�s��̉�]������I�u�W�F�N�g��ݒ肷��B
	g_worldRotation->AddBackGround(this);
	// �n�ʂ̃��[���h�s��͊�̍s��Ƃ��ĕʂł��L�^���Ă���
	if (m_typeNum == 0) {
		g_worldRotation->SetGroundWorldMatrix(m_worldMatrix);
	}

	m_player = FindGO<Player>("player");
	return true;
}

void BackGround::Update()
{
	{
		constexpr float kRotateTime = 0.7f;
		if (m_rotateTimer < kRotateTime)
		{
			m_rotateTimer += g_gameTime->GetFrameDeltaTime();
			m_rotateTimer = min(m_rotateTimer, kRotateTime);

			const float rate = m_rotateTimer / kRotateTime;
			Matrix mat;
			mat.v[0].Lerp(rate, m_prevMatrix.v[0], m_nextMatrix.v[0]);
			mat.v[1].Lerp(rate, m_prevMatrix.v[1], m_nextMatrix.v[1]);
			mat.v[2].Lerp(rate, m_prevMatrix.v[2], m_nextMatrix.v[2]);
			mat.v[3].Lerp(rate, m_prevMatrix.v[3], m_nextMatrix.v[3]);

			m_modelRender.SetWorldMatrix(mat);
		}
	}
	// ���݂̃��[���h�s�񂪋L�����ꂽ���[���h�s��ƈ�v����ꍇ���삳���Ȃ�
	Matrix nowWorldMatrix = m_modelRender.GetWorldMatrix();
	if (m_worldMatrix == nowWorldMatrix) {
		return;
	}
	// �w�i���ێ����铖���蔻���V���ȃ��[���h�s��ɑ΂��čĐ�������B
	m_physicsStaticObject.Release();
	m_physicsStaticObject.CreateFromModel(
		m_modelRender.GetModel(),
		m_modelRender.GetWorldMatrix()
	);
	m_worldMatrix = nowWorldMatrix;

	m_player->SetIsTouchObject(true);
}

void BackGround::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}

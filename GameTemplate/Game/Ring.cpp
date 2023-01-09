#include "stdafx.h"
#include "Ring.h"

bool Ring::Start()
{
	ModelInitData modelInitData;
	modelInitData.m_tkmFilePath = "Assets/modelData/ring/ring.tkm";
	modelInitData.m_fxFilePath = "Assets/shader/ring.fx";
	modelInitData.m_vsEntryPointFunc = "VSMain";
	modelInitData.m_psEntryPointFunc = "PSMain";
	modelInitData.m_modelUpAxis = enModelUpAxisZ;

	m_modelRender.InitDirectlyNotifyForwardRendering(modelInitData);
	//// ���f����������
	//m_modelRender.Init(
	//	"Assets/modelData/ring/ring.tkm",
	//	nullptr,
	//	0,
	//	enModelUpAxisZ,
	//	false,
	//	false,
	//	false,
	//	false
	//);

	m_modelRender.SetTRS(m_position, m_rotation, m_scale);
	m_modelRender.Update();

	// ���[���h�s����L�^���Ă���
	//m_worldMatrix = m_modelRender.GetWorldMatrix();

	// �f�o�b�O�p�@�����蔻��`��
	//PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();
	m_modelRender.Update();

	// ���[���h�s��̉�]������I�u�W�F�N�g��ݒ肷��B
	//g_worldRotation->AddBackGround(this);
	//// �n�ʂ̃��[���h�s��͊�̍s��Ƃ��ĕʂł��L�^���Ă���
	//if (m_typeNum == 0) {
	//	g_worldRotation->SetGroundWorldMatrix(m_worldMatrix);
	//}

	//m_player = FindGO<Player>("player");
	return true;
}

void Ring::Update()
{
	
}

void Ring::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}
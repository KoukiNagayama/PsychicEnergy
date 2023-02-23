#include "stdafx.h"
#include "system/system.h"
#include "Game.h"
#include "Title.h"


// K2EngineLow�̃O���[�o���A�N�Z�X�|�C���g�B
K2EngineLow* g_k2EngineLow = nullptr;

/// <summary>
/// ���C���֐�
/// </summary>
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	// �Q�[���̏������B
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("PsychicEnergy"));
	
	// k2EngineLow�̏������B
	g_k2EngineLow = new K2EngineLow();
	g_k2EngineLow->Init(g_hWnd, FRAME_BUFFER_W, FRAME_BUFFER_H);
	g_camera3D->SetPosition({ 0.0f, 70.0f, 200.0f });
	g_camera3D->SetTarget({ 0.0f, 50.0f, 0.0f });

	// �����_�����O�R���e�L�X�g
	auto& renderContext = g_graphicsEngine->GetRenderContext();

	//auto engineTest = NewGO<EngineTest>(0, "engineTest");
	//auto game = NewGO<Game>(0, "game");
	auto title = NewGO<nsPsychicEnergy::nsTitle::Title>(0, "title");

	// �����_�����O�G���W����������
	g_renderingEngine = new RenderingEngine;
	g_renderingEngine->Init();

	// ���E�̉�]�N���X�̃C���X�^���X�𐶐��B
	nsK2EngineLow::nsWorldRotation::g_worldRotation = new nsK2EngineLow::nsWorldRotation::WorldRotation;

	// ��������Q�[�����[�v�B
	while (DispatchWindowMessage())
	{
		// �t���[���̊J�n���ɌĂяo���K�v�����鏈�������s
		g_k2EngineLow->BeginFrame();

		// �Q�[���I�u�W�F�N�g�}�l�[�W���[�̍X�V�������Ăяo���B
		g_k2EngineLow->ExecuteUpdate();

		// ���E�̉�]
		nsK2EngineLow::nsWorldRotation::g_worldRotation->RotationWorldMatrix();

		nsK2EngineLow::nsWorldRotation::g_worldRotation->ResetWorldMatrix();

		// �Q�[���I�u�W�F�N�g�}�l�[�W���[�̕`�揈�����Ăяo���B
		g_k2EngineLow->ExecuteRender();

		// �����_�����O�G���W���̕`�揈�������s�B
		g_renderingEngine->Execute(renderContext);

		// ���E�̉�]�N���X�̍X�V���������s�B
		nsK2EngineLow::nsWorldRotation::g_worldRotation->Update();

		// �f�o�b�O�`�揈�������s����B
		g_k2EngineLow->DebubDrawWorld();

		// �t���[���̏I�����ɌĂяo���K�v�����鏈�������s�B
		g_k2EngineLow->EndFrame();
	}

	delete g_k2EngineLow;
	delete g_renderingEngine;
	delete nsK2EngineLow::nsWorldRotation::g_worldRotation;

	return 0;
}


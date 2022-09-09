#include "k2EngineLowPreCompile.h"
#include "RenderingEngine.h"

namespace nsK2EngineLow
{
	void RenderingEngine::Init()
	{
		InitMainRenderTarget();
		InitCopyMainRenderTargetToFrameBufferSprite();
	}

	void RenderingEngine::InitMainRenderTarget()
	{
		// ���C�������_�����O�^�[�Q�b�g���쐬
		m_mainRenderTarget.Create(
			g_graphicsEngine->GetFrameBufferWidth(),	
			g_graphicsEngine->GetFrameBufferHeight(),
			1,
			1,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_D32_FLOAT
		);
	}

	void RenderingEngine::InitCopyMainRenderTargetToFrameBufferSprite()
	{
		// �X�v���C�g�̏������I�u�W�F�N�g
		SpriteInitData spriteInitData;
		// �e�N�X�`��
		spriteInitData.m_textures[0] = &m_mainRenderTarget.GetRenderTargetTexture();
		// �𑜓x
		spriteInitData.m_width = g_graphicsEngine->GetFrameBufferWidth();
		spriteInitData.m_height = g_graphicsEngine->GetFrameBufferHeight();
		// �V�F�[�_�[�̃t�@�C���p�X���w��
		spriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";

		// �������I�u�W�F�N�g�����ƂɃX�v���C�g��������
		m_copyMainRtToFrameBufferSprite.Init(spriteInitData);
	}

	void RenderingEngine::Execute(RenderContext& rc)
	{
		// �t�H���[�h�����_�����O
		ForwardRendering(rc);

		// ���C�������_�����O�^�[�Q�b�g�̓��e���t���[���o�b�t�@�ɃR�s�[
		CopyMainRenderTargetToFrameBuffer(rc);

		// �o�^���ꂽ�I�u�W�F�N�g�̏����N���A
		m_forwardRenderModels.clear();
	}

	void RenderingEngine::ForwardRendering(RenderContext& rc)
	{
		// �����_�����O�^�[�Q�b�g�Ƃ��ė��p�\�ɂȂ�܂ő҂�
		rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);

		// �����_�����O�^�[�Q�b�g��ݒ肷��
		rc.SetRenderTargetAndViewport(m_mainRenderTarget);

		// �����_�����O�^�[�Q�b�g���N���A����
		rc.ClearRenderTargetView(m_mainRenderTarget);

		for (auto& model : m_forwardRenderModels)
		{
			// ���f����`��
			model->Draw(rc);
		}
		// �������ݏI���҂�
		rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);
	}

	void RenderingEngine::CopyMainRenderTargetToFrameBuffer(RenderContext& rc)
	{
		rc.SetRenderTarget(
			g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
			g_graphicsEngine->GetCurrentFrameBuffuerDSV()
		);

		// �r���[�|�[�g���w�肷��
		D3D12_VIEWPORT viewport;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = static_cast<FLOAT>(g_graphicsEngine->GetFrameBufferWidth());
		viewport.Height = static_cast<FLOAT>(g_graphicsEngine->GetFrameBufferHeight());
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;

		rc.SetViewportAndScissor(viewport);
		m_copyMainRtToFrameBufferSprite.Draw(rc);
	}
	RenderingEngine g_renderingEngine;
}

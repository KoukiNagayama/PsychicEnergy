#include "k2EngineLowPreCompile.h"
#include "RenderingEngine.h"

namespace nsK2EngineLow
{
	void RenderingEngine::Init()
	{
		m_sceneLight.Init();
		InitMainRenderTarget();
		InitDepthForOutLineRenderTarget();
		Init2DRenderTarget();
		InitCopyMainRenderTargetToFrameBufferSprite();
		m_postEffect.Init(m_mainRenderTarget, m_depthForOutLineRenderTarget, m_sLightingCb.m_isFloating);
		InitShadowMapRender();
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

	void RenderingEngine::InitDepthForOutLineRenderTarget()
	{
		// �����_�����O�^�[�Q�b�g�ƂȂ�e�N�X�`���𖄂߂�l
		//float clearColor[4] = { 10000.0f,10000.0f,10000.0f,1.0f };
		// �[�x�l�L�^�p�̃����_�����O�^�[�Q�b�g���쐬
		m_depthForOutLineRenderTarget.Create(
			m_mainRenderTarget.GetWidth(),
			m_mainRenderTarget.GetHeight(),
			1,
			1,
			DXGI_FORMAT_R32_FLOAT,
			DXGI_FORMAT_D32_FLOAT
			//clearColor
		);
	}

	void RenderingEngine::Init2DRenderTarget()
	{
		float clearColor[4] = { 0.0f,0.0f,0.0f,0.0f };

		m_2DRenderTarget.Create(
			UI_SPACE_WIDTH,
			UI_SPACE_HEIGHT,
			1,
			1,
			DXGI_FORMAT_R8G8B8A8_UNORM,
			DXGI_FORMAT_UNKNOWN,
			clearColor
		);

		// �ŏI�����p�̃X�v���C�g������������
		SpriteInitData spriteInitData;
		//�e�N�X�`����2D�����_�����O�^�[�Q�b�g�B
		spriteInitData.m_textures[0] = &m_2DRenderTarget.GetRenderTargetTexture();
		// �𑜓x��mainRenderTarget�̕��ƍ���
		spriteInitData.m_width = m_mainRenderTarget.GetWidth();
		spriteInitData.m_height = m_mainRenderTarget.GetHeight();
		// 2D�p�̃V�F�[�_�[���g�p����
		spriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";
		spriteInitData.m_vsEntryPointFunc = "VSMain";
		spriteInitData.m_psEntryPoinFunc = "PSMain";
		// �㏑���B
		spriteInitData.m_alphaBlendMode = AlphaBlendMode_None;
		// �����_�����O�^�[�Q�b�g�̃t�H�[�}�b�g�B
		spriteInitData.m_colorBufferFormat[0] = m_mainRenderTarget.GetColorBufferFormat();

		m_2DSprite.Init(spriteInitData);

		// �e�N�X�`���̓��C�������_�����O�^�[�Q�b�g�B
		spriteInitData.m_textures[0] = &m_mainRenderTarget.GetRenderTargetTexture();

		// �𑜓x��2D�p�����_�����O�^�[�Q�b�g�̕��ƍ���
		spriteInitData.m_width = m_2DRenderTarget.GetWidth();
		spriteInitData.m_height = m_2DRenderTarget.GetHeight();
		// �����_�����O�^�[�Q�b�g�̃t�H�[�}�b�g�B
		spriteInitData.m_colorBufferFormat[0] = m_2DRenderTarget.GetColorBufferFormat();

		m_mainSprite.Init(spriteInitData);
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

	void RenderingEngine::InitShadowMapRender()
	{
		m_shadowMapRender.Init();
	}

	void RenderingEngine::Execute(RenderContext& rc)
	{
		// �r���[�J�����O�p�̃r���[�v���W�F�N�V�����s����X�V
		CalcViewProjectionMatrixForViewCulling();
		// �V�[���̃W�I���g�������X�V
		m_sceneGeometryData.Update();
		// �V�[�����C�g�̍X�V
		m_sceneLight.Update();
		m_sLightingCb.m_light = m_sceneLight.GetLightData();

		// �V���h�E�}�b�v�փ��f����`��
		RenderToShadowMap(rc);

		// ���C�g�r���[�v���W�F�N�V�����s����擾�B
		for (int areaNo = 0; areaNo < NUM_SHADOW_MAP; areaNo++)
		{
			m_sLightingCb.m_lvp[areaNo] = m_shadowMapRender.GetLVPMatrix(areaNo);
		}

		// �|�X�g�G�t�F�N�g�I�ȗ֊s���`��Ɏg�p����[�x�𒊏o
		DepthForOutLine(rc);

		// �t�@�[�N���b�v���擾�B
		for (int i = 0; i < NUM_SHADOW_MAP; i++) {
			m_sLightingCb.m_farList[i] = m_shadowMapRender.GetFarClip(i);
		}

		// �t�H���[�h�����_�����O
		ForwardRendering(rc);

		// �w�ʉ����o���ł̗֊s���`��Ɏg�p����w�ʗp���f����`��
		RenderingModelsForOutLine(rc);

		// �|�X�g�G�t�F�N�g
		m_postEffect.Render(rc, m_mainRenderTarget);

		RenderSprite(rc);

		// ���C�������_�����O�^�[�Q�b�g�̓��e���t���[���o�b�t�@�ɃR�s�[
		CopyMainRenderTargetToFrameBuffer(rc);

		// �o�^���ꂽ�I�u�W�F�N�g�̏����N���A
		m_forwardRenderModels.clear();
		m_frontCullingModels.clear();
		m_depthForOutLineModels.clear();
		for (int i = 0; i < NUM_SHADOW_MAP; i++) {
			m_shadowMapModels[i].clear();
		}
		m_sprites.clear();
	}

	void RenderingEngine::RenderToShadowMap(RenderContext& rc)
	{
		if (m_sceneGeometryData.IsBuildshadowCasterGeometryData() == false) {
			return;
		}
		m_shadowMapRender.Render(
			rc,
			1,
			m_sceneLight.GetLightData().directionalLight[0].direction
		);
	}


	void RenderingEngine::ForwardRendering(RenderContext& rc)
	{
		// �����_�����O�^�[�Q�b�g�Ƃ��ė��p�\�ɂȂ�܂ő҂�
		rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);

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

	void RenderingEngine::RenderingModelsForOutLine(RenderContext& rc)
	{
		// �����_�����O�^�[�Q�b�g�Ƃ��ė��p�\�ɂȂ�܂ő҂�
		rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);

		// �����_�����O�^�[�Q�b�g��ݒ肷��
		rc.SetRenderTargetAndViewport(m_mainRenderTarget);

		for (auto& model : m_frontCullingModels)
		{
			// ���f����`��
			model->Draw(rc);
		}
		// �������ݏI���҂�
		rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);
	}

	void RenderingEngine::DepthForOutLine(RenderContext& rc)
	{
		rc.WaitUntilToPossibleSetRenderTarget(m_depthForOutLineRenderTarget);

		rc.SetRenderTargetAndViewport(m_depthForOutLineRenderTarget);

		// �����_�����O�^�[�Q�b�g���N���A
		rc.ClearRenderTargetView(m_depthForOutLineRenderTarget);


		for (auto& model : m_depthForOutLineModels)
		{
			model->Draw(rc);
		}

		rc.WaitUntilFinishDrawingToRenderTarget(m_depthForOutLineRenderTarget);
	}

	void RenderingEngine::RenderSprite(RenderContext& rc)
	{
		// �����_�����O�^�[�Q�b�g�Ƃ��ė��p�ł���܂ő҂�
		rc.WaitUntilToPossibleSetRenderTarget(m_2DRenderTarget);

		// �����_�����O�^�[�Q�b�g��ݒ�
		rc.SetRenderTargetAndViewport(m_2DRenderTarget);

		// �����_�����O�^�[�Q�b�g���N���A
		rc.ClearRenderTargetView(m_2DRenderTarget);

		m_mainSprite.Draw(rc);

		for (auto& sprite : m_sprites) {
			// �X�v���C�g��`��
			sprite->Draw(rc);
		}

		// 2D�`��p�����_�����O�^�[�Q�b�g�ւ̏������ݏI���҂�
		rc.WaitUntilFinishDrawingToRenderTarget(m_2DRenderTarget);

		// �����_�����O�^�[�Q�b�g�Ƃ��ė��p�ł���܂ő҂�
		rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);

		// �����_�����O�^�[�Q�b�g��ݒ�
		rc.SetRenderTargetAndViewport(m_mainRenderTarget);

		m_2DSprite.Draw(rc);

		// ���C�������_�����O�^�[�Q�b�g�ւ̏������ݏI���҂�
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

	void RenderingEngine::CalcViewProjectionMatrixForViewCulling()
	{
		Matrix projMatrix;
		projMatrix.MakeProjectionMatrix(
			g_camera3D->GetViewAngle() * 1.5f,
			g_camera3D->GetAspect(),
			g_camera3D->GetNear(),
			g_camera3D->GetFar()
		);
		m_viewProjMatrixForViewCulling.Multiply(g_camera3D->GetViewMatrix(), projMatrix);
	}
	RenderingEngine* g_renderingEngine = nullptr;
}

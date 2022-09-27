#include "k2EngineLowPreCompile.h"
#include "OutLine.h"

namespace nsK2EngineLow
{
	void OutLine::Init(RenderTarget& mainRenderTarget, RenderTarget& depthRenderTarget)
	{
		// �X�v���C�g�̏������f�[�^
		SpriteInitData edgeSpriteInitData;
		// �e�N�X�`���̓��C�������_�����O�^�[�Q�b�g�̂��̂��g�p�B
		edgeSpriteInitData.m_textures[0] = &depthRenderTarget.GetRenderTargetTexture();
		// �����_�����O�^�[�Q�b�g�̕����w��B
		edgeSpriteInitData.m_width = mainRenderTarget.GetWidth();
		edgeSpriteInitData.m_height = mainRenderTarget.GetHeight();
		// �V�F�[�_�[��fx�t�@�C���p�X���w��B
		edgeSpriteInitData.m_fxFilePath = "Assets/shader/postEffect/outLine.fx";
		// �G���g���[�|�C���g���w��B
		edgeSpriteInitData.m_vsEntryPointFunc = "VSMain";
		edgeSpriteInitData.m_psEntryPoinFunc = "PSSamplingEdge";
		// �������f�[�^�����ƂɃX�v���C�g���������B
		m_edgeSprite.Init(edgeSpriteInitData);



		// 
		m_outLineRenderTarget.Create(
			mainRenderTarget.GetWidth(),
			mainRenderTarget.GetHeight(),
			1,
			1,
			DXGI_FORMAT_R16_FLOAT,
			DXGI_FORMAT_UNKNOWN
		);

		// 
		SpriteInitData spriteInitData;
		//spriteInitData.m_textures[0] = &mainRenderTarget.GetRenderTargetTexture();
		spriteInitData.m_textures[0] = &m_outLineRenderTarget.GetRenderTargetTexture();
		spriteInitData.m_width = mainRenderTarget.GetWidth();
		spriteInitData.m_height = mainRenderTarget.GetHeight();
		spriteInitData.m_fxFilePath = "Assets/shader/postEffect/outLine.fx";
		spriteInitData.m_vsEntryPointFunc = "VSMain";
		spriteInitData.m_psEntryPoinFunc = "PSOutLineFinal";
		spriteInitData.m_alphaBlendMode = AlphaBlendMode_Multiply;
		m_sprite.Init(spriteInitData);


/*		
		spriteInitData.m_width = mainRenderTarget.GetWidth();
		spriteInitData.m_height = mainRenderTarget.GetHeight();
		spriteInitData.m_colorBufferFormat[0] = mainRenderTarget.GetColorBufferFormat();
		spriteInitData.m_textures[0] = &m_outLineRenderTarget.GetRenderTargetTexture();
		spriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";

		m_copyMainRtSprite.Init(spriteInitData); */ 

	}

	void OutLine::Render(RenderContext& rc, RenderTarget& mainRenderTarget)
	{
		// �����_�����O�^�[�Q�b�g�Ƃ��ė��p�ł���܂ő҂�
		rc.WaitUntilToPossibleSetRenderTarget(m_outLineRenderTarget);
		// �����_�����O�^�[�Q�b�g��ݒ�
		rc.SetRenderTargetAndViewport(m_outLineRenderTarget);
		//�`��B
		m_edgeSprite.Draw(rc);
		// �����_�����O�^�[�Q�b�g�ւ̏������ݏI���҂�
		//���C�������_�\�^�[�Q�b�g��RENDERTARGET����PRESENT�ցB
		rc.WaitUntilFinishDrawingToRenderTarget(m_outLineRenderTarget);

		// �����_�����O�^�[�Q�b�g�Ƃ��ė��p�ł���܂ő҂�
		rc.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
		// �����_�����O�^�[�Q�b�g��ݒ�
		rc.SetRenderTargetAndViewport(mainRenderTarget);
		// �|�X�g�G�t�F�N�g�̌��ʂ����C�������_�����O�^�[�Q�b�g�ɔ��f�B
		m_sprite.Draw(rc);
		// �����_�����O�^�[�Q�b�g�ւ̏������ݏI���҂�
		rc.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);
	}
}

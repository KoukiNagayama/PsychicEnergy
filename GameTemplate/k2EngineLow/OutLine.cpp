#include "k2EngineLowPreCompile.h"
#include "OutLine.h"

namespace nsK2EngineLow
{
	void OutLine::Init(RenderTarget& mainRenderTarget, RenderTarget& depthRenderTarget)
	{
		// �G�b�W���o�p�X�v���C�g�̏������f�[�^
		SpriteInitData edgeSpriteInitData;
		// �e�N�X�`���͐[�x�L�^�p�̃����_�����O�^�[�Q�b�g�̂��̂��g�p�B
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



		// �A�E�g���C���`��p�̃����_�����O�^�[�Q�b�g���쐬�B
		m_outLineRenderTarget.Create(
			mainRenderTarget.GetWidth(),
			mainRenderTarget.GetHeight(),
			1,
			1,
			DXGI_FORMAT_R16_FLOAT,
			DXGI_FORMAT_UNKNOWN
		);

		// �ŏI�����p�̃X�v���C�g�̏������f�[�^�B
		SpriteInitData finalSpriteInitData;
		// �e�N�X�`���̓A�E�g���C���`��p�̃����_�����O�^�[�Q�b�g�̂��̂��g�p�B
		finalSpriteInitData.m_textures[0] = &m_outLineRenderTarget.GetRenderTargetTexture();
		// �X�v���C�g�̕����w��B
		finalSpriteInitData.m_width = mainRenderTarget.GetWidth();
		finalSpriteInitData.m_height = mainRenderTarget.GetHeight();
		// �V�F�[�_�[��fx�t�@�C���p�X���w��B
		finalSpriteInitData.m_fxFilePath = "Assets/shader/postEffect/outLine.fx";
		// �G���g���[�|�C���g���w��B
		finalSpriteInitData.m_vsEntryPointFunc = "VSMain";
		finalSpriteInitData.m_psEntryPoinFunc = "PSOutLineFinal";
		// �A���t�@�u�����f�B���O���[�h����Z�����Ɏw��B
		finalSpriteInitData.m_alphaBlendMode = AlphaBlendMode_Multiply;
		// �������f�[�^�����ƂɃX�v���C�g���������B
		m_finalSprite.Init(finalSpriteInitData);

	}

	void OutLine::Render(RenderContext& rc, RenderTarget& mainRenderTarget)
	{
		// �����_�����O�^�[�Q�b�g�Ƃ��ė��p�ł���܂ő҂�
		rc.WaitUntilToPossibleSetRenderTarget(m_outLineRenderTarget);
		// �����_�����O�^�[�Q�b�g��ݒ�
		rc.SetRenderTargetAndViewport(m_outLineRenderTarget);
		// �G�b�W���o���s���B
		m_edgeSprite.Draw(rc);
		// �����_�����O�^�[�Q�b�g�ւ̏������ݏI���҂�
		//���C�������_�\�^�[�Q�b�g��RENDERTARGET����PRESENT�ցB
		rc.WaitUntilFinishDrawingToRenderTarget(m_outLineRenderTarget);

		// �����_�����O�^�[�Q�b�g�Ƃ��ė��p�ł���܂ő҂�
		rc.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
		// �����_�����O�^�[�Q�b�g��ݒ�
		rc.SetRenderTargetAndViewport(mainRenderTarget);
		// ��Z��������B
		m_finalSprite.Draw(rc);
		// �����_�����O�^�[�Q�b�g�ւ̏������ݏI���҂�
		rc.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);
	}
}

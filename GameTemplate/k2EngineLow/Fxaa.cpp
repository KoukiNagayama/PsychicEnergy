#include "k2EngineLowPreCompile.h"
#include "Fxaa.h"

namespace nsK2EngineLow
{
    void Fxaa::Init(RenderTarget& mainRenderTarget)
    {
        // �ŏI�����p�̃X�v���C�g������������
        SpriteInitData spriteInitData;
        spriteInitData.m_textures[0] = &mainRenderTarget.GetRenderTargetTexture();
        // �𑜓x��mainRenderTarget�̕��ƍ���
        spriteInitData.m_width = mainRenderTarget.GetWidth();
        spriteInitData.m_height = mainRenderTarget.GetHeight();
        // �V�F�[�_�[�̃t�@�C���p�X
        spriteInitData.m_fxFilePath = "Assets/shader/postEffect/fxaa.fx";
        // �G���g���[�|�C���g
        spriteInitData.m_vsEntryPointFunc = "VSMain";
        spriteInitData.m_psEntryPoinFunc = "PSMain";

        // �A���t�@�u�����f�B���O���[�h��ݒ�
        spriteInitData.m_alphaBlendMode = AlphaBlendMode_None;

        //�𑜓x��GPU�ɑ��邽�߂̒萔�o�b�t�@��ݒ肷��B
        spriteInitData.m_expandConstantBuffer = (void*)&m_cB;
        spriteInitData.m_expandConstantBufferSize = sizeof(FaxxBuffer) +
            (16 - (sizeof(FaxxBuffer) % 16));
        // 
        m_finalSprite.Init(spriteInitData);

        m_fxaaRt.Create(
            mainRenderTarget.GetWidth(),
            mainRenderTarget.GetHeight(),
            1,
            1,
            DXGI_FORMAT_R8G8B8A8_UNORM,
            DXGI_FORMAT_UNKNOWN
        );

        SpriteInitData initData;
        initData.m_width = mainRenderTarget.GetWidth();
        initData.m_height = mainRenderTarget.GetHeight();
        initData.m_colorBufferFormat[0] = mainRenderTarget.GetColorBufferFormat();
        initData.m_fxFilePath = "Assets/shader/Sprite.fx";
        initData.m_textures[0] = &m_fxaaRt.GetRenderTargetTexture();
        m_copyMainRtSprite.Init(initData);
    }

    void Fxaa::Render(RenderContext& rc, RenderTarget& mainRenderTarget)
    {
        // �����_�����O�^�[�Q�b�g�Ƃ��ė��p�ł���܂ő҂�
        rc.WaitUntilToPossibleSetRenderTarget(m_fxaaRt);
        // �����_�����O�^�[�Q�b�g��ݒ�
        rc.SetRenderTargetAndViewport(m_fxaaRt);
        m_cB.bufferW = static_cast<float>(g_graphicsEngine->GetFrameBufferWidth());
        m_cB.bufferH = static_cast<float>(g_graphicsEngine->GetFrameBufferHeight());
        //�`��B
        m_finalSprite.Draw(rc);
        // �����_�����O�^�[�Q�b�g�ւ̏������ݏI���҂�
        //���C�������_�\�^�[�Q�b�g��RENDERTARGET����PRESENT�ցB
        rc.WaitUntilFinishDrawingToRenderTarget(m_fxaaRt);

        // �����_�����O�^�[�Q�b�g�Ƃ��ė��p�ł���܂ő҂�
        rc.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
        // �����_�����O�^�[�Q�b�g��ݒ�
        rc.SetRenderTargetAndViewport(mainRenderTarget);
        // �|�X�g�G�t�F�N�g�̌��ʂ����C�������_�����O�^�[�Q�b�g�ɔ��f�B
        m_copyMainRtSprite.Draw(rc);
        // �����_�����O�^�[�Q�b�g�ւ̏������ݏI���҂�
        rc.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);
    }
}

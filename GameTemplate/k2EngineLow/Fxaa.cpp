#include "k2EngineLowPreCompile.h"
#include "Fxaa.h"

namespace nsK2EngineLow
{
    void Fxaa::Init(RenderTarget& mainRenderTarget)
    {
        // 最終合成用のスプライトを初期化する
        SpriteInitData spriteInitData;
        spriteInitData.m_textures[0] = &mainRenderTarget.GetRenderTargetTexture();
        // 解像度はmainRenderTargetの幅と高さ
        spriteInitData.m_width = mainRenderTarget.GetWidth();
        spriteInitData.m_height = mainRenderTarget.GetHeight();
        // シェーダーのファイルパス
        spriteInitData.m_fxFilePath = "Assets/shader/postEffect/fxaa.fx";
        // エントリーポイント
        spriteInitData.m_vsEntryPointFunc = "VSMain";
        spriteInitData.m_psEntryPoinFunc = "PSMain";

        // アルファブレンディングモードを設定
        spriteInitData.m_alphaBlendMode = AlphaBlendMode_None;

        //解像度をGPUに送るための定数バッファを設定する。
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
        // レンダリングターゲットとして利用できるまで待つ
        rc.WaitUntilToPossibleSetRenderTarget(m_fxaaRt);
        // レンダリングターゲットを設定
        rc.SetRenderTargetAndViewport(m_fxaaRt);
        m_cB.bufferW = static_cast<float>(g_graphicsEngine->GetFrameBufferWidth());
        m_cB.bufferH = static_cast<float>(g_graphicsEngine->GetFrameBufferHeight());
        //描画。
        m_finalSprite.Draw(rc);
        // レンダリングターゲットへの書き込み終了待ち
        //メインレンダ―ターゲットをRENDERTARGETからPRESENTへ。
        rc.WaitUntilFinishDrawingToRenderTarget(m_fxaaRt);

        // レンダリングターゲットとして利用できるまで待つ
        rc.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
        // レンダリングターゲットを設定
        rc.SetRenderTargetAndViewport(mainRenderTarget);
        // ポストエフェクトの結果をメインレンダリングターゲットに反映。
        m_copyMainRtSprite.Draw(rc);
        // レンダリングターゲットへの書き込み終了待ち
        rc.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);
    }
}

#include "k2EngineLowPreCompile.h"
#include "OutLine.h"

namespace nsK2EngineLow
{
	void OutLine::Init(RenderTarget& mainRenderTarget, RenderTarget& depthRenderTarget)
	{
		// スプライトの初期化データ
		SpriteInitData edgeSpriteInitData;
		// テクスチャはメインレンダリングターゲットのものを使用。
		edgeSpriteInitData.m_textures[0] = &depthRenderTarget.GetRenderTargetTexture();
		// レンダリングターゲットの幅を指定。
		edgeSpriteInitData.m_width = mainRenderTarget.GetWidth();
		edgeSpriteInitData.m_height = mainRenderTarget.GetHeight();
		// シェーダーのfxファイルパスを指定。
		edgeSpriteInitData.m_fxFilePath = "Assets/shader/postEffect/outLine.fx";
		// エントリーポイントを指定。
		edgeSpriteInitData.m_vsEntryPointFunc = "VSMain";
		edgeSpriteInitData.m_psEntryPoinFunc = "PSSamplingEdge";
		// 初期化データをもとにスプライトを初期化。
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
		// レンダリングターゲットとして利用できるまで待つ
		rc.WaitUntilToPossibleSetRenderTarget(m_outLineRenderTarget);
		// レンダリングターゲットを設定
		rc.SetRenderTargetAndViewport(m_outLineRenderTarget);
		//描画。
		m_edgeSprite.Draw(rc);
		// レンダリングターゲットへの書き込み終了待ち
		//メインレンダ―ターゲットをRENDERTARGETからPRESENTへ。
		rc.WaitUntilFinishDrawingToRenderTarget(m_outLineRenderTarget);

		// レンダリングターゲットとして利用できるまで待つ
		rc.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
		// レンダリングターゲットを設定
		rc.SetRenderTargetAndViewport(mainRenderTarget);
		// ポストエフェクトの結果をメインレンダリングターゲットに反映。
		m_sprite.Draw(rc);
		// レンダリングターゲットへの書き込み終了待ち
		rc.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);
	}
}

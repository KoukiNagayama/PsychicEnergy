#include "k2EngineLowPreCompile.h"
#include "OutLine.h"

namespace nsK2EngineLow
{
	void OutLine::Init(RenderTarget& mainRenderTarget, RenderTarget& depthRenderTarget)
	{
		// エッジ抽出用スプライトの初期化データ
		SpriteInitData edgeSpriteInitData;
		// テクスチャは深度記録用のレンダリングターゲットのものを使用。
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



		// アウトライン描画用のレンダリングターゲットを作成。
		m_outLineRenderTarget.Create(
			mainRenderTarget.GetWidth(),
			mainRenderTarget.GetHeight(),
			1,
			1,
			DXGI_FORMAT_R16_FLOAT,
			DXGI_FORMAT_UNKNOWN
		);

		// 最終合成用のスプライトの初期化データ。
		SpriteInitData finalSpriteInitData;
		// テクスチャはアウトライン描画用のレンダリングターゲットのものを使用。
		finalSpriteInitData.m_textures[0] = &m_outLineRenderTarget.GetRenderTargetTexture();
		// スプライトの幅を指定。
		finalSpriteInitData.m_width = mainRenderTarget.GetWidth();
		finalSpriteInitData.m_height = mainRenderTarget.GetHeight();
		// シェーダーのfxファイルパスを指定。
		finalSpriteInitData.m_fxFilePath = "Assets/shader/postEffect/outLine.fx";
		// エントリーポイントを指定。
		finalSpriteInitData.m_vsEntryPointFunc = "VSMain";
		finalSpriteInitData.m_psEntryPoinFunc = "PSOutLineFinal";
		// アルファブレンディングモードを乗算合成に指定。
		finalSpriteInitData.m_alphaBlendMode = AlphaBlendMode_Multiply;
		// 初期化データをもとにスプライトを初期化。
		m_finalSprite.Init(finalSpriteInitData);

	}

	void OutLine::Render(RenderContext& rc, RenderTarget& mainRenderTarget)
	{
		// レンダリングターゲットとして利用できるまで待つ
		rc.WaitUntilToPossibleSetRenderTarget(m_outLineRenderTarget);
		// レンダリングターゲットを設定
		rc.SetRenderTargetAndViewport(m_outLineRenderTarget);
		// エッジ検出を行う。
		m_edgeSprite.Draw(rc);
		// レンダリングターゲットへの書き込み終了待ち
		//メインレンダ―ターゲットをRENDERTARGETからPRESENTへ。
		rc.WaitUntilFinishDrawingToRenderTarget(m_outLineRenderTarget);

		// レンダリングターゲットとして利用できるまで待つ
		rc.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
		// レンダリングターゲットを設定
		rc.SetRenderTargetAndViewport(mainRenderTarget);
		// 乗算合成する。
		m_finalSprite.Draw(rc);
		// レンダリングターゲットへの書き込み終了待ち
		rc.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);
	}
}

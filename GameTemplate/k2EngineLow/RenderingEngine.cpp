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
		// メインレンダリングターゲットを作成
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
		// スプライトの初期化オブジェクト
		SpriteInitData spriteInitData;
		// テクスチャ
		spriteInitData.m_textures[0] = &m_mainRenderTarget.GetRenderTargetTexture();
		// 解像度
		spriteInitData.m_width = g_graphicsEngine->GetFrameBufferWidth();
		spriteInitData.m_height = g_graphicsEngine->GetFrameBufferHeight();
		// シェーダーのファイルパスを指定
		spriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";

		// 初期化オブジェクトをもとにスプライトを初期化
		m_copyMainRtToFrameBufferSprite.Init(spriteInitData);
	}

	void RenderingEngine::Execute(RenderContext& rc)
	{
		// フォワードレンダリング
		ForwardRendering(rc);

		// メインレンダリングターゲットの内容をフレームバッファにコピー
		CopyMainRenderTargetToFrameBuffer(rc);

		// 登録されたオブジェクトの情報をクリア
		m_forwardRenderModels.clear();
	}

	void RenderingEngine::ForwardRendering(RenderContext& rc)
	{
		// レンダリングターゲットとして利用可能になるまで待つ
		rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);

		// レンダリングターゲットを設定する
		rc.SetRenderTargetAndViewport(m_mainRenderTarget);

		// レンダリングターゲットをクリアする
		rc.ClearRenderTargetView(m_mainRenderTarget);

		for (auto& model : m_forwardRenderModels)
		{
			// モデルを描画
			model->Draw(rc);
		}
		// 書き込み終了待ち
		rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);
	}

	void RenderingEngine::CopyMainRenderTargetToFrameBuffer(RenderContext& rc)
	{
		rc.SetRenderTarget(
			g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
			g_graphicsEngine->GetCurrentFrameBuffuerDSV()
		);

		// ビューポートを指定する
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

#include "k2EngineLowPreCompile.h"
#include "RenderingEngine.h"

namespace nsK2EngineLow
{
	void RenderingEngine::Init()
	{
		m_sceneLight.Init();
		InitMainRenderTarget();
		InitDepthForOutLineRenderTarget();
		InitCopyMainRenderTargetToFrameBufferSprite();
		m_postEffect.Init(m_mainRenderTarget, m_depthForOutLineRenderTarget);
		InitShadowMapRender();
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

	void RenderingEngine::InitDepthForOutLineRenderTarget()
	{
		// レンダリングターゲットとなるテクスチャを埋める値
		//float clearColor[4] = { 10000.0f,10000.0f,10000.0f,1.0f };
		// 深度値記録用のレンダリングターゲットを作成
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

	void RenderingEngine::InitShadowMapRender()
	{
		m_shadowMapRender.Init();
	}

	void RenderingEngine::Execute(RenderContext& rc)
	{
		// シーンライトの更新
		m_sceneLight.Update();

		RenderToShadowMap(rc);

		// ポストエフェクト的な輪郭線描画に使用する深度を抽出
		DepthForOutLine(rc);

		// フォワードレンダリング
		ForwardRendering(rc);

		// 背面押し出しでの輪郭線描画に使用する背面用モデルを描画
		RenderingModelsForOutLine(rc);

		// ポストエフェクト
		m_postEffect.Render(rc, m_mainRenderTarget);

		// メインレンダリングターゲットの内容をフレームバッファにコピー
		CopyMainRenderTargetToFrameBuffer(rc);

		// 登録されたオブジェクトの情報をクリア
		m_forwardRenderModels.clear();
		m_frontCullingModels.clear();
		m_depthForOutLineModels.clear();
		for (int i = 0; i < NUM_SHADOW_MAP; i++) {
			m_shadowMapModels[i].clear();
		}
	}

	void RenderingEngine::RenderToShadowMap(RenderContext& rc)
	{
		m_shadowMapRender.Render(
			rc,
			1,
			m_sceneLight.GetLightData().directionalLight[0].direction
		);
	}


	void RenderingEngine::ForwardRendering(RenderContext& rc)
	{
		// レンダリングターゲットとして利用可能になるまで待つ
		rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);

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

	void RenderingEngine::RenderingModelsForOutLine(RenderContext& rc)
	{
		// レンダリングターゲットとして利用可能になるまで待つ
		rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);

		// レンダリングターゲットを設定する
		rc.SetRenderTargetAndViewport(m_mainRenderTarget);

		for (auto& model : m_frontCullingModels)
		{
			// モデルを描画
			model->Draw(rc);
		}
		// 書き込み終了待ち
		rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);
	}

	void RenderingEngine::DepthForOutLine(RenderContext& rc)
	{
		rc.WaitUntilToPossibleSetRenderTarget(m_depthForOutLineRenderTarget);

		rc.SetRenderTargetAndViewport(m_depthForOutLineRenderTarget);

		// レンダリングターゲットをクリア
		rc.ClearRenderTargetView(m_depthForOutLineRenderTarget);


		for (auto& model : m_depthForOutLineModels)
		{
			model->Draw(rc);
		}

		rc.WaitUntilFinishDrawingToRenderTarget(m_depthForOutLineRenderTarget);
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
	RenderingEngine* g_renderingEngine = nullptr;
}

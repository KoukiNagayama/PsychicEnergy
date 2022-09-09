#pragma once
namespace nsK2EngineLow
{
	class RenderingEngine
	{
	public:
		RenderingEngine() {};
		~RenderingEngine() {};

		/// <summary>
		/// レンダリングパイプラインを初期化。
		/// </summary>
		void Init();
		/// <summary>
		/// レンダリングパイプラインを実行。
		/// </summary>
		/// <param name="rc"></param>
		void Execute(RenderContext& rc);
		/// <summary>
		/// フォワードレンダリングの描画パスにモデルを追加
		/// </summary>
		/// <param name="model"></param>
		void Add3DModelToForwardRenderPass(Model& model)
		{
			m_forwardRenderModels.push_back(&model);
		}
	private:
		/// <summary>
		/// メインレンダリングターゲットを初期化
		/// </summary>
		void InitMainRenderTarget();
		/// <summary>
		/// メインレンダリングターゲットのカラーバッファの内容をフレームバッファにコピーするためのスプライトを初期化
		/// </summary>
		void InitCopyMainRenderTargetToFrameBufferSprite();
		/// <summary>
		/// フォワードレンダリング
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト</param>
		void ForwardRendering(RenderContext& rc);
		/// <summary>
		/// メインレンダリングターゲットの内容をフレームバッファにコピー
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト</param>
		void CopyMainRenderTargetToFrameBuffer(RenderContext& rc);
	private:
		std::vector<Model*>		m_forwardRenderModels;					// フォワードレンダリングの描画パスで描画されるモデル
		RenderTarget			m_mainRenderTarget;						// メインレンダリングターゲット
		Sprite					m_copyMainRtToFrameBufferSprite;		// メインレンダリングターゲットの内容をフレームバッファにコピーするためのスプライト
	};

	extern RenderingEngine g_renderingEngine;
}



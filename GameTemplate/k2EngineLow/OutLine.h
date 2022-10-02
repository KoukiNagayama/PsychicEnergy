#pragma once
namespace nsK2EngineLow
{
	class OutLine : public Noncopyable
	{
	public:
		/// <summary>
		/// 初期化。
		/// </summary>
		/// <param name="mainRenderTarget">メインレンダリングターゲット</param>
		/// <param name="depthRenderTarget">深度値を記録したレンダリングターゲット</param>
		void Init(RenderTarget& mainRenderTarget, RenderTarget& depthRenderTarget);
		/// <summary>
		/// 描画。
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト。</param>
		/// <param name="mainRenderTarget">メインレンダリングターゲット。</param>
		void Render(RenderContext& rc, RenderTarget& mainRenderTarget);
	private:
		RenderTarget	m_outLineRenderTarget;	
		Sprite			m_edgeSprite;       //
		Sprite			m_finalSprite;
		Sprite			m_copyMainRtSprite;  //
	};
}



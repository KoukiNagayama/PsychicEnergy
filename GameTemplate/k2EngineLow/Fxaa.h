#pragma once

namespace nsK2EngineLow
{
	class Fxaa : public Noncopyable
	{
	public:
		/// <summary>
		/// 初期化
		/// </summary>
		/// <param name="mainRenderTarget">メインレンダリングターゲット</param>
		void Init(RenderTarget& mainRenderTarget);
		/// <summary>
		/// 描画
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト</param>
		/// <param name="mainRenderTarget">メインレンダリングターゲット</param>
		void Render(RenderContext& rc, RenderTarget& mainRenderTarget);
	private:
		struct FaxxBuffer
		{
			float bufferW;		// 横幅
			float bufferH;		// 縦幅
		};
		RenderTarget m_fxaaRt;		// FXAAを行うレンダリングターゲット。
		Sprite m_finalSprite;		// 最終合成用のスプライト
		Sprite m_copyMainRtSprite;  // メインレンダリングターゲットに対してコピーを行うためのスプライト　
		FaxxBuffer m_cB;			// 解像度をGPUに送るための定数バッファ―。
	};
}



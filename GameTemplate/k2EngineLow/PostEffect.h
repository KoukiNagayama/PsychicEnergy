#pragma once
#include "Fxaa.h"
#include "OutLine.h"

namespace nsK2EngineLow
{
	class PostEffect : public Noncopyable
	{
	public:
		/// <summary>
		/// 初期化
		/// </summary>
		/// <param name="mainRenderTarget">メインレンダリングターゲット</param>
		void Init(RenderTarget& mainRenderTarget, RenderTarget& depthRenderTarget);
		/// <summary>
		/// 描画
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト</param>
		/// <param name="mainRenderTarget">メインレンダリングターゲット</param>
		void Render(RenderContext& rc, RenderTarget& mainRenderTarget);
	private:
		Fxaa			m_fxaa;				// FXAA
		OutLine			m_outLine;			// 輪郭線
	};
}



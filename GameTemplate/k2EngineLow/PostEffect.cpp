#include "k2EngineLowPreCompile.h"
#include "PostEffect.h"

namespace nsK2EngineLow
{
	void PostEffect::Init(RenderTarget& mainRenderTarget, RenderTarget& depthRenderTarget)
	{
		// FXAA‚Ì‰Šú‰»
		m_fxaa.Init(mainRenderTarget);

		m_outLine.Init(mainRenderTarget, depthRenderTarget);
	}

	void PostEffect::Render(RenderContext& rc, RenderTarget& mainRenderTarget)
	{
		// FXAA
		m_fxaa.Render(rc, mainRenderTarget);

		m_outLine.Render(rc, mainRenderTarget);
	}
}

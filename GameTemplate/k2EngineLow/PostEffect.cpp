#include "k2EngineLowPreCompile.h"
#include "PostEffect.h"

namespace nsK2EngineLow
{
	void PostEffect::Init(RenderTarget& mainRenderTarget)
	{
		// FXAA‚Ì‰Šú‰»
		m_fxaa.Init(mainRenderTarget);
	}

	void PostEffect::Render(RenderContext& rc, RenderTarget& mainRenderTarget)
	{
		// FXAA
		m_fxaa.Render(rc, mainRenderTarget);
	}
}

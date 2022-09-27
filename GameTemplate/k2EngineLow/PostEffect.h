#pragma once
#include "Fxaa.h"
#include "OutLine.h"

namespace nsK2EngineLow
{
	class PostEffect : public Noncopyable
	{
	public:
		/// <summary>
		/// ������
		/// </summary>
		/// <param name="mainRenderTarget">���C�������_�����O�^�[�Q�b�g</param>
		void Init(RenderTarget& mainRenderTarget, RenderTarget& depthRenderTarget);
		/// <summary>
		/// �`��
		/// </summary>
		/// <param name="rc">�����_�����O�R���e�L�X�g</param>
		/// <param name="mainRenderTarget">���C�������_�����O�^�[�Q�b�g</param>
		void Render(RenderContext& rc, RenderTarget& mainRenderTarget);
	private:
		Fxaa			m_fxaa;				// FXAA
		OutLine			m_outLine;			// �֊s��
	};
}



#pragma once
namespace nsK2EngineLow
{
	class OutLine : public Noncopyable
	{
	public:
		/// <summary>
		/// �������B
		/// </summary>
		/// <param name="mainRenderTarget">���C�������_�����O�^�[�Q�b�g</param>
		/// <param name="depthRenderTarget">�[�x�l���L�^���������_�����O�^�[�Q�b�g</param>
		void Init(RenderTarget& mainRenderTarget, RenderTarget& depthRenderTarget);
		/// <summary>
		/// �`��B
		/// </summary>
		/// <param name="rc">�����_�����O�R���e�L�X�g�B</param>
		/// <param name="mainRenderTarget">���C�������_�����O�^�[�Q�b�g�B</param>
		void Render(RenderContext& rc, RenderTarget& mainRenderTarget);
	private:
		RenderTarget	m_outLineRenderTarget;	
		Sprite			m_edgeSprite;       //
		Sprite			m_finalSprite;
		Sprite			m_copyMainRtSprite;  //
	};
}



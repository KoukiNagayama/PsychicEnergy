#pragma once

namespace nsK2EngineLow
{
	class Fxaa : public Noncopyable
	{
	public:
		/// <summary>
		/// ������
		/// </summary>
		/// <param name="mainRenderTarget">���C�������_�����O�^�[�Q�b�g</param>
		void Init(RenderTarget& mainRenderTarget);
		/// <summary>
		/// �`��
		/// </summary>
		/// <param name="rc">�����_�����O�R���e�L�X�g</param>
		/// <param name="mainRenderTarget">���C�������_�����O�^�[�Q�b�g</param>
		void Render(RenderContext& rc, RenderTarget& mainRenderTarget);
	private:
		struct FaxxBuffer
		{
			float bufferW;		// ����
			float bufferH;		// �c��
		};
		RenderTarget m_fxaaRt;		// FXAA���s�������_�����O�^�[�Q�b�g�B
		Sprite m_finalSprite;		// �ŏI�����p�̃X�v���C�g
		Sprite m_copyMainRtSprite;  // ���C�������_�����O�^�[�Q�b�g�ɑ΂��ăR�s�[���s�����߂̃X�v���C�g�@
		FaxxBuffer m_cB;			// �𑜓x��GPU�ɑ��邽�߂̒萔�o�b�t�@�\�B
	};
}



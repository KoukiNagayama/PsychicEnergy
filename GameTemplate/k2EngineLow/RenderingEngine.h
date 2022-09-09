#pragma once
namespace nsK2EngineLow
{
	class RenderingEngine
	{
	public:
		RenderingEngine() {};
		~RenderingEngine() {};

		/// <summary>
		/// �����_�����O�p�C�v���C�����������B
		/// </summary>
		void Init();
		/// <summary>
		/// �����_�����O�p�C�v���C�������s�B
		/// </summary>
		/// <param name="rc"></param>
		void Execute(RenderContext& rc);
		/// <summary>
		/// �t�H���[�h�����_�����O�̕`��p�X�Ƀ��f����ǉ�
		/// </summary>
		/// <param name="model"></param>
		void Add3DModelToForwardRenderPass(Model& model)
		{
			m_forwardRenderModels.push_back(&model);
		}
	private:
		/// <summary>
		/// ���C�������_�����O�^�[�Q�b�g��������
		/// </summary>
		void InitMainRenderTarget();
		/// <summary>
		/// ���C�������_�����O�^�[�Q�b�g�̃J���[�o�b�t�@�̓��e���t���[���o�b�t�@�ɃR�s�[���邽�߂̃X�v���C�g��������
		/// </summary>
		void InitCopyMainRenderTargetToFrameBufferSprite();
		/// <summary>
		/// �t�H���[�h�����_�����O
		/// </summary>
		/// <param name="rc">�����_�����O�R���e�L�X�g</param>
		void ForwardRendering(RenderContext& rc);
		/// <summary>
		/// ���C�������_�����O�^�[�Q�b�g�̓��e���t���[���o�b�t�@�ɃR�s�[
		/// </summary>
		/// <param name="rc">�����_�����O�R���e�L�X�g</param>
		void CopyMainRenderTargetToFrameBuffer(RenderContext& rc);
	private:
		std::vector<Model*>		m_forwardRenderModels;					// �t�H���[�h�����_�����O�̕`��p�X�ŕ`�悳��郂�f��
		RenderTarget			m_mainRenderTarget;						// ���C�������_�����O�^�[�Q�b�g
		Sprite					m_copyMainRtToFrameBufferSprite;		// ���C�������_�����O�^�[�Q�b�g�̓��e���t���[���o�b�t�@�ɃR�s�[���邽�߂̃X�v���C�g
	};

	extern RenderingEngine g_renderingEngine;
}



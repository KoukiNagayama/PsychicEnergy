#pragma once
#include "Fxaa.h"
#include "PostEffect.h"
#include "SceneLight.h"

namespace nsK2EngineLow
{
	class RenderingEngine : public Noncopyable
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
		/// �t�H���[�h�����_�����O�̕`��p�X�Ƀ��f����ǉ��B
		/// </summary>
		/// <param name="model"></param>
		void Add3DModelToForwardRenderPass(Model& model)
		{
			m_forwardRenderModels.push_back(&model);
		}
		/// <summary>
		/// �w�ʉ����o���ł̗֊s���`��p�w�ʃ��f���̕`��p�X�Ƀ��f����ǉ��B
		/// </summary>
		/// <param name="model"></param>
		void Add3DModelToRenderingModelsForOutLine(Model& model)
		{
			m_frontCullingModels.push_back(&model);
		}
		/// <summary>
		/// �|�X�g�G�t�F�N�g�I�ȗ֊s���`��Ɏg�p����[�x�𒊏o���邽�ߕ`��p�X�Ƀ��f����ǉ��B
		/// </summary>
		/// <param name="model"></param>
		void Add3DModelToDepthForOutLinePass(Model& model)
		{
			m_depthForOutLineModels.push_back(&model);
		}
		/// <summary>
		/// �V�[�����C�g���擾�B
		/// </summary>
		/// <returns></returns>
		SceneLight& GetSceneLight()
		{
			return m_sceneLight;
		}
		/// <summary>
/// �[�x�l�L�^�p�̃����_�����O�^�[�Q�b�g���擾
/// </summary>
/// <returns>�[�x�l�L�^�p�����_�����O�^�[�Q�b�g</returns>
		RenderTarget& GetDepthValue()
		{
			return m_depthForOutLineRenderTarget;
		}
	private:
		/// <summary>
		/// ���C�������_�����O�^�[�Q�b�g��������
		/// </summary>
		void InitMainRenderTarget();
		/// <summary>
		/// �֊s���`��Ɏg�p����[�x�l�L�^�p�̃����_�����O�^�[�Q�b�g��������
		/// </summary>
		void InitDepthForOutLineRenderTarget();
		/// <summary>
		/// ���C�������_�����O�^�[�Q�b�g�̃J���[�o�b�t�@�̓��e���t���[���o�b�t�@�ɃR�s�[���邽�߂̃X�v���C�g��������
		/// </summary>
		void InitCopyMainRenderTargetToFrameBufferSprite();
		/// <summary>
		/// �֊s���p���f����`��
		/// </summary>
		void RenderingModelsForOutLine(RenderContext& rc);
		/// <summary>
		/// �t�H���[�h�����_�����O
		/// </summary>
		/// <param name="rc">�����_�����O�R���e�L�X�g</param>
		void ForwardRendering(RenderContext& rc);
		/// <summary>
		/// �[�x�l�L�^�p�X
		/// </summary>
		/// <param name="rc"></param>
		void DepthForOutLine(RenderContext& rc);
		/// <summary>
		/// ���C�������_�����O�^�[�Q�b�g�̓��e���t���[���o�b�t�@�ɃR�s�[
		/// </summary>
		/// <param name="rc">�����_�����O�R���e�L�X�g</param>
		void CopyMainRenderTargetToFrameBuffer(RenderContext& rc);
	private:
		std::vector<Model*>		m_forwardRenderModels;					// �t�H���[�h�����_�����O�̕`��p�X�ŕ`�悳��郂�f��
		std::vector<Model*>		m_frontCullingModels;					// �t�����g�J�����O���ꂽ���f��
		std::vector<Model*>     m_depthForOutLineModels;				// �[�x�l���L�^���邽�߂̃��f��
		RenderTarget			m_mainRenderTarget;						// ���C�������_�����O�^�[�Q�b�g
		RenderTarget			m_depthForOutLineRenderTarget;			// �֊s���p�̐[�x�l�����_�����O�^�[�Q�b�g
		Sprite					m_copyMainRtToFrameBufferSprite;		// ���C�������_�����O�^�[�Q�b�g�̓��e���t���[���o�b�t�@�ɃR�s�[���邽�߂̃X�v���C�g
		PostEffect				m_postEffect;							// �|�X�g�G�t�F�N�g
		SceneLight				m_sceneLight;
	};

	extern RenderingEngine* g_renderingEngine;
}



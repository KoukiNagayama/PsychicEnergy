#pragma once
#include "Fxaa.h"
#include "PostEffect.h"
#include "SceneLight.h"

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
		/// �t�H���[�h�����_�����O�̕`��p�X�Ƀ��f����ǉ��B
		/// </summary>
		/// <param name="model"></param>
		void Add3DModelToForwardRenderPass(Model& model)
		{
			m_forwardRenderModels.push_back(&model);
		}
		/// <summary>
		/// �֊s���p���f���̕`��p�X�Ƀ��f����ǉ��B
		/// </summary>
		/// <param name="model"></param>
		void Add3DModelToRenderingModelsForOutLine(Model& model)
		{
			m_frontCullingModels.push_back(&model);
		}
		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		SceneLight& GetSceneLight()
		{
			return m_sceneLight;
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
		/// �֊s���p���f����`��
		/// </summary>
		void RenderingModelsForOutLine(RenderContext& rc);
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
		std::vector<Model*>		m_frontCullingModels;
		RenderTarget			m_mainRenderTarget;						// ���C�������_�����O�^�[�Q�b�g
		Sprite					m_copyMainRtToFrameBufferSprite;		// ���C�������_�����O�^�[�Q�b�g�̓��e���t���[���o�b�t�@�ɃR�s�[���邽�߂̃X�v���C�g
		PostEffect				m_postEffect;							// �|�X�g�G�t�F�N�g
		SceneLight				m_sceneLight;
	};

	extern RenderingEngine* g_renderingEngine;
}



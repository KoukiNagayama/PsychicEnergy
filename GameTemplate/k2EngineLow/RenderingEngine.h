#pragma once
#include "Fxaa.h"
#include "PostEffect.h"
#include "SceneLight.h"
#include "ShadowMapRender.h"
#include "SceneGeometryData.h"

namespace nsK2EngineLow
{
	class RenderingEngine : public Noncopyable
	{
		struct SLightingCB
		{
			Light m_light;						// ���C�g
			Matrix m_lvp[NUM_SHADOW_MAP];		// ���C�g�r���[�v���W�F�N�V�����s��
			int m_isFloating = 0;
			float m_farList[NUM_SHADOW_MAP];	
			
		};
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
		/// �V���h�E�}�b�v�`��̃p�X�Ƀ��f����ǉ�
		/// </summary>
		/// <param name="model">���f��</param>
		/// <param name="areaNo">�J�X�P�[�h�V���h�E�ŕ������ꂽ�G���A</param>
		void Add3DModelToRenderToShadowMapPass(Model& model, int areaNo)
		{
			m_shadowMapModels[areaNo].push_back(&model);
		}
		/// <summary>
		/// �V���h�E�}�b�v�ɕ`�悷�郂�f�����擾
		/// </summary>
		/// <param name="areaNo">�J�X�P�[�h�V���h�E�ŕ������ꂽ�G���A</param>
		/// <returns>�V���h�E�}�b�v�ɕ`�悷�郂�f���̔z��</returns>
		const std::vector<Model*>& GetShadowMapModel(int areaNo) const 
		{
			return m_shadowMapModels[areaNo];
		}
		Texture& GetShadowMap(int areaNo)
		{
			return m_shadowMapRender.GetShadowMap(areaNo);
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
		/// ���f���`�掞�Ɏg�p����萔�o�b�t�@�p�\���̂��擾
		/// </summary>
		/// <returns></returns>
		SLightingCB& GetLightingCB()
		{
			return m_sLightingCb;
		}
		/// <summary>
		/// �[�x�l�L�^�p�̃����_�����O�^�[�Q�b�g���擾
		/// </summary>
		/// <returns>�[�x�l�L�^�p�����_�����O�^�[�Q�b�g</returns>
		RenderTarget& GetDepthValue()
		{
			return m_depthForOutLineRenderTarget;
		}
		/// <summary>
		/// ���V����ݒ�B
		/// </summary>
		/// <param name="isFloating">���V���Ă��邩�B</param>
		void SetIsFloating(const bool isFloating)
		{
			if (isFloating) {
				m_sLightingCb.m_isFloating = 1;
			}
			else {
				m_sLightingCb.m_isFloating = 0;
			}
		}
		/// <summary>
		/// �r���[�J�����O�̂��߂̃r���[�v���W�F�N�V�����s����擾�B
		/// </summary>
		/// <returns>�r���[�J�����O�̂��߂̃r���[�v���W�F�N�V�����s��</returns>
		const Matrix& GetViewProjectionMatrixForViewCulling() const
		{
			return m_viewProjMatrixForViewCulling;
		}
		/// <summary>
		/// �􉽊w�f�[�^��o�^
		/// </summary>
		/// <param name="geomData">�􉽊w�f�[�^</param>
		void RegisterGeometryData(GeometryData* geomData)
		{
			m_sceneGeometryData.RegisterGeometryData(geomData);
		}
		/// <summary>
		/// �􉽊w�f�[�^�̓o�^�����B
		/// </summary>
		/// <param name="geomData"></param>
		void UnregisterGeometryData(GeometryData* geomData)
		{
			m_sceneGeometryData.UnregisterGeometryData(geomData);
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
		/// �V���h�E�}�b�v�����_�[��������
		/// </summary>
		void InitShadowMapRender();
		/// <summary>
		/// �V���h�E�}�b�v�Ƀ��f����`��
		/// </summary>
		/// <param name="rc">�����_�����O�R���e�L�X�g</param>
		void RenderToShadowMap(RenderContext& rc);
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
		/// <summary>
		/// �r���[�J�����O�p�̃r���[�v���W�F�N�V�����s����v�Z�B
		/// </summary>
		void CalcViewProjectionMatrixForViewCulling();
	private:
		std::vector<Model*>		m_forwardRenderModels;					// �t�H���[�h�����_�����O�̕`��p�X�ŕ`�悳��郂�f��
		std::vector<Model*>		m_frontCullingModels;					// �t�����g�J�����O���ꂽ���f��
		std::vector<Model*>     m_depthForOutLineModels;				// �[�x�l���L�^���邽�߂̃��f��
		std::vector<Model*>		m_shadowMapModels[NUM_SHADOW_MAP];
		RenderTarget			m_mainRenderTarget;						// ���C�������_�����O�^�[�Q�b�g
		RenderTarget			m_depthForOutLineRenderTarget;			// �֊s���p�̐[�x�l�����_�����O�^�[�Q�b�g
		Sprite					m_copyMainRtToFrameBufferSprite;		// ���C�������_�����O�^�[�Q�b�g�̓��e���t���[���o�b�t�@�ɃR�s�[���邽�߂̃X�v���C�g
		PostEffect				m_postEffect;							// �|�X�g�G�t�F�N�g
		SceneLight				m_sceneLight;
		ShadowMapRender			m_shadowMapRender;
		SLightingCB				m_sLightingCb;
		Matrix					m_viewProjMatrixForViewCulling;			// �r���[�J�����O�p�̃r���[�v���W�F�N�V�����s��B
		SceneGeometryData		m_sceneGeometryData;
	};

	extern RenderingEngine* g_renderingEngine;
}



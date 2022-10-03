#include "k2EngineLowPreCompile.h"
#include "ShadowMapRender.h"

namespace nsK2EngineLow
{
	void ShadowMapRender::Init()
	{
		// �����_�����O�^�[�Q�b�g���N���A����J���[
		float clearColor[4] = { 1.0f,1.0f,1.0f,1.0f };
		// �J���[�o�b�t�@�̃t�H�[�}�b�g
		DXGI_FORMAT colorFormat = DXGI_FORMAT_R32_FLOAT;
		// �f�v�X�X�e���V���o�b�t�@�̃t�H�[�}�b�g
		DXGI_FORMAT depthFormat = DXGI_FORMAT_D32_FLOAT;
		
		// �߉e�p�̃V���h�E�}�b�v�p�����_�����O�^�[�Q�b�g���쐬�B
		m_shadowMaps[SHADOW_MAP_AREA_NEAR].Create(
			2048,
			2048,
			1,
			1,
			colorFormat,
			depthFormat,
			clearColor
		);
		// ���e�p�̃V���h�E�}�b�v�p�����_�����O�^�[�Q�b�g���쐬�B
		m_shadowMaps[SHADOW_MAP_AREA_MIDDLE].Create(
			1024,
			1024,
			1,
			1,
			colorFormat,
			depthFormat,
			clearColor
		);
		// ���e�p�̃V���h�E�}�b�v�p�����_�����O�^�[�Q�b�g���쐬�B
		m_shadowMaps[SHADOW_MAP_AREA_FAR].Create(
			512,
			512,
			1,
			1,
			colorFormat,
			depthFormat,
			clearColor
		);
		

		// ���C�g�܂ł̃x�N�g��
		Vector3 camPos = g_camera3D->GetPosition();
		Vector3 toLigDir = { 1, 2, 1 };
		toLigDir.Normalize();
		toLigDir *= 5000.0f;    // ���C�g�܂ł̋���
		camPos += toLigDir;

		// �J�����̈ʒu��ݒ�B����̓��C�g�̈ʒu
		m_lightCamera.SetPosition(camPos);

		// �J�����̒����_��ݒ�B���ꂪ���C�g���Ƃ炵�Ă���ꏊ
		m_lightCamera.SetTarget({ 0, 0, 0 });

		// �������ݒ�B����̓��C�g���^���������Ă���̂ŁAX��������ɂ��Ă���
		m_lightCamera.SetUp(1, 0, 0);

		m_lightCamera.SetUpdateProjMatrixFunc(Camera::enUpdateProjMatrixFunc_Ortho);
		m_lightCamera.SetWidth(5000.0f);
		m_lightCamera.SetHeight(5000.0f);
		m_lightCamera.SetNear(1.0f);
		m_lightCamera.SetFar(10000.0f);

		// ���C�g�r���[�v���W�F�N�V�����s����v�Z���Ă���
		m_lightCamera.Update();
	}

	void ShadowMapRender::Render(
		RenderContext& rc,
		int ligNo,
		Vector3& lightDirection
	)
	{
		Vector3 lightPos = g_camera3D->GetPosition();
		m_lightCamera.SetTarget(g_camera3D->GetPosition());
		// ���C�g�̍�����50m���ߑł��B
		float lightMaxHeight = 5000.0f;
		lightPos += (lightDirection) * (lightMaxHeight / lightDirection.y);
		m_lightCamera.SetPosition(lightPos);
		m_lightCamera.Update();

		if (lightDirection.LengthSq() < 0.001f){
			return;
		}

		// �V���h�E�}�b�v�ɕ`�悷�郂�f���̔z��
		std::vector<Model*> shadowMapModelArray[NUM_SHADOW_MAP];

		// �V���h�E�}�b�v�̐��̕��������[�v������
		for (int i = 0; i < NUM_SHADOW_MAP; i++) {
			// �����_�����O�^�[�Q�b�g�Ƃ��Ďg�p�\�ɂȂ�܂ő҂B
			rc.WaitUntilToPossibleSetRenderTarget(m_shadowMaps[i]);
			// �����_�����O�^�[�Q�b�g�ƃr���[�|�[�g��ݒ肷��B
			rc.SetRenderTargetAndViewport(m_shadowMaps[i]);
			// �����_�����O�^�[�Q�b�g���N���A����B
			rc.ClearRenderTargetView(m_shadowMaps[i]);

			// ���C�g�̍ő�̍������v�Z����B
			m_cascadeShadowMapMatrix.CalcLightViewProjectionCropMatrix(
				lightDirection,
				m_cascadeAreaRateArray,
				m_lightCamera
			);

			// ���f���̔z����擾�B
			shadowMapModelArray[i] = g_renderingEngine->GetShadowMapModel(i);

			for (auto& model : shadowMapModelArray[i])
			{
				//model->Draw(
				//	rc,
				//	g_camera3D->GetViewMatrix(),
				//	g_camera3D->GetProjectionMatrix()
				//);
				
				model->Draw(
					rc,
					g_matIdentity,
					m_cascadeShadowMapMatrix.GetLightViewProjectionCropMatrix(i)
				);
				
			}

			// �����_�����O�^�[�Q�b�g�ւ̕`��I����҂B
			rc.WaitUntilFinishDrawingToRenderTarget(m_shadowMaps[i]);
		}

	}
}

#pragma once
#include "MyRenderer.h"
#include "CascadeShadowMapMatrix.h"

namespace nsK2EngineLow
{
	class ShadowMapRender : public Noncopyable
	{
	public:

		void Init();
		void Render(
            RenderContext& rc,
            int ligNo,
            Vector3& lightDirection
        );
        const float& GetFarClip(int areaNo) const
        {
            return m_cascadeShadowMapMatrix.GetFarClip(areaNo);
        }
        /// <summary>
        /// �V���h�E�}�b�v���擾
        /// </summary>
        /// <param name="areaNo">�G���A�ԍ�</param>
        /// <returns>�w�肳�ꂽ�G���A�̃V���h�E�}�b�v</returns>
        Texture& GetShadowMap(int areaNo)
        {
            return m_shadowMaps[areaNo].GetRenderTargetTexture();
        }
        /// <summary>
        /// ���C�g�r���[�v���W�F�N�V�����s����擾
        /// </summary>
        /// <returns></returns>
        const Matrix& GetLVPMatrix(int areaNo) const
        {
            return m_cascadeShadowMapMatrix.GetLightViewProjectionCropMatrix(areaNo);
        }
        /// <summary>
        /// �J�X�P�[�h�V���h�E�̃G���A����ݒ�B
        /// </summary>
        /// <remark>
        /// �Q�[���J�����̋ߕ��ʂ��牓���ʂ܂ł̃G���A�́A��%�͈̔͂��ߋ����p�̍��𑜓x�̃V���h�E�}�b�v�ɁA
        /// ��%�𒆋����p�̃V���h�E�}�b�v�ɁA��%���������p�̃V���h�E�}�b�v�ɕ`�悷�邩���w�肵�����́B
        /// </remark>
        /// <param name="nearArea">�߉e�G���A���B</param>
        /// <param name="middleArea">���e�G���A���B</param>
        /// <param name="farArea">���e�G���A���B</param>
        void SetCascadeNearAreaRates(float nearArea, float middleArea, float farArea)
        {
            // �������G���A�͈̔͂��ߋ����G���A�͈̔͂�菬�����Ȃ��Ă����
            // �v�Z��s�s�����N����̂ŁA�␳�B
            middleArea = max(nearArea + 0.01f, middleArea);
            // ���������G���A�͈̔͂��������G���A�͈̔͂�菬�����Ȃ��Ă����
            // �v�Z��s�s�����N����̂ŁA�␳�B
            farArea = max(middleArea + 0.01f, farArea);
            m_cascadeAreaRateArray[SHADOW_MAP_AREA_NEAR] = nearArea;
            m_cascadeAreaRateArray[SHADOW_MAP_AREA_MIDDLE] = middleArea;
            m_cascadeAreaRateArray[SHADOW_MAP_AREA_FAR] = farArea;
        }
	private:
		RenderTarget			m_shadowMaps[NUM_SHADOW_MAP];									// �V���h�E�}�b�v
		CascadeShadowMapMatrix  m_cascadeShadowMapMatrix;										// �J�X�P�[�h�V���h�E�̍s��
		float					m_cascadeAreaRateArray[NUM_SHADOW_MAP] = { 0.1f, 0.4f, 1.0f };	// �J�X�P�[�h�V���h�E�ɂ���ĕ������ꂽ�G���A��
        Camera                  m_lightCamera;

	};

}


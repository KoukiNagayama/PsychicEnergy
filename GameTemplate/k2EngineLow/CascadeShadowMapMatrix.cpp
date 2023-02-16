#include "k2EngineLowPreCompile.h"
#include "CascadeShadowMapMatrix.h"

namespace nsK2EngineLow {
    void CascadeShadowMapMatrix::CalcLightViewProjectionCropMatrix(
        Vector3 lightDirection,
        float cascadeAreaRateTbl[NUM_SHADOW_MAP],
        Camera& lightCamera
    )
    {   
        const auto& lvpMatrix = lightCamera.GetViewProjectionMatrix();
        float maxFar = g_camera3D->GetFar() * cascadeAreaRateTbl[NUM_SHADOW_MAP -1];
        float cascadeAreaTbl[NUM_SHADOW_MAP] = {
            maxFar * cascadeAreaRateTbl[SHADOW_MAP_AREA_NEAR],      // �߉e���f���ő�[�x�l
            maxFar * cascadeAreaRateTbl[SHADOW_MAP_AREA_MIDDLE],    // ���e���f���ő�[�x�l
            maxFar * cascadeAreaRateTbl[SHADOW_MAP_AREA_FAR]        // ���e���f���ő�[�x�l
        };
        

        // �J�����̑O�����A�E�����A����������߂�
        const auto& cameraForward = g_camera3D->GetForward();
        const auto& cameraRight = g_camera3D->GetRight();
        Vector3 cameraUp;
        cameraUp.Cross(cameraForward, cameraRight);


        // �G���A�̍ŏ��[�x�l
        float nearDepth = g_camera3D->GetNear();

        for (int areaNo = 0; areaNo < NUM_SHADOW_MAP; areaNo++)
        {
            // �G���A�����鎋�����8���_�����߂�
            // �G���A�̋ߕ��ʂ̒��S����̏�ʁA���ʂ܂ł̋��������߂�

            // �G���A�̋ߕ��ʂ�Y�l
            float nearY = tanf(g_camera3D->GetViewAngle() * 0.5f) * nearDepth;

            // �G���A�̋ߕ��ʂ�X�l
            float nearX = nearY * g_camera3D->GetAspect();

            // �G���A�̉����ʂ�Y�l
            float farY = tanf(g_camera3D->GetViewAngle() * 0.5f) * cascadeAreaTbl[areaNo];

            // �G���A�̉����ʂ�X�l
            float farX = farY * g_camera3D->GetAspect();

            Vector3 nearPos = g_camera3D->GetPosition() + cameraForward * nearDepth;

            Vector3 farPos = g_camera3D->GetPosition() + cameraForward * cascadeAreaTbl[areaNo];

            // 8���_
            Vector3 vertex[8];

            vertex[0] += nearPos + cameraUp * nearY + cameraRight * nearX;

            vertex[1] += nearPos + cameraUp * nearY + cameraRight * -nearX;

            vertex[2] += nearPos + cameraUp * -nearY + cameraRight * nearX;

            vertex[3] += nearPos + cameraUp * -nearY + cameraRight * -nearX;

            vertex[4] += farPos + cameraUp * farY + cameraRight * farX;

            vertex[5] += farPos + cameraUp * farY + cameraRight * -farX;

            vertex[6] += farPos + cameraUp * -farY + cameraRight * farX;

            vertex[7] += farPos + cameraUp * -farY + cameraRight * -farX;

            // 8���_��ϊ����čő�l�A�ŏ��l�����߂�
            Vector3 vMax, vMin;
            vMax = { -FLT_MAX, -FLT_MAX, -FLT_MAX };
            vMin = { FLT_MAX, FLT_MAX, FLT_MAX };
            for (auto& v : vertex)
            {
                lvpMatrix.Apply(v);
                vMax.Max(v);
                vMin.Min(v);
            }
            // �N���b�v�s������߂�
            float xScale = 2.0f / (vMax.x - vMin.x);
            float yScale = 2.0f / (vMax.y - vMin.y);
            float xOffset = (vMax.x + vMin.x) * -0.5f * xScale;
            float yOffset = (vMax.y + vMin.y) * -0.5f * yScale;
            
            // �N���b�v�s��
            Matrix clopMatrix;
            clopMatrix.m[0][0] = xScale;
            clopMatrix.m[1][1] = yScale;
            clopMatrix.m[3][0] = xOffset;
            clopMatrix.m[3][1] = yOffset;

            // ���C�g�r���[�v���W�F�N�V�����s��ɃN���b�v�s�����Z����
            m_lvpcMatrix[areaNo] = lvpMatrix * clopMatrix;

            m_far[areaNo] = cascadeAreaTbl[areaNo];

            // ���̃G���A�̋ߕ��ʂ܂ł̋�����������
            nearDepth = cascadeAreaTbl[areaNo];
        }
    }
}
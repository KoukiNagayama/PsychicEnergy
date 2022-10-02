#include "k2EngineLowPreCompile.h"
#include "CascadeShadowMapMatrix.h"

namespace nsK2EngineLow {
    void CascadeShadowMapMatrix::CalcLightViewProjectionCropMatrix(
        Vector3 lightDirection,
        float cascadeAreaRateTbl[NUM_SHADOW_MAP],
        Camera& lightCamera
    )
    {
        float cascadeAreaTbl[NUM_SHADOW_MAP] = {
            500,
            2000,
            g_camera3D->GetFar(),
        };
        const auto& lvpMatrix = lightCamera.GetViewProjectionMatrix();

        // step-6 カメラの前方向、右方向、上方向を求める
        const auto& cameraForward = g_camera3D->GetForward();
        const auto& cameraRight = g_camera3D->GetRight();

        Vector3 cameraUp;
        cameraUp.Cross(cameraForward, cameraRight);
        // nearDepthはエリアの最小深度値を表す
        // 一番近いエリアの最小深度値はカメラのニアクリップ
        float nearDepth = g_camera3D->GetNear();
        for (int areaNo = 0; areaNo < NUM_SHADOW_MAP; areaNo++)
        {
            // step-7 エリアを内包する視錐台の8頂点を求める
            // エリアの近平面の中心からの上面、下面までの距離を求める
            float nearY = tanf(g_camera3D->GetViewAngle() * 0.5f) * nearDepth;

            float nearX = nearY * g_camera3D->GetAspect();

            float farY = tanf(g_camera3D->GetViewAngle() * 0.5f) * cascadeAreaTbl[areaNo];

            float farX = farY * g_camera3D->GetAspect();

            Vector3 nearPos = g_camera3D->GetPosition() + cameraForward * nearDepth;

            Vector3 farPos = g_camera3D->GetPosition() + cameraForward * cascadeAreaTbl[areaNo];

            // 8頂点を求める
            Vector3 vertex[8];

            vertex[0] += nearPos + cameraUp * nearY + cameraRight * nearX;

            vertex[1] += nearPos + cameraUp * nearY + cameraRight * -nearX;

            vertex[2] += nearPos + cameraUp * -nearY + cameraRight * nearX;

            vertex[3] += nearPos + cameraUp * -nearY + cameraRight * -nearX;

            vertex[4] += farPos + cameraUp * farY + cameraRight * farX;

            vertex[5] += farPos + cameraUp * farY + cameraRight * -farX;

            vertex[6] += farPos + cameraUp * -farY + cameraRight * farX;

            vertex[7] += farPos + cameraUp * -farY + cameraRight * -farX;

            // step-8 8頂点を変換して最大値、最小値を求める
            Vector3 vMax, vMin;
            vMax = { -FLT_MAX, -FLT_MAX, -FLT_MAX };
            vMin = { FLT_MAX, FLT_MAX, FLT_MAX };
            for (auto& v : vertex)
            {
                lvpMatrix.Apply(v);
                vMax.Max(v);
                vMin.Min(v);
            }
            // step-9 クロップ行列を求める
            float xScale = 2.0f / (vMax.x - vMin.x);
            float yScale = 2.0f / (vMin.y - vMin.y);
            float xOffset = (vMax.x + vMin.x) * -0.5f * xScale;
            float yOffset = (vMax.y + vMin.y) * -0.5f * yScale;
            Matrix clopMatrix;
            clopMatrix.m[0][0] = xScale;
            clopMatrix.m[1][1] = yScale;
            clopMatrix.m[3][0] = xOffset;
            clopMatrix.m[3][1] = yOffset;

            // step-10 ライトビュープロジェクション行列にクロップ行列を乗算する
            m_lvpcMatrix[areaNo] = lvpMatrix * clopMatrix;

            // 次のエリアの近平面までの距離を代入する
            nearDepth = cascadeAreaTbl[areaNo];
        }
    }
}
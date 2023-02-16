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
            maxFar * cascadeAreaRateTbl[SHADOW_MAP_AREA_NEAR],      // 近影を映す最大深度値
            maxFar * cascadeAreaRateTbl[SHADOW_MAP_AREA_MIDDLE],    // 中影を映す最大深度値
            maxFar * cascadeAreaRateTbl[SHADOW_MAP_AREA_FAR]        // 遠影を映す最大深度値
        };
        

        // カメラの前方向、右方向、上方向を求める
        const auto& cameraForward = g_camera3D->GetForward();
        const auto& cameraRight = g_camera3D->GetRight();
        Vector3 cameraUp;
        cameraUp.Cross(cameraForward, cameraRight);


        // エリアの最小深度値
        float nearDepth = g_camera3D->GetNear();

        for (int areaNo = 0; areaNo < NUM_SHADOW_MAP; areaNo++)
        {
            // エリアを内包する視錐台の8頂点を求める
            // エリアの近平面の中心からの上面、下面までの距離を求める

            // エリアの近平面のY値
            float nearY = tanf(g_camera3D->GetViewAngle() * 0.5f) * nearDepth;

            // エリアの近平面のX値
            float nearX = nearY * g_camera3D->GetAspect();

            // エリアの遠平面のY値
            float farY = tanf(g_camera3D->GetViewAngle() * 0.5f) * cascadeAreaTbl[areaNo];

            // エリアの遠平面のX値
            float farX = farY * g_camera3D->GetAspect();

            Vector3 nearPos = g_camera3D->GetPosition() + cameraForward * nearDepth;

            Vector3 farPos = g_camera3D->GetPosition() + cameraForward * cascadeAreaTbl[areaNo];

            // 8頂点
            Vector3 vertex[8];

            vertex[0] += nearPos + cameraUp * nearY + cameraRight * nearX;

            vertex[1] += nearPos + cameraUp * nearY + cameraRight * -nearX;

            vertex[2] += nearPos + cameraUp * -nearY + cameraRight * nearX;

            vertex[3] += nearPos + cameraUp * -nearY + cameraRight * -nearX;

            vertex[4] += farPos + cameraUp * farY + cameraRight * farX;

            vertex[5] += farPos + cameraUp * farY + cameraRight * -farX;

            vertex[6] += farPos + cameraUp * -farY + cameraRight * farX;

            vertex[7] += farPos + cameraUp * -farY + cameraRight * -farX;

            // 8頂点を変換して最大値、最小値を求める
            Vector3 vMax, vMin;
            vMax = { -FLT_MAX, -FLT_MAX, -FLT_MAX };
            vMin = { FLT_MAX, FLT_MAX, FLT_MAX };
            for (auto& v : vertex)
            {
                lvpMatrix.Apply(v);
                vMax.Max(v);
                vMin.Min(v);
            }
            // クロップ行列を求める
            float xScale = 2.0f / (vMax.x - vMin.x);
            float yScale = 2.0f / (vMax.y - vMin.y);
            float xOffset = (vMax.x + vMin.x) * -0.5f * xScale;
            float yOffset = (vMax.y + vMin.y) * -0.5f * yScale;
            
            // クロップ行列
            Matrix clopMatrix;
            clopMatrix.m[0][0] = xScale;
            clopMatrix.m[1][1] = yScale;
            clopMatrix.m[3][0] = xOffset;
            clopMatrix.m[3][1] = yOffset;

            // ライトビュープロジェクション行列にクロップ行列を乗算する
            m_lvpcMatrix[areaNo] = lvpMatrix * clopMatrix;

            m_far[areaNo] = cascadeAreaTbl[areaNo];

            // 次のエリアの近平面までの距離を代入する
            nearDepth = cascadeAreaTbl[areaNo];
        }
    }
}
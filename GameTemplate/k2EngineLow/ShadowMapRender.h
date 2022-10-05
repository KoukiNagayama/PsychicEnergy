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
        /// シャドウマップを取得
        /// </summary>
        /// <param name="areaNo">エリア番号</param>
        /// <returns>指定されたエリアのシャドウマップ</returns>
        Texture& GetShadowMap(int areaNo)
        {
            return m_shadowMaps[areaNo].GetRenderTargetTexture();
        }
        /// <summary>
        /// ライトビュープロジェクション行列を取得
        /// </summary>
        /// <returns></returns>
        const Matrix& GetLVPMatrix(int areaNo) const
        {
            return m_cascadeShadowMapMatrix.GetLightViewProjectionCropMatrix(areaNo);
        }
        /// <summary>
        /// カスケードシャドウのエリア率を設定。
        /// </summary>
        /// <remark>
        /// ゲームカメラの近平面から遠平面までのエリアの、何%の範囲を近距離用の高解像度のシャドウマップに、
        /// 何%を中距離用のシャドウマップに、何%を遠距離用のシャドウマップに描画するかを指定したもの。
        /// </remark>
        /// <param name="nearArea">近影エリア率。</param>
        /// <param name="middleArea">中影エリア率。</param>
        /// <param name="farArea">遠影エリア率。</param>
        void SetCascadeNearAreaRates(float nearArea, float middleArea, float farArea)
        {
            // 中距離エリアの範囲が近距離エリアの範囲より小さくなっていると
            // 計算上不都合が起きるので、補正。
            middleArea = max(nearArea + 0.01f, middleArea);
            // 遠い距離エリアの範囲が中距離エリアの範囲より小さくなっていると
            // 計算上不都合が起きるので、補正。
            farArea = max(middleArea + 0.01f, farArea);
            m_cascadeAreaRateArray[SHADOW_MAP_AREA_NEAR] = nearArea;
            m_cascadeAreaRateArray[SHADOW_MAP_AREA_MIDDLE] = middleArea;
            m_cascadeAreaRateArray[SHADOW_MAP_AREA_FAR] = farArea;
        }
	private:
		RenderTarget			m_shadowMaps[NUM_SHADOW_MAP];									// シャドウマップ
		CascadeShadowMapMatrix  m_cascadeShadowMapMatrix;										// カスケードシャドウの行列
		float					m_cascadeAreaRateArray[NUM_SHADOW_MAP] = { 0.1f, 0.4f, 1.0f };	// カスケードシャドウによって分割されたエリア率
        Camera                  m_lightCamera;

	};

}


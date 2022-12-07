#pragma once
#include "GeometryData.h"

namespace nsK2EngineLow
{
	/// <summary>
	/// シーンのジオメトリ情報
	/// </summary>
	class SceneGeometryData : public Noncopyable
	{
	public:
        /// <summary>
        /// ジオメトリ情報を登録
        /// </summary>
        /// <param name="geomData">幾何学データ</param>
        void RegisterGeometryData(GeometryData* geomData)
        {
            m_geometryDatas.push_back(geomData);
        }
        /// <summary>
        /// ジオメトリ情報の登録解除。
        /// </summary>
        /// <param name="geomData"></param>
        void UnregisterGeometryData(GeometryData* geomData)
        {
            auto it = std::find(
                m_geometryDatas.begin(),
                m_geometryDatas.end(),
                geomData
            );
            if (it != m_geometryDatas.end()) {
                m_geometryDatas.erase(it);
            }
        }
        /// <summary>
        /// 幾何学情報を更新。
        /// </summary>
        void Update();
        /// <summary>
        /// ビューフラスタムに含まれているシャドウキャスターの最大座標を取得
        /// </summary>
        /// <returns>ビューフラスタムに含まれているシャドウキャスターの最大座標</returns>
        const Vector3& GetShadowCasterMaxPositionInViewFrustum() const
        {
            return m_shadowCasterMaxPositionInViewFrustum;
        }
        /// <summary>
        /// ビューフラスタムに含まれているシャドウキャスターの最小座標
        /// </summary>
        /// <returns>ビューフラスタムに含まれているシャドウキャスターの最小座標</returns>
        const Vector3& GetShadowCasterMinPositionInViewFrustum() const
        {
            return m_shadowCasterMinPositionInViewFrustum;
        }
        /// <summary>
        /// シャドウキャスターに関するジオメトリ情報を構築できたか
        /// </summary>
        /// <returns></returns>
        bool IsBuildshadowCasterGeometryData() const
        {
            return m_isBuildshadowCasterGeometryData;
        }
    private:
        Vector3 m_shadowCasterMaxPositionInViewFrustum;     // ビューフラスタムに含まれているシャドウキャスターの最大座標。
        Vector3 m_shadowCasterMinPositionInViewFrustum;     // ビューフラスタムに含まれているシャドウキャスターの最小座標。
        bool m_isBuildshadowCasterGeometryData = false;     // シャドウキャスターに関するジオメトリ情報を構築できた？
        std::list< GeometryData* > m_geometryDatas;         // シーンを構成するジオメトリ情報のリスト。
    };
}



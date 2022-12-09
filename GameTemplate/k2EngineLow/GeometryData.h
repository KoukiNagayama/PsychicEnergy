#pragma once
namespace nsK2EngineLow
{
	class ModelRender;

	/// <summary>
	/// ジオメトリ情報
	/// </summary>
	class GeometryData {
	public:
		/// <summary>
		/// ジオメトリ情報の初期化
		/// </summary>
		/// <param name="modelRender">モデルレンダー</param>
		void Init(ModelRender* modelRender);
		/// <summary>
		/// ビューフラスタムに含まれている？
		/// </summary>
		/// <returns></returns>
		bool IsInViewFrustum() const
		{
			return m_isInViewFrustum;
		}
		/// <summary>
		/// ワールド座標系でのAABBの最大座標を取得。
		/// </summary>
		/// <returns></returns>
		const Vector3& GetAabbMaxPositionInWorld() const
		{
			return m_aabbMax;
		}
		/// <summary>
		/// ワールド座標系でのAABBの最小座標を取得。
		/// </summary>
		/// <returns></returns>
		const Vector3& GetAabbMinPositionInWorld() const
		{
			return m_aabbMin;
		}
		/// <summary>
		/// シャドウキャスターのジオメトリ情報は？
		/// </summary>
		/// <returns></returns>
		bool IsShadowCaster() const;
		/// <summary>
		/// ジオメトリ情報を更新
		/// </summary>
		void Update();
	private:
		bool			m_isInViewFrustum = false;		// ビューフラスタムに含まれているか。
		AABB			m_aabb;							// AABB
		Vector3			m_aabbWorldVertexPositions[8];	// AABBのワールド空間での8頂点。
		Vector3			m_aabbMax;						// ワールド空間でのAABBの最大座標。
		Vector3			m_aabbMin;						// ワールド空間でのAABBの最小座標。
		ModelRender*	m_modelRender = nullptr;		// このジオメトリ情報と関連づいているモデルレンダラー。
	};
}


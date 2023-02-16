#include "k2EngineLowPreCompile.h"
#include "GeometryData.h"

namespace nsK2EngineLow
{
	void GeometryData::Update()
	{
		Matrix worldMatrix = m_modelRender->GetWorldMatrix();
		m_aabb.CalcVertexPositions(
			m_aabbWorldVertexPositions,
			worldMatrix
		);

		// ビュープロジェクション空間での最大値と最小値
		Vector4 vMax = { -FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX };
		Vector4 vMin = { FLT_MAX,  FLT_MAX,  FLT_MAX, FLT_MAX };

		// ビューカリングのためのビュープロジェクション行列を取得。
		const auto& viewProjMatrix = g_renderingEngine->GetViewProjectionMatrixForViewCulling();

		// AABB空間での最大値と最小値を設定。
		m_aabbMax = { -FLT_MIN, -FLT_MIN, -FLT_MIN };
		m_aabbMin = { FLT_MAX,  FLT_MAX,  FLT_MAX };

		// ビューフラスタムの中に映っているか
		m_isInViewFrustum = false;

		// AABBの頂点を計算する。
		for (Vector4 v : m_aabbWorldVertexPositions) {

			m_aabbMax.Max({ v.x, v.y, v.z });
			m_aabbMin.Min({ v.x, v.y, v.z });

			// AABBの頂点のワールド座標をビュープロジェクション座標に変換する。
			viewProjMatrix.Apply(v);
			
			// 頂点を正規化する
			v.x /= fabsf(v.w);
			v.y /= fabsf(v.w);
			v.z /= fabsf(v.w);

			vMax.Max(v);
			vMin.Min(v);
		}

		if (vMax.x > -1.0f
			&& vMax.y > -1.0f
			&& vMax.z > 0.0f
			&& vMin.x < 1.0f
			&& vMin.y < 1.0f
			&& vMin.z < 1.0f
			) {
			//画面に映る。
			m_isInViewFrustum = true;
		}
	}
	bool GeometryData::IsShadowCaster() const
	{
		return m_modelRender->IsShadowCaster();
	}
	void GeometryData::Init(ModelRender* modelRender)
	{
		m_modelRender = modelRender;
		// tkmファイルからモデルの最小座標と最大座標を調べる。
		Vector3 vMax = { -FLT_MAX, -FLT_MAX, -FLT_MAX };
		Vector3 vMin = { FLT_MAX,  FLT_MAX,  FLT_MAX };

		const auto& tkmFile = modelRender->GetModel().GetTkmFile();
		tkmFile.QueryMeshParts([&](const TkmFile::SMesh& mesh) {
			for (const auto& vertex : mesh.vertexBuffer) {
				vMax.Max(vertex.pos);
				vMin.Min(vertex.pos);
			}
			});

		// 最大座標と最小座標からAABBを初期化する。
		m_aabb.Init(vMax, vMin);
	}
}

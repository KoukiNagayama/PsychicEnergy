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
			Light m_light;						// ライト
			Matrix m_lvp[NUM_SHADOW_MAP];		// ライトビュープロジェクション行列
			int m_isFloating = 0;
			float m_farList[NUM_SHADOW_MAP];	
			
		};
	public:
		RenderingEngine() {};
		~RenderingEngine() {};

		/// <summary>
		/// レンダリングパイプラインを初期化。
		/// </summary>
		void Init();
		/// <summary>
		/// レンダリングパイプラインを実行。
		/// </summary>
		/// <param name="rc"></param>
		void Execute(RenderContext& rc);
		/// <summary>
		/// フォワードレンダリングの描画パスにモデルを追加。
		/// </summary>
		/// <param name="model"></param>
		void Add3DModelToForwardRenderPass(Model& model)
		{
			m_forwardRenderModels.push_back(&model);
		}
		/// <summary>
		/// 背面押し出しでの輪郭線描画用背面モデルの描画パスにモデルを追加。
		/// </summary>
		/// <param name="model"></param>
		void Add3DModelToRenderingModelsForOutLine(Model& model)
		{
			m_frontCullingModels.push_back(&model);
		}
		/// <summary>
		/// ポストエフェクト的な輪郭線描画に使用する深度を抽出するため描画パスにモデルを追加。
		/// </summary>
		/// <param name="model"></param>
		void Add3DModelToDepthForOutLinePass(Model& model)
		{
			m_depthForOutLineModels.push_back(&model);
		}
		/// <summary>
		/// シャドウマップ描画のパスにモデルを追加
		/// </summary>
		/// <param name="model">モデル</param>
		/// <param name="areaNo">カスケードシャドウで分割されたエリア</param>
		void Add3DModelToRenderToShadowMapPass(Model& model, int areaNo)
		{
			m_shadowMapModels[areaNo].push_back(&model);
		}
		/// <summary>
		/// シャドウマップに描画するモデルを取得
		/// </summary>
		/// <param name="areaNo">カスケードシャドウで分割されたエリア</param>
		/// <returns>シャドウマップに描画するモデルの配列</returns>
		const std::vector<Model*>& GetShadowMapModel(int areaNo) const 
		{
			return m_shadowMapModels[areaNo];
		}
		Texture& GetShadowMap(int areaNo)
		{
			return m_shadowMapRender.GetShadowMap(areaNo);
		}
		/// <summary>
		/// シーンライトを取得。
		/// </summary>
		/// <returns></returns>
		SceneLight& GetSceneLight()
		{
			return m_sceneLight;
		}
		/// <summary>
		/// モデル描画時に使用する定数バッファ用構造体を取得
		/// </summary>
		/// <returns></returns>
		SLightingCB& GetLightingCB()
		{
			return m_sLightingCb;
		}
		/// <summary>
		/// 深度値記録用のレンダリングターゲットを取得
		/// </summary>
		/// <returns>深度値記録用レンダリングターゲット</returns>
		RenderTarget& GetDepthValue()
		{
			return m_depthForOutLineRenderTarget;
		}
		/// <summary>
		/// 浮遊情報を設定。
		/// </summary>
		/// <param name="isFloating">浮遊しているか。</param>
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
		/// ビューカリングのためのビュープロジェクション行列を取得。
		/// </summary>
		/// <returns>ビューカリングのためのビュープロジェクション行列</returns>
		const Matrix& GetViewProjectionMatrixForViewCulling() const
		{
			return m_viewProjMatrixForViewCulling;
		}
		/// <summary>
		/// 幾何学データを登録
		/// </summary>
		/// <param name="geomData">幾何学データ</param>
		void RegisterGeometryData(GeometryData* geomData)
		{
			m_sceneGeometryData.RegisterGeometryData(geomData);
		}
		/// <summary>
		/// 幾何学データの登録解除。
		/// </summary>
		/// <param name="geomData"></param>
		void UnregisterGeometryData(GeometryData* geomData)
		{
			m_sceneGeometryData.UnregisterGeometryData(geomData);
		}

	private:
		/// <summary>
		/// メインレンダリングターゲットを初期化
		/// </summary>
		void InitMainRenderTarget();
		/// <summary>
		/// 輪郭線描画に使用する深度値記録用のレンダリングターゲットを初期化
		/// </summary>
		void InitDepthForOutLineRenderTarget();
		/// <summary>
		/// メインレンダリングターゲットのカラーバッファの内容をフレームバッファにコピーするためのスプライトを初期化
		/// </summary>
		void InitCopyMainRenderTargetToFrameBufferSprite();
		/// <summary>
		/// シャドウマップレンダーを初期化
		/// </summary>
		void InitShadowMapRender();
		/// <summary>
		/// シャドウマップにモデルを描画
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト</param>
		void RenderToShadowMap(RenderContext& rc);
		/// <summary>
		/// 輪郭線用モデルを描画
		/// </summary>
		void RenderingModelsForOutLine(RenderContext& rc);
		/// <summary>
		/// フォワードレンダリング
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト</param>
		void ForwardRendering(RenderContext& rc);
		/// <summary>
		/// 深度値記録パス
		/// </summary>
		/// <param name="rc"></param>
		void DepthForOutLine(RenderContext& rc);
		/// <summary>
		/// メインレンダリングターゲットの内容をフレームバッファにコピー
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト</param>
		void CopyMainRenderTargetToFrameBuffer(RenderContext& rc);
		/// <summary>
		/// ビューカリング用のビュープロジェクション行列を計算。
		/// </summary>
		void CalcViewProjectionMatrixForViewCulling();
	private:
		std::vector<Model*>		m_forwardRenderModels;					// フォワードレンダリングの描画パスで描画されるモデル
		std::vector<Model*>		m_frontCullingModels;					// フロントカリングされたモデル
		std::vector<Model*>     m_depthForOutLineModels;				// 深度値を記録するためのモデル
		std::vector<Model*>		m_shadowMapModels[NUM_SHADOW_MAP];
		RenderTarget			m_mainRenderTarget;						// メインレンダリングターゲット
		RenderTarget			m_depthForOutLineRenderTarget;			// 輪郭線用の深度値レンダリングターゲット
		Sprite					m_copyMainRtToFrameBufferSprite;		// メインレンダリングターゲットの内容をフレームバッファにコピーするためのスプライト
		PostEffect				m_postEffect;							// ポストエフェクト
		SceneLight				m_sceneLight;
		ShadowMapRender			m_shadowMapRender;
		SLightingCB				m_sLightingCb;
		Matrix					m_viewProjMatrixForViewCulling;			// ビューカリング用のビュープロジェクション行列。
		SceneGeometryData		m_sceneGeometryData;
	};

	extern RenderingEngine* g_renderingEngine;
}



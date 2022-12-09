#pragma once
#include "Fxaa.h"
#include "PostEffect.h"
#include "SceneLight.h"
#include "ShadowMapRender.h"
#include "SceneGeometryData.h"

namespace nsK2EngineLow
{
	/// <summary>
	/// レンダリングエンジン
	/// </summary>
	class RenderingEngine : public Noncopyable
	{
		/// <summary>
		/// 定数バッファ用ライティング構造体
		/// </summary>
		struct SLightingCB
		{
			Light m_light;						// ライト
			Matrix m_lvp[NUM_SHADOW_MAP];		// ライトビュープロジェクション行列
			int m_isFloating = 0;				// 浮遊しているかどうか。
			float m_farList[NUM_SHADOW_MAP];	// シャドウマップに使用しているファークリップのリスト
			
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
		/// スプライトの描画パスで描画するスプライトを追加
		/// </summary>
		/// <param name="sprite"></param>
		void AddSpriteToRenderSpritePass(Sprite& sprite)
		{
			m_sprites.push_back(&sprite);
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
		/// ジオメトリデータを登録
		/// </summary>
		/// <param name="geomData">幾何学データ</param>
		void RegisterGeometryData(GeometryData* geomData)
		{
			m_sceneGeometryData.RegisterGeometryData(geomData);
		}
		/// <summary>
		/// ジオメトリデータの登録解除。
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
		void Init2DRenderTarget();
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
		/// スプライト描画
		/// </summary>
		/// <param name="rc"></param>
		void RenderSprite(RenderContext& rc);
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
		std::vector<Model*>     m_depthForOutLineModels;				// 輪郭線描画用に深度値を記録するためのモデル
		std::vector<Model*>		m_shadowMapModels[NUM_SHADOW_MAP];		// シャドウマップに描画するモデル
		std::vector<Sprite*>	m_sprites;								// 描画するスプライト
		RenderTarget			m_mainRenderTarget;						// メインレンダリングターゲット
		RenderTarget			m_depthForOutLineRenderTarget;			// 輪郭線用の深度値レンダリングターゲット
		RenderTarget			m_2DRenderTarget;
		Sprite					m_2DSprite;
		Sprite					m_mainSprite;
		Sprite					m_copyMainRtToFrameBufferSprite;		// メインレンダリングターゲットの内容をフレームバッファにコピーするためのスプライト
		PostEffect				m_postEffect;							// ポストエフェクト
		SceneLight				m_sceneLight;							// シーンライト
		ShadowMapRender			m_shadowMapRender;						// シャドウマップレンダー
		SLightingCB				m_sLightingCb;							// 定数バッファ用ライティング構造体
		Matrix					m_viewProjMatrixForViewCulling;			// ビューカリング用のビュープロジェクション行列。
		SceneGeometryData		m_sceneGeometryData;					// シーンのジオメトリ情報
	};

	extern RenderingEngine* g_renderingEngine;
}



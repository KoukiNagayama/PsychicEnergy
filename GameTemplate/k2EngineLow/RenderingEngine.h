#pragma once
#include "Fxaa.h"
#include "PostEffect.h"
#include "SceneLight.h"

namespace nsK2EngineLow
{
	class RenderingEngine : public Noncopyable
	{
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
		/// シーンライトを取得。
		/// </summary>
		/// <returns></returns>
		SceneLight& GetSceneLight()
		{
			return m_sceneLight;
		}
		/// <summary>
/// 深度値記録用のレンダリングターゲットを取得
/// </summary>
/// <returns>深度値記録用レンダリングターゲット</returns>
		RenderTarget& GetDepthValue()
		{
			return m_depthForOutLineRenderTarget;
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
	private:
		std::vector<Model*>		m_forwardRenderModels;					// フォワードレンダリングの描画パスで描画されるモデル
		std::vector<Model*>		m_frontCullingModels;					// フロントカリングされたモデル
		std::vector<Model*>     m_depthForOutLineModels;				// 深度値を記録するためのモデル
		RenderTarget			m_mainRenderTarget;						// メインレンダリングターゲット
		RenderTarget			m_depthForOutLineRenderTarget;			// 輪郭線用の深度値レンダリングターゲット
		Sprite					m_copyMainRtToFrameBufferSprite;		// メインレンダリングターゲットの内容をフレームバッファにコピーするためのスプライト
		PostEffect				m_postEffect;							// ポストエフェクト
		SceneLight				m_sceneLight;
	};

	extern RenderingEngine* g_renderingEngine;
}



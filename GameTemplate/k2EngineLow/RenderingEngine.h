#pragma once
#include "Fxaa.h"
#include "PostEffect.h"
#include "SceneLight.h"

namespace nsK2EngineLow
{
	class RenderingEngine
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
		/// 輪郭線用モデルの描画パスにモデルを追加。
		/// </summary>
		/// <param name="model"></param>
		void Add3DModelToRenderingModelsForOutLine(Model& model)
		{
			m_frontCullingModels.push_back(&model);
		}
		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		SceneLight& GetSceneLight()
		{
			return m_sceneLight;
		}
	private:
		/// <summary>
		/// メインレンダリングターゲットを初期化
		/// </summary>
		void InitMainRenderTarget();
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
		/// メインレンダリングターゲットの内容をフレームバッファにコピー
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト</param>
		void CopyMainRenderTargetToFrameBuffer(RenderContext& rc);
	private:
		std::vector<Model*>		m_forwardRenderModels;					// フォワードレンダリングの描画パスで描画されるモデル
		std::vector<Model*>		m_frontCullingModels;
		RenderTarget			m_mainRenderTarget;						// メインレンダリングターゲット
		Sprite					m_copyMainRtToFrameBufferSprite;		// メインレンダリングターゲットの内容をフレームバッファにコピーするためのスプライト
		PostEffect				m_postEffect;							// ポストエフェクト
		SceneLight				m_sceneLight;
	};

	extern RenderingEngine* g_renderingEngine;
}



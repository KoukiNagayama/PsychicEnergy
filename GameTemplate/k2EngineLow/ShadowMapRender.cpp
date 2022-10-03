#include "k2EngineLowPreCompile.h"
#include "ShadowMapRender.h"

namespace nsK2EngineLow
{
	void ShadowMapRender::Init()
	{
		// レンダリングターゲットをクリアするカラー
		float clearColor[4] = { 1.0f,1.0f,1.0f,1.0f };
		// カラーバッファのフォーマット
		DXGI_FORMAT colorFormat = DXGI_FORMAT_R32_FLOAT;
		// デプスステンシルバッファのフォーマット
		DXGI_FORMAT depthFormat = DXGI_FORMAT_D32_FLOAT;
		
		// 近影用のシャドウマップ用レンダリングターゲットを作成。
		m_shadowMaps[SHADOW_MAP_AREA_NEAR].Create(
			2048,
			2048,
			1,
			1,
			colorFormat,
			depthFormat,
			clearColor
		);
		// 中影用のシャドウマップ用レンダリングターゲットを作成。
		m_shadowMaps[SHADOW_MAP_AREA_MIDDLE].Create(
			1024,
			1024,
			1,
			1,
			colorFormat,
			depthFormat,
			clearColor
		);
		// 遠影用のシャドウマップ用レンダリングターゲットを作成。
		m_shadowMaps[SHADOW_MAP_AREA_FAR].Create(
			512,
			512,
			1,
			1,
			colorFormat,
			depthFormat,
			clearColor
		);
		

		// ライトまでのベクトル
		Vector3 camPos = g_camera3D->GetPosition();
		Vector3 toLigDir = { 1, 2, 1 };
		toLigDir.Normalize();
		toLigDir *= 5000.0f;    // ライトまでの距離
		camPos += toLigDir;

		// カメラの位置を設定。これはライトの位置
		m_lightCamera.SetPosition(camPos);

		// カメラの注視点を設定。これがライトが照らしている場所
		m_lightCamera.SetTarget({ 0, 0, 0 });

		// 上方向を設定。今回はライトが真下を向いているので、X方向を上にしている
		m_lightCamera.SetUp(1, 0, 0);

		m_lightCamera.SetUpdateProjMatrixFunc(Camera::enUpdateProjMatrixFunc_Ortho);
		m_lightCamera.SetWidth(5000.0f);
		m_lightCamera.SetHeight(5000.0f);
		m_lightCamera.SetNear(1.0f);
		m_lightCamera.SetFar(10000.0f);

		// ライトビュープロジェクション行列を計算している
		m_lightCamera.Update();
	}

	void ShadowMapRender::Render(
		RenderContext& rc,
		int ligNo,
		Vector3& lightDirection
	)
	{
		Vector3 lightPos = g_camera3D->GetPosition();
		m_lightCamera.SetTarget(g_camera3D->GetPosition());
		// ライトの高さは50m決め打ち。
		float lightMaxHeight = 5000.0f;
		lightPos += (lightDirection) * (lightMaxHeight / lightDirection.y);
		m_lightCamera.SetPosition(lightPos);
		m_lightCamera.Update();

		if (lightDirection.LengthSq() < 0.001f){
			return;
		}

		// シャドウマップに描画するモデルの配列
		std::vector<Model*> shadowMapModelArray[NUM_SHADOW_MAP];

		// シャドウマップの数の分だけループさせる
		for (int i = 0; i < NUM_SHADOW_MAP; i++) {
			// レンダリングターゲットとして使用可能になるまで待つ。
			rc.WaitUntilToPossibleSetRenderTarget(m_shadowMaps[i]);
			// レンダリングターゲットとビューポートを設定する。
			rc.SetRenderTargetAndViewport(m_shadowMaps[i]);
			// レンダリングターゲットをクリアする。
			rc.ClearRenderTargetView(m_shadowMaps[i]);

			// ライトの最大の高さを計算する。
			m_cascadeShadowMapMatrix.CalcLightViewProjectionCropMatrix(
				lightDirection,
				m_cascadeAreaRateArray,
				m_lightCamera
			);

			// モデルの配列を取得。
			shadowMapModelArray[i] = g_renderingEngine->GetShadowMapModel(i);

			for (auto& model : shadowMapModelArray[i])
			{
				//model->Draw(
				//	rc,
				//	g_camera3D->GetViewMatrix(),
				//	g_camera3D->GetProjectionMatrix()
				//);
				
				model->Draw(
					rc,
					g_matIdentity,
					m_cascadeShadowMapMatrix.GetLightViewProjectionCropMatrix(i)
				);
				
			}

			// レンダリングターゲットへの描画終了を待つ。
			rc.WaitUntilFinishDrawingToRenderTarget(m_shadowMaps[i]);
		}

	}
}

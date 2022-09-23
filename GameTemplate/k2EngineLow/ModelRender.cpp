#include "k2EngineLowPreCompile.h"
#include "ModelRender.h"

namespace nsK2EngineLow
{
	void ModelRender::Init(const char* filePath,
		AnimationClip* animationClips,
		int numAnimationClips,
		EnModelUpAxis enModelUpAxis,
		bool isDrawOutLine,
		int maxInstance
	)
	{
		// スケルトンを初期化。
		InitSkeleton(filePath);
		// アニメーションを初期化。
		InitAnimation(animationClips, numAnimationClips);

		InitBackGroundModelWithPBR(filePath, enModelUpAxis);

		if (isDrawOutLine == true) {
			InitModelWithOutLine(filePath, enModelUpAxis);
		}
		//else {
		//	// フォワードレンダリング用のモデルを初期化。
		//	InitForwardRenderingModel(filePath, enModelUpAxis);
		//	// 

		//}

	}

	void ModelRender::InitSkeleton(const char* filePath)
	{
		std::string skeletonFilePath = filePath;
		int pos = (int)skeletonFilePath.find(".tkm");
		skeletonFilePath.replace(pos, 4, ".tks");
		m_skeleton.Init(skeletonFilePath.c_str());
	}

	void ModelRender::InitAnimation(AnimationClip* animationClips,
		int numAnimationClips)
	{
		m_numAnimationClips = numAnimationClips;
		m_animationClips = animationClips;

		if (m_animationClips != nullptr) {
			// アニメーションを初期化
			m_animation.Init(
				m_skeleton,					// アニメーションを流し込むスケルトン
				m_animationClips,			// アニメーションクリップ
				m_numAnimationClips			// アニメーションの数
			);
		}
	}

	void ModelRender::InitForwardRenderingModel(const char* filePath,
		EnModelUpAxis enModelUpAxis
	)
	{
		ModelInitData modelInitData;
		// tkmファイルのファイルパスを指定する。
		modelInitData.m_tkmFilePath = filePath;
		// シェーダーファイルのファイルパスを指定する。
		modelInitData.m_fxFilePath = "Assets/shader/model.fx";
		// エントリーポイントを指定する。
		if (m_animationClips != nullptr) {
			//スケルトンを指定する。
			modelInitData.m_skeleton = &m_skeleton;
			//スキンメッシュ用の頂点シェーダーのエントリーポイントを指定。
			modelInitData.m_vsSkinEntryPointFunc = "VSSkinMain";
		}
		else {
			//ノンスキンメッシュ用の頂点シェーダーのエントリーポイントを指定する。
			modelInitData.m_vsEntryPointFunc = "VSMain";
		}
		// カラーバッファのフォーマットを指定する。
		modelInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
		// モデルの上方向を指定する。
		modelInitData.m_modelUpAxis = enModelUpAxis;
		// 音源データを定数バッファとして設定する
		// 作成した初期化データをもとにモデルを初期化する。
		m_model.Init(modelInitData);
	}

	void ModelRender::InitBackGroundModelWithPBR(const char* filePath,
		EnModelUpAxis enModelUpAxis
	)
	{
		ModelInitData modelInitData;

		// モデルのtkmファイルパスを指定。
		modelInitData.m_tkmFilePath = filePath;
		// シェーダーのfxファイルパスを指定。
		modelInitData.m_fxFilePath = "Assets/shader/pbr.fx";
		// モデルの上方向を指定。
		modelInitData.m_modelUpAxis = enModelUpAxis;
		// エントリーポイントを指定する。
		if (m_animationClips != nullptr) {
			//スケルトンを指定する。
			modelInitData.m_skeleton = &m_skeleton;
			//スキンメッシュ用の頂点シェーダーのエントリーポイントを指定。
			modelInitData.m_vsSkinEntryPointFunc = "VSSkinMain";
		}
		else {
			//ノンスキンメッシュ用の頂点シェーダーのエントリーポイントを指定する。
			modelInitData.m_vsEntryPointFunc = "VSMain";
		}

		modelInitData.m_expandConstantBuffer = &g_renderingEngine->GetSceneLight().GetLightData();
		modelInitData.m_expandConstantBufferSize = sizeof(g_renderingEngine->GetSceneLight().GetLightData());

		m_model.Init(modelInitData);
	}

	void ModelRender::InitModelWithOutLine(const char* filePath,
		EnModelUpAxis enModelUpAxis
	)
	{
		// モデルの初期化データ
		ModelInitData modelInitData;
		// モデルのtkmファイルパスを指定。
		modelInitData.m_tkmFilePath = filePath;
		// シェーダーのfxファイルパスを指定。
		modelInitData.m_fxFilePath = "Assets/shader/backModel.fx";
		// カリングモードを指定。フロントカリングを使用。
		modelInitData.m_cullMode = D3D12_CULL_MODE_FRONT;
		// モデルの上方向を指定。
		modelInitData.m_modelUpAxis = enModelUpAxis;
		// エントリーポイントを指定する。
		if (m_animationClips != nullptr) {
			//スケルトンを指定する。
			modelInitData.m_skeleton = &m_skeleton;
			//スキンメッシュ用の頂点シェーダーのエントリーポイントを指定。
			modelInitData.m_vsSkinEntryPointFunc = "VSSkinMain";
		}
		else {
			//ノンスキンメッシュ用の頂点シェーダーのエントリーポイントを指定する。
			modelInitData.m_vsEntryPointFunc = "VSMain";
		}
		// 初期化データをもとにモデルを初期化。
		m_frontCullingModel.Init(modelInitData);
	}

	void ModelRender::UpdateInstancingData(const Vector3& pos, const Quaternion& rot, const Vector3& scale)
	{
		K2_ASSERT(m_numInstance < m_maxInstance, "インスタンスの数が多すぎです。");
		if (!m_isEnableInstancingDraw) {
			return;
		}
		auto wlorldMatrix = m_model.CalcWorldMatrix(pos, rot, scale);

		// インスタンシング描画を行う。
		m_worldMatrixArray[m_numInstance] = wlorldMatrix;
		if (m_numInstance == 0) {
			//インスタンス数が0の場合のみアニメーション関係の更新を行う。
			// スケルトンを更新。
			// 各インスタンスのワールド空間への変換は、
			// インスタンスごとに行う必要があるので、頂点シェーダーで行う。
			// なので、単位行列を渡して、モデル空間でボーン行列を構築する。
			m_skeleton.Update(g_matIdentity);
			//アニメーションを進める。
			m_animation.Progress(g_gameTime->GetFrameDeltaTime() * m_animationSpeed);
		}
		m_numInstance++;
	}

	void ModelRender::Update()
	{
		// ワールド座標の更新
		UpdateWorldMatrix();

		if (m_skeleton.IsInited()) {
			// スケルトンを更新
			m_skeleton.Update(m_model.GetWorldMatrix());
		}

		// アニメーションを更新
		m_animation.Progress(g_gameTime->GetFrameDeltaTime() * m_animationSpeed);
	}

	void ModelRender::UpdateWorldMatrix()
	{
		// モデルのワールド座標の更新。
		if (m_model.IsInited())
		{
			m_model.UpdateWorldMatrix(m_position, m_rotation, m_scale);
		}
		if (m_frontCullingModel.IsInited())
		{
			m_frontCullingModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);
		}
	}

	void ModelRender::Draw(RenderContext& rc)
	{
		// モデルを描画パスに追加
		if (m_model.IsInited())
		{
			g_renderingEngine->Add3DModelToForwardRenderPass(m_model);
		}
		if (m_frontCullingModel.IsInited())
		{
			g_renderingEngine->Add3DModelToRenderingModelsForOutLine(m_frontCullingModel);
		}
	}
}

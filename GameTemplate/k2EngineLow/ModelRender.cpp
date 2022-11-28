#include "k2EngineLowPreCompile.h"
#include "ModelRender.h"

namespace nsK2EngineLow
{
	void ModelRender::Init(const char* filePath,
		AnimationClip* animationClips,
		int numAnimationClips,
		EnModelUpAxis enModelUpAxis,
		bool isDrawOutLine,
		bool isCharacterModel,
		bool isShadowCaster,
		int maxInstance
	)
	{

		// スケルトンを初期化。
		InitSkeleton(filePath);
		// アニメーションを初期化。
		InitAnimation(animationClips, numAnimationClips);

		if (isDrawOutLine == true) {
			// モデルの背面用モデルを初期化。
			InitModelForBackWithOutLine(filePath, enModelUpAxis);
			InitDepthModel(filePath, enModelUpAxis);
		}

		if (isCharacterModel == true) {
			// トゥーンマップを初期化。
			m_toonMap.Init();
			//フォワードレンダリング用のモデルを初期化。
			InitForwardRenderingModel(filePath, enModelUpAxis);
		}
		else {
			// 背景用モデルを初期化。
			InitBackGroundModelWithPBR(filePath, enModelUpAxis);
		}
		if (isShadowCaster) {
			InitShadowMapModel(filePath, enModelUpAxis);
		}

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
		// モデルの初期化データ。
		ModelInitData modelInitData;
		// tkmファイルのファイルパスを指定する。
		modelInitData.m_tkmFilePath = filePath;
		// シェーダーファイルのファイルパスを指定する。
		modelInitData.m_fxFilePath = "Assets/shader/toonModel.fx";
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
		// 拡張SRVにトゥーンマップを設定。
		modelInitData.m_expandShaderResoruceView[0] = &m_toonMap.GetToonMap();
		// 定数バッファを設定。
		modelInitData.m_expandConstantBuffer = &g_renderingEngine->GetLightingCB();
		modelInitData.m_expandConstantBufferSize = sizeof(g_renderingEngine->GetLightingCB());
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
		// モデルの初期化データ。
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
		// 定数バッファを設定。
		modelInitData.m_expandConstantBuffer = &g_renderingEngine->GetLightingCB();
		modelInitData.m_expandConstantBufferSize = sizeof(g_renderingEngine->GetLightingCB());

		for (int areaNo = 0; areaNo < NUM_SHADOW_MAP; areaNo++) {
			modelInitData.m_expandShaderResoruceView[areaNo] = &g_renderingEngine->GetShadowMap(areaNo);
		}
		// 初期化データをもとにモデルを初期化。
		m_model.Init(modelInitData);
	}

	void ModelRender::InitModelForBackWithOutLine(const char* filePath,
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

	void ModelRender::InitDepthModel(const char* filePath,
		EnModelUpAxis enModelUpAxis
	)
	{
		// モデルの初期化データ。
		ModelInitData modelInitData;
		// モデルの上方向を指定する
		modelInitData.m_modelUpAxis = enModelUpAxis;
		// シェーダーファイルのファイルパスを指定する
		modelInitData.m_fxFilePath = "Assets/shader/depthForOutLine.fx";
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
		// tkmファイルのファイルパスを指定する
		modelInitData.m_tkmFilePath = filePath;
		// 初期化データをもとにモデルを初期化
		m_depthModel.Init(modelInitData);
	}

	void ModelRender::InitShadowMapModel(const char* filePath,
		EnModelUpAxis enModelUpAxis
	)
	{
		
		//// 初期化データをもとにモデルを初期化する。
		//m_shadowMapModel.Init(modelInitData);
		for (int i = 0; i < NUM_SHADOW_MAP; i++) {
			// モデルの初期化データ。
			ModelInitData modelInitData;
			// モデルのファイルパスを指定する。
			modelInitData.m_tkmFilePath = filePath;
			// シェーダーのfxファイルパスを指定する。
			modelInitData.m_fxFilePath = "Assets/shader/drawShadowMap.fx";
			// カラーバッファのフォーマットを指定する。
			modelInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32_FLOAT;
			// エントリーポイントを指定する。
			if (m_animationClips != nullptr) {
				modelInitData.m_skeleton = &m_skeleton;
				modelInitData.m_vsSkinEntryPointFunc = "VSSkinMain";
			}
			else {
				modelInitData.m_vsEntryPointFunc = "VSMain";
			}
			// 初期化データをもとに初期化する。
			m_shadowMapModel[i].Init(modelInitData);
		}
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
		if (m_depthModel.IsInited())
		{
			m_depthModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);
		}
		for (int i = 0; i < NUM_SHADOW_MAP; i++) {
			if (m_shadowMapModel[i].IsInited())
			{
				m_shadowMapModel[i].UpdateWorldMatrix(m_position, m_rotation, m_scale);
			}
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
		if (m_depthModel.IsInited())
		{
			g_renderingEngine->Add3DModelToDepthForOutLinePass(m_depthModel);
		}
		for (int i = 0; i < NUM_SHADOW_MAP; i++) {
			if (m_shadowMapModel[i].IsInited())
			{
				g_renderingEngine->Add3DModelToRenderToShadowMapPass(m_shadowMapModel[i], i);
			}
		}
	}
}
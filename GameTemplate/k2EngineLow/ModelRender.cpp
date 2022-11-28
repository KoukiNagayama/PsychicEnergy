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

		// �X�P���g�����������B
		InitSkeleton(filePath);
		// �A�j���[�V�������������B
		InitAnimation(animationClips, numAnimationClips);

		if (isDrawOutLine == true) {
			// ���f���̔w�ʗp���f�����������B
			InitModelForBackWithOutLine(filePath, enModelUpAxis);
			InitDepthModel(filePath, enModelUpAxis);
		}

		if (isCharacterModel == true) {
			// �g�D�[���}�b�v���������B
			m_toonMap.Init();
			//�t�H���[�h�����_�����O�p�̃��f�����������B
			InitForwardRenderingModel(filePath, enModelUpAxis);
		}
		else {
			// �w�i�p���f�����������B
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
			// �A�j���[�V������������
			m_animation.Init(
				m_skeleton,					// �A�j���[�V�����𗬂����ރX�P���g��
				m_animationClips,			// �A�j���[�V�����N���b�v
				m_numAnimationClips			// �A�j���[�V�����̐�
			);
		}
	}

	void ModelRender::InitForwardRenderingModel(const char* filePath,
		EnModelUpAxis enModelUpAxis
	)
	{
		// ���f���̏������f�[�^�B
		ModelInitData modelInitData;
		// tkm�t�@�C���̃t�@�C���p�X���w�肷��B
		modelInitData.m_tkmFilePath = filePath;
		// �V�F�[�_�[�t�@�C���̃t�@�C���p�X���w�肷��B
		modelInitData.m_fxFilePath = "Assets/shader/toonModel.fx";
		// �G���g���[�|�C���g���w�肷��B
		if (m_animationClips != nullptr) {
			//�X�P���g�����w�肷��B
			modelInitData.m_skeleton = &m_skeleton;
			//�X�L�����b�V���p�̒��_�V�F�[�_�[�̃G���g���[�|�C���g���w��B
			modelInitData.m_vsSkinEntryPointFunc = "VSSkinMain";
		}
		else {
			//�m���X�L�����b�V���p�̒��_�V�F�[�_�[�̃G���g���[�|�C���g���w�肷��B
			modelInitData.m_vsEntryPointFunc = "VSMain";
		}
		// �J���[�o�b�t�@�̃t�H�[�}�b�g���w�肷��B
		modelInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
		// �g��SRV�Ƀg�D�[���}�b�v��ݒ�B
		modelInitData.m_expandShaderResoruceView[0] = &m_toonMap.GetToonMap();
		// �萔�o�b�t�@��ݒ�B
		modelInitData.m_expandConstantBuffer = &g_renderingEngine->GetLightingCB();
		modelInitData.m_expandConstantBufferSize = sizeof(g_renderingEngine->GetLightingCB());
		// ���f���̏�������w�肷��B
		modelInitData.m_modelUpAxis = enModelUpAxis;
		// �����f�[�^��萔�o�b�t�@�Ƃ��Đݒ肷��
		// �쐬�����������f�[�^�����ƂɃ��f��������������B
		m_model.Init(modelInitData);

	}

	void ModelRender::InitBackGroundModelWithPBR(const char* filePath,
		EnModelUpAxis enModelUpAxis
	)
	{
		// ���f���̏������f�[�^�B
		ModelInitData modelInitData;

		// ���f����tkm�t�@�C���p�X���w��B
		modelInitData.m_tkmFilePath = filePath;
		// �V�F�[�_�[��fx�t�@�C���p�X���w��B
		modelInitData.m_fxFilePath = "Assets/shader/pbr.fx";
		// ���f���̏�������w��B
		modelInitData.m_modelUpAxis = enModelUpAxis;
		// �G���g���[�|�C���g���w�肷��B
		if (m_animationClips != nullptr) {
			//�X�P���g�����w�肷��B
			modelInitData.m_skeleton = &m_skeleton;
			//�X�L�����b�V���p�̒��_�V�F�[�_�[�̃G���g���[�|�C���g���w��B
			modelInitData.m_vsSkinEntryPointFunc = "VSSkinMain";
		}
		else {
			//�m���X�L�����b�V���p�̒��_�V�F�[�_�[�̃G���g���[�|�C���g���w�肷��B
			modelInitData.m_vsEntryPointFunc = "VSMain";
		}
		// �萔�o�b�t�@��ݒ�B
		modelInitData.m_expandConstantBuffer = &g_renderingEngine->GetLightingCB();
		modelInitData.m_expandConstantBufferSize = sizeof(g_renderingEngine->GetLightingCB());

		for (int areaNo = 0; areaNo < NUM_SHADOW_MAP; areaNo++) {
			modelInitData.m_expandShaderResoruceView[areaNo] = &g_renderingEngine->GetShadowMap(areaNo);
		}
		// �������f�[�^�����ƂɃ��f�����������B
		m_model.Init(modelInitData);
	}

	void ModelRender::InitModelForBackWithOutLine(const char* filePath,
		EnModelUpAxis enModelUpAxis
	)
	{
		// ���f���̏������f�[�^
		ModelInitData modelInitData;
		// ���f����tkm�t�@�C���p�X���w��B
		modelInitData.m_tkmFilePath = filePath;
		// �V�F�[�_�[��fx�t�@�C���p�X���w��B
		modelInitData.m_fxFilePath = "Assets/shader/backModel.fx";
		// �J�����O���[�h���w��B�t�����g�J�����O���g�p�B
		modelInitData.m_cullMode = D3D12_CULL_MODE_FRONT;
		// ���f���̏�������w��B
		modelInitData.m_modelUpAxis = enModelUpAxis;
		// �G���g���[�|�C���g���w�肷��B
		if (m_animationClips != nullptr) {
			//�X�P���g�����w�肷��B
			modelInitData.m_skeleton = &m_skeleton;
			//�X�L�����b�V���p�̒��_�V�F�[�_�[�̃G���g���[�|�C���g���w��B
			modelInitData.m_vsSkinEntryPointFunc = "VSSkinMain";
		}
		else {
			//�m���X�L�����b�V���p�̒��_�V�F�[�_�[�̃G���g���[�|�C���g���w�肷��B
			modelInitData.m_vsEntryPointFunc = "VSMain";
		}
		// �������f�[�^�����ƂɃ��f�����������B
		m_frontCullingModel.Init(modelInitData);
	}

	void ModelRender::InitDepthModel(const char* filePath,
		EnModelUpAxis enModelUpAxis
	)
	{
		// ���f���̏������f�[�^�B
		ModelInitData modelInitData;
		// ���f���̏�������w�肷��
		modelInitData.m_modelUpAxis = enModelUpAxis;
		// �V�F�[�_�[�t�@�C���̃t�@�C���p�X���w�肷��
		modelInitData.m_fxFilePath = "Assets/shader/depthForOutLine.fx";
		// �G���g���[�|�C���g���w�肷��B
		if (m_animationClips != nullptr) {
			//�X�P���g�����w�肷��B
			modelInitData.m_skeleton = &m_skeleton;
			//�X�L�����b�V���p�̒��_�V�F�[�_�[�̃G���g���[�|�C���g���w��B
			modelInitData.m_vsSkinEntryPointFunc = "VSSkinMain";
		}
		else {
			//�m���X�L�����b�V���p�̒��_�V�F�[�_�[�̃G���g���[�|�C���g���w�肷��B
			modelInitData.m_vsEntryPointFunc = "VSMain";
		}
		// tkm�t�@�C���̃t�@�C���p�X���w�肷��
		modelInitData.m_tkmFilePath = filePath;
		// �������f�[�^�����ƂɃ��f����������
		m_depthModel.Init(modelInitData);
	}

	void ModelRender::InitShadowMapModel(const char* filePath,
		EnModelUpAxis enModelUpAxis
	)
	{
		
		//// �������f�[�^�����ƂɃ��f��������������B
		//m_shadowMapModel.Init(modelInitData);
		for (int i = 0; i < NUM_SHADOW_MAP; i++) {
			// ���f���̏������f�[�^�B
			ModelInitData modelInitData;
			// ���f���̃t�@�C���p�X���w�肷��B
			modelInitData.m_tkmFilePath = filePath;
			// �V�F�[�_�[��fx�t�@�C���p�X���w�肷��B
			modelInitData.m_fxFilePath = "Assets/shader/drawShadowMap.fx";
			// �J���[�o�b�t�@�̃t�H�[�}�b�g���w�肷��B
			modelInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32_FLOAT;
			// �G���g���[�|�C���g���w�肷��B
			if (m_animationClips != nullptr) {
				modelInitData.m_skeleton = &m_skeleton;
				modelInitData.m_vsSkinEntryPointFunc = "VSSkinMain";
			}
			else {
				modelInitData.m_vsEntryPointFunc = "VSMain";
			}
			// �������f�[�^�����Ƃɏ���������B
			m_shadowMapModel[i].Init(modelInitData);
		}
	}

	void ModelRender::Update()
	{
		// ���[���h���W�̍X�V
		UpdateWorldMatrix();

		if (m_skeleton.IsInited()) {
			// �X�P���g�����X�V
			m_skeleton.Update(m_model.GetWorldMatrix());
		}

		// �A�j���[�V�������X�V
		m_animation.Progress(g_gameTime->GetFrameDeltaTime() * m_animationSpeed);
	}

	void ModelRender::UpdateWorldMatrix()
	{
		// ���f���̃��[���h���W�̍X�V�B
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
		// ���f����`��p�X�ɒǉ�
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
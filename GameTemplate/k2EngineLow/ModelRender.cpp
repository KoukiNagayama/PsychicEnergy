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
		// �X�P���g�����������B
		InitSkeleton(filePath);
		// �A�j���[�V�������������B
		InitAnimation(animationClips, numAnimationClips);

		InitBackGroundModelWithPBR(filePath, enModelUpAxis);

		if (isDrawOutLine == true) {
			InitModelWithOutLine(filePath, enModelUpAxis);
		}
		//else {
		//	// �t�H���[�h�����_�����O�p�̃��f�����������B
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
		ModelInitData modelInitData;
		// tkm�t�@�C���̃t�@�C���p�X���w�肷��B
		modelInitData.m_tkmFilePath = filePath;
		// �V�F�[�_�[�t�@�C���̃t�@�C���p�X���w�肷��B
		modelInitData.m_fxFilePath = "Assets/shader/model.fx";
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

		modelInitData.m_expandConstantBuffer = &g_renderingEngine->GetSceneLight().GetLightData();
		modelInitData.m_expandConstantBufferSize = sizeof(g_renderingEngine->GetSceneLight().GetLightData());

		m_model.Init(modelInitData);
	}

	void ModelRender::InitModelWithOutLine(const char* filePath,
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

	void ModelRender::UpdateInstancingData(const Vector3& pos, const Quaternion& rot, const Vector3& scale)
	{
		K2_ASSERT(m_numInstance < m_maxInstance, "�C���X�^���X�̐����������ł��B");
		if (!m_isEnableInstancingDraw) {
			return;
		}
		auto wlorldMatrix = m_model.CalcWorldMatrix(pos, rot, scale);

		// �C���X�^���V���O�`����s���B
		m_worldMatrixArray[m_numInstance] = wlorldMatrix;
		if (m_numInstance == 0) {
			//�C���X�^���X����0�̏ꍇ�̂݃A�j���[�V�����֌W�̍X�V���s���B
			// �X�P���g�����X�V�B
			// �e�C���X�^���X�̃��[���h��Ԃւ̕ϊ��́A
			// �C���X�^���X���Ƃɍs���K�v������̂ŁA���_�V�F�[�_�[�ōs���B
			// �Ȃ̂ŁA�P�ʍs���n���āA���f����ԂŃ{�[���s����\�z����B
			m_skeleton.Update(g_matIdentity);
			//�A�j���[�V������i�߂�B
			m_animation.Progress(g_gameTime->GetFrameDeltaTime() * m_animationSpeed);
		}
		m_numInstance++;
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
	}
}

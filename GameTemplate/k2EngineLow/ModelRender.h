#pragma once
#include "ToonMap.h"
#include "MyRenderer.h"
#include "GeometryData.h"
namespace nsK2EngineLow
{
	class ModelRender
	{
	public:
		ModelRender();
		~ModelRender();

		/// <summary>
		/// ����������
		/// </summary>
		/// <param name="filePath">�t�@�C���p�X�B</param>
		/// <param name="animationClips">�A�j���[�V�����N���b�v�B</param>
		/// <param name="numAnimationClips">�A�j���[�V�����N���b�v�̐��B</param>
		/// <param name="enModelUpAxis">���f���̏�����B</param>
		/// <param name="isDrawOutLine">�֊s����`�悷�邩�B</param>
		/// <param name="isCharacterModel">�L�����N�^�[�̃��f�����B</param>
		void Init(const char* filePath,
			AnimationClip* animationClips = nullptr,
			int numAnimationClips = 0,
			EnModelUpAxis enModelUpAxis = enModelUpAxisZ,
			bool isDrawOutLine = false,
			bool isCharacterModel = false,
			bool isShadowCaster = true,
			bool isFloating = false
		);
		/// <summary>
		/// ����ȃt�H���[�h�����_�����O�p���f���̏������֐�
		/// </summary>
		/// <param name="initData">���f���̏������f�[�^</param>
		void InitDirectlyNotifyForwardRendering(ModelInitData initData);
		/// <summary>
		/// �X�V����
		/// </summary>
		void Update();
		/// <summary>
		/// �`�揈��
		/// </summary>
		void Draw(RenderContext& rc);
		/// <summary>
		/// �A�j���[�V�����Đ�
		/// </summary>
		/// <param name="animNo">�A�j���[�V�����N���b�v�̔ԍ��B</param>
		/// <param name="interpolateTime">��Ԏ���(�P�ʁF�b)�B</param>
		void PlayAnimation(int animNo, float interpolateTime = 0.0f)
		{
			m_animation.Play(animNo, interpolateTime);
		}
		/// <summary>
		/// �A�j���[�V�����̍Đ����H
		/// </summary>
		/// <returns>�Đ����Ȃ��true�B�Đ����Ă��Ȃ��Ȃ��false</returns>
		bool IsPlayingAnimation() const
		{
			return m_animation.IsPlaying();
		}
		/// <summary>
		/// ���W�A��]�A�g�嗦��ݒ�
		/// </summary>
		/// <param name="position">���W�B</param>
		/// <param name="rotation">��]�B</param>
		/// <param name="scale">�g�嗦�B</param>
		void SetTRS(const Vector3& position, const Quaternion& rotation, const Vector3& scale)
		{
			SetPosition(position);
			SetRotation(rotation);
			SetScale(scale);
		}
		/// <summary>
		/// ���W��ݒ�
		/// </summary>
		/// <param name="position">���W�B</param>
		void SetPosition(const Vector3& position)
		{
			m_position = position;
		}
		/// <summary>
		/// ���W��ݒ�
		/// </summary>
		/// <param name="x">x���W�B</param>
		/// <param name="y">y���W�B</param>
		/// <param name="z">z���W�B</param>
		void SetPosition(float x, float y, float z)
		{
			SetPosition({ x, y, z });
		}
		/// <summary>
		/// ��]��ݒ�
		/// </summary>
		/// <param name="rotation">��]�B</param>
		void SetRotation(const Quaternion& rotation)
		{
			m_rotation = rotation;
		}
		/// <summary>
		/// �g�嗦��ݒ�
		/// </summary>
		/// <param name="scale">�g�嗦�B</param>
		void SetScale(const Vector3& scale)
		{
			m_scale = scale;
		}
		/// <summary>
		/// �g�嗦��ݒ�
		/// </summary>
		/// <param name="x">x���W�B</param>
		/// <param name="y">y���W�B</param>
		/// <param name="z">z���W�B</param>
		void SetScale(float x, float y, float z)
		{
			SetScale({ x, y, z });
		}
		/// <summary>
		/// �A�j���[�V�����̍Đ����x��ݒ肷��B
		/// </summary>
		/// <param name="animationSpeed">�A�j���[�V�����̍Đ����x�B���l�̕��������x���{�ɂȂ�B</param>
		void SetAnimationSpeed(const float animationSpeed)
		{
			m_animationSpeed = animationSpeed;
		}
		/// <summary>
		/// �A�j���[�V�����C�x���g�̒ǉ�
		/// </summary>
		/// <param name="eventListener"></param>
		void AddAnimationEvent(AnimationEventListener eventListener)
		{
			m_animation.AddAnimationEventListener(eventListener);
		}
		/// <summary>
		/// ���V��Ԃ��ǂ�����ݒ�B
		/// </summary>
		void SetIsFloating(const bool isFloating)
		{
			if (isFloating) {
				m_floating = 1;
			}
			else {
				m_floating = 0;
			}
			
		}
		bool IsFloating() const
		{
			if (m_floating == 0) {
				return false;
			}
			else {
				return true;
			}
		}
		/// <summary>
		/// ���f�����擾
		/// </summary>
		/// <returns>���f���B</returns>
		Model& GetModel()
		{
			return m_model;
		}
		/// <summary>
		/// ���[���h�s����擾�B
		/// </summary>
		/// <param name="instanceId">�C���X�^���XID�B</param>
		/// <returns>���[���h�s��B</returns>
		const Matrix& GetWorldMatrix() const
		{
			return m_model.GetWorldMatrix();
		}
		/// <summary>
		/// �V���h�E�L���X�^�[�H
		/// </summary>
		/// <returns>�V���h�E�L���X�^�[�ł����true�B�����łȂ����false�B</returns>
		bool IsShadowCaster() const
		{
			return m_isShadowCaster;
		}
	private:
		/// <summary>
		/// �X�P���g���̏�����
		/// </summary>
		/// <param name="filePath">�t�@�C���p�X�B</param>
		void InitSkeleton(const char* filePath);
		/// <summary>
		/// �A�j���[�V�����̏�����
		/// </summary>
		/// <param name="animationClips">�A�j���[�V�����N���b�v�B</param>
		/// <param name="numAnimationClips">�A�j���[�V�����N���b�v�̐��B</param>
		void InitAnimation(AnimationClip* animationClips,
			int numAnimationClips);
		/// <summary>
		/// �t�H���[�h�����_�����O�p���f����������
		/// </summary>
		/// <param name="filePath">�t�@�C���p�X�B</param>
		/// <param name="enModelUpAxis">���f���̏�����B</param>
		void InitForwardRenderingModel(const char* filePath,
			EnModelUpAxis enModelUpAxis = enModelUpAxisZ
		);
		/// <summary>
		/// PBR�𗘗p���ĕ`�悷��w�i���f����������
		/// </summary>
		/// <param name="filePath"></param>
		/// <param name="enModelUpAxis"></param>
		void InitBackGroundModelWithPBR(const char* filePath,
			EnModelUpAxis enModelUpAxis = enModelUpAxisZ
		);
		/// <summary>
		/// �L�����N�^�[�̃��f����������
		/// </summary>
		/// <param name="filePath"></param>
		/// <param name="enModelUpAxis"></param>
		void InitModelForBackWithOutLine(const char* filePath,
			EnModelUpAxis enModelUpAxis = enModelUpAxisZ,
			bool isFloating = false
		);
		/// <summary>
		/// �[�x�l�L�^�p���f����������
		/// </summary>
		/// <param name="filePath"></param>
		/// <param name="enModelUpAxis"></param>
		void InitDepthModel(const char* filePath,
			EnModelUpAxis enModelUpAxis = enModelUpAxisZ
		);
		/// <summary>
		/// �V���h�E�}�b�v�ɕ`�悷�郂�f����������
		/// </summary>
		/// <param name="filePath">�t�@�C���p�X</param>
		/// <param name="enModelUpAxis">���f���̏����</param>
		void InitShadowMapModel(const char* filePath,
			EnModelUpAxis enModelUpAxis = enModelUpAxisZ
		);
		/// <summary>
		/// �W�I���g������������
		/// </summary>
		void InitGeometryData();
		/// <summary>
		/// ���[���h�s��̍X�V
		/// </summary>
		void UpdateWorldMatrix();

	private:
		Model									m_model;										// �t�H���[�h�����_�����O�ŕ`�悳��郂�f��
		Model									m_frontCullingModel;							// �t�����g�J�����O���f��
		Model									m_depthModel;									// �[�x�l���o�p���f��
		Model									m_shadowMapModel[NUM_SHADOW_MAP];				// �V���h�E�}�b�v�ɕ`�悷�郂�f��
		Skeleton								m_skeleton;										// �X�P���g��
		Animation								m_animation;									// �A�j���[�V����
		AnimationClip*							m_animationClips = nullptr;						// �A�j���[�V�����N���b�v
		int										m_numAnimationClips;							// �A�j���[�V�����N���b�v�̐�
		Vector3									m_position = Vector3::Zero;						// ���W
		Quaternion								m_rotation = Quaternion::Identity;				// ��]
		Vector3									m_scale = Vector3::One;							// �g�嗦
		float									m_animationSpeed = 1.0f;						// �A�j���[�V�����̍Đ����x
		ToonMap									m_toonMap;										// �g�D�[���}�b�v
		int										m_floating = 0;									// ���V��Ԃ�
		bool									m_isShadowCaster = true;						// �V���h�E�L���X�^�[��
		GeometryData							m_geometryData;									// �W�I���g�����
	};
}



#pragma once
#include "sound/SoundSource.h"
#include "WorldRotation.h"
class IPlayerState;

/// <summary>
/// �v���C���[�N���X
/// </summary>
class Player : public IGameObject
{
public:
	enum EnAnimationClip {
		enAnimationClip_Idle,		// �ҋ@
		enAnimationClip_Walk,		// ����
		enAnimationClip_Run,		// ����
		enAnimationClip_Slide,		// �X���C�f�B���O
		enAnimationClip_NormalJump,	// �ʏ�̃W�����v
		enAnimationClip_DashJump,	// �_�b�V���W�����v
		enAnimationClip_IdleAir,	// �󒆂ł̑ҋ@
		enAnimationClip_FallAir,	// �󒆂ł̗���
		enAnimationClip_Num			// �A�j���[�V�����N���b�v�̐�
	};
public:
	Player() {}
	~Player() {}

	/// <summary>
	/// �J�n�����B
	/// �C���X�^���X�������ɏ��������s���Btrue��Ԃ����Ƃ��ɌĂ΂�Ȃ��Ȃ�B
	/// </summary>
	bool Start();
	/// <summary>
	/// �X�V�����B
	/// </summary>
	void Update();
	/// <summary>
	/// �`�揈���B
	/// </summary>
	/// <param name="rc"></param>
	void Render(RenderContext& rc);
	/// <summary>
	/// �Đ�����A�j���[�V�����N���b�v��ݒ肷��B
	/// </summary>
	/// <param name="enAnimationClip">�A�j���[�V�����N���b�v</param>
	void SetAnimation(EnAnimationClip enAnimationClip)
	{
		m_currentAnimationClip = enAnimationClip;
	}
	/// <summary>
	/// ���W��ݒ肷��B
	/// </summary>
	/// <param name="position">���W�B</param>
	void SetPosition(Vector3& position)
	{
		m_position = position;
	}
	/// <summary>
	/// ��]��ݒ�B
	/// </summary>
	/// <param name="rotation">��]�B</param>
	void SetRotation(Quaternion& rotation)
	{
		m_rotation = rotation;
	}
	/// <summary>
	/// �g�嗦��ݒ�B
	/// </summary>
	/// <param name="scale">�g�嗦�B</param>
	void SetScale(Vector3& scale)
	{
		m_scale = scale;
	}
	/// <summary>
	/// �I�u�W�F�N�g�Ƃ̐ڒn�����ݒ�B
	/// </summary>
	/// <param name="isTouchObject">�I�u�W�F�N�g�Ƃ̐ڒn����B</param>
	void SetIsTouchObject(bool isTouchObject)
	{
		m_isTouchObject = isTouchObject;
	}
	/// <summary>
	/// ���W���擾�B
	/// </summary>
	/// <returns>���W�B</returns>
	const Vector3& GetPosition() const 
	{
		return m_position;
	}
	/// <summary>
	/// ���݂̃��f���̏�������擾�B
	/// </summary>
	/// <returns>���f���̏�����B</returns>
	const Vector3& GetCurrentModelUpAxis() const
	{
		return m_currentModelUpAxis;
	}
	/// <summary>
	/// ��]���擾�B
	/// </summary>
	/// <returns>��]�B</returns>
	const Quaternion& GetRotation() const
	{
		return m_rotation;
	}
	/// <summary>
	/// �v���C���[���I�u�W�F�N�g�Ƃ̐ڒn������擾�B
	/// </summary>
	/// <returns>�I�u�W�F�N�g�̐ڒn����B</returns>
	const bool& IsPlayerTouchObject() const
	{
		return m_isTouchObject;
	}
	/// <summary>
	/// �X���C�h������������B
	/// </summary>
	const void InitSlideParam()
	{
		m_isSlide = true;
	}
	/// <summary>
	/// �X���C�h�����Z�b�g����B
	/// </summary>
	const void ResetSlideParam()
	{
		m_isSlide = false;
		if (m_isRingingSlideSound) {
			m_isRingingSlideSound = false;
			DeleteGO(m_slideSound);
		}
	}
	/// <summary>
	/// �X���C�h����
	/// </summary>
	/// <returns>�X���C�h�����ǂ���</returns>
	const bool& IsSlide() const
	{
		return m_isSlide;
	}
	/// <summary>
	/// ���ʕ������擾�B
	/// </summary>
	/// <returns>�v���C���[�̃��f���̐��ʕ����B</returns>
	const Vector3& GetForward() const
	{
		return m_forward;
	}
	/// <summary>
	/// �A�j���[�V�������Đ����Ă��邩�B
	/// </summary>
	const bool& IsPlayingAnimation() const
	{
		return m_modelRender.IsPlayingAnimation();
	}
	/// <summary>
	/// �A�j���[�V�����X�s�[�h��ݒ�B
	/// </summary>
	/// <param name="speed"></param>
	const void SetAnimationSpeed(float speed)
	{
		m_modelRender.SetAnimationSpeed(speed);
	}
	/// <summary>
	/// �W�����v�I����Ƀ��Z�b�g����B
	/// </summary>
	void ResetJump()
	{
		m_modelRender.SetAnimationSpeed(1.0f);

	}
	/// <summary>
	/// �n�ʏ�ɂ��邩
	/// </summary>
	const bool& IsOnGround() const 
	{
		return m_charaCon.IsOnGround();
	}
	/// <summary>
	/// ���[�h�̕ύX�B
	/// </summary>
	/// <param name="isFloating">���V��Ԃł��邩</param>
	void FloatModeChange(bool isFloating);
	/// <summary>
	/// ���V���Ă��邩�B
	/// </summary>
	/// <returns></returns>
	const bool& IsFloating() const 
	{
		return m_isFloating;
	}
	void SetDisablePlayerMove(const bool& disablePlayerMove)
	{
		m_disablePlayerMove = disablePlayerMove;
	}
private:
	friend class PlayerIdleState;
	friend class PlayerWalkState;
	friend class PlayerSlideState;
	friend class PlayerIdleInAirState;
	friend class PlayerFallInAirState;
	friend class PlayerJumpState;
	friend class PlayerFallState;
	/// <summary>
	/// �����������B
	/// </summary>
	void Init();
	/// <summary>
	/// ��]�����B
	/// </summary>
	void Rotation();
	void RotationFallAir();
	/// <summary>
	/// �ړ����x���v�Z
	/// </summary>
	/// <param name="speed">���x</param>
	Vector3 CalcMoveSpeed(float speed);
	/// <summary>
	/// �����B
	/// </summary>
	void MoveOnGround();
	/// <summary>
	/// �n��ł̃A�j���[�V������I���B
	/// </summary>
	void SelectAnimationOnGround();
	/// <summary>
	/// �ړ����������߂�B
	/// </summary>
	void DecideMoveDirection();
	/// <summary>
	/// �󒆂ł̈ړ������B
	/// </summary>
	void MoveOnAirspace();
	/// <summary>
	/// �X���C�f�B���O�B
	/// </summary>
	void Slide();
	/// <summary>
	/// �W�����v�B
	/// </summary>
	void Jump();
	/// <summary>
	/// �W�����v�̃A�j���[�V������I���B
	/// </summary>
	void SelectJumpAnimation();
	/// <summary>
	/// �A�j���[�V�����C�x���g�p�֐��B
	/// </summary>
	/// <param name="clipName"></param>
	/// <param name="eventName"></param>
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);
	/// <summary>
	/// �A�j���[�V�������Đ�����B
	/// </summary>
	void PlayAnimation(EnAnimationClip currentAnimationClip);
private:
	ModelRender				m_modelRender;										// ���f��
	Vector3					m_position = Vector3::Zero;						// ���W
	Vector3					m_scale = Vector3::One;							// �g�嗦
	Quaternion				m_rotation = Quaternion::Identity;				// ��]
	AnimationClip			m_animationClips[enAnimationClip_Num];			// �A�j���[�V�����N���b�v
	Vector3					m_currentModelUpAxis;							// ���݂̃��f���̏����
	bool					m_isTouchObject = true;							// ���f���ƃI�u�W�F�N�g�Ƃ̐ڒn����
	Vector3					m_moveSpeed = Vector3::Zero;					// �ړ����x
	CharacterController		m_charaCon;										// �L�����N�^�[�R���g���[���[
	IPlayerState*			m_playerState = nullptr;						// �X�e�[�g
	EnAnimationClip			m_currentAnimationClip;							// ���ݐݒ肳��Ă���A�j���[�V�����N���b�v
	Vector3					m_moveVectorInAir = Vector3::One;				// �󒆂ł̈ړ�����
	Vector3					m_forward = Vector3::AxisZ;						// ���ʕ���
	SoundSource*			m_runFootstep = nullptr;						// ����̑���
	Quaternion				m_slideRot = Quaternion::Identity;				// �X���C�h���̐i�s�����̉�]
	bool					m_isSlide = false;								// �X���C�h���H	
	SoundSource*			m_slideSound = nullptr;							// �X���C�h�̊��艹
	bool					m_isRingingSlideSound = false;					// �X���C�f�B���O�̉������Ă��邩
	SoundSource*			m_landingSound = nullptr;						// ���n��
	SoundSource*			m_modeChangeSound = nullptr;					// ���[�h�ύX���̌��ʉ�
	bool					m_isFloating = false;
	float					m_lastMoveSpeedY = 0.0f;
	EffectEmitter*			m_effectEmitterWind = nullptr;
	bool					m_disablePlayerMove = false;
};


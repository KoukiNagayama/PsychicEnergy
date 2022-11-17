#pragma once
#include "sound/SoundSource.h"

class PlayerState;

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
		enAnimationClip_Slide,
		enAnimationClip_Num			// �A�j���[�V�����N���b�v�̐�
	};
public:
	Player() {}
	~Player() {}

	/// <summary>
	/// �J�n�����B
	/// �C���X�^���X�������Ɉ�x�������s�B
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
	const void InitSlide()
	{
		m_isSlide = true;
	}
	/// <summary>
	/// �X���C�h�����Z�b�g����B
	/// </summary>
	const void ResetSlide()
	{
		m_isSlide = false;
	}
	/// <summary>
	/// �X���C�h����
	/// </summary>
	/// <returns>�X���C�h�����ǂ���</returns>
	const bool& IsSlide() const
	{
		return m_isSlide;
	}
	const Vector3& GetForward() const
	{
		return m_forward;
	}

private:
	friend class PlayerIdleState;
	friend class PlayerWalkState;
	friend class PlayerSlideState;
	friend class PlayerIdleInAirState;
	friend class PlayerFallInAirState;
	/// <summary>
	/// �����������B
	/// </summary>
	void Init();
	/// <summary>
	/// ��]�����B
	/// </summary>
	void Rotation();
	/// <summary>
	/// �����B
	/// </summary>
	void MoveOnGround();
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
	/// �A�j���[�V�����C�x���g�p�֐��B
	/// </summary>
	/// <param name="clipName"></param>
	/// <param name="eventName"></param>
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);

	void TestRotation();
	/// <summary>
	/// �A�j���[�V�������Đ�����B
	/// </summary>
	void PlayAnimation(EnAnimationClip currentAnimationClip);
	void A(Vector3& vector, const Vector3& axis, float deg, const Quaternion& q);
private:
	ModelRender				m_model;										// ���f��
	Vector3					m_position = Vector3::Zero;						// ���W
	Vector3					m_scale = Vector3::One;							// �g�嗦
	Quaternion				m_rotation = Quaternion::Identity;				// ��]
	AnimationClip			m_animationClips[enAnimationClip_Num];			// �A�j���[�V�����N���b�v
	Vector3					m_currentModelUpAxis;							// ���݂̃��f���̏����
	bool					m_isTouchObject = true;							// ���f���ƃI�u�W�F�N�g�Ƃ̐ڒn����
	Vector3					m_moveSpeed = Vector3::Zero;					// �ړ����x
	CharacterController		m_charaCon;										// �L�����N�^�[�R���g���[���[
	PlayerState*			m_playerState = nullptr;						// �X�e�[�g
	EnAnimationClip			m_currentAnimationClip;							// ���ݐݒ肳��Ă���A�j���[�V�����N���b�v
	Vector3					m_yawPitchRoll = Vector3::Zero;					// YawPitchRoll�̉�]�v�f
	Vector3					m_moveVectorInAir = Vector3::Zero;				// �󒆂ł̈ړ�����
	Vector3					m_forward = Vector3::AxisZ;						// ���ʕ���
	SoundSource*			m_runFootstep = nullptr;						// ����̑���
	Quaternion				m_slideRot = Quaternion::Identity;				// �X���C�h���̐i�s�����̉�]
	bool					m_isSlide = false;								// �X���C�h���H	
	Vector3					m_forward2 = Vector3::Zero;
};


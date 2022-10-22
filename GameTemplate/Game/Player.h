#pragma once
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
	/// �����B
	/// </summary>
	void WalkOnGround();
	/// <summary>
	/// �ړ����������߂�B
	/// </summary>
	void DecideMoveDirection();
	/// <summary>
	/// �󒆂ł̈ړ������B
	/// </summary>
	void MoveOnAirspace();

private:
	/// <summary>
	/// �����������B
	/// </summary>
	void Init();

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
};


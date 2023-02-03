#pragma once
class Title;
class TitleCharacter : public IGameObject
{
public:
	TitleCharacter() {}
	~TitleCharacter();

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
	void Render(RenderContext& rc);
	/// <summary>
	/// ���W��ݒ�B
	/// </summary>
	/// <param name="position">�ݒ肷����W�B</param>
	void SetPosition(Vector3& position)
	{
		m_position = position;
	}
	/// <summary>
	/// ��]��ݒ�B
	/// </summary>
	/// <param name="rotation">�ݒ肷���]�l�B</param>
	void SetRotation(Quaternion& rotation)
	{
		m_rotation = rotation;
	}
	/// <summary>
	/// �g�嗦��ݒ�B
	/// </summary>
	/// <param name="scale">�ݒ肷��g�嗦�B</param>
	void SetScale(Vector3& scale)
	{
		m_scale = scale;
	}
	/// <summary>
	/// ���W���擾�B
	/// </summary>
	/// <returns>���W�B</returns>
	const Vector3& GetPosition() const
	{
		return m_position;
	}
private:
	/// <summary>
	/// �A�j���[�V�������Đ��B
	/// </summary>
	void PlayAnimation();
private:
	enum EnAnimationClip {
		enAnimationClip_Idle,		// �ҋ@
		enAnimationClip_IdleAir,	// �󒆂ł̑ҋ@
		enAnimationClip_FallAir,	// �󒆂ł̗���
		enAnimationClip_Num			// �A�j���[�V�����N���b�v�̐�
	};
	ModelRender			m_modelRender;							// ���f�������_�[
	Vector3				m_position = Vector3::Zero;				// ���W
	Quaternion			m_rotation = Quaternion::Identity;		// ��]
	Vector3				m_scale = Vector3::One;					// �g�嗦
	AnimationClip		m_animationClips[enAnimationClip_Num];	// �A�j���[�V�����N���b�v
	EnAnimationClip		m_currentAnimationClip = enAnimationClip_Idle;
	bool				m_isFloating = false;
	float				m_floatingCount = 0.0f;
	Title*				m_title = nullptr;						// �^�C�g���N���X
	bool				m_isCompletedStateChange = false;
};


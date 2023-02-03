#pragma once
class Title;
class TitleCharacter;
/// <summary>
/// �^�C�g���J�����N���X
/// </summary>
class TitleCamera : public IGameObject
{
public:
	TitleCamera() {}
	~TitleCamera() {}
	/// <summary>
	/// �J�n�����B
	/// �C���X�^���X�������ɏ��������s���Btrue��Ԃ����Ƃ��ɌĂ΂�Ȃ��Ȃ�B
	/// </summary>
	bool Start();
	/// <summary>
	/// �X�V�����B
	/// </summary>
	void Update();
	bool IsStartFadeOut()
	{
		return m_isStartFadeOut;
	}
private:
	Title*			m_title = nullptr;								// �^�C�g���N���X
	TitleCharacter* m_titleCharacter = nullptr;						// �^�C�g���L�����N�^�[�N���X
	Vector3			m_position = Vector3::Zero;						// �J�����̍��W
	Quaternion		m_rotation = Quaternion::Identity;				// �J�����̉�]
	Vector3			m_toCameraPos = Vector3::Zero;					// �����_���王�_�ւ̃x�N�g��
	Vector3			m_charaPos = Vector3::Zero;
	bool			m_isStartFadeOut = false;
};


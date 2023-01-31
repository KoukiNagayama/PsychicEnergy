#pragma once
/// <summary>
/// �^�C���A�^�b�N�p�^�C�}�[�N���X
/// </summary>
class DisplayGameTimer : public IGameObject
{
public:
	DisplayGameTimer() {}
	~DisplayGameTimer() {}

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
	/// ���Ԃ��擾�B
	/// </summary>
	/// <returns>���݂̎��ԁB</returns>
	int GetTime() const
	{
		return (int)m_currentTime;
	}
	/// <summary>
	/// �`��ƍX�V�𖳌�������
	/// </summary>
	void Disable()
	{
		m_disable = true;
	}
private:
	/// <summary>
	/// �e���̐��l���v�Z����
	/// </summary>
	void CalculateNumOfEachDigit();
private:
	SpriteRender	m_numberSprite[3];				// ������\������X�v���C�g
	SpriteRender	m_timerBGSprite;				
	SpriteRender	m_timerBGFrameSprite;
	float			m_currentTime = 0.0f;			// ���݂̎���
	int	  			m_lastSecond = -1;				// ���O�̎���(�b)
	bool			m_disable = false;
};


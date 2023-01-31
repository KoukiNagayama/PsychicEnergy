#pragma once
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
	const int GetTime() const
	{
		return (int)m_currentTime;
	}
private:
	/// <summary>
	/// �e���̐��l���v�Z����
	/// </summary>
	void CalculateNumOfEachDigit();
private:
	SpriteRender	m_numberSprite[3];				// ������\������X�v���C�g
	float			m_currentTime = 0.0f;			// ���݂̎���
	int	  			m_lastSecond = -1;					// 	
};


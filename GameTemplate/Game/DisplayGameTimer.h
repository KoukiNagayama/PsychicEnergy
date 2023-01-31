#pragma once
class DisplayGameTimer : public IGameObject
{
public:
	DisplayGameTimer() {}
	~DisplayGameTimer() {}

	bool Start();
	void Update();
	void Render(RenderContext& rc);
private:
	/// <summary>
	/// �e���̐��l���v�Z����
	/// </summary>
	void CalculateNumOfEachDigit();
private:
	SpriteRender m_numberSprite[3];			// ������\������X�v���C�g
	float	m_currentTime = 0.0f;			// ���݂̎���
	int	  	m_oldTime = -1;				
};


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
	/// 各桁の数値を計算する
	/// </summary>
	void CalculateNumOfEachDigit();
private:
	SpriteRender m_numberSprite[3];			// 数字を表示するスプライト
	float	m_currentTime = 0.0f;			// 現在の時間
	int	  	m_oldTime = -1;				
};


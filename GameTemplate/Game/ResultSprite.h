#pragma once
#include "Result.h"
class Result;
class ResultSprite : public IGameObject
{
public:
	ResultSprite() {}
	~ResultSprite() {}

	bool Start();
	void Render(RenderContext& rc);
private:
	/// <summary>
	/// スプライトを初期化
	/// </summary>
	void InitSprite();
private:
	SpriteRender	m_numberSprite[3];
	SpriteRender	m_textSprite;
	Result*			m_result = nullptr;
	bool			m_isTimeUp = false;
};


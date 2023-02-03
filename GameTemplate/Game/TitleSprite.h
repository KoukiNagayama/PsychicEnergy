#pragma once
class TitleSprite : public IGameObject
{
public:
	TitleSprite() {}
	~TitleSprite() {}

	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:

private:
	SpriteRender			m_titleLogoSprite;
	SpriteRender			m_textSprite;

};


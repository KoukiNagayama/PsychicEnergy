#include "stdafx.h"
#include "TitleSprite.h"

namespace
{
	const Vector2 LOGO_SPRITE_SIZE = { 1280.0f, 720.0f };			// ロゴのスプライトのサイズ
	const Vector3 LOGO_SPRITE_POS = { 461.0f, 265.0f, 0.0f };		// ロゴのスプライトの座標
	const Vector3 LOGO_SPRITE_SCA = { 1.0f, 1.0f, 1.0f };			// ロゴのスプライトの拡大率
	const Vector2 TEXT_SPRITE_SIZE = { 512.0f, 512.0f };			// テキストのスプライトのサイズ
	const Vector3 TEXT_SPRITE_POS = { 456.0f, -300.0f, 0.0f };		// テキストのスプライトの座標
	const Vector3 TEXT_SPRITE_SCA = { 1.84f, 1.84f, 1.0f };			// テキストのスプライトの拡大率
}

bool TitleSprite::Start()
{
	// タイトルロゴ用スプライトを初期化。
	m_titleLogoSprite.Init("Assets/sprite/title/logo/titleLogo.DDS",
		LOGO_SPRITE_SIZE.x,
		LOGO_SPRITE_SIZE.y
	);
	// 座標と拡大率を設定。
	m_titleLogoSprite.SetPosition(LOGO_SPRITE_POS);
	m_titleLogoSprite.SetScale(LOGO_SPRITE_SCA);

	// テキストのスプライトを初期化。
	m_textSprite.Init("Assets/sprite/title/text/text.DDS",
		TEXT_SPRITE_SIZE.x,
		TEXT_SPRITE_SIZE.y
	);
	// 座標と拡大率を設定。
	m_textSprite.SetPosition(TEXT_SPRITE_POS);
	m_textSprite.SetScale(TEXT_SPRITE_SCA);

	// スプライトを更新。
	m_titleLogoSprite.Update();
	m_textSprite.Update();

	return true;
}

void TitleSprite::Update()
{
	
}

void TitleSprite::Render(RenderContext& rc)
{
	m_titleLogoSprite.Draw(rc);
	m_textSprite.Draw(rc);
}

#include "stdafx.h"
#include "TitleSprite.h"

namespace
{
	const Vector2 LOGO_SPRITE_SIZE = { 1280.0f, 720.0f };
	const Vector3 LOGO_SPRITE_POS = { 560.0f, 300.0f, 0.0f };
	const Vector3 LOGO_SPRITE_SCA = { 0.776f, 0.776f, 1.0f };
	const Vector2 TEXT_SPRITE_SIZE = { 512.0f, 512.0f };
	const Vector3 TEXT_SPRITE_POS = { 480.0f, -500.0f, 0.0f };
	const Vector3 TEXT_SPRITE_SCA = { 1.84f, 1.84f, 1.0f };
}

bool TitleSprite::Start()
{
	// �^�C�g�����S�p�X�v���C�g���������B
	m_titleLogoSprite.Init("Assets/sprite/title/logo/titleLogo.DDS",
		LOGO_SPRITE_SIZE.x,
		LOGO_SPRITE_SIZE.y
	);
	// ���W�Ɗg�嗦��ݒ�B
	m_titleLogoSprite.SetPosition(LOGO_SPRITE_POS);
	m_titleLogoSprite.SetScale(LOGO_SPRITE_SCA);

	// �e�L�X�g�̃X�v���C�g���������B
	m_textSprite.Init("Assets/sprite/title/text/text.DDS",
		TEXT_SPRITE_SIZE.x,
		TEXT_SPRITE_SIZE.y
	);
	// ���W�Ɗg�嗦��ݒ�B
	m_textSprite.SetPosition(TEXT_SPRITE_POS);
	m_textSprite.SetScale(TEXT_SPRITE_SCA);

	// �X�v���C�g���X�V�B
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

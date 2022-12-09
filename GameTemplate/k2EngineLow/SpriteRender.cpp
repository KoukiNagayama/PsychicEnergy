#include "k2EngineLowPreCompile.h"
#include "SpriteRender.h"

namespace nsK2EngineLow
{
	void SpriteRender::Init(const char* filePath,
		const float w,
		const float h,
		AlphaBlendMode alphaBlendMode
	)
	{
		SpriteInitData initData;
		// DDS�t�@�C��(�摜�f�[�^)�̃t�@�C���p�X���w�肷��B
		initData.m_ddsFilePath[0] = filePath;
		// Sprite�\���p�̃V�F�[�_�[�̃t�@�C���p�X���w�肷��B
		initData.m_fxFilePath = "Assets/shader/sprite.fx";
		// �X�v���C�g�̕��ƍ������w�肷��B
		initData.m_width = static_cast<UINT>(w);
		initData.m_height = static_cast<UINT>(h);
		// �g�p����A���t�@�u�����f�B���O���[�h���w�肷��B
		initData.m_alphaBlendMode = alphaBlendMode;

		m_sprite.Init(initData);
	}

	void SpriteRender::InitSpecialImage(SpriteInitData spriteInitData)
	{
		m_sprite.Init(spriteInitData);
	}

	void SpriteRender::Update()
	{
		m_sprite.Update(
			m_position,
			m_rotation,
			m_scale,
			m_pivot
		);
	}

	void SpriteRender::Draw(RenderContext& rc)
	{
		g_renderingEngine->AddSpriteToRenderSpritePass(m_sprite);
	}
}

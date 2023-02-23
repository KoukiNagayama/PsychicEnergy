#include "stdafx.h"
#include "GravityGauge.h"
#include "Player.h"

namespace
{
	const Vector3 SPRITE_POS = { -740.0f, 330.0f, 0.0f };	// スプライトの座標
	const Vector3 SPRITE_SCALE = { 0.59f, 0.59f, 1.0f };	// スプライトの拡大率
	const float WIDTH = 512.0f;								// 横幅
	const float HEIGHT = 512.0f;							// 縦幅
	const float DECREASE_PER_FRAME = 0.2f;					// フレームごとの減少量
	const float INCREASE_PER_FRAME = 5.0f;					// フレームごとの増加量
	const float MAX_ANGLE_DEG_TO_DISPLAY = 360.0f;			// 表示する最大角度(Degree)
	const float MIN_ANGLE_DEG_TO_DISPLAY = 0.0f;			// 表示する最小角度(Degree)
}

namespace nsPsychicEnergy
{
	namespace nsGravityGauge
	{
		bool GravityGauge::Start()
		{
			// スプライトの初期化データ
			SpriteInitData spriteInitData;
			spriteInitData.m_ddsFilePath[0] = "Assets/sprite/gravityGauge/gauge2.DDS";
			spriteInitData.m_fxFilePath = "Assets/shader/gravityGauge.fx";
			spriteInitData.m_width = WIDTH;
			spriteInitData.m_height = HEIGHT;
			spriteInitData.m_expandConstantBuffer = &m_displayAreaRad;
			spriteInitData.m_expandConstantBufferSize = sizeof(m_displayAreaRad);
			spriteInitData.m_alphaBlendMode = AlphaBlendMode_Trans;
			// 初期化データをもとにスプライトを初期化する。
			m_gaugeSprite.InitSpecialImage(spriteInitData);
			m_gaugeSprite.SetPosition(SPRITE_POS);
			m_gaugeSprite.SetScale(SPRITE_SCALE);

			m_gaugeFrameSprite.Init(
				"Assets/sprite/gravityGauge/frame.DDS",
				WIDTH,
				HEIGHT
			);
			m_gaugeFrameSprite.SetPosition(SPRITE_POS);
			m_gaugeFrameSprite.SetScale(SPRITE_SCALE);

			m_player = FindGO<nsPlayer::Player>("player");

			return true;
		}

		void GravityGauge::Update()
		{
			CalcDisplayArea(m_player->IsFloating());

			m_gaugeSprite.Update();
			m_gaugeFrameSprite.Update();
		}

		void GravityGauge::CalcDisplayArea(bool isFloating)
		{
			// 表示する角度。Degree単位
			float displayAreaDeg = Math::RadToDeg(m_displayAreaRad);
			if (isFloating) {
				displayAreaDeg -= DECREASE_PER_FRAME;
				if (displayAreaDeg < MIN_ANGLE_DEG_TO_DISPLAY) {
					displayAreaDeg = MIN_ANGLE_DEG_TO_DISPLAY;
				}
			}
			else {
				displayAreaDeg += INCREASE_PER_FRAME;
				if (displayAreaDeg >= MAX_ANGLE_DEG_TO_DISPLAY) {
					displayAreaDeg = MAX_ANGLE_DEG_TO_DISPLAY;
				}
			}
			// 表示角度をRadian単位に変換する。
			m_displayAreaRad = Math::DegToRad(displayAreaDeg);
		}

		void GravityGauge::Render(RenderContext& rc)
		{
			m_gaugeFrameSprite.Draw(rc);
			m_gaugeSprite.Draw(rc);
		}

	}
}


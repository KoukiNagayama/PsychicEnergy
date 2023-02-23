#include "stdafx.h"
#include "ResultSprite.h"
#include "Result.h"
#include "CommonDataForTimer.h"

namespace
{
	const Vector2 SPRITE_SIZE = { 512.0f, 512.0f };
	const Vector3 NUMBER_SPRITE_SCALE = { 0.8f, 0.8f, 1.0f };
	const Vector3 FIRST_DIGIT_POS = { 215.0f, -13.0f, 0.0f };
	const Vector3 VALUE_OF_POS_SHIFT = { -215.0f, 0.0f, 0.0f };
	const Vector3 TEXT_SPRITE_SCALE = { 2.0f, 2.0f, 1.0f };
	const Vector3 TEXT_SPRITE_POS = { 0.0f, -13.0f, 0.0f };
}

namespace nsPsychicEnergy
{

	namespace nsResult
	{
		bool ResultSprite::Start()
		{
			m_result = FindGO<Result>("result");

			InitSprite();


			return true;
		}

		void ResultSprite::InitSprite()
		{
			// クリアできたか
			auto& resultState = m_result->GetResultState();

			if (resultState == Result::enResultState_GameClear) {
				// 記録された時間
				int recordedTime = m_result->GetRecordedTime();
				// 現在求めようとしている桁
				int calculationDigit = 1;
				// 各桁の表示座標
				Vector3 numberPos = FIRST_DIGIT_POS;

				// 数字用スプライトを初期化
				for (int i = nsTimer::NUMBER_OF_DIGIT - 1; i >= 0; i--) {
					int num = (recordedTime / calculationDigit) % 10;
					calculationDigit *= 10;

					// 求めた数値の画像のファイルパスの文字列を作成
					std::string filePath;
					filePath = "Assets/sprite/number/";
					filePath += std::to_string(num);
					filePath += ".DDS";

					// スプライトの初期化
					m_numberSprite[i].Init(filePath.c_str(), SPRITE_SIZE.x, SPRITE_SIZE.y);
					m_numberSprite[i].SetPosition(numberPos);
					m_numberSprite[i].SetScale(NUMBER_SPRITE_SCALE);
					m_numberSprite[i].Update();
					numberPos += VALUE_OF_POS_SHIFT;
				}
			}
			else {
				m_isTimeUp = true;
				// テキスト用スプライトを初期化
				m_textSprite.Init("Assets/sprite/result/timeUp.DDS",
					SPRITE_SIZE.x,
					SPRITE_SIZE.y
				);
				m_textSprite.SetPosition(TEXT_SPRITE_POS);
				m_textSprite.SetScale(TEXT_SPRITE_SCALE);
				m_textSprite.Update();
			}
		}

		void ResultSprite::Render(RenderContext& rc)
		{
			// 描画。
			if (m_isTimeUp) {
				m_textSprite.Draw(rc);
			}
			else {
				for (int i = 0; i < nsTimer::NUMBER_OF_DIGIT; i++) {
					m_numberSprite[i].Draw(rc);
				}
			}
		}

	}
}

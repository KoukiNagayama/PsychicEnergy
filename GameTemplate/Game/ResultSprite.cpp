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
			// �N���A�ł�����
			auto& resultState = m_result->GetResultState();

			if (resultState == Result::enResultState_GameClear) {
				// �L�^���ꂽ����
				int recordedTime = m_result->GetRecordedTime();
				// ���݋��߂悤�Ƃ��Ă��錅
				int calculationDigit = 1;
				// �e���̕\�����W
				Vector3 numberPos = FIRST_DIGIT_POS;

				// �����p�X�v���C�g��������
				for (int i = nsTimer::NUMBER_OF_DIGIT - 1; i >= 0; i--) {
					int num = (recordedTime / calculationDigit) % 10;
					calculationDigit *= 10;

					// ���߂����l�̉摜�̃t�@�C���p�X�̕�������쐬
					std::string filePath;
					filePath = "Assets/sprite/number/";
					filePath += std::to_string(num);
					filePath += ".DDS";

					// �X�v���C�g�̏�����
					m_numberSprite[i].Init(filePath.c_str(), SPRITE_SIZE.x, SPRITE_SIZE.y);
					m_numberSprite[i].SetPosition(numberPos);
					m_numberSprite[i].SetScale(NUMBER_SPRITE_SCALE);
					m_numberSprite[i].Update();
					numberPos += VALUE_OF_POS_SHIFT;
				}
			}
			else {
				m_isTimeUp = true;
				// �e�L�X�g�p�X�v���C�g��������
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
			// �`��B
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

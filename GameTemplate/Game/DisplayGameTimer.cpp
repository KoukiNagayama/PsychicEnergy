#include "stdafx.h"
#include "DisplayGameTimer.h"

namespace
{
	const int NUMBER_OF_DIGIT = 3;								// ����	
	const Vector2 NUMBER_SPRITE_SIZE = { 50.0f,80.0f };		// �X�v���C�g�̃T�C�Y
	const float MAX_VALUE_OF_TIMER = 1000.0f;					// �^�C�}�[�̍ő�l
	const Vector3 HUNDREDS_DIGIT_POS = { -60.0f, 430.0f, 0.0f };	// 100�̈ʂ̍��W
	const Vector3 VALUE_OF_POS_SHIFT = { 40.0f, 0.0f, 0.0f };	// ���W�����炷�l
}

bool DisplayGameTimer::Start()
{
	// �e���̕\���ʒu���m�肳����
	Vector3 numberPos = HUNDREDS_DIGIT_POS;
	for (int i = 0; i < NUMBER_OF_DIGIT; i++) {
		m_numberSprite[i].SetPosition(numberPos);
		m_numberSprite[i].Update();
		numberPos += VALUE_OF_POS_SHIFT;
	}
	return true;
}

void DisplayGameTimer::Update()
{
	// ���݂̎��Ԃ��v�Z
	m_currentTime += g_gameTime->GetFrameDeltaTime();
	if (m_currentTime >= MAX_VALUE_OF_TIMER) {
		return;
	}
	// �e���̐��l���v�Z����
	CalculateNumOfEachDigit();
}

void DisplayGameTimer::CalculateNumOfEachDigit()
{
	// ���݂̎���
	int currentTime = (int)m_currentTime;
	
	// �X�v���C�g�̏����������𖈃t���[�����Ə������d���ׁA
	// 1�b��1�񂾂��s�킹��悤�ɂ���
	if (m_lastSecond == currentTime) {
		return;
	}
	// ���݂̎���(�b)���L�^
	m_lastSecond = currentTime;

	// ���݋��߂悤�Ƃ��Ă��錅
	int calculationDigit = 1;

	for (int i = NUMBER_OF_DIGIT - 1; i >= 0; i--) {
		int num = (currentTime / calculationDigit) % 10;
		calculationDigit *= 10;

		// ���߂����l�̉摜�̃t�@�C���p�X�̕�������쐬
		std::string filePath;
		filePath = "Assets/sprite/number/";
		filePath += std::to_string(num);
		filePath += ".DDS";

		// �X�v���C�g�̏�����
		m_numberSprite[i].Init(filePath.c_str(),
			NUMBER_SPRITE_SIZE.x,
			NUMBER_SPRITE_SIZE.y
		);
	}
}

void DisplayGameTimer::Render(RenderContext& rc)
{
	for (int i = 0; i < NUMBER_OF_DIGIT; i++) {
		m_numberSprite[i].Draw(rc);
	}
}

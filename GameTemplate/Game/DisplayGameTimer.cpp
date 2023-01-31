#include "stdafx.h"
#include "DisplayGameTimer.h"

namespace
{
	const int NUMBER_OF_DIGIT = 3;									// ����	
	const Vector2 NUMBER_SPRITE_SIZE = { 512.0f,512.0f };			// �X�v���C�g�̃T�C�Y
	const Vector3 NUMBER_SPRITE_SCALE = { 0.3f, 0.3f, 1.0f };
	const float MAX_VALUE_OF_TIMER = 1000.0f;						// �^�C�}�[�̍ő�l
	const Vector3 HUNDREDS_DIGIT_POS = { 650.0f, 373.0f, 0.0f };	// 100�̈ʂ̍��W
	const Vector3 VALUE_OF_POS_SHIFT = { 50.0f, 0.0f, 0.0f };		// ���W�����炷�l
	const Vector3 TIMER_BG_SPRITE_POS = { 700.0f, 360.0f, 0.0f };
	const Vector3 TIMER_BG_SPRITE_SCA = { 0.9f, 0.9f, 1.0f };
	const Vector4 TIMER_BG_SPRITE_MUL_COLOR = { 1.0f, 1.0f, 1.0f, 0.8f };

}

bool DisplayGameTimer::Start()
{
	// �e���̕\���ʒu���m�肳����
	Vector3 numberPos = HUNDREDS_DIGIT_POS;
	for (int i = 0; i < NUMBER_OF_DIGIT; i++) {
		m_numberSprite[i].SetPosition(numberPos);
		m_numberSprite[i].SetScale(NUMBER_SPRITE_SCALE);
		m_numberSprite[i].Update();
		numberPos += VALUE_OF_POS_SHIFT;
	}
	
	// �^�C�}�[�̔w�i�̃X�v���C�g��������
	m_timerBGSprite.Init("Assets/sprite/timerBG/timerBG.DDS", NUMBER_SPRITE_SIZE.x, NUMBER_SPRITE_SIZE.y);
	m_timerBGSprite.SetPosition(TIMER_BG_SPRITE_POS);
	m_timerBGSprite.SetScale(TIMER_BG_SPRITE_SCA);
	m_timerBGSprite.SetMulColor(TIMER_BG_SPRITE_MUL_COLOR);
	m_timerBGSprite.Update();

	// �^�C�}�[�̔w�i�̘g�̃X�v���C�g��������
	// �w�i�̃X�v���C�g�̓��l��ύX���邽�߃X�v���C�g�𕪊�
	m_timerBGFrameSprite.Init("Assets/sprite/timerBG/timerBGFrame.DDS", NUMBER_SPRITE_SIZE.x, NUMBER_SPRITE_SIZE.y);
	m_timerBGFrameSprite.SetPosition(TIMER_BG_SPRITE_POS);
	m_timerBGFrameSprite.SetScale(TIMER_BG_SPRITE_SCA);
	m_timerBGFrameSprite.Update();
	return true;
}

void DisplayGameTimer::Update()
{
	if (m_disable) {
		// ����������Ă���ꍇ�͍X�V���Ȃ�
		return;
	}
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
	
	// �X�v���C�g�̏����������𖈃t���[�����K�v�����Ȃ��������d���ׁA
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
		m_numberSprite[i].Init(filePath.c_str(), NUMBER_SPRITE_SIZE.x, NUMBER_SPRITE_SIZE.y);
	}
}

void DisplayGameTimer::Render(RenderContext& rc)
{
	if (m_disable) {
		// ����������Ă���ꍇ�͕`�悵�Ȃ�
		return;
	}
	// �e��X�v���C�g��`��
	m_timerBGSprite.Draw(rc);
	m_timerBGFrameSprite.Draw(rc);
	for (int i = 0; i < NUMBER_OF_DIGIT; i++) {
		m_numberSprite[i].Draw(rc);
	}
}

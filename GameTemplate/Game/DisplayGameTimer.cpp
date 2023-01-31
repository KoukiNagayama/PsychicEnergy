#include "stdafx.h"
#include "DisplayGameTimer.h"

namespace
{
	const int NUMBER_OF_DIGIT = 3;								// 桁数	
	const Vector2 NUMBER_SPRITE_SIZE = { 50.0f,80.0f };		// スプライトのサイズ
	const float MAX_VALUE_OF_TIMER = 1000.0f;					// タイマーの最大値
	const Vector3 HUNDREDS_DIGIT_POS = { -60.0f, 430.0f, 0.0f };	// 100の位の座標
	const Vector3 VALUE_OF_POS_SHIFT = { 40.0f, 0.0f, 0.0f };	// 座標をずらす値
}

bool DisplayGameTimer::Start()
{
	// 各桁の表示位置を確定させる
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
	// 現在の時間を計算
	m_currentTime += g_gameTime->GetFrameDeltaTime();
	if (m_currentTime >= MAX_VALUE_OF_TIMER) {
		return;
	}
	// 各桁の数値を計算する
	CalculateNumOfEachDigit();
}

void DisplayGameTimer::CalculateNumOfEachDigit()
{
	// 現在の時間
	int currentTime = (int)m_currentTime;
	
	// スプライトの初期化処理を毎フレームやると処理が重い為、
	// 1秒に1回だけ行わせるようにする
	if (m_lastSecond == currentTime) {
		return;
	}
	// 現在の時間(秒)を記録
	m_lastSecond = currentTime;

	// 現在求めようとしている桁
	int calculationDigit = 1;

	for (int i = NUMBER_OF_DIGIT - 1; i >= 0; i--) {
		int num = (currentTime / calculationDigit) % 10;
		calculationDigit *= 10;

		// 求めた数値の画像のファイルパスの文字列を作成
		std::string filePath;
		filePath = "Assets/sprite/number/";
		filePath += std::to_string(num);
		filePath += ".DDS";

		// スプライトの初期化
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

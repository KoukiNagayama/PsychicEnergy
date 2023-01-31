#include "stdafx.h"
#include "DisplayGameTimer.h"

namespace
{
	const int NUMBER_OF_DIGIT = 3;									// 桁数	
	const Vector2 NUMBER_SPRITE_SIZE = { 512.0f,512.0f };			// スプライトのサイズ
	const Vector3 NUMBER_SPRITE_SCALE = { 0.3f, 0.3f, 1.0f };
	const float MAX_VALUE_OF_TIMER = 1000.0f;						// タイマーの最大値
	const Vector3 HUNDREDS_DIGIT_POS = { 650.0f, 373.0f, 0.0f };	// 100の位の座標
	const Vector3 VALUE_OF_POS_SHIFT = { 50.0f, 0.0f, 0.0f };		// 座標をずらす値
	const Vector3 TIMER_BG_SPRITE_POS = { 700.0f, 360.0f, 0.0f };
	const Vector3 TIMER_BG_SPRITE_SCA = { 0.9f, 0.9f, 1.0f };
	const Vector4 TIMER_BG_SPRITE_MUL_COLOR = { 1.0f, 1.0f, 1.0f, 0.8f };

}

bool DisplayGameTimer::Start()
{
	// 各桁の表示位置を確定させる
	Vector3 numberPos = HUNDREDS_DIGIT_POS;
	for (int i = 0; i < NUMBER_OF_DIGIT; i++) {
		m_numberSprite[i].SetPosition(numberPos);
		m_numberSprite[i].SetScale(NUMBER_SPRITE_SCALE);
		m_numberSprite[i].Update();
		numberPos += VALUE_OF_POS_SHIFT;
	}
	
	// タイマーの背景のスプライトを初期化
	m_timerBGSprite.Init("Assets/sprite/timerBG/timerBG.DDS", NUMBER_SPRITE_SIZE.x, NUMBER_SPRITE_SIZE.y);
	m_timerBGSprite.SetPosition(TIMER_BG_SPRITE_POS);
	m_timerBGSprite.SetScale(TIMER_BG_SPRITE_SCA);
	m_timerBGSprite.SetMulColor(TIMER_BG_SPRITE_MUL_COLOR);
	m_timerBGSprite.Update();

	// タイマーの背景の枠のスプライトを初期化
	// 背景のスプライトはα値を変更するためスプライトを分割
	m_timerBGFrameSprite.Init("Assets/sprite/timerBG/timerBGFrame.DDS", NUMBER_SPRITE_SIZE.x, NUMBER_SPRITE_SIZE.y);
	m_timerBGFrameSprite.SetPosition(TIMER_BG_SPRITE_POS);
	m_timerBGFrameSprite.SetScale(TIMER_BG_SPRITE_SCA);
	m_timerBGFrameSprite.Update();
	return true;
}

void DisplayGameTimer::Update()
{
	if (m_disable) {
		// 無効化されている場合は更新しない
		return;
	}
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
	
	// スプライトの初期化処理を毎フレームやる必要性がなく処理も重い為、
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
		m_numberSprite[i].Init(filePath.c_str(), NUMBER_SPRITE_SIZE.x, NUMBER_SPRITE_SIZE.y);
	}
}

void DisplayGameTimer::Render(RenderContext& rc)
{
	if (m_disable) {
		// 無効化されている場合は描画しない
		return;
	}
	// 各種スプライトを描画
	m_timerBGSprite.Draw(rc);
	m_timerBGFrameSprite.Draw(rc);
	for (int i = 0; i < NUMBER_OF_DIGIT; i++) {
		m_numberSprite[i].Draw(rc);
	}
}

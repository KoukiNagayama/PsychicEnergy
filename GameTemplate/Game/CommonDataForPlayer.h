#pragma once
/// <summary>
/// PsychicEnergyの名前空間。
/// </summary>
namespace nsPsychicEnergy
{

	/// <summary>
	/// プレイヤーの名前空間。
	/// </summary>
	namespace nsPlayer
	{

		// 定数
		const float NORMAL_ANIMATION_SPEED = 1.0f;			// 通常のアニメーションスピード
		const float MIN_INPUT_QUANTITY = 0.001f;			// 入力量の最低値
		const float MIN_REMAINING_GAUGE = 0.00f;			// ゲージの最低残量
		const float	TIME_TO_REGENERATE_EFFECT = 0.8f;		// エフェクトを再生成する時間
		const float	RESET_REGENERATE_EFFECT_TIMER = 0.0f;	// タイマーのリセット
	}
}

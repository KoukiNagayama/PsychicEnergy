#pragma once

/// <summary>
/// 音の関する共通データ
/// </summary>
namespace nsSound
{
	/// <summary>
	/// 音の登録番号
	/// </summary>
	enum EnSoundNumber
	{
		enSoundNumber_PlayerFootStep,		// プレイヤーの足音
		enSoundNumber_PlayerSliding,		// プレイヤーのスライディング音
		enSoundNumber_PlayerLanding,		// プレイヤーの着地音
		enSoundNumber_PlayerModeChange,		// プレイヤーのモードチェンジ音
		enSoundNumber_TitleBGM,				// タイトルで流れるBGM
		enSoundNumber_MainBGM,				// インゲーム中に流れるBGM
		enSoundNumber_Decision,				// 決定音
		enSoundNumber_Acquisition,			// リングの取得音
		enSoundNumber_Wind,					// 風の音
		enSoundNumber_ResultBGM,			// リザルトで流れるBGM
	};

	const float INITIAL_VOLUME = 0.0f;		// 最初の音量
	const float FINAL_VOLUME = 0.0f;		// 最後の音量	
	const float BGM_MIN_VOLUME = 0.0f;		// BGMの最小音量
	const float BGM_MAX_VOLUME = 0.4f;		// BGMの最大音量
	const float DECISION_VOLUME = 0.5f;		// 決定音の音量
}


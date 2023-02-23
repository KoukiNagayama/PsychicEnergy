#pragma once
#include "sound/SoundSource.h"

/// <summary>
/// PsychicEnergyの名前空間。
/// </summary>
namespace nsPsychicEnergy
{
	/// <summary>
	/// リザルトに関する名前空間。
	/// </summary>
	namespace nsResult
	{
		/// <summary>
		/// リザルト時に流れるBGM
		/// </summary>
		class ResultBGM : public IGameObject
		{
		public:
			/// <summary>
			/// コンストラクタ。
			/// </summary>
			ResultBGM() {}
			/// <summary>
			/// デストラクタ。
			/// </summary>
			~ResultBGM() {}
			/// <summary>
			/// 開始処理。
			/// インスタンス生成時に初期化を行う。trueを返したときに呼ばれなくなる。
			/// </summary>
			bool Start();
			/// <summary>
			/// 更新処理。
			/// </summary>
			void Update();
			/// <summary>
			/// フェードアウトを開始する。
			/// </summary>
			void StartFadeOut()
			{
				m_isStartFadeOut = true;
			}
		private:
			SoundSource*	m_resultBGM = nullptr;			// リザルト時に流れるBGMの音源
			float			m_volume = 0.0f;				// 音量。
			bool			m_isStartFadeOut = false;		// フェードアウトを開始するか
		};
	}
}



#pragma once
#include "sound/SoundSource.h"

/// <summary>
/// PsychicEnergyの名前空間。
/// </summary>
namespace nsPsychicEnergy
{

	/// <summary>
	/// サウンドに関する名前空間。
	/// </summary>
	namespace nsSound
	{
		/// <summary>
		/// メインBGMクラス。
		/// </summary>
		class MainBGM : public IGameObject
		{
		public:
			/// <summary>
			/// コンストラクタ。
			/// </summary>
			MainBGM() {}
			/// <summary>
			/// デストラクタ。
			/// </summary>
			~MainBGM() {}
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
			SoundSource*	m_mainBGM = nullptr;			// 音源クラス。
			float			m_volume = 0.0f;				// 音量。
			bool			m_isStartFadeOut = false;		// フェードアウトが開始するか？
		};

	}
}


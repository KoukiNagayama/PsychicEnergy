#pragma once
#include "sound/SoundSource.h"

/// <summary>
/// PsychicEnergyの名前空間。
/// </summary>
namespace nsPsychicEnergy
{

	/// <summary>
	/// タイトルに関する名前空間。
	/// </summary>
	namespace nsTitle
	{
		/// <summary>
		/// タイトルで流れるBGMクラス。
		/// </summary>
		class TitleBGM : public IGameObject
		{
		public:
			/// <summary>
			/// コンストラクタ。
			/// </summary>
			TitleBGM() {}
			/// <summary>
			/// デストラクタ。
			/// </summary>
			~TitleBGM() {}
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
			SoundSource*		m_titleBGM = nullptr;			// タイトルで流れるBGMの音源
			float				m_volume = 0.0f;				// 音量
			bool				m_isStartFadeOut = false;		// フェードアウトをするか？
		};
	}
}



#pragma once

/// <summary>
/// PsychicEnergyの名前空間。
/// </summary>
namespace nsPsychicEnergy
{
	/// <summary>
	/// タイマーに関する名前空間。
	/// </summary>
	namespace nsTimer
	{
		/// <summary>
		/// タイムアタック用タイマークラス
		/// </summary>
		class DisplayGameTimer : public IGameObject
		{
		public:
			/// <summary>
			/// コンストラクタ。
			/// </summary>
			DisplayGameTimer() {}
			/// <summary>
			/// デストラクタ。
			/// </summary>
			~DisplayGameTimer() {}
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
			/// 描画処理。
			/// </summary>
			/// <param name="rc"></param>
			void Render(RenderContext& rc);
			/// <summary>
			/// 時間を取得。
			/// </summary>
			/// <returns>現在の時間。</returns>
			int GetTime() const
			{
				return (int)m_currentTime;
			}
			/// <summary>
			/// 描画と更新を無効化する
			/// </summary>
			void Disable()
			{
				m_disable = true;
			}
		private:
			/// <summary>
			/// 各桁の数値を計算する
			/// </summary>
			void CalculateNumOfEachDigit();
		private:
			SpriteRender	m_numberSprite[3];				// 数字を表示するスプライト
			SpriteRender	m_timerBGSprite;				// タイマーの背景のスプライト
			SpriteRender	m_timerBGFrameSprite;			// タイマーの背景の縁のスプライト
			float			m_currentTime = 0.0f;			// 現在の時間
			int	  			m_lastSecond = -1;				// 直前の時間(秒)
			bool			m_disable = false;				// 機能の無効化
		};

	}
}


#pragma once
/// <summary>
/// PsychicEnergyの名前空間。
/// </summary>
namespace nsPsychicEnergy
{
	// 前方宣言。
	namespace nsFade { class Fade; }
	namespace nsTimer { class DisplayGameTimer; }

	/// <summary>
	/// リザルトに関する名前空間。
	/// </summary>
	namespace nsResult
	{
		// 前方宣言。
		class ResultBGM;
		class ResultSprite;

		/// <summary>
		/// リザルトクラス
		/// </summary>
		class Result : public IGameObject
		{
		public:
			/// <summary>
			/// リザルトの状態。
			/// </summary>
			enum EnResultState
			{
				enResultState_GameClear,	// ゲームクリア。
				enResultState_TimeUp		// タイムアップ。
			};
		public:
			/// <summary>
			/// コンストラクタ。
			/// </summary>
			Result() {}
			/// <summary>
			/// デストラクタ。
			/// </summary>
			~Result();
			/// <summary>
			/// 開始処理。
			/// インスタンス生成時に初期化を行う。trueを返したときに呼ばれなくなる。
			/// </summary>
			bool Start();
			/// <summary>
			/// 更新処理
			/// </summary>
			void Update();
			/// <summary>
			/// 描画処理
			/// </summary>
			/// <param name="rc"></param>
			void Render(RenderContext& rc);
			/// <summary>
			/// リザルトのステートを取得。
			/// </summary>
			EnResultState& GetResultState()
			{
				return m_resultState;
			}
			/// <summary>
			/// 記録された時間を取得。
			/// </summary>
			/// <returns>プレイ終了までにかかった時間。</returns>
			int& GetRecordedTime()
			{
				return m_recordedTime;
			}
		private:
			void FadeOut();
		private:

			EnResultState				m_resultState;					// リザルトのステート
			SpriteRender				m_resultBGSprite;				// リザルトの背景のスプライト
			int							m_recordedTime = -1;			// 記録された時間
			//float				m_pushDeltaTime = 10000.0f;	// 
			bool						m_disable = true;				// 無効化。
			nsFade::Fade*				m_fade = nullptr;				// フェード
			bool						m_isWaitFadeout = false;		// フェードアウトを待機しているか
			SoundSource*				m_decisionSound = nullptr;		// 決定音。		
			nsTimer::DisplayGameTimer*	m_displayGameTimer = nullptr;	// タイマー
			ResultBGM*					m_resultBGM = nullptr;			// リザルト時に流れるBGM
			ResultSprite*				m_resultSprite = nullptr;		// リザルトで使用するスプライト
		};

	}
}


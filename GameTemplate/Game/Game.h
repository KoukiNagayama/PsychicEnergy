#pragma once
#include "LevelRender.h"
#include "SkyCube.h"
#include "WorldRotation.h"

/// <summary>
/// PsychicEnergyの名前空間
/// </summary>
namespace nsPsychicEnergy 
{
	// 前方宣言。
	namespace nsPlayer { class Player; }
	namespace nsFade { class Fade; }
	namespace nsBackGround { class BackGround; }
	namespace nsRing { class Ring; }
	namespace nsGameCamera { class GameCamera; }
	namespace nsGravityGauge { class GravityGauge; }
	namespace nsSight { class Sight; }
	namespace nsLockOn { class LockOn; }
	namespace nsTimer { class DisplayGameTimer; }
	namespace nsSound { class MainBGM; }

	/// <summary>
	/// ゲームの名前空間
	/// </summary>
	namespace nsGame 
	{

		/// <summary>
		/// インゲームクラス
		/// </summary>
		class Game : public IGameObject
		{
		public:
			/// <summary>
			/// コンストラクタ
			/// </summary>
			Game();
			/// <summary>
			/// デストラクタ
			/// </summary>
			~Game();
			/// <summary>
			/// 開始処理。
			/// インスタンス生成時に一度だけ実行。
			/// </summary>
			bool Start();
			/// <summary>
			/// 更新処理。
			/// </summary>
			void Update();
			/// <summary>
			/// リングを取得
			/// </summary>
			void GetRing()
			{
				m_numOfGetRing++;
			}
			/// <summary>
			/// インゲームは終了しているか
			/// </summary>
			bool& IsFinishedInGame()
			{
				return m_isFinishedInGame;
			}
		private:
			/// <summary>
			/// フェードアウト。
			/// </summary>
			void FadeOut();
		private:
			LevelRender									m_levelRender;					// レベルレンダー
			nsPlayer::Player*							m_player = nullptr;				// プレイヤー
			nsGameCamera::GameCamera*					m_gameCamera = nullptr;			// ゲームカメラ
			std::vector<nsBackGround::BackGround*>		m_backGroundArray;				// 背景
			SkyCube*									m_skyCube = nullptr;			// スカイキューブ						
			nsGravityGauge::GravityGauge*				m_gravityGauge = nullptr;		// 重力ゲージ
			nsSight::Sight*								m_sight = nullptr;				// 照準
			nsRing::Ring*								m_ring = nullptr;				// リング
			nsLockOn::LockOn*							m_lockOn = nullptr;				// ロックオン
			nsTimer::DisplayGameTimer*					m_displayGameTimer = nullptr;	// 表示するタイマー
			nsFade::Fade*								m_fade = nullptr;				// フェード
			nsSound::MainBGM*							m_mainBGM = nullptr;			// メインBGM
			int											m_maxRing = 0;					// リングの最大数
			int											m_numOfGetRing = 0;				// 取得したリングの数
			bool										m_isFinishedInGame = false;		// インゲームは終了したか？
		};
	}
}



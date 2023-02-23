#pragma once
#include "LevelRender.h"
#include "SkyCube.h"
#include "Fade.h"
#include "sound/SoundSource.h"



/// <summary>
/// PsychicEnergyに関する名前空間。
/// </summary>
namespace nsPsychicEnergy
{
	// 前方宣言。
	namespace nsFade { class Fade; }


	/// <summary>
	/// タイトルに関する名前空間。
	/// </summary>
	namespace nsTitle
	{
		// 前方宣言。
		class TitleBackGround;
		class TitleCharacter;
		class TitleCamera;
		class TitleSprite;
		class TitleBGM;

		/// <summary>
		/// タイトルクラス
		/// </summary>
		class Title : public IGameObject
		{
		public:
			/// <summary>
			/// コンストラクタ。
			/// </summary>
			Title() {}
			///<summary>
			/// デストラクタ
			///</summary>
			~Title();
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
			/// Bボタンは押されたか。
			/// </summary>
			bool& IsPushedB()
			{
				return m_isPushedB;
			}
			/// <summary>
			/// フェードアウトを開始。
			/// </summary>
			void StartFadeOut()
			{
				m_isFadeOut = true;
			}
		private:
			/// <summary>
			/// フェードアウト。
			/// </summary>
			void FadeOut();

		private:
			LevelRender						m_titleLevel;							// タイトルのレベル
			std::vector<TitleBackGround*>	m_titleBackGroundArray;					// タイトルで使用する背景モデル
			TitleCharacter*					m_titleCharacter = nullptr;				// タイトルで使用するキャラクターモデル
			TitleCamera*					m_titleCamera = nullptr;				// タイトルで使用するカメラ
			TitleSprite*					m_titleSprite = nullptr;				// タイトルで使用するスプライト
			SpriteRender					m_titleTextSprite;						// タイトルに使用するテキストのスプライト
			float							m_currentAlpha = 0.0f;					// スプライトの乗算する現在のα値
			SkyCube*						m_skyCube = nullptr;					// スカイキューブ
			nsFade::Fade*					m_fade = nullptr;						// フェード
			bool							m_isWaitFadeout = false;				// フェードアウトを待機しているか
			bool							m_isFadeOut = false;					// フェードアウト中か
			TitleBGM*						m_titleBGM = nullptr;					// タイトルBGMクラス
			SoundSource*					m_decisionSound = nullptr;				// 決定音の音源
			bool							m_isPushedB = false;					// Bボタンは押されたか

		};
	}
}



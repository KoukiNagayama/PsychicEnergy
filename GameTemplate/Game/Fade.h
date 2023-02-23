#pragma once

namespace nsPsychicEnergy
{
	/// <summary>
	/// フェードの名前空間
	/// </summary>
	namespace nsFade
	{

		/// <summary>
		/// フェードクラス
		/// </summary>
		class Fade : public IGameObject
		{
		public:
			/// <summary>
			/// コンストラクタ
			/// </summary>
			Fade();
			/// <summary>
			/// デストラクタ
			/// </summary>
			~Fade();
			/// <summary>
			/// 開始処理。
			/// インスタンス生成時に一度だけ実行。
			/// </summary>
			bool Start() override;
			/// <summary>
			/// 更新処理。
			/// </summary>
			void Update() override;
			/// <summary>
			/// 描画処理。
			/// </summary>
			/// <param name="rc">レンダリングコンテキスト。</param>
			void Render(RenderContext& rc) override;
			/// <summary>
			/// フェードイン。
			/// </summary>
			void StartFadeIn()
			{
				m_state = enFadeState_FadeIn;
			}
			/// <summary>
			/// フェードアウト。
			/// </summary>
			void StartFadeOut()
			{
				m_state = enFadeState_FadeOut;
			}
			/// <summary>
			/// フェード中＞？
			/// </summary>
			/// <returns>フェード中ならtrue。</returns>
			const bool IsFade() const
			{
				return m_state != enFadeState_Idle;
			}
			/// <summary>
			/// α値を取得。
			/// </summary>
			/// <returns>α値。</returns>
			const float GetCurrentAlpha() const
			{
				return m_currentAlpha;
			}
		private:
			/// <summary>
			/// ステート
			/// </summary>
			enum EnFadeState {
				enFadeState_FadeIn,			//フェードイン中。
				enFadeState_FadeOut,		//フェードアウト中。
				enFadeState_Idle,			//アイドル中。
			};

			SpriteRender			m_spriteRender;					//スプライトレンダ―。
			EnFadeState				m_state = enFadeState_Idle;		//状態。
			float					m_currentAlpha = 1.0f;			//現在のα値。ここを変更すると画像が透明になる。
		};
	}
}




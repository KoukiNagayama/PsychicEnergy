#pragma once
/// <summary>
/// PsychicEnergyの名前空間。
/// </summary>
namespace nsPsychicEnergy
{
	// 前方宣言。
	namespace nsPlayer { class Player; }

	/// <summary>
	/// 重力ゲージに関する名前空間。
	/// </summary>
	namespace nsGravityGauge
	{
		/// <summary>
		/// 重力ゲージクラス
		/// </summary>
		class GravityGauge : public IGameObject
		{
		public:
			/// <summary>
			/// コンストラクタ。
			/// </summary>
			GravityGauge() {}
			/// <summary>
			/// デストラクタ。
			/// </summary>
			~GravityGauge() {}
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
			/// <param name="rc">レンダリングコンテキスト</param>
			void Render(RenderContext& rc);
			/// <summary>
			/// Degree単位のゲージの表示範囲の角度を取得
			/// </summary>
			/// <returns></returns>
			float GetDisplayAreaAngleDeg() const
			{
				float deg = Math::RadToDeg(m_displayAreaRad);
				return deg;
			}
		private:
			/// <summary>
			/// 表示する範囲を計算する。
			/// </summary>
			void CalcDisplayArea(bool isFloating);
		private:
			SpriteRender		m_gaugeSprite;						// ゲージのスプライト
			SpriteRender		m_gaugeFrameSprite;					// ゲージの枠のスプライト
			float				m_displayAreaRad = Math::PI2;		// 表示範囲
			nsPlayer::Player*	m_player = nullptr;					// プレイヤークラス
		};
	}
}



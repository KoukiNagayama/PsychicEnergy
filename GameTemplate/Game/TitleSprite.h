#pragma once
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
		/// タイトルで使用するスプライトクラス。
		/// </summary>
		class TitleSprite : public IGameObject
		{
		public:
			/// <summary>
			/// コンストラクタ。
			/// </summary>
			TitleSprite() {}
			/// <summary>
			/// デストラクタ。
			/// </summary>
			~TitleSprite() {}
			/// <summary>
			/// 開始処理。
			/// インスタンス生成時に初期化を行う。trueを返したときに呼ばれなくなる。
			/// </summary>
			bool Start();
			/// <summary>
			/// 描画処理。
			/// </summary>
			/// <param name="rc"></param>
			void Render(RenderContext& rc);
		private:
			SpriteRender			m_titleLogoSprite;			// タイトルロゴのスプライト
			SpriteRender			m_textSprite;				// テキストのスプライト
		};
	}
}



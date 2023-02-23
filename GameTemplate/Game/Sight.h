#pragma once
/// <summary>
/// PsychicEnergyの名前空間
/// </summary>
namespace nsPsychicEnergy
{

	/// <summary>
	/// 照準の名前空間
	/// </summary>
	namespace nsSight
	{
		/// <summary>
		/// 照準クラス
		/// </summary>
		class Sight : public IGameObject
		{
		public:
			/// <summary>
			/// コンストラクタ。
			/// </summary>
			Sight() {}
			/// <summary>
			/// デストラクタ。
			/// </summary>
			~Sight() {}
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
			/// 照準を表示するかどうか設定する。
			/// </summary>
			/// <param name="isDrawSight">照準を表示するか</param>
			void SetIsDrawSight(const bool& isDrawSight)
			{
				m_isDrawSight = isDrawSight;
			}
		private:
			SpriteRender		m_spriteRender;			// スプライトレンダー
			float				m_currentAlpha = 0.0f;	// 現在のα値
			bool				m_isDrawSight = false;	// 照準を表示するか
		};

	}
}



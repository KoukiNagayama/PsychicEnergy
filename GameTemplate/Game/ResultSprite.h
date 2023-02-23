#pragma once
#include "Result.h"

/// <summary>
/// PsychicEnergyの名前空間。
/// </summary>
namespace nsPsychicEnergy
{

	/// <summary>
	/// リザルトに関する名前空間。
	/// </summary>
	namespace nsResult
	{
		// 前方宣言。
		class Result;

		/// <summary>
		/// リザルトで使用するスプライトクラス。
		/// </summary>
		class ResultSprite : public IGameObject
		{
		public:
			/// <summary>
			/// コンストラクタ。
			/// </summary>
			ResultSprite() {}
			/// <summary>
			/// デストラクタ。
			/// </summary>
			~ResultSprite() {}
			/// <summary>
			/// 開始処理。
			/// インスタンス生成時に初期化を行う。trueを返したときに呼ばれなくなる。
			/// </summary>
			bool Start();
			/// <summary>
			/// 描画処理。
			/// </summary>
			/// <param name="rc">レンダリングコンテキスト。</param>
			void Render(RenderContext& rc);
		private:
			/// <summary>
			/// スプライトを初期化
			/// </summary>
			void InitSprite();
		private:
			SpriteRender	m_numberSprite[3];			// 数字のスプライト(3桁分)
			SpriteRender	m_textSprite;				// 表示するテキストのスプライト
			Result*			m_result = nullptr;			// リザルトクラス
			bool			m_isTimeUp = false;			// タイムアップであったか？
		};
	}
}



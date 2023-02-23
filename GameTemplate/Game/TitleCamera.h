#pragma once
class Title;
class TitleCharacter;

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
		// 前方宣言。
		class Title;
		class TitleCharacter;


		/// <summary>
		/// タイトルカメラクラス
		/// </summary>
		class TitleCamera : public IGameObject
		{
		public:
			/// <summary>
			/// コンストラクタ。
			/// </summary>
			TitleCamera() {}
			/// <summary>
			/// デストラクタ。
			/// </summary>
			~TitleCamera() {}
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
			/// フェードアウトを始めるか？
			/// </summary>
			bool IsStartFadeOut()
			{
				return m_isStartFadeOut;
			}
		private:
			Title*				m_title = nullptr;								// タイトルクラス
			TitleCharacter*		m_titleCharacter = nullptr;						// タイトルキャラクタークラス
			Vector3				m_position = Vector3::Zero;						// カメラの座標
			Quaternion			m_rotation = Quaternion::Identity;				// カメラの回転
			Vector3				m_toCameraPos = Vector3::Zero;					// 注視点から視点へのベクトル
			Vector3				m_charaPos = Vector3::Zero;						// キャラクターの座標
			bool				m_isStartFadeOut = false;						// フェードアウトを始めるか
		};

	}
}


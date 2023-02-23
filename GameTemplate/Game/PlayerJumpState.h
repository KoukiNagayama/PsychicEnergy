#pragma once
#include "IPlayerState.h"
/// <summary>
/// PsychicEnergyの名前空間。
/// </summary>
namespace nsPsychicEnergy
{
	// 前方宣言。
	namespace nsGravityGauge { class GravityGauge; }

	/// <summary>
	/// プレイヤーの名前空間。
	/// </summary>
	namespace nsPlayer
	{
		/// <summary>
		/// プレイヤーのジャンプステートクラス。
		/// </summary>
		class PlayerJumpState : public IPlayerState
		{
		public:
			/// <summary>
			/// コンストラクタ。
			/// </summary>
			/// <param name="player">プレイヤーのインスタンス。</param>
			PlayerJumpState(Player* player) :
				IPlayerState(player) {}
			/// <summary>
			/// デストラクタ。
			/// </summary>
			~PlayerJumpState() override;
			/// <summary>
			/// ステート開始時の処理。
			/// </summary>
			void Enter() override;
			/// <summary>
			/// ステートの遷移処理。
			/// </summary>
			/// <returns>遷移するステート</returns>
			IPlayerState* StateChange() override;
			/// <summary>
			/// ステートにおける更新処理。
			/// </summary>
			void Update() override;
		private:
			float							m_timerToStateChange = 0.0f;// ステートを変化させるカウント
			nsGravityGauge::GravityGauge*	m_gravityGauge = nullptr;	// 重力ゲージ
		};

	}
}


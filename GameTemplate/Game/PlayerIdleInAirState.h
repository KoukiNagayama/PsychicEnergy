#pragma once
#include "IPlayerState.h"
/// <summary>
/// PsychicEnergyの名前空間。
/// </summary>
namespace nsPsychicEnergy
{
	// 前方宣言。
	namespace nsSight { class Sight; }
	namespace nsGravityGauge { class GravityGauge; }

	/// <summary>
	/// プレイヤーの名前空間。
	/// </summary>
	namespace nsPlayer
	{
		/// <summary>
		/// プレイヤーの空中での待機ステートクラス
		/// </summary>
		class PlayerIdleInAirState : public IPlayerState
		{
		public:
			/// <summary>
			/// コンストラクタ。
			/// </summary>
			/// <param name="player">プレイヤーのインスタンス。</param>
			PlayerIdleInAirState(Player* player) :
				IPlayerState(player) {}
			/// <summary>
			/// デストラクタ。
			/// </summary>
			~PlayerIdleInAirState();
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
			nsSight::Sight*					m_sight = nullptr;				// 照準
			nsGravityGauge::GravityGauge*	m_gravityGauge = nullptr;		// 重力ゲージ
		};
	}
}



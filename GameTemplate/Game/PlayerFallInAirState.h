#pragma once
#include "IPlayerState.h"
/// <summary>
/// PsychicEnergyの名前空間。
/// </summary>
namespace nsPsychicEnergy
{
	// 前方宣言。
	namespace nsGameCamera { class GameCamera; }
	namespace nsGravityGauge { class GravityGauge; }

	/// <summary>
	/// プレイヤーの名前空間。
	/// </summary>
	namespace nsPlayer
	{

		/// <summary>
		/// プレイヤーが浮かんでいる状態での落下ステート
		/// </summary>
		class PlayerFallInAirState : public IPlayerState
		{
		public:
			/// <summary>
			/// コンストラクタ。
			/// </summary>
			/// <param name="player">プレイヤーのインスタンス。</param>
			PlayerFallInAirState(Player* player) :
				IPlayerState(player) {}
			/// <summary>
			/// デストラクタ。
			/// </summary>
			~PlayerFallInAirState() override;
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
			float							m_secToRegenerateWindEffect = 0.0f;		// 風のエフェクトを再生成するまでの時間。
			nsGravityGauge::GravityGauge*	m_gravityGauge = nullptr;				// 重力ゲージ
			nsGameCamera::GameCamera*		m_gameCamera = nullptr;					// ゲームカメラ
		};

	}
}


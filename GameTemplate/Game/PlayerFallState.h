#pragma once
#include "IPlayerState.h"
#include "sound/SoundSource.h"
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
		// 前方宣言。
		class Player;

		/// <summary>
		/// プレイヤーの落下ステートクラス
		/// </summary>
		class PlayerFallState : public IPlayerState
		{
		public:
			/// <summary>
			/// コンストラクタ。
			/// </summary>
			/// <param name="player">プレイヤーのインスタンス。</param>
			PlayerFallState(Player* player) :
				IPlayerState(player) {}
			/// <summary>
			/// デストラクタ。
			/// </summary>
			~PlayerFallState() override;
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
			SoundSource* m_landingSound = nullptr;		// 着地音の音源。
		};
	}
}



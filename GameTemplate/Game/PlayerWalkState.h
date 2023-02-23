#pragma once
#include "IPlayerState.h"
/// <summary>
/// PsychicEnergyの名前空間。
/// </summary>
namespace nsPsychicEnergy
{
	/// <summary>
	/// プレイヤーの名前空間。
	/// </summary>
	namespace nsPlayer
	{
		/// <summary>
		/// プレイヤーの地上での歩行ステートクラス。
		/// </summary>
		class PlayerWalkState : public IPlayerState
		{
		public:
			/// <summary>
			/// コンストラクタ。
			/// </summary>
			/// <param name="player">プレイヤーのインスタンス。</param>
			PlayerWalkState(Player* player) :
				IPlayerState(player) {}
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
		};
	}
}



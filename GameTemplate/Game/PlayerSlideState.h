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
		/// プレイヤーのスライディングステートクラス。
		/// </summary>
		class PlayerSlideState : public IPlayerState
		{
		public:
			/// <summary>
			/// コンストラクタ。
			/// </summary>
			/// <param name="player">プレイヤーのインスタンス。</param>
			PlayerSlideState(Player* player) :
				IPlayerState(player) {}
			/// <summary>
			/// デストラクタ。
			/// </summary>
			~PlayerSlideState() override;
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
			float m_secToRegenerateWindEffect = 0.0f;
		};
	}
}



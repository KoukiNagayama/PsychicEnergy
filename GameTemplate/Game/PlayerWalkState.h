#pragma once
#include "PlayerState.h"

/// <summary>
/// プレイヤーの地上での歩行ステートクラス
/// </summary>
class PlayerWalkState : public PlayerState
{
public:
	/// <summary>
	/// ステート開始時の処理。
	/// </summary>
	void Enter(Player* player) override;
	/// <summary>
	/// ステートの遷移処理。
	/// </summary>
	/// <returns>遷移するステート</returns>
	PlayerState* HandleInput(Player* player) override;
	/// <summary>
	/// ステートにおける更新処理。
	/// </summary>
	void Update(Player* player) override;
};


#pragma once
#include "IPlayerState.h"
/// <summary>
/// プレイヤーの地上での歩行ステートクラス
/// </summary>
class PlayerWalkState : public IPlayerState
{
public:
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


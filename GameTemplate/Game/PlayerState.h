#pragma once
#include "Player.h"
class Player;

/// <summary>
/// プレイヤーのステート基底クラス
/// </summary>
class PlayerState
{
public:
	/// <summary>
	/// デスストラクタ
	/// </summary>
	virtual ~PlayerState() {}
	/// <summary>
	/// ステート開始時の処理。
	/// </summary>
	virtual void Enter(Player* player) = 0;
	/// <summary>
	/// ステートの遷移処理。
	/// </summary>
	/// <returns>遷移するステート</returns>
	virtual PlayerState* StateChange(Player* player) = 0;
	/// <summary>
	/// 各ステートにおける更新処理。
	/// </summary>
	virtual void Update(Player* player) = 0;
};


#pragma once
#include "IPlayerState.h"

class Sight;
/// <summary>
/// プレイヤーの空中での待機ステートクラス
/// </summary>
class PlayerIdleInAirState : public IPlayerState
{
public:
	PlayerIdleInAirState(Player* player) :
		IPlayerState(player) {}
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
	Sight* m_sight = nullptr;
};


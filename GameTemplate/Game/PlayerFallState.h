#pragma once
#include "IPlayerState.h"
#include "sound/SoundSource.h"
class Player;
class GravityGauge;
class PlayerFallState : public IPlayerState
{
public:
	PlayerFallState(Player* player) :
		IPlayerState(player) {}
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
	SoundSource*		m_landingSound = nullptr;
};


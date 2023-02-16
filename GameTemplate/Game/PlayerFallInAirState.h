#pragma once
#include "IPlayerState.h"

class GameCamera;
class GravityGauge;
/// <summary>
/// プレイヤーが浮かんでいる状態での落下ステート
/// </summary>
class PlayerFallInAirState : public IPlayerState
{
public:
	PlayerFallInAirState(Player* player) :
		IPlayerState(player) {}
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

	void GenerateWindEffect();
private:
	float				m_secToRegenerateWindEffect = 0.0f;
	GravityGauge*		m_gravityGauge = nullptr;
	GameCamera*			m_gameCamera = nullptr;
};


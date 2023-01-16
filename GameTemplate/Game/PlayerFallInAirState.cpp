#include "stdafx.h"
#include "PlayerFallInAirState.h"
#include "PlayerIdleInAirState.h"
#include "PlayerIdleState.h"
#include "GravityGauge.h"

PlayerFallInAirState::~PlayerFallInAirState()
{
	DeleteGO(m_player->m_effectEmitterWind);
}

void PlayerFallInAirState::Enter()
{

	m_gravityGauge = FindGO<GravityGauge>("gravityGauge");

	// アニメーションを設定する。
	m_player->SetAnimation(Player::enAnimationClip_FallAir);
	// 移動方向を決定する。
	m_player->DecideMoveDirection();
	// プレイヤーのモデルの向きを決める。
	m_player->RotationFallAir();
	// 衝突した場合に法線を取得するように設定。
	g_worldRotation->SetIsGetNormal(true);
	// 風のエフェクトを生成。
	GenerateWindEffect();

}

IPlayerState* PlayerFallInAirState::StateChange()
{
	if (g_pad[0]->IsTrigger(enButtonRB1)) {
		// 空中での待機ステートに遷移する。
		return new PlayerIdleInAirState(m_player);
	}
	if (g_pad[0]->IsTrigger(enButtonLB1)
		|| m_player->IsPlayerTouchObject()
		|| m_gravityGauge->GetDisplayAreaAngleDeg() <= 0.0f
		) {
		// 通常の待機ステートに遷移する。
		m_player->FloatModeChange(false);
		return new PlayerIdleState(m_player);
	}
	if (m_player->IsPlayerTouchObject()) {
		m_player->FloatModeChange(false);
		return new PlayerIdleState(m_player);
	}
	if (m_gravityGauge->GetDisplayAreaAngleDeg() <= 0.0f) {
		return new PlayerIdleState(m_player);
	}
	// ここまで来たら遷移しない。
	return nullptr;
}

void PlayerFallInAirState::Update()
{
	// 空中での移動処理。
	m_player->MoveOnAirspace();

	m_player->m_effectEmitterWind->SetPosition(m_player->m_position);

	m_secToRegenerateWindEffect += g_gameTime->GetFrameDeltaTime();

	if (m_secToRegenerateWindEffect >= 0.8f) {
		DeleteGO(m_player->m_effectEmitterWind);
		GenerateWindEffect();
		m_secToRegenerateWindEffect = 0.0f;
	}
}

void PlayerFallInAirState::GenerateWindEffect()
{
	m_player->m_effectEmitterWind = NewGO<EffectEmitter>(0, "effectWind");
	m_player->m_effectEmitterWind->Init(0);
	m_player->m_effectEmitterWind->SetPosition(m_player->m_position);
	m_player->m_effectEmitterWind->SetRotation(m_player->m_rotation);

	m_player->m_effectEmitterWind->SetScale(Vector3::One * 100.0f);
	m_player->m_effectEmitterWind->Play();
}
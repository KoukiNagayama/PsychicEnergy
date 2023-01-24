#include "stdafx.h"
#include "LockOn.h"
#include "Ring.h"
#include "Player.h"

namespace
{
	const float RESET_RATING_POINT = 1000000000.0f;				// 評価点をリセットする値
	const float TARGET_POS_Y_UP = 30.0f;					// 目標となる座標が足元のため少し座標を上げる
}

bool LockOn::Start()
{
	// 全てのリングを検索し、格納しておく
	m_ringArray = FindGOs<Ring>("ring");
	m_player = FindGO<Player>("player");

	m_lockOnSprite.Init(
		"Assets/sprite/sight/sight.DDS",
		512.0f,
		512.0f
	);

	m_lockOnSprite.SetScale(Vector3(0.1f, 0.1f, 1.0f));
	m_lockOnSprite.Update();

	// 初期の目標を決める
	DecideTarget();

	return true;
}

void LockOn::Update()
{
	//if (m_targetRing->IsDead()) {
	//	DecideTarget();
	//}
	if (g_pad[0]->IsTrigger(enButtonRB2)) {
		DecideTarget();
	}
	CalculatePositionOfSprite();
	m_lockOnSprite.Update();

}

void LockOn::DecideTarget()
{
	// 距離と角度から評価点を計算
	// 距離が近く、角度が小さいものほど評価点が低い
	// 評価点が一番低いものをロックオンする
	
	// 最小の評価点を高めに設定しておく
	m_minRatingPoint = RESET_RATING_POINT;
	// カメラの正面方向
	Vector3 camForward = g_camera3D->GetForward();

	for (auto& ring : m_ringArray) {
		// カメラからリングへ延びるベクトル
		Vector3 toRingPos = ring->GetPosition() - g_camera3D->GetPosition();
		// 角度
		float angle = acosf(camForward.Dot(toRingPos));
		// 距離
		float distance = toRingPos.Length();
		
		// 角度と距離から評価点を計算
		float ratingPoint = angle * distance;
		if (ratingPoint <= m_minRatingPoint) {
			m_targetRing = ring;
			m_minRatingPoint = ratingPoint;
		}
	}
}

void LockOn::CalculatePositionOfSprite()
{
	Vector3 targetPos = m_targetRing->GetPosition();
	targetPos.y += TARGET_POS_Y_UP;
	g_camera3D->CalcScreenPositionFromWorldPosition(m_screenPos, targetPos);
	m_lockOnSprite.SetPosition(Vector3(m_screenPos.x, m_screenPos.y, 0.0f));
}

void LockOn::Render(RenderContext& rc)
{
	m_lockOnSprite.Draw(rc);
}

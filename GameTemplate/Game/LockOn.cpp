#include "stdafx.h"
#include "LockOn.h"
#include "Ring.h"
#include "Player.h"

namespace
{
	const float RESET_RATING_POINT = 1000000000.0f;						// 評価点をリセットする値
	const float RIDIAN_ANGLE_IDENTIFY_TARGET_IS_IN_VIEW = Math::PI / 4;	// 目標が視界内にあると識別する角度(Degree単位)
	const float HALF_HEIGHT_OF_FRAME_BUFFER = FRAME_BUFFER_H * 0.5f;	// フレームバッファの縦幅の半分
	const float HALF_WIDTH_OF_FRAME_BUFFER = FRAME_BUFFER_W * 0.5f;		// フレームバッファの横幅の半分
	const float SPACING_FROM_EDGE_IN_SCREEN_POSITION = 50.0f;			// スクリーン座標における画面端との間隔
}

bool LockOn::Start()
{

	m_player = FindGO<Player>("player");

	m_lockOnSprite.Init(
		"Assets/sprite/lockOn/lockOn/lockOn.DDS",
		512.0f,
		512.0f
	);
	m_lockOnSprite.SetScale(Vector3(0.3f, 0.3f, 1.0f));
	m_lockOnSprite.Update();

	m_arrowSprite.Init(
		"Assets/sprite/lockOn/arrow/arrow.DDS",
		512.0f,
		512.0f
	);
	m_arrowSprite.SetScale(Vector3(0.3f, 0.3f, 1.0f));
	m_arrowSprite.Update();

	m_camForward = g_camera3D->GetForward();
	// 初期の目標を決める
	DecideTarget();

	return true;
}

void LockOn::Update()
{
	m_camForward = g_camera3D->GetForward();
	if (m_targetRing->IsDead()
		|| g_pad[0]->IsTrigger(enButtonRB2)) {
		DecideTarget();
	}

	if (m_isDisable) {
		return;
	}

	IdentifyIfTargetIsInView();
	if (m_isTargetInView) {
		m_lockOnSprite.SetPosition(Vector3(m_screenPos.x, m_screenPos.y, 0.0f));
		m_lockOnSprite.Update();
	}
	else {
		CalculateScreenPositionOfSpriteForArrow();
	}


}

void LockOn::DecideTarget()
{
	// 全てのリングを検索し、格納しておく
	m_ringArray = FindGOs<Ring>("ring");
	// 距離と角度から評価点を計算
	// 距離が近く、角度が小さいものほど評価点が低い
	// 評価点が一番低いものをロックオンする
	//if (sizeof(m_ringArray) < sizeof(Ring)) {
	//	m_isDisable = true;
	//	return;
	//}
	
	// 最小の評価点を高めに設定しておく
	m_minRatingPoint = RESET_RATING_POINT;

	for (auto& ring : m_ringArray) {
		// カメラからリングへ延びるベクトル
		Vector3 toRingPos = ring->GetPosition() - g_camera3D->GetPosition();
		// 距離
		float distance = toRingPos.Length();

		toRingPos.Normalize();
		// 角度
		float angle = acosf(m_camForward.Dot(toRingPos));

		// 角度と距離から評価点を計算
		float ratingPoint = angle * distance;
		if (ratingPoint > m_minRatingPoint) {
			return;
		}
		// 評価点とそのリングを記録する
		m_targetRing = ring;
		m_minRatingPoint = ratingPoint;

	}
}

void LockOn::IdentifyIfTargetIsInView()
{
	Vector3 targetPos = m_targetRing->GetPosition();
	g_camera3D->CalcScreenPositionFromWorldPosition(m_screenPos, targetPos);

	m_playerToTargetPos = targetPos - m_player->GetPosition();
	Vector3 camForward = g_camera3D->GetForward();
	m_playerToTargetPos.Normalize();
	if (camForward.Dot(m_playerToTargetPos) >= 0.0f) {
		// スクリーン座標は中心が(0,0)のためフレームバッファのそれぞれの幅の半分の値を用意
		if (HALF_HEIGHT_OF_FRAME_BUFFER >= m_screenPos.y
			&& -HALF_HEIGHT_OF_FRAME_BUFFER <= m_screenPos.y
			&& HALF_WIDTH_OF_FRAME_BUFFER >= m_screenPos.x
			&& -HALF_WIDTH_OF_FRAME_BUFFER <= m_screenPos.x) {
			m_isTargetInView = true;

		}
		else {
			m_isTargetInView = false;
		}
		m_isBehind = false;
	}
	else{
		m_isTargetInView = false;
		m_isBehind = true;

	}
}

void LockOn::CalculateScreenPositionOfSpriteForArrow()
{

	// x座標の修正
	// 目標に対して逆向きの場合の修正
	if (m_isBehind) {
		// 回転時に外側に向けるために逆にする
		m_screenPos.x = -m_screenPos.x;
		m_screenPos.y = -m_screenPos.y;

		SeekWhichEdgeIsClose();

		if (m_isNearRightEdge) {
			m_screenPos.x = HALF_WIDTH_OF_FRAME_BUFFER - SPACING_FROM_EDGE_IN_SCREEN_POSITION;
		}
		else {
			m_screenPos.x = -HALF_WIDTH_OF_FRAME_BUFFER + SPACING_FROM_EDGE_IN_SCREEN_POSITION;
		}
	}
	else {
		// 正面向きの場合の修正
		if (HALF_WIDTH_OF_FRAME_BUFFER - SPACING_FROM_EDGE_IN_SCREEN_POSITION < m_screenPos.x) {
			m_screenPos.x = HALF_WIDTH_OF_FRAME_BUFFER - SPACING_FROM_EDGE_IN_SCREEN_POSITION;
		}
		else if (-HALF_WIDTH_OF_FRAME_BUFFER + SPACING_FROM_EDGE_IN_SCREEN_POSITION > m_screenPos.x) {
			m_screenPos.x = -HALF_WIDTH_OF_FRAME_BUFFER + SPACING_FROM_EDGE_IN_SCREEN_POSITION;
		}
	}

	// y座標の修正方法は同じのため
	if (HALF_HEIGHT_OF_FRAME_BUFFER - SPACING_FROM_EDGE_IN_SCREEN_POSITION < m_screenPos.y) {
		m_screenPos.y = HALF_HEIGHT_OF_FRAME_BUFFER - SPACING_FROM_EDGE_IN_SCREEN_POSITION;
	}
	else if (-HALF_HEIGHT_OF_FRAME_BUFFER + SPACING_FROM_EDGE_IN_SCREEN_POSITION > m_screenPos.y) {
		m_screenPos.y = -HALF_HEIGHT_OF_FRAME_BUFFER + SPACING_FROM_EDGE_IN_SCREEN_POSITION;
	}

	m_arrowSprite.SetPosition(Vector3(m_screenPos.x, m_screenPos.y, 0.0f));

	// オブジェクトに矢印を向ける
	Quaternion rot;
	rot.SetRotation(Vector3(0.0f, 1.0f, 0.0f), Vector3(m_screenPos.x, m_screenPos.y, 0.0f));
	m_arrowSprite.SetRotation(rot);

	m_arrowSprite.Update();
}

void LockOn::SeekWhichEdgeIsClose()
{
	// 左右どちらの画面端に近いか求める
	// 右
	//if (m_screenPos.x >= 0.0f
	//	&& HALF_WIDTH_OF_FRAME_BUFFER - m_screenPos.x <= HALF_HEIGHT_OF_FRAME_BUFFER - m_screenPos.y
	//		&& HALF_WIDTH_OF_FRAME_BUFFER - m_screenPos.x <= m_screenPos.x + HALF_WIDTH_OF_FRAME_BUFFER) {
	//	m_isNearRightEdge = true;
	//}
	//// 左
	//else if (m_screenPos.x < 0.0f
	//	&& HALF_WIDTH_OF_FRAME_BUFFER + m_screenPos.x <= HALF_HEIGHT_OF_FRAME_BUFFER - m_screenPos.y
	//		&& HALF_WIDTH_OF_FRAME_BUFFER + m_screenPos.x <= m_screenPos.x + HALF_WIDTH_OF_FRAME_BUFFER) {
	//	m_isNearRightEdge = false;
	//}

	if (m_playerToTargetPos.Dot(g_camera3D->GetRight()) >= 0.0f) {
		m_isNearRightEdge = true;
	}
	else {
		m_isNearRightEdge = false;
	}

}

void LockOn::Render(RenderContext& rc)
{
	if (m_isDisable) {
		// 描画しない
		return;
	}
	if (m_isTargetInView) {
		m_lockOnSprite.Draw(rc);
	}
	else {
		m_arrowSprite.Draw(rc);
	}
}

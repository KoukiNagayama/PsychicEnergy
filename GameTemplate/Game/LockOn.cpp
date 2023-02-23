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
	const int	MIN_NUM_TARGET = 1;										// 目標の最少数
	const Vector3 SPRITE_SCALE = { 0.3f, 0.3f, 1.0f };					// スプライトの拡大率
	const Vector2 SPRITE_SIZE = { 512.0f, 512.0f };						// スプライトの幅
	const float MISALIGNMENT_OF_CAMERA_FRONT_AND_TARGET = -0.102f;		// カメラの正面と目標のずれ
}

namespace nsPsychicEnergy
{

	namespace nsLockOn
	{
		bool LockOn::Start()
		{

			m_player = FindGO<nsPlayer::Player>("player");

			m_lockOnSprite.Init(
				"Assets/sprite/lockOn/lockOn/lockOn.DDS",
				SPRITE_SIZE.x,
				SPRITE_SIZE.y
			);
			m_lockOnSprite.SetScale(SPRITE_SCALE);
			m_lockOnSprite.Update();

			m_arrowSprite.Init(
				"Assets/sprite/lockOn/arrow/arrow_2.DDS",
				SPRITE_SIZE.x,
				SPRITE_SIZE.y
			);
			m_arrowSprite.SetScale(SPRITE_SCALE);
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
			m_ringArray = FindGOs<nsRing::Ring>("ring");
			// 距離と角度から評価点を計算
			// 距離が近く、角度が小さいものほど評価点が低い
			// 評価点が一番低いものをロックオンする
			if (m_ringArray.size() < MIN_NUM_TARGET) {
				m_isDisable = true;
				return;
			}

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
			// 目標のワールド座標。
			Vector3 targetPos = m_targetRing->GetPosition();
			// 目標のワールド座標からスクリーン座標を計算する。
			g_camera3D->CalcScreenPositionFromWorldPosition(m_screenPos, targetPos);

			// ワールド空間でのプレイヤーの座標から目標の座標へのベクトルを求める。
			m_playerToTargetPos = targetPos - m_player->GetPosition();
			// カメラの正面座標。
			Vector3 camForward = g_camera3D->GetForward();
			m_playerToTargetPos.Normalize();

			// カメラの正面方向が目標へのベクトルに対して、どれほど離れているか。
			// 目標に近い。
			if (camForward.Dot(m_playerToTargetPos) > MISALIGNMENT_OF_CAMERA_FRONT_AND_TARGET) {

				// スクリーン座標は中心が(0,0)のためフレームバッファのそれぞれの幅の半分の値を用意
				if (HALF_HEIGHT_OF_FRAME_BUFFER >= m_screenPos.y
					&& -HALF_HEIGHT_OF_FRAME_BUFFER <= m_screenPos.y
					&& HALF_WIDTH_OF_FRAME_BUFFER >= m_screenPos.x
					&& -HALF_WIDTH_OF_FRAME_BUFFER <= m_screenPos.x) {
					// 画面内に目標がある
					m_isTargetInView = true;

				}
				else {
					// 画面内に目標がない
					m_isTargetInView = false;
				}
				// 目標は後ろにはない
				m_isBehind = false;
			}
			// カメラの正面方向が目標に対して遠い。
			else {
				// 画面内に目標がない
				m_isTargetInView = false;
				// 目標は後ろにある
				m_isBehind = true;
			}
		}

		void LockOn::CalculateScreenPositionOfSpriteForArrow()
		{
			// 矢印を表示する場合のy座標の修正方法はどの場合でも同じ
			// y座標が画面上部の範囲を超えた場合。
			if (HALF_HEIGHT_OF_FRAME_BUFFER - SPACING_FROM_EDGE_IN_SCREEN_POSITION < m_screenPos.y) {
				// 上部に固定。
				m_screenPos.y = HALF_HEIGHT_OF_FRAME_BUFFER - SPACING_FROM_EDGE_IN_SCREEN_POSITION;
			}
			// y座標が画面下部の範囲を超えた場合。
			else if (-HALF_HEIGHT_OF_FRAME_BUFFER + SPACING_FROM_EDGE_IN_SCREEN_POSITION > m_screenPos.y) {
				// 下部に固定。
				m_screenPos.y = -HALF_HEIGHT_OF_FRAME_BUFFER + SPACING_FROM_EDGE_IN_SCREEN_POSITION;
			}

			// x座標の修正
			// 目標に対して逆向きの場合の修正
			if (m_isBehind) {

				// どの端に近いか求める。
				SeekWhichEdgeIsClose();

				// 画面右にスクリーン座標が近い場合。
				if (m_isNearRightEdge) {
					// 画面右に固定。
					m_screenPos.x = HALF_WIDTH_OF_FRAME_BUFFER - SPACING_FROM_EDGE_IN_SCREEN_POSITION;
				}
				// 画面左にスクリーン座標が近い場合。
				else {
					// 画面左に固定。
					m_screenPos.x = -HALF_WIDTH_OF_FRAME_BUFFER + SPACING_FROM_EDGE_IN_SCREEN_POSITION;
				}
				// 回転時に外側に向けるために逆にする
				m_screenPos.y = -m_screenPos.y;


			}
			// 正面向きの場合の修正
			else {
				// x座標が画面右の範囲を超えた場合。
				if (HALF_WIDTH_OF_FRAME_BUFFER - SPACING_FROM_EDGE_IN_SCREEN_POSITION < m_screenPos.x) {
					// 画面右に固定。
					m_screenPos.x = HALF_WIDTH_OF_FRAME_BUFFER - SPACING_FROM_EDGE_IN_SCREEN_POSITION;
				}
				// x座標が画面左の範囲を超えた場合。
				else if (-HALF_WIDTH_OF_FRAME_BUFFER + SPACING_FROM_EDGE_IN_SCREEN_POSITION > m_screenPos.x) {
					// 画面左に固定。
					m_screenPos.x = -HALF_WIDTH_OF_FRAME_BUFFER + SPACING_FROM_EDGE_IN_SCREEN_POSITION;
				}
			}
			// スプライトの表示座標を更新。
			m_arrowSprite.SetPosition(Vector3(m_screenPos.x, m_screenPos.y, 0.0f));

			// オブジェクトに矢印を向ける。
			Quaternion rot;
			rot.SetRotation(Vector3::Up, Vector3(m_screenPos.x, m_screenPos.y, 0.0f));
			m_arrowSprite.SetRotation(rot);

			// スプライトを更新。
			m_arrowSprite.Update();
		}

		void LockOn::SeekWhichEdgeIsClose()
		{
			// 左右どちらの画面端に近いか求める

			// 右側
			if (m_playerToTargetPos.Dot(g_camera3D->GetRight()) > 0.0f) {
				m_isNearRightEdge = true;
			}
			// 左側
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

	}
}


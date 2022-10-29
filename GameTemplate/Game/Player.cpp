#include "stdafx.h"
#include "Player.h"
#include "PlayerState.h"
#include "PlayerIdleState.h"

namespace
{
	const float WALK_SPEED = 300.0f;		// 歩く速さ
	const float COLLIDER_RADIUS = 30.0f;	// プレイヤーに割り当てるコライダーの半径
	const float COLLIDER_HEIGHT = 70.0f; // プレイヤーに割り当てるコライダーの高さ
}

bool Player::Start()
{
	m_animationClips[enAnimationClip_Idle].Load("Assets/animData/idle.tka");
	m_animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Walk].Load("Assets/animData/walk.tka");
	m_animationClips[enAnimationClip_Walk].SetLoopFlag(true);

	// モデルを初期化。
	m_model.Init(
		"Assets/modelData/unityChan.tkm",
		m_animationClips,
		enAnimationClip_Num,
		enModelUpAxisY,
		true,
		true
	);

	m_position.y += 30.0f;
	m_model.SetTRS(m_position, m_rotation, m_scale);
	// キャラクターコントローターを初期化。
	m_charaCon.Init(
		COLLIDER_RADIUS,
		COLLIDER_HEIGHT,
		m_position,
		m_yawPitchRoll
	);
	// 現在のモデルの上方向を指定。
	m_currentModelUpAxis = Vector3::AxisY;
	// 初期ステートを設定。
	m_playerState = new PlayerIdleState();
	m_playerState->Enter(this);
	return true;
}

void Player::Update()
{

	TestRotation();

	// ステートを更新するか
	PlayerState* playerState = m_playerState->HandleInput(this);
	// 次に更新するステートが設定されているならば
	if (playerState != nullptr) {
		// ステートを更新する。
		delete m_playerState;
		m_playerState = playerState;
		m_playerState->Enter(this);
	}

	// 各ステートの更新処理を実行。
	m_playerState->Update(this);

	// モデルを更新する。

	m_model.Update();


	// アニメーションを再生する。
	m_model.PlayAnimation(m_currentAnimationClip);

}

void Player::WalkOnGround()
{
	// 移動速度を0に戻す
	m_moveSpeed = Vector3::Zero;

	// Lスティックの入力量
	Vector3 LStick;
	LStick.x = g_pad[0]->GetLStickXF();
	LStick.y = g_pad[0]->GetLStickYF();

	// カメラの正面方向
	Vector3 forward = g_camera3D->GetForward();
	// カメラの右方向
	Vector3 right = g_camera3D->GetRight();

	//forward.y = 0.0f;
	//right.y = 0.0f;

	forward.Normalize();
	right.Normalize();

	// 正面と右に対してどれほど進むか計算
	right *= LStick.x * WALK_SPEED;
	forward *= LStick.y * WALK_SPEED;

	// 計算結果をもとに最終的な移動速度を決める
	m_moveSpeed += right + forward;

	// モデルの下方向に落下 ※要調整
	//m_moveSpeed += m_currentModelUpAxis * -1.0f * 100.0f;

	m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime(), m_yawPitchRoll);

	m_model.SetPosition(m_position);
}

void Player::DecideMoveDirection()
{
	// カメラの中心の先へと移動ベクトルを設定する。
	m_moveVectorInAir = g_camera3D->GetForward();
}

void Player::MoveOnAirspace()
{
	// 計算前に移動速度を0に戻す	
	m_moveSpeed = Vector3::Zero;

	// 移動速度を求める。
	m_moveSpeed += m_moveVectorInAir * 1500.0f;
	
	// 移動する。
	m_position = m_charaCon.Float(m_moveSpeed, g_gameTime->GetFrameDeltaTime(), m_yawPitchRoll);

	m_model.SetPosition(m_position);
}

void Player::TestRotation()
{
	if (g_pad[0]->IsTrigger(enButtonA)) {
		Vector3 rotationAxis = Vector3::AxisZ;
		rotationAxis.Normalize();

		m_rotation.AddRotationDeg(rotationAxis,-180.0f);
		A(m_yawPitchRoll, rotationAxis, -180.0f, m_rotation);

		m_model.SetRotation(m_rotation);

		// 現在の上方向はY方向に対してクォータニオンを適用することで求める。
		m_currentModelUpAxis = Vector3::AxisY;
		m_rotation.Apply(m_currentModelUpAxis);
		m_currentModelUpAxis.Normalize();
	}
}

void Player::PlayAnimation(EnAnimationClip currentAnimationClip)
{
	// アニメーションを再生
	m_model.PlayAnimation(currentAnimationClip, 1.0f);
}

void Player::Render(RenderContext& rc)
{
	// 描画
	m_model.Draw(rc);
}

void Player::A(Vector3& vector, const Vector3& axis, float angle, const Quaternion& q)
{
	// クォータニオンをオイラー角に変換する
	// ヨー角
	vector.x = atan2(2.0 * (q.y * q.z + q.w * q.x), q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z);
	// ピッチ角
	vector.y = asin(-2.0 * (q.x * q.z - q.w * q.y));
	// ロール角
	vector.z = atan2(2.0 * (q.x * q.y + q.w * q.z), q.w * q.w + q.x * q.x - q.y * q.y - q.z * q.z);
}
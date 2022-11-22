#include "stdafx.h"
#include "Player.h"
#include "PlayerState.h"
#include "PlayerIdleState.h"
#include "sound/SoundEngine.h"


namespace
{
	const float WALK_SPEED = 450.0f;		// 歩く速さ
	const float COLLIDER_RADIUS = 30.0f;	// プレイヤーに割り当てるコライダーの半径
	const float COLLIDER_HEIGHT = 70.0f; // プレイヤーに割り当てるコライダーの高さ
	const float ANIMATION_INTERPORATE_TIME = 0.3f;	// アニメーションの補間時間
	const float ANIMATION_SPEED = 1.1f;				// アニメーションスピード
	const float MIN_INPUT_AMOUNT = 0.001f;			// 入力量の最低値
	const float SLIDING_SPEED = 600.0f;				// スライディングの速さ
}

bool Player::Start()
{
	// アニメーションクリップをロード。
	m_animationClips[enAnimationClip_Idle].Load("Assets/animData/WD/idle_2.tka");
	m_animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Walk].Load("Assets/animData/WD/walk_2.tka");
	m_animationClips[enAnimationClip_Walk].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Run].Load("Assets/animData/WD/run_4.tka");
	m_animationClips[enAnimationClip_Run].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Slide].Load("Assets/animData/WD/slide_2.tka");
	m_animationClips[enAnimationClip_Slide].SetLoopFlag(false);
	m_animationClips[enAnimationClip_NormalJump].Load("Assets/animData/WD/jump_2.tka");
	m_animationClips[enAnimationClip_NormalJump].SetLoopFlag(false);

	// モデルを初期化。
	m_modelRender.Init(
		"Assets/modelData/WD.tkm",
		m_animationClips,
		enAnimationClip_Num,
		enModelUpAxisZ,
		true,
		true
	);
	m_modelRender.SetTRS(m_position, m_rotation, m_scale);
	
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
	});

	// キャラクターコントローターを初期化。
	m_charaCon.Init(
		COLLIDER_RADIUS,
		COLLIDER_HEIGHT,
		m_position
	);
	// 現在のモデルの上方向を指定。
	m_currentModelUpAxis = Vector3::AxisY;
	// 初期ステートを設定。
	m_playerState = new PlayerIdleState();
	m_playerState->Enter(this);

	// wavファイルを登録する。
	g_soundEngine->ResistWaveFileBank(0, "Assets/sound/run_footstep.wav");
	g_soundEngine->ResistWaveFileBank(1, "Assets/sound/slide2.wav");
	return true;
}

void Player::Update()
{
	// ステートを変更するか
	PlayerState* playerState = m_playerState->StateChange(this);
	// 変更先のステートが設定されているならば
	if (playerState != nullptr) {
		// ステートを変更する。
		delete m_playerState;
		m_playerState = playerState;
		m_playerState->Enter(this);
	}

	// 各ステートの更新処理を実行。
	m_playerState->Update(this);

	// 回転処理。
	Rotation();

	// アニメーションを再生する。
	PlayAnimation(m_currentAnimationClip);

	// モデルを更新する。
	m_modelRender.Update();
}

void Player::MoveOnGround()
{
	// 移動速度を0に戻す
	m_moveSpeed = Vector3::Zero;

	// Lスティックの入力量
	Vector2 LStick;
	LStick.x = g_pad[0]->GetLStickXF();
	LStick.y = g_pad[0]->GetLStickYF();

	// カメラの正面方向
	Vector3 forward = g_camera3D->GetForward();
	// カメラの右方向
	Vector3 right = g_camera3D->GetRight();

	forward.y = 0.0f;
	right.y = 0.0f;

	forward.Normalize();
	right.Normalize();

	// 正面と右に対してどれほど進むか計算
	right *= LStick.x * WALK_SPEED;
	forward *= LStick.y * WALK_SPEED;

	// 計算結果をもとに最終的な移動速度を決める
	m_moveSpeed += right + forward;

	if (m_moveSpeed.LengthSq() <= 440.0f*440.0f) {
		m_currentAnimationClip = enAnimationClip_Walk;
	}
	else {
		m_currentAnimationClip = enAnimationClip_Run;
	}

	// モデルの下方向に落下 ※要調整
	//m_moveSpeed += m_currentModelUpAxis * -1.0f * 100.0f;

	m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());

	m_modelRender.SetPosition(m_position);
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
	//m_position = m_charaCon.Float(m_moveSpeed, g_gameTime->GetFrameDeltaTime());

	m_modelRender.SetPosition(m_position);
}

void Player::Slide()
{
	m_moveSpeed = Vector3::Zero;

	// 左スティックの横方向入力量
	float lStickX = g_pad[0]->GetLStickXF();

	// 正面ベクトルを回転させる。
	Quaternion slideRot = Quaternion::Identity;
	slideRot.AddRotationDegY(lStickX);
	slideRot.Apply(m_forward);

	Vector3 forward = m_forward;
	forward.y = 0.0f;
	forward.Normalize();
	forward *= SLIDING_SPEED;

	// 移動速度を計算。
	m_moveSpeed += m_forward * SLIDING_SPEED;

	m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());

	m_modelRender.SetPosition(m_position);

	if (m_isRingingSlideSound == false) {
		m_slideSound = NewGO<SoundSource>(0);
		m_slideSound->Init(1);
		m_slideSound->SetVolume(0.5f);
		m_slideSound->Play(true);
		m_isRingingSlideSound = true;
	}
}

void Player::Jump()
{
	// 移動速度を0に戻す
	m_moveSpeed = Vector3::Zero;

	// Lスティックの入力量
	Vector2 LStick;
	LStick.x = g_pad[0]->GetLStickXF();
	LStick.y = g_pad[0]->GetLStickYF();

	// カメラの正面方向
	Vector3 forward = g_camera3D->GetForward();
	// カメラの右方向
	Vector3 right = g_camera3D->GetRight();

	forward.y = 0.0f;
	right.y = 0.0f;

	forward.Normalize();
	right.Normalize();

	// 正面と右に対してどれほど進むか計算
	right *= LStick.x * WALK_SPEED;
	forward *= LStick.y * WALK_SPEED;

	// 計算結果をもとに最終的な移動速度を決める
	m_moveSpeed += right + forward;

	// モデルの下方向に落下 ※要調整
	//m_moveSpeed += m_currentModelUpAxis * -1.0f * 100.0f;

	m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());

	m_modelRender.SetPosition(m_position);
}

void Player::Rotation()
{
	if (
		fabsf(m_moveSpeed.x) >= MIN_INPUT_AMOUNT 
		|| fabsf(m_moveSpeed.z) >= MIN_INPUT_AMOUNT
	)
	{
		m_rotation.SetRotationYFromDirectionXZ(m_moveSpeed);
		m_modelRender.SetRotation(m_rotation);

		m_forward = Vector3::AxisZ;
		m_rotation.Apply(m_forward);
	}
}

void Player::PlayAnimation(EnAnimationClip currentAnimationClip)
{
	m_modelRender.SetAnimationSpeed(ANIMATION_SPEED);
	// アニメーションを再生
	m_modelRender.PlayAnimation(currentAnimationClip, ANIMATION_INTERPORATE_TIME);
}

void Player::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	if (wcscmp(eventName, L"run_footstep1") == 0 || wcscmp(eventName, L"run_footstep2") == 0) {
		m_runFootstep = NewGO<SoundSource>(0);
		m_runFootstep->Init(0);
		m_runFootstep->Play(false);
	}
	if (wcscmp(eventName, L"walk_footstep1") == 0 || wcscmp(eventName, L"walk_footstep2") == 0) {
		m_runFootstep = NewGO<SoundSource>(0);
		m_runFootstep->Init(0);
		m_runFootstep->Play(false);
	}
}

void Player::Render(RenderContext& rc)
{
	// 描画
	m_modelRender.Draw(rc);
}
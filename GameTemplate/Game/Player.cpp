#include "stdafx.h"
#include "Player.h"
#include "IPlayerState.h"
#include "PlayerIdleState.h"
#include "sound/SoundEngine.h"
#include "WorldRotation.h"
#include "CommonDataForSound.h"

#include "graphics/effect/EffectEmitter.h"


namespace
{
	const float WALK_SPEED = 450.0f;		// 歩く速さ
	const float COLLIDER_RADIUS = 30.0f;	// プレイヤーに割り当てるコライダーの半径
	const float COLLIDER_HEIGHT = 70.0f; // プレイヤーに割り当てるコライダーの高さ
	const float ANIMATION_INTERPORATE_TIME = 0.3f;	// アニメーションの補間時間
	const float ANIMATION_SPEED = 1.1f;				// アニメーションスピード
	const float MIN_INPUT_AMOUNT = 0.001f;			// 入力量の最低値
	const float SLIDING_SPEED = 600.0f;				// スライディングの速さ
	const float MIN_SPEED_FOR_RUN = 440.0f;
	const float ADD_SPEED = 10.0f;
	const float GRAVITY = 14.0f;					// 重力
	const float INERTIAL_FORCE = 0.99f;				// 慣性力
	const float FOOTSTEP_VOLUME = 0.6f;				// 足音の音量
	const float LANDING_VOLUME = 0.8f;				// 着地音の音量
	const float WIND_VOLUME = 0.3f;					// 風の音量
	const float SLIDING_VOLUME = 0.5f;				// スライディングの音量
}

Player::~Player()
{
	// サウンドオブジェクトを削除
	DeleteGO(m_slideSound);
	DeleteGO(m_landingSound);
	DeleteGO(m_runFootstep);
	DeleteGO(m_modeChangeSound);
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
	m_animationClips[enAnimationClip_NormalJump].Load("Assets/animData/WD/jump_type2_1.tka");
	m_animationClips[enAnimationClip_NormalJump].SetLoopFlag(false);
	m_animationClips[enAnimationClip_DashJump].Load("Assets/animData/WD/jump_type1_1.tka");
	m_animationClips[enAnimationClip_DashJump].SetLoopFlag(false);
	m_animationClips[enAnimationClip_IdleAir].Load("Assets/animData/WD/idle_air_4.tka");
	m_animationClips[enAnimationClip_IdleAir].SetLoopFlag(true);
	m_animationClips[enAnimationClip_FallAir].Load("Assets/animData/WD/fall_air.tka");
	m_animationClips[enAnimationClip_FallAir].SetLoopFlag(true);

	// モデルを初期化。
	m_modelRender.Init(
		"Assets/modelData/WD/WD.tkm",
		m_animationClips,
		enAnimationClip_Num,
		enModelUpAxisZ,
		true,
		true,
		true,
		true
	);
	m_modelRender.SetTRS(m_position, m_rotation, m_scale);
	
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
	});
	m_modelRender.SetAnimationSpeed(ANIMATION_SPEED);


	// キャラクターコントローターを初期化。
	m_charaCon.Init(
		COLLIDER_RADIUS,
		COLLIDER_HEIGHT,
		m_position
	);
	// 現在のモデルの上方向を指定。
	m_currentModelUpAxis = Vector3::AxisY;
	// 初期ステートを設定。
	m_playerState = new PlayerIdleState(this);
	m_playerState->Enter();

	const Matrix& mat = m_modelRender.GetWorldMatrix();
	g_worldRotation->RegisterPlayerWorldMatrix(mat);

	// wavファイルを登録する。
	g_soundEngine->ResistWaveFileBank(nsSound::enSoundNumber_PlayerFootStep, "Assets/sound/player/run_footstep.wav");
	g_soundEngine->ResistWaveFileBank(nsSound::enSoundNumber_PlayerSliding, "Assets/sound/player/slide2.wav");
	g_soundEngine->ResistWaveFileBank(nsSound::enSoundNumber_PlayerLanding, "Assets/sound/player/landing.wav");
	g_soundEngine->ResistWaveFileBank(nsSound::enSoundNumber_PlayerModeChange, "Assets/sound/player/modeChange.wav");
	g_soundEngine->ResistWaveFileBank(nsSound::enSoundNumber_Wind, "Assets/sound/other/wind.wav");

	EffectEngine::GetInstance()->ResistEffect(0, u"Assets/effect/wind4.efk");
	
	return true;
}

void Player::Update()
{
	if (m_disablePlayerMove) {
		return;
	}
	// ステートを変更するか
	IPlayerState* playerState = m_playerState->StateChange();
	// 変更先のステートが設定されているならば
	if (playerState != nullptr) {
		// ステートを変更する。
		delete m_playerState;
		m_playerState = playerState;
		m_playerState->Enter();
	}

	// 各ステートの更新処理を実行。
	m_playerState->Update();
	// アニメーションを再生する。
	PlayAnimation(m_currentAnimationClip);
	// モデルを更新する。
	m_modelRender.Update();

}

void Player::MoveOnGround()
{
	// 移動速度を計算する。
	Vector3 tempMoveSpeed = CalcMoveSpeed(WALK_SPEED);
	m_moveSpeed.x = tempMoveSpeed.x;
	m_moveSpeed.z = tempMoveSpeed.z;
	// y成分だけは徐々に数値を減少させるために減少させた数値を加算する。
	m_moveSpeed.y += tempMoveSpeed.y;

	m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());

	m_modelRender.SetPosition(m_position);
}

void Player::SelectAnimationOnGround()
{
	if (m_moveSpeed.LengthSq() <= MIN_SPEED_FOR_RUN * MIN_SPEED_FOR_RUN) {
		m_currentAnimationClip = enAnimationClip_Walk;
	}
	else {
		m_currentAnimationClip = enAnimationClip_Run;
	}
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

	Vector3 move = m_moveSpeed * g_gameTime->GetFrameDeltaTime();
	
	Vector3 hitPos, hitNormal;
	if( PhysicsWorld::GetInstance()->RayTest(
		m_position,
		m_position + move,
		hitPos,
		hitNormal))
	{
		Vector3 playerDir = m_moveSpeed;
		playerDir.y = 0.0f;
		playerDir.Normalize();
		m_rotation.SetRotationYFromDirectionXZ(playerDir);
		m_modelRender.SetRotation(m_rotation);
		m_position = hitPos;
		m_moveSpeed = Vector3::Zero;
		g_worldRotation->SetHitNormal(hitNormal);
		SetIsTouchObject(true);
	}
	else {
		m_position += move;
	}
	m_charaCon.SetPosition(m_position);
	m_modelRender.SetPosition(m_position);
	
}

void Player::Slide()
{
	m_moveSpeed = Vector3::Zero;
	m_moveSpeed.y = m_lastMoveSpeedY;

	// 左スティックの横方向入力量
	float lStickX = g_pad[0]->GetLStickXF();

	// 正面ベクトルを回転させる。
	Quaternion slideRot = Quaternion::Identity;
	slideRot.AddRotationDegY(lStickX);
	slideRot.Apply(m_forward);

	// 移動速度を計算。
	m_moveSpeed += m_forward * SLIDING_SPEED;

	// 重力を加算。
	m_moveSpeed.y -= GRAVITY;

	if (m_charaCon.IsOnGround()) {
		m_moveSpeed.y = 0;
		if (m_isRingingSlideSound == false) {
			m_slideSound = NewGO<SoundSource>(0);
			m_slideSound->Init(nsSound::enSoundNumber_PlayerSliding);
			m_slideSound->SetVolume(SLIDING_VOLUME);
			m_slideSound->Play(true);
			m_isRingingSlideSound = true;
		}
	}
	// Y成分を保存
	m_lastMoveSpeedY = m_moveSpeed.y;

	// 移動。
	m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());

	m_modelRender.SetPosition(m_position);

}

void Player::Jump()
{
	// 移動速度に加算する値。
	Vector3 addMoveSpeed = CalcMoveSpeed(ADD_SPEED);
	m_moveSpeed += addMoveSpeed;
	// 慣性
	m_moveSpeed *= INERTIAL_FORCE;

	m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());

	m_modelRender.SetPosition(m_position);
}

void Player::SelectJumpAnimation()
{
	if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f) {
		m_currentAnimationClip = enAnimationClip_DashJump;
	}
	else {
		m_currentAnimationClip = enAnimationClip_NormalJump;
	}
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

void Player::RotationFallAir()
{
	m_rotation.SetRotation(Vector3::AxisZ, m_moveVectorInAir);
	m_modelRender.SetRotation(m_rotation);

	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
}

void Player::RotationToCorrectForward()
{
	Vector3 forwardXZ = m_forward;
	forwardXZ.y = 0.0f;
	m_rotation.SetRotationYFromDirectionXZ(forwardXZ);
	m_modelRender.SetRotation(m_rotation);

	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
}

void Player::FloatModeChange(bool isFloating)
{
	// 各種クラスの浮遊状態の設定を変更
	m_isFloating = isFloating;
	m_modelRender.SetIsFloating(m_isFloating);
	g_renderingEngine->SetIsFloating(m_isFloating);

	// モード変更の効果音を発生させる
	m_modeChangeSound = NewGO<SoundSource>(0);
	m_modeChangeSound->Init(nsSound::enSoundNumber_PlayerModeChange);
	m_modeChangeSound->Play(false);
}

void Player::GenerateWindEffect()
{
	m_effectEmitterWind = NewGO<EffectEmitter>(0, "effectWind");
	m_effectEmitterWind->Init(0);
	m_effectEmitterWind->SetPosition(m_position);
	m_effectEmitterWind->SetRotation(m_rotation);

	m_effectEmitterWind->SetScale(Vector3::One * 100.0f);
	m_effectEmitterWind->Play();

	//m_windSound = NewGO<SoundSource>(0);
	//m_windSound->Init(nsSound::enSoundNumber_Wind);
	//m_windSound->SetVolume(WIND_VOLUME);
	//m_windSound->Play(false);
}

void Player::PlayAnimation(EnAnimationClip currentAnimationClip)
{
	// アニメーションを再生
	m_modelRender.PlayAnimation(currentAnimationClip, ANIMATION_INTERPORATE_TIME);
}

void Player::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	if (wcscmp(eventName, L"run_footstep1") == 0 || wcscmp(eventName, L"run_footstep2") == 0) {
		m_runFootstep = NewGO<SoundSource>(0);
		m_runFootstep->Init(nsSound::enSoundNumber_PlayerFootStep);
		m_runFootstep->SetVolume(FOOTSTEP_VOLUME);
		m_runFootstep->Play(false);
	}
	if (wcscmp(eventName, L"walk_footstep1") == 0 || wcscmp(eventName, L"walk_footstep2") == 0) {
		m_runFootstep = NewGO<SoundSource>(0);
		m_runFootstep->Init(nsSound::enSoundNumber_PlayerFootStep);
		m_runFootstep->SetVolume(FOOTSTEP_VOLUME);
		m_runFootstep->Play(false);
	}
	if (wcscmp(eventName, L"jump1_jump") == 0 || wcscmp(eventName, L"jump2_jump") == 0 ){
		m_runFootstep = NewGO<SoundSource>(0);
		m_runFootstep->Init(nsSound::enSoundNumber_PlayerFootStep);
		m_runFootstep->SetVolume(FOOTSTEP_VOLUME);
		m_runFootstep->Play(false);
	}
	if (m_charaCon.IsOnGround()) {
		if (wcscmp(eventName, L"jump1_landing1") == 0 || wcscmp(eventName, L"jump1_landing2") == 0) {
			m_landingSound = NewGO<SoundSource>(0);
			m_landingSound->Init(nsSound::enSoundNumber_PlayerLanding);
			m_landingSound->SetVolume(LANDING_VOLUME);
			m_landingSound->Play(false);
		}
	}
}

void Player::Render(RenderContext& rc)
{
	// 描画
	m_modelRender.Draw(rc);
}

Vector3 Player::CalcMoveSpeed(float speed)
{
	Vector3 moveSpeed = Vector3::Zero;
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
	right *= LStick.x * speed;
	forward *= LStick.y * speed;

	// 計算結果をもとに最終的な移動速度を決める
	moveSpeed += right + forward;

	// 重力を加算。
	moveSpeed.y -= GRAVITY;

	if (m_charaCon.IsOnGround()) {
		moveSpeed.y = 0;
	}

	return moveSpeed;
}
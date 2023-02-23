#include "stdafx.h"
#include "Player.h"
#include "IPlayerState.h"
#include "PlayerIdleState.h"
#include "sound/SoundEngine.h"
#include "WorldRotation.h"
#include "CommonDataForSound.h"
#include "CommonDataForPlayer.h"
#include "CommonDataForEffect.h"
#include "CommonDataForPriority.h"

#include "graphics/effect/EffectEmitter.h"


namespace
{
	const float WALK_SPEED = 450.0f;				// 歩く速さ
	const float COLLIDER_RADIUS = 30.0f;			// プレイヤーに割り当てるコライダーの半径
	const float COLLIDER_HEIGHT = 70.0f;			// プレイヤーに割り当てるコライダーの高さ
	const float ANIMATION_INTERPORATE_TIME = 0.3f;	// アニメーションの補間時間
	const float ANIMATION_SPEED = 1.1f;				// アニメーションスピード
	const float SLIDING_SPEED = 600.0f;				// スライディングの速さ
	const float MIN_SPEED_FOR_RUN = 440.0f;			// 走りの最低速度。
	const float ADD_SPEED = 10.0f;					// 移動の加算量。
	const float GRAVITY = 14.0f;					// 重力
	const float INERTIAL_FORCE = 0.99f;				// 慣性力
	const float FOOTSTEP_VOLUME = 0.6f;				// 足音の音量
	const float LANDING_VOLUME = 0.8f;				// 着地音の音量
	const float WIND_VOLUME = 0.3f;					// 風の音量
	const float SLIDING_VOLUME = 0.5f;				// スライディングの音量
	const float MOVE_SPEED_IN_AIR = 1500.0f;		// 空中での移動速度
	const float WIND_EFFECT_SCALE = 100.0f;			// 風のエフェクトの拡大率
	//const int	
}

namespace nsPsychicEnergy
{

	namespace nsPlayer
	{

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
			// 待機アニメーション。
			m_animationClips[enAnimationClip_Idle].Load("Assets/animData/WD/idle_2.tka");
			m_animationClips[enAnimationClip_Idle].SetLoopFlag(true);
			// 歩きアニメーション。
			m_animationClips[enAnimationClip_Walk].Load("Assets/animData/WD/walk_2.tka");
			m_animationClips[enAnimationClip_Walk].SetLoopFlag(true);
			// 走りアニメーション。
			m_animationClips[enAnimationClip_Run].Load("Assets/animData/WD/run_4.tka");
			m_animationClips[enAnimationClip_Run].SetLoopFlag(true);
			// スライディングアニメーション。
			m_animationClips[enAnimationClip_Slide].Load("Assets/animData/WD/slide_2.tka");
			m_animationClips[enAnimationClip_Slide].SetLoopFlag(false);
			// ジャンプアニメーション。
			m_animationClips[enAnimationClip_NormalJump].Load("Assets/animData/WD/jump_type2_1.tka");
			m_animationClips[enAnimationClip_NormalJump].SetLoopFlag(false);
			// 走りジャンプアニメーション。
			m_animationClips[enAnimationClip_DashJump].Load("Assets/animData/WD/jump_type1_1.tka");
			m_animationClips[enAnimationClip_DashJump].SetLoopFlag(false);
			// 空中での待機アニメーション。
			m_animationClips[enAnimationClip_IdleAir].Load("Assets/animData/WD/idle_air_4.tka");
			m_animationClips[enAnimationClip_IdleAir].SetLoopFlag(true);
			// 空中での落下(移動)アニメーション。
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
			// モデルの座標、回転、拡大率の設定。
			m_modelRender.SetTRS(m_position, m_rotation, m_scale);

			// アニメーションイベントの追加。
			m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
				OnAnimationEvent(clipName, eventName);
				});

			// アニメーションスピードを設定。
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

			// プレイヤーのワールド行列を登録する。
			const Matrix& mat = m_modelRender.GetWorldMatrix();
			nsK2EngineLow::nsWorldRotation::g_worldRotation->RegisterPlayerWorldMatrix(mat);

			// wavファイルを登録する。
			g_soundEngine->ResistWaveFileBank(nsSound::enSoundNumber_PlayerFootStep, "Assets/sound/player/run_footstep.wav");
			g_soundEngine->ResistWaveFileBank(nsSound::enSoundNumber_PlayerSliding, "Assets/sound/player/slide2.wav");
			g_soundEngine->ResistWaveFileBank(nsSound::enSoundNumber_PlayerLanding, "Assets/sound/player/landing.wav");
			g_soundEngine->ResistWaveFileBank(nsSound::enSoundNumber_PlayerModeChange, "Assets/sound/player/modeChange.wav");
			g_soundEngine->ResistWaveFileBank(nsSound::enSoundNumber_Wind, "Assets/sound/other/wind.wav");

			// エフェクトを登録する。
			// 風のエフェクト。
			EffectEngine::GetInstance()->ResistEffect(nsEffect::enEffectNumber_Wind, u"Assets/effect/wind4.efk");

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
			// アニメーションを選択。
			// 歩き。
			if (m_moveSpeed.LengthSq() <= MIN_SPEED_FOR_RUN * MIN_SPEED_FOR_RUN) {
				m_currentAnimationClip = enAnimationClip_Walk;
			}
			// 走り。
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
			m_moveSpeed += m_moveVectorInAir * MOVE_SPEED_IN_AIR;

			// 1フレームあたりの移動距離
			Vector3 move = m_moveSpeed * g_gameTime->GetFrameDeltaTime();

			// 衝突点の座標と法線
			Vector3 hitPos, hitNormal;

			// レイを飛ばして着地を判断する。
			if (PhysicsWorld::GetInstance()->RayTest(
				m_position,
				m_position + move,
				hitPos,
				hitNormal))
			{
				// プレイヤーの向きから回転を求める。
				Vector3 playerDir = m_moveSpeed;
				playerDir.y = 0.0f;
				playerDir.Normalize();
				m_rotation.SetRotationYFromDirectionXZ(playerDir);
				m_modelRender.SetRotation(m_rotation);

				// 正面方向を更新する。
				m_forward = Vector3::AxisZ;
				m_rotation.Apply(m_forward);

				// プレイヤーの座標はレイが衝突した座標。
				m_position = hitPos;
				m_moveSpeed = Vector3::Zero;

				// 取得した衝突点の法線を世界の回転に使用する。
				nsK2EngineLow::nsWorldRotation::g_worldRotation->SetHitNormal(hitNormal);

				// プレイヤーがオブジェクトに接地していると設定する。
				SetIsTouchObject(true);
			}
			else {
				// レイが衝突していないため、着地していないと判断。
				m_position += move;
			}

			// 座標を更新。
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

			// プレイヤーが地面上にいるか判断。
			if (m_charaCon.IsOnGround()) {

				m_moveSpeed.y = 0;
				// スライディングの音がなっていない場合は音を鳴らす。
				if (m_isRingingSlideSound == false) {
					m_slideSound = NewGO<SoundSource>(nsPriority::enPriorityFirst);
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

			// 移動させる。
			m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());

			m_modelRender.SetPosition(m_position);
		}

		void Player::SelectJumpAnimation()
		{
			// ジャンプ時のアニメーションを選択。
			// XZ方面に移動していれば、移動時用ジャンプアニメーション。
			if (fabsf(m_moveSpeed.x) >= MIN_INPUT_QUANTITY 
				|| fabsf(m_moveSpeed.z) >= MIN_INPUT_QUANTITY) 
			{
				m_currentAnimationClip = enAnimationClip_DashJump;
			}
			// 通常のジャンプアニメーション。
			else {
				m_currentAnimationClip = enAnimationClip_NormalJump;
			}
		}

		void Player::Rotation()
		{
			// 通常の回転。
			if (
				fabsf(m_moveSpeed.x) >= MIN_INPUT_QUANTITY
				|| fabsf(m_moveSpeed.z) >= MIN_INPUT_QUANTITY
				)
			{
				// 移動方向に向く。
				m_rotation.SetRotationYFromDirectionXZ(m_moveSpeed);
				m_modelRender.SetRotation(m_rotation);

				// 正面方向を更新する。
				m_forward = Vector3::AxisZ;
				m_rotation.Apply(m_forward);
			}
		}

		void Player::RotationFallAir()
		{
			// 空中での回転。
			// 移動方向に向く。
			m_rotation.SetRotation(Vector3::AxisZ, m_moveVectorInAir);
			m_modelRender.SetRotation(m_rotation);
			// 正面方向を更新する。
			m_forward = Vector3::AxisZ;
			m_rotation.Apply(m_forward);
		}

		void Player::RotationToCorrectForward()
		{
			//Vector3 forwardXZ = m_forward;
			//forwardXZ.y = 0.0f;
			//m_rotation.SetRotationYFromDirectionXZ(forwardXZ);
			//m_modelRender.SetRotation(m_rotation);

			//m_forward = Vector3::AxisZ;
			//m_rotation.Apply(m_forward);
		}

		void Player::FloatModeChange(bool isFloating)
		{
			// 各種クラスの浮遊状態の設定を変更
			m_isFloating = isFloating;
			m_modelRender.SetIsFloating(m_isFloating);
			g_renderingEngine->SetIsFloating(m_isFloating);

			// モード変更の効果音を発生させる
			m_modeChangeSound = NewGO<SoundSource>(nsPriority::enPriorityFirst);
			m_modeChangeSound->Init(nsSound::enSoundNumber_PlayerModeChange);
			m_modeChangeSound->Play(false);
		}

		void Player::GenerateWindEffect()
		{
			// 風のエフェクトを生成する。
			m_effectEmitterWind = NewGO<EffectEmitter>(nsPriority::enPriorityFirst, "effectWind");
			m_effectEmitterWind->Init(nsEffect::enEffectNumber_Wind);
			// エフェクトの座標と回転と拡大率を設定する。
			m_effectEmitterWind->SetPosition(m_position);
			m_effectEmitterWind->SetRotation(m_rotation);
			m_effectEmitterWind->SetScale(Vector3::One * WIND_EFFECT_SCALE);
			// エフェクトを再生。
			m_effectEmitterWind->Play();
		}

		void Player::PlayAnimation(EnAnimationClip currentAnimationClip)
		{
			// アニメーションを再生
			m_modelRender.PlayAnimation(currentAnimationClip, ANIMATION_INTERPORATE_TIME);
		}

		void Player::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
		{
			if (wcscmp(eventName, L"run_footstep1") == 0 || wcscmp(eventName, L"run_footstep2") == 0) {
				m_runFootstep = NewGO<SoundSource>(nsPriority::enPriorityFirst);
				m_runFootstep->Init(nsSound::enSoundNumber_PlayerFootStep);
				m_runFootstep->SetVolume(FOOTSTEP_VOLUME);
				m_runFootstep->Play(false);
			}
			if (wcscmp(eventName, L"walk_footstep1") == 0 || wcscmp(eventName, L"walk_footstep2") == 0) {
				m_runFootstep = NewGO<SoundSource>(nsPriority::enPriorityFirst);
				m_runFootstep->Init(nsSound::enSoundNumber_PlayerFootStep);
				m_runFootstep->SetVolume(FOOTSTEP_VOLUME);
				m_runFootstep->Play(false);
			}
			if (wcscmp(eventName, L"jump1_jump") == 0 || wcscmp(eventName, L"jump2_jump") == 0) {
				m_runFootstep = NewGO<SoundSource>(nsPriority::enPriorityFirst);
				m_runFootstep->Init(nsSound::enSoundNumber_PlayerFootStep);
				m_runFootstep->SetVolume(FOOTSTEP_VOLUME);
				m_runFootstep->Play(false);
			}
			if (m_charaCon.IsOnGround()) {
				if (wcscmp(eventName, L"jump1_landing1") == 0 || wcscmp(eventName, L"jump1_landing2") == 0) {
					m_landingSound = NewGO<SoundSource>(nsPriority::enPriorityFirst);
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
	}
}

#include "stdafx.h"
#include "Player.h"
#include "IPlayerState.h"
#include "PlayerIdleState.h"
#include "sound/SoundEngine.h"


namespace
{
	const float WALK_SPEED = 450.0f;		// ��������
	const float COLLIDER_RADIUS = 30.0f;	// �v���C���[�Ɋ��蓖�Ă�R���C�_�[�̔��a
	const float COLLIDER_HEIGHT = 70.0f; // �v���C���[�Ɋ��蓖�Ă�R���C�_�[�̍���
	const float ANIMATION_INTERPORATE_TIME = 0.3f;	// �A�j���[�V�����̕�Ԏ���
	const float ANIMATION_SPEED = 1.1f;				// �A�j���[�V�����X�s�[�h
	const float MIN_INPUT_AMOUNT = 0.001f;			// ���͗ʂ̍Œ�l
	const float SLIDING_SPEED = 600.0f;				// �X���C�f�B���O�̑���
	const float MIN_SPEED_FOR_RUN = 440.0f;
	const float ADD_SPEED = 10.0f;
	const float GRAVITY = 14.0f;					// �d��
	const float INERTIAL_FORCE = 0.99f;				// ������
}

bool Player::Start()
{
	// �A�j���[�V�����N���b�v�����[�h�B
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
	m_animationClips[enAnimationClip_IdleAir].Load("Assets/animData/WD/idle_air_2.tka");
	m_animationClips[enAnimationClip_IdleAir].SetLoopFlag(true);

	// ���f�����������B
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


	// �L�����N�^�[�R���g���[�^�[���������B
	m_charaCon.Init(
		COLLIDER_RADIUS,
		COLLIDER_HEIGHT,
		m_position
	);
	// ���݂̃��f���̏�������w��B
	m_currentModelUpAxis = Vector3::AxisY;
	// �����X�e�[�g��ݒ�B
	m_playerState = new PlayerIdleState(this);
	m_playerState->Enter();

	// wav�t�@�C����o�^����B
	g_soundEngine->ResistWaveFileBank(0, "Assets/sound/run_footstep.wav");
	g_soundEngine->ResistWaveFileBank(1, "Assets/sound/slide2.wav");
	g_soundEngine->ResistWaveFileBank(2, "Assets/sound/landing.wav");
	g_soundEngine->ResistWaveFileBank(3, "Assets/sound/modeChange.wav");
	return true;
}

void Player::Update()
{
	// �X�e�[�g��ύX���邩
	IPlayerState* playerState = m_playerState->StateChange();
	// �ύX��̃X�e�[�g���ݒ肳��Ă���Ȃ��
	if (playerState != nullptr) {
		// �X�e�[�g��ύX����B
		delete m_playerState;
		m_playerState = playerState;
		m_playerState->Enter();
	}

	// �e�X�e�[�g�̍X�V���������s�B
	m_playerState->Update();

	// ��]�����B
	Rotation();

	// �A�j���[�V�������Đ�����B
	PlayAnimation(m_currentAnimationClip);

	// ���f�����X�V����B
	m_modelRender.Update();
}

void Player::MoveOnGround()
{
	// �ړ����x���v�Z����B
	Vector3 tempMoveSpeed = CalcMoveSpeed(WALK_SPEED);
	m_moveSpeed.x = tempMoveSpeed.x;
	m_moveSpeed.z = tempMoveSpeed.z;
	// y���������͏��X�ɐ��l�����������邽�߂Ɍ������������l�����Z����B
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
	// �J�����̒��S�̐�ւƈړ��x�N�g����ݒ肷��B
	m_moveVectorInAir = g_camera3D->GetForward();
}

void Player::MoveOnAirspace()
{
	// �v�Z�O�Ɉړ����x��0�ɖ߂�	
	m_moveSpeed = Vector3::Zero;

	// �ړ����x�����߂�B
	m_moveSpeed += m_moveVectorInAir * 1500.0f;
	
	m_modelRender.SetPosition(m_position);
}

void Player::Slide()
{
	m_moveSpeed = Vector3::Zero;
	m_moveSpeed.y = m_lastMoveSpeedY;

	// ���X�e�B�b�N�̉��������͗�
	float lStickX = g_pad[0]->GetLStickXF();

	// ���ʃx�N�g������]������B
	Quaternion slideRot = Quaternion::Identity;
	slideRot.AddRotationDegY(lStickX);
	slideRot.Apply(m_forward);

	// �ړ����x���v�Z�B
	m_moveSpeed += m_forward * SLIDING_SPEED;

	// �d�͂����Z�B
	m_moveSpeed.y -= GRAVITY;

	if (m_charaCon.IsOnGround()) {
		m_moveSpeed.y = 0;
		if (m_isRingingSlideSound == false) {
			m_slideSound = NewGO<SoundSource>(0);
			m_slideSound->Init(1);
			m_slideSound->SetVolume(0.5f);
			m_slideSound->Play(true);
			m_isRingingSlideSound = true;
		}
	}
	// Y������ۑ�
	m_lastMoveSpeedY = m_moveSpeed.y;

	// �ړ��B
	m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());

	m_modelRender.SetPosition(m_position);

}

void Player::Jump()
{
	// �ړ����x�ɉ��Z����l�B
	Vector3 addMoveSpeed = CalcMoveSpeed(ADD_SPEED);
	m_moveSpeed += addMoveSpeed;
	// ����
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

void Player::FloatModeChange(bool isFloating)
{
	m_isFloating = isFloating;
	m_modelRender.SetIsFloating(m_isFloating);
	g_renderingEngine->SetIsFloating(m_isFloating);

	m_modeChangeSound = NewGO<SoundSource>(0);
	m_modeChangeSound->Init(3);
	m_modeChangeSound->Play(false);
}

void Player::PlayAnimation(EnAnimationClip currentAnimationClip)
{
	// �A�j���[�V�������Đ�
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
	if (wcscmp(eventName, L"jump1_jump") == 0 || wcscmp(eventName, L"jump2_jump") == 0 ){
		m_runFootstep = NewGO<SoundSource>(0);
		m_runFootstep->Init(0);
		m_runFootstep->Play(false);
	}
	if (m_charaCon.IsOnGround()) {
		if (wcscmp(eventName, L"jump1_landing1") == 0 || wcscmp(eventName, L"jump1_landing2") == 0) {
			m_landingSound = NewGO<SoundSource>(0);
			m_landingSound->Init(2);
			m_landingSound->Play(false);
		}
	}
}

void Player::Render(RenderContext& rc)
{
	// �`��
	m_modelRender.Draw(rc);
}

Vector3 Player::CalcMoveSpeed(float speed)
{
	Vector3 moveSpeed = Vector3::Zero;
	// L�X�e�B�b�N�̓��͗�
	Vector2 LStick;
	LStick.x = g_pad[0]->GetLStickXF();
	LStick.y = g_pad[0]->GetLStickYF();

	// �J�����̐��ʕ���
	Vector3 forward = g_camera3D->GetForward();
	// �J�����̉E����
	Vector3 right = g_camera3D->GetRight();

	forward.y = 0.0f;
	right.y = 0.0f;

	forward.Normalize();
	right.Normalize();

	// ���ʂƉE�ɑ΂��Ăǂ�قǐi�ނ��v�Z
	right *= LStick.x * speed;
	forward *= LStick.y * speed;

	// �v�Z���ʂ����ƂɍŏI�I�Ȉړ����x�����߂�
	moveSpeed += right + forward;

	// �d�͂����Z�B
	moveSpeed.y -= GRAVITY;

	if (m_charaCon.IsOnGround()) {
		moveSpeed.y = 0;
	}

	return moveSpeed;
}
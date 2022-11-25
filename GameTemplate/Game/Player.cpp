#include "stdafx.h"
#include "Player.h"
#include "PlayerState.h"
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
	const float RESIST_SPEED = 100.0f;
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
	m_animationClips[enAnimationClip_NormalJump].Load("Assets/animData/WD/jump_2.tka");
	m_animationClips[enAnimationClip_NormalJump].SetLoopFlag(false);

	// ���f�����������B
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

	// �L�����N�^�[�R���g���[�^�[���������B
	m_charaCon.Init(
		COLLIDER_RADIUS,
		COLLIDER_HEIGHT,
		m_position
	);
	// ���݂̃��f���̏�������w��B
	m_currentModelUpAxis = Vector3::AxisY;
	// �����X�e�[�g��ݒ�B
	m_playerState = new PlayerIdleState();
	m_playerState->Enter(this);

	// wav�t�@�C����o�^����B
	g_soundEngine->ResistWaveFileBank(0, "Assets/sound/run_footstep.wav");
	g_soundEngine->ResistWaveFileBank(1, "Assets/sound/slide2.wav");
	return true;
}

void Player::Update()
{
	// �X�e�[�g��ύX���邩
	PlayerState* playerState = m_playerState->StateChange(this);
	// �ύX��̃X�e�[�g���ݒ肳��Ă���Ȃ��
	if (playerState != nullptr) {
		// �X�e�[�g��ύX����B
		delete m_playerState;
		m_playerState = playerState;
		m_playerState->Enter(this);
	}

	// �e�X�e�[�g�̍X�V���������s�B
	m_playerState->Update(this);

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
	m_moveSpeed = CalcMoveSpeed(WALK_SPEED);

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
	
	// �ړ�����B
	//m_position = m_charaCon.Float(m_moveSpeed, g_gameTime->GetFrameDeltaTime());

	m_modelRender.SetPosition(m_position);
}

void Player::Slide()
{
	m_moveSpeed = Vector3::Zero;

	// ���X�e�B�b�N�̉��������͗�
	float lStickX = g_pad[0]->GetLStickXF();

	// ���ʃx�N�g������]������B
	Quaternion slideRot = Quaternion::Identity;
	slideRot.AddRotationDegY(lStickX);
	slideRot.Apply(m_forward);

	// �ړ����x���v�Z�B
	m_moveSpeed += m_forward * SLIDING_SPEED;

	// �d�͂����Z�B
	m_moveSpeed.y -= 7.0f;

	// �ړ��B
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
	// �ړ����x�ɉ��Z����l�B
	Vector3 addMoveSpeed = CalcMoveSpeed(RESIST_SPEED);
	//// ���Z����l�̐��ʃx�N�g���̒���
	float forwardVectorLength = addMoveSpeed.Dot(m_forward);

	// ���ʃx�N�g�����폜
	addMoveSpeed -= m_forward * forwardVectorLength;

	// �ړ����x���m��B
	if (fabsf(addMoveSpeed.x) >= 0.0001f || fabsf(addMoveSpeed.z) >= 0.0001f) {
		m_moveSpeed += addMoveSpeed;
		if (m_moveSpeed.LengthSq() > 1.0f) {
			m_moveSpeed.Normalize();
		}
		m_moveSpeed *= WALK_SPEED;
	}
	else {
		m_moveSpeed = Vector3::Zero;
		m_moveSpeed.y += addMoveSpeed.y;
	}




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
	moveSpeed.y -= 7.0f;

	return moveSpeed;
}
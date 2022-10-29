#include "stdafx.h"
#include "Player.h"
#include "PlayerState.h"
#include "PlayerIdleState.h"

namespace
{
	const float WALK_SPEED = 300.0f;		// ��������
	const float COLLIDER_RADIUS = 30.0f;	// �v���C���[�Ɋ��蓖�Ă�R���C�_�[�̔��a
	const float COLLIDER_HEIGHT = 70.0f; // �v���C���[�Ɋ��蓖�Ă�R���C�_�[�̍���
}

bool Player::Start()
{
	m_animationClips[enAnimationClip_Idle].Load("Assets/animData/idle.tka");
	m_animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Walk].Load("Assets/animData/walk.tka");
	m_animationClips[enAnimationClip_Walk].SetLoopFlag(true);

	// ���f�����������B
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
	// �L�����N�^�[�R���g���[�^�[���������B
	m_charaCon.Init(
		COLLIDER_RADIUS,
		COLLIDER_HEIGHT,
		m_position,
		m_yawPitchRoll
	);
	// ���݂̃��f���̏�������w��B
	m_currentModelUpAxis = Vector3::AxisY;
	// �����X�e�[�g��ݒ�B
	m_playerState = new PlayerIdleState();
	m_playerState->Enter(this);
	return true;
}

void Player::Update()
{

	TestRotation();

	// �X�e�[�g���X�V���邩
	PlayerState* playerState = m_playerState->HandleInput(this);
	// ���ɍX�V����X�e�[�g���ݒ肳��Ă���Ȃ��
	if (playerState != nullptr) {
		// �X�e�[�g���X�V����B
		delete m_playerState;
		m_playerState = playerState;
		m_playerState->Enter(this);
	}

	// �e�X�e�[�g�̍X�V���������s�B
	m_playerState->Update(this);

	// ���f�����X�V����B

	m_model.Update();


	// �A�j���[�V�������Đ�����B
	m_model.PlayAnimation(m_currentAnimationClip);

}

void Player::WalkOnGround()
{
	// �ړ����x��0�ɖ߂�
	m_moveSpeed = Vector3::Zero;

	// L�X�e�B�b�N�̓��͗�
	Vector3 LStick;
	LStick.x = g_pad[0]->GetLStickXF();
	LStick.y = g_pad[0]->GetLStickYF();

	// �J�����̐��ʕ���
	Vector3 forward = g_camera3D->GetForward();
	// �J�����̉E����
	Vector3 right = g_camera3D->GetRight();

	//forward.y = 0.0f;
	//right.y = 0.0f;

	forward.Normalize();
	right.Normalize();

	// ���ʂƉE�ɑ΂��Ăǂ�قǐi�ނ��v�Z
	right *= LStick.x * WALK_SPEED;
	forward *= LStick.y * WALK_SPEED;

	// �v�Z���ʂ����ƂɍŏI�I�Ȉړ����x�����߂�
	m_moveSpeed += right + forward;

	// ���f���̉������ɗ��� ���v����
	//m_moveSpeed += m_currentModelUpAxis * -1.0f * 100.0f;

	m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime(), m_yawPitchRoll);

	m_model.SetPosition(m_position);
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

		// ���݂̏������Y�����ɑ΂��ăN�H�[�^�j�I����K�p���邱�Ƃŋ��߂�B
		m_currentModelUpAxis = Vector3::AxisY;
		m_rotation.Apply(m_currentModelUpAxis);
		m_currentModelUpAxis.Normalize();
	}
}

void Player::PlayAnimation(EnAnimationClip currentAnimationClip)
{
	// �A�j���[�V�������Đ�
	m_model.PlayAnimation(currentAnimationClip, 1.0f);
}

void Player::Render(RenderContext& rc)
{
	// �`��
	m_model.Draw(rc);
}

void Player::A(Vector3& vector, const Vector3& axis, float angle, const Quaternion& q)
{
	// �N�H�[�^�j�I�����I�C���[�p�ɕϊ�����
	// ���[�p
	vector.x = atan2(2.0 * (q.y * q.z + q.w * q.x), q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z);
	// �s�b�`�p
	vector.y = asin(-2.0 * (q.x * q.z - q.w * q.y));
	// ���[���p
	vector.z = atan2(2.0 * (q.x * q.y + q.w * q.z), q.w * q.w + q.x * q.x - q.y * q.y - q.z * q.z);
}
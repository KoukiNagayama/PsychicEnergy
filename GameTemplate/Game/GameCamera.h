#pragma once
#include "SpringCamera.h"

class Player;

/// <summary>
/// �Q�[���J�����N���X
/// </summary>
class GameCamera : public IGameObject
{
public:
	/// <summary>
	/// �J�n�����B
	/// </summary>
	/// <returns></returns>
	bool Start();
	/// <summary>
	/// �X�V�B
	/// </summary>
	void Update();
private:
	/// <summary>
	/// �������B
	/// </summary>
	void Init();
	/// <summary>
	/// �v���C���[�̑����n�ʂɒ����Ă��鎞�̍X�V�����B
	/// </summary>
	void UpdateTouchingObject();
	/// <summary>
	/// �v���C���[���󒆂ɂ��鎞�̍X�V�����B
	/// </summary>
	void UpdateOnAirspace();

private:
	Player*			m_player = nullptr;								// �v���C���[
	Vector3			m_position = Vector3::Zero;						// �J�����̍��W
	Quaternion		m_rotation = Quaternion::Identity;				// �J�����̉�]
	Vector3			m_target = Vector3::Zero;						// �����_
	Vector3			m_toCameraPos = Vector3::Zero;					// �����_���王�_�ւ̃x�N�g��
	SpringCamera	m_springCamera;									// �΂˃J����							
	float			m_degreeX = 0.0f;								// Degree�P�ʂ�X�������̉�]��
	float			m_degreeY = 0.0f;								// Degree�P�ʂ�Y�������̉�]��
};
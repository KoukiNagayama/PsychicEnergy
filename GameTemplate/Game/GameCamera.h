#pragma once
#include "SpringCamera.h"

class Player;
class GameCamera : public IGameObject
{
public:
	bool Start();
	void Update();
private:
	void Init();

private:
	Player*			m_player = nullptr;										// �v���C���[
	Vector3			m_position = Vector3::Zero;						// �J�����̍��W
	Quaternion		m_rotation = Quaternion::Identity;				// �J�����̉�]
	Vector3			m_target = Vector3::Zero;						// �����_
	Vector3			m_toCameraPos = Vector3::Zero;					// �����_���王�_�ւ̃x�N�g��
	SpringCamera	m_springCamera;
};


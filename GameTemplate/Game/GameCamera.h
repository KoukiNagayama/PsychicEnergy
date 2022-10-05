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
	Player*			m_player = nullptr;										// プレイヤー
	Vector3			m_position = Vector3::Zero;						// カメラの座標
	Quaternion		m_rotation = Quaternion::Identity;				// カメラの回転
	Vector3			m_target = Vector3::Zero;						// 注視点
	Vector3			m_toCameraPos = Vector3::Zero;					// 注視点から視点へのベクトル
	SpringCamera	m_springCamera;
};


#pragma once
#include "IPlayerState.h"

class GameCamera;
class GravityGauge;
/// <summary>
/// �v���C���[��������ł����Ԃł̗����X�e�[�g
/// </summary>
class PlayerFallInAirState : public IPlayerState
{
public:
	PlayerFallInAirState(Player* player) :
		IPlayerState(player) {}
	~PlayerFallInAirState() override;
	/// <summary>
	/// �X�e�[�g�J�n���̏����B
	/// </summary>
	void Enter() override;
	/// <summary>
	/// �X�e�[�g�̑J�ڏ����B
	/// </summary>
	/// <returns>�J�ڂ���X�e�[�g</returns>
	IPlayerState* StateChange() override;
	/// <summary>
	/// �X�e�[�g�ɂ�����X�V�����B
	/// </summary>
	void Update() override;

	void GenerateWindEffect();
private:
	float				m_secToRegenerateWindEffect = 0.0f;
	GravityGauge*		m_gravityGauge = nullptr;
	GameCamera*			m_gameCamera = nullptr;
};


#pragma once
#include "IPlayerState.h"
class PlayerJumpState : public IPlayerState
{
public:
	PlayerJumpState(Player* player) :
		IPlayerState(player) {}
	~PlayerJumpState() override;
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
private:
	float m_count = 0.0f;

};


#pragma once
#include "IPlayerState.h"
class Player;
class PlayerFallState : public IPlayerState
{
public:
	PlayerFallState(Player* player) :
		IPlayerState(player) {}
	~PlayerFallState() override;
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
};


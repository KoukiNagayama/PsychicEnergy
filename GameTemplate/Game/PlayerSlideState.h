#pragma once
#include "PlayerState.h"

class PlayerSlideState : public PlayerState
{
public:
	~PlayerSlideState() override;
	/// <summary>
	/// �X�e�[�g�J�n���̏����B
	/// </summary>
	void Enter(Player* player) override;
	/// <summary>
	/// �X�e�[�g�̑J�ڏ����B
	/// </summary>
	/// <returns>�J�ڂ���X�e�[�g</returns>
	PlayerState* StateChange(Player* player) override;
	/// <summary>
	/// �X�e�[�g�ɂ�����X�V�����B
	/// </summary>
	void Update(Player* player) override;
};


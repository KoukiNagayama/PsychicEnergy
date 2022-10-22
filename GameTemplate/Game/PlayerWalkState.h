#pragma once
#include "PlayerState.h"

/// <summary>
/// �v���C���[�̒n��ł̕��s�X�e�[�g�N���X
/// </summary>
class PlayerWalkState : public PlayerState
{
public:
	/// <summary>
	/// �X�e�[�g�J�n���̏����B
	/// </summary>
	void Enter(Player* player) override;
	/// <summary>
	/// �X�e�[�g�̑J�ڏ����B
	/// </summary>
	/// <returns>�J�ڂ���X�e�[�g</returns>
	PlayerState* HandleInput(Player* player) override;
	/// <summary>
	/// �X�e�[�g�ɂ�����X�V�����B
	/// </summary>
	void Update(Player* player) override;
};


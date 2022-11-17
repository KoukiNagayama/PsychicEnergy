#pragma once
#include "Player.h"
class Player;

/// <summary>
/// �v���C���[�̃X�e�[�g���N���X
/// </summary>
class PlayerState
{
public:
	/// <summary>
	/// �f�X�X�g���N�^
	/// </summary>
	virtual ~PlayerState() {}
	/// <summary>
	/// �X�e�[�g�J�n���̏����B
	/// </summary>
	virtual void Enter(Player* player) = 0;
	/// <summary>
	/// �X�e�[�g�̑J�ڏ����B
	/// </summary>
	/// <returns>�J�ڂ���X�e�[�g</returns>
	virtual PlayerState* StateChange(Player* player) = 0;
	/// <summary>
	/// �e�X�e�[�g�ɂ�����X�V�����B
	/// </summary>
	virtual void Update(Player* player) = 0;
};


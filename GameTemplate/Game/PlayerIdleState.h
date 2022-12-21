#pragma once
#include "IPlayerState.h"
class Player;

/// <summary>
/// �v���C���[�̒n��ł̑ҋ@�X�e�[�g�N���X
/// </summary>
class PlayerIdleState : public IPlayerState
{
public:
	PlayerIdleState(Player* player) :
		IPlayerState(player) {}
	~PlayerIdleState() override;
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


#pragma once
#include "IPlayerState.h"
/// <summary>
/// �v���C���[�̒n��ł̕��s�X�e�[�g�N���X
/// </summary>
class PlayerWalkState : public IPlayerState
{
public:
	PlayerWalkState(Player* player) :
		IPlayerState(player) {}
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


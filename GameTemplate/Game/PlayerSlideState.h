#pragma once
#include "IPlayerState.h"
class PlayerSlideState : public IPlayerState
{
public:
	PlayerSlideState(Player* player) :
		IPlayerState(player) {}

	~PlayerSlideState() override;
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


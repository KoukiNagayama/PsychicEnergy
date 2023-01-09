#pragma once
#include "IPlayerState.h"

class Sight;
/// <summary>
/// �v���C���[�̋󒆂ł̑ҋ@�X�e�[�g�N���X
/// </summary>
class PlayerIdleInAirState : public IPlayerState
{
public:
	PlayerIdleInAirState(Player* player) :
		IPlayerState(player) {}
	~PlayerIdleInAirState();
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
	Sight* m_sight = nullptr;
};


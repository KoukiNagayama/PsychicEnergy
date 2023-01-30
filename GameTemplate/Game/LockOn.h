#pragma once
class Ring;
class Player;
/// <summary>
/// ���b�N�I��
/// </summary>
class LockOn : public IGameObject
{
public:
	LockOn() {}
	~LockOn() {}
	/// <summary>
	/// �J�n�����B
	/// �C���X�^���X�������ɏ��������s���Btrue��Ԃ����Ƃ��ɌĂ΂�Ȃ��Ȃ�B
	/// </summary>
	bool Start();
	/// <summary>
	/// �X�V
	/// </summary>
	void Update();
	/// <summary>
	/// �`��
	/// </summary>
	void Render(RenderContext& rc);
private:
	/// <summary>
	/// �ڕW�����߂�
	/// </summary>
	void DecideTarget();
	/// <summary>
	/// �ڕW�����E���ɂ��邩���ʂ���
	/// </summary>
	void IdentifyIfTargetIsInView();
	/// <summary>
	/// ���p�X�v���C�g�̃X�N���[�����W���v�Z����
	/// </summary>
	void CalculateScreenPositionOfSpriteForArrow();
	/// <summary>
	/// �ǂ̒[�ɋ߂������߂�
	/// </summary>
	void SeekWhichEdgeIsClose();
private:
	SpriteRender				m_lockOnSprite;						// ���b�N�I�����Ɏg�p����摜
	SpriteRender				m_arrowSprite;						// ���̉摜
	std::vector<Ring*>			m_ringArray;						// �����O�̔z��
	Player*						m_player = nullptr;					// �v���C���[
	bool						m_isLockOn = false;					// ���b�N�I�����Ă���H
	Vector3						m_targetPos = Vector3::Zero;		// �ڕW�ƂȂ���W
	Ring*						m_targetRing = nullptr;				// �ڕW�ƂȂ郊���O
	float						m_minRatingPoint = 10000.0f;		// �ŏ��̕]���_
	Vector2						m_screenPos = Vector2::Zero;		// �X�v���C�g��\������X�N���[�����W
	bool						m_isTargetInView = false;			// ���E���ɂ��邩�H
	Vector3						m_camForward = Vector3::AxisZ;		// �J�����̐��ʕ���
	bool						m_isBehind = false;
	bool						m_isNearRightEdge = false;
	bool						m_isDisable = false;
	Vector3						m_playerToTargetPos = Vector3::Zero;
};	


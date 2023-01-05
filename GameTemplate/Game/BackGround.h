#pragma once
#include "WorldRotation.h"

class Player;
/// <summary>
/// �w�i���f���N���X
/// </summary>
class BackGround : public IGameObject
{
public:
	BackGround() {}
	~BackGround() {}
	/// <summary>
	/// �J�n�����B
	/// �C���X�^���X�������ɏ��������s���Btrue��Ԃ����Ƃ��ɌĂ΂�Ȃ��Ȃ�B
	/// </summary>
	bool Start();
	/// <summary>
	/// �X�V�����B
	/// </summary>
	void Update();
	/// <summary>
	/// �`�揈���B
	/// </summary>
	/// <param name="rc"></param>
	void Render(RenderContext& rc);
	/// <summary>
	/// ���W��ݒ�B
	/// </summary>
	/// <param name="position">�ݒ肷����W�B</param>
	void SetPosition(Vector3& position)
	{
		m_position = position;
	}
	/// <summary>
	/// ��]��ݒ�B
	/// </summary>
	/// <param name="rotation">�ݒ肷���]�l�B</param>
	void SetRotation(Quaternion& rotation)
	{
		m_rotation = rotation;
	}
	/// <summary>
	/// �g�嗦��ݒ�B
	/// </summary>
	/// <param name="scale">�ݒ肷��g�嗦�B</param>
	void SetScale(Vector3& scale)
	{
		m_scale = scale;
	}
	/// <summary>
	/// ���f���̃^�C�v��ݒ�B
	/// </summary>
	/// <param name="num">�ݒ肷��^�C�v�̔ԍ��B</param>
	void SetTypeNum(int num)
	{
		m_typeNum = num;
	}
	ModelRender& GetModelRender()
	{
		return m_modelRender;
	}
	inline void SetWorldMatrixWithLerp(const Matrix& worldMat)
	{
		m_prevMatrix = m_modelRender.GetWorldMatrix();
		m_nextMatrix = worldMat;
		m_rotateTimer = 0.0f;
	}
public:
	enum enModelType
	{
		enModelType_Ground,		// �n��
		enModelType_Box1,		// ��1
		enModelType_Box2,		// ��2
		enModelType_Box3		// ��3
	};
private:
	ModelRender				m_modelRender;				// ���f�������_�[
	Vector3					m_position;					// ���W
	Vector3					m_scale;					// �g�嗦
	Quaternion				m_rotation;					// ��]
	int						m_typeNum;					// ���f���̃^�C�v
	PhysicsStaticObject		m_physicsStaticObject;		// �ÓI�����I�u�W�F�N�g
	Matrix					m_worldMatrix = Matrix::Identity;
	Player*					m_player = nullptr;
	Matrix					m_prevMatrix = Matrix::Identity;
	Matrix					m_nextMatrix = Matrix::Identity;
	float					m_rotateTimer = 1.0f;

};


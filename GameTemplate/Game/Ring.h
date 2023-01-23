#pragma once
class Player;
class Ring : public IGameObject
{
public:
	Ring() {}
	~Ring() {}

	bool Start();
	void Update();
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
	/// ���W���擾�B
	/// </summary>
	/// <returns>���W�B</returns>
	const Vector3& GetPosition() const
	{
		return m_position;
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
private:
	/// <summary>
	/// ��]
	/// </summary>
	void Rotation();
	/// <summary>
	/// �����蔻��
	/// </summary>
	void Collision();
	/// <summary>
	/// �v���C���[�Ƃ̋������v�Z
	/// </summary>
	float CalcDistanceToPlayer();
private:
	ModelRender				m_modelRender;				// ���f�������_�[
	Vector3					m_position;					// ���W
	Vector3					m_scale;					// �g�嗦
	Quaternion				m_rotation;					// ��]
	Player*					m_player = nullptr;			// �v���C���[
	Matrix					m_prevMatrix = Matrix::Identity;
	Matrix					m_nextMatrix = Matrix::Identity;
	float					m_rotateTimer = 1.0f;
};


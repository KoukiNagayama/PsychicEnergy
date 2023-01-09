#pragma once
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
private:

private:
	ModelRender				m_modelRender;				// ���f�������_�[
	Vector3					m_position;					// ���W
	Vector3					m_scale;					// �g�嗦
	Quaternion				m_rotation;					// ��]
};


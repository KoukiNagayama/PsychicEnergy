#pragma once
class TestModel : public IGameObject
{
public:
	TestModel() {}
	~TestModel() {}
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	void SetPosition(Vector3& pos)
	{
		m_pos = pos;
	}
	void SetRotation(Quaternion& rot)
	{
		m_rot = rot;
	}
	void SetScale(Vector3& sca)
	{
		m_sca = sca;
	}
	void SetTypeNum(int num)
	{
		m_typeNum = num;
	}
private:
	ModelRender		m_model;
	Vector3			m_pos;
	Vector3			m_sca;
	Quaternion		m_rot;
	int				m_typeNum;
	PhysicsStaticObject m_physicsStaticObject;
};


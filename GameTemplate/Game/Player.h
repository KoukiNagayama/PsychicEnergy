#pragma once
class Player : public IGameObject
{
public:
	Player() {};
	~Player() {};

	bool Start();
	void Update();
	void Render(RenderContext& rc);
	void SetPosition(Vector3& position)
	{
		m_position = position;
	}
	void SetRotation(Quaternion& rotation)
	{
		m_rotation = rotation;
	}
	void SetScale(Vector3& scale)
	{
		m_scale = scale;
	}
	const Vector3& GetPosition() const 
	{
		return m_position;
	}
	const Vector3& GetCurrentModelUpAxis() const
	{
		return m_currentModelUpAxis;
	}
private:
	void Init();
	void Move();
	void TestRotation();
private:
	ModelRender m_model;
	Vector3 m_position = Vector3::Zero;
	Vector3 m_scale = Vector3::One;
	Quaternion m_rotation = Quaternion::Identity;
	enum EnAnimationClip {
		enAnimationClip_Idle,
		enAnimationClip_Walk,
		enAnimationClip_Num
	};
	AnimationClip	m_animationClips[enAnimationClip_Num];
	Vector3 m_currentModelUpAxis;
};


#pragma once
class EngineTest : public IGameObject
{
public:
	EngineTest() {};
	~EngineTest() {};

	bool Start();
	void Update();
	void Render(RenderContext& rc);
private:
	void Move();
	void Rotation();
private:
	ModelRender		m_testModel;
	Vector3			m_modelPos = Vector3::Zero;
	Vector3			m_modelSca = Vector3::One;
	Quaternion		m_modelRot = Quaternion::Identity;
	enum EnAnimationClip {
		enAnimationClip_Idle,
		enAnimationClip_Walk,
		enAnimationClip_Num
	};
	AnimationClip	m_animationClips[enAnimationClip_Num];
};


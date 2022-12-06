#pragma once
#include "LevelRender.h"
#include "SkyCube.h"
class TestModel;
class Player;
class GameCamera;
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
	void CamUpdate();
private:
	ModelRender		m_unityChan;
	ModelRender		m_bgModel;
	Vector3			m_modelPos = Vector3::Zero;
	Vector3			m_modelSca = Vector3::One;
	Quaternion		m_modelRot = Quaternion::Identity;
	enum EnAnimationClip {
		enAnimationClip_Idle,
		enAnimationClip_Walk,
		enAnimationClip_Num
	};
	AnimationClip	m_animationClips[enAnimationClip_Num];
	LevelRender		m_levelRender;
	TestModel*		m_testModel0;
	TestModel* m_testModel1;
	TestModel* m_testModel2;
	Player* m_player = nullptr;
	GameCamera* m_gameCamera;
	SkyCube* m_skyCube;
};


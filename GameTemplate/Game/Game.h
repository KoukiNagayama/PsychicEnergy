#pragma once
#include "LevelRender.h"
#include "SkyCube.h"
#include "WorldRotation.h"
class TestModel;
class Player;
class GameCamera;
class GravityGauge;
class Sight;
class BackGround;

class Game : public IGameObject
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Game();
	/// <summary>
	/// �f�X�X�g���N�^
	/// </summary>
	~Game();	
	/// <summary>
	/// �J�n�����B
	/// �C���X�^���X�������Ɉ�x�������s�B
	/// </summary>
	bool Start();
	/// <summary>
	/// �X�V�����B
	/// </summary>
	void Update();

private:

private:
	LevelRender		m_levelRender;						// ���x�������_�[
	Player*			m_player = nullptr;					// �v���C���[
	GameCamera*		m_gameCamera = nullptr;				// �Q�[���J����
	BackGround*		m_backGround = nullptr;
	SkyCube*		m_skyCube = nullptr;				// �X�J�C�L���[�u
	SpriteRender	m_sprite;
	GravityGauge*	m_gravityGauge = nullptr;
	Sight*			m_sight = nullptr;
};


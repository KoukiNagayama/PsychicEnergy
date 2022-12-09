#pragma once
#include "LevelRender.h"
#include "SkyCube.h"
class TestModel;
class Player;
class GameCamera;

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
	void Render(RenderContext& rc);
private:

private:
	LevelRender		m_levelRender;						// ���x�������_�[
	Player*			m_player = nullptr;					// �v���C���[
	GameCamera*		m_gameCamera = nullptr;				// �Q�[���J����
	TestModel*		m_testModel0 = nullptr;				// �e�X�g���f��1
	TestModel*		m_testModel1 = nullptr;				// �e�X�g���f��2
	TestModel*		m_testModel2 = nullptr;				// �e�X�g���f��3
	SkyCube*		m_skyCube = nullptr;				// �X�J�C�L���[�u
	SpriteRender	m_sprite;
};


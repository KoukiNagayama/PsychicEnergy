#pragma once
#include "LevelRender.h"
#include "SkyCube.h"
#include "WorldRotation.h"
class Player;
class GameCamera;
class GravityGauge;
class Sight;
class BackGround;
class Ring;
class LockOn;
class DisplayGameTimer;
class Fade;
class MainBGM;
/// <summary>
/// �C���Q�[���N���X
/// </summary>
class Game : public IGameObject
{
public:
	Game();
	/// <summary>
	/// �f�X�g���N�^
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
	/// <summary>
	/// �����O���擾
	/// </summary>
	void GetRing()
	{
		m_numOfGetRing++;
	}
	/// <summary>
	/// �C���Q�[���͏I�����Ă��邩
	/// </summary>
	bool& IsFinishedInGame()
	{
		return m_isFinishedInGame;
	}
private:
	/// <summary>
	/// �t�F�[�h�A�E�g�B
	/// </summary>
	void FadeOut();
private:
	LevelRender		m_levelRender;						// ���x�������_�[
	Player*			m_player = nullptr;					// �v���C���[
	GameCamera*		m_gameCamera = nullptr;				// �Q�[���J����
	std::vector<BackGround*> m_backGroundArray;			// �w�i
	SkyCube*		m_skyCube = nullptr;				// �X�J�C�L���[�u						
	GravityGauge*	m_gravityGauge = nullptr;			// �d�̓Q�[�W
	Sight*			m_sight = nullptr;					// �Ə�
	Ring*			m_ring = nullptr;					// �����O
	LockOn*			m_lockOn = nullptr;
	DisplayGameTimer* m_displayGameTimer = nullptr;
	Fade*			m_fade = nullptr;					// �t�F�[�h
	MainBGM*		m_mainBGM = nullptr;				// ���C��BGM
	int				m_maxRing = 0;
	int				m_numOfGetRing = 0;					// �擾���������O�̐�
	bool			m_isFinishedInGame = false;			// �C���Q�[���͏I���������H
};


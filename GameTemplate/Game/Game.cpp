#include "stdafx.h"
#include "Game.h"
#include "Player.h"
#include "GameCamera.h"
#include "GravityGauge.h"
#include "Sight.h"
#include "BackGround.h"
#include "Ring.h"
#include "LockOn.h"
#include "DisplayGameTimer.h"
#include "Result.h"
#include "MainBGM.h"
#include "CommonDataForBackGround.h"
#include "CommonDataForSound.h"
#include "Fade.h"
#include "MainBGM.h"


Game::Game()
{

}

Game::~Game()
{

}

bool Game::Start()
{


	m_levelRender.Init("Assets/level3D/stageLevel_3.tkl", [&](LevelObjectData& objData)
		{
			// �n��
			if (objData.EqualObjectName(L"ground") == true) {
				m_backGround = NewGO<BackGround>(0, "ground");
				m_backGround->SetTypeNum(nsBackGround::enModelType_Ground);
				m_backGround->SetPosition(objData.position);
				m_backGround->SetRotation(objData.rotation);
				m_backGround->SetScale(objData.scale);
				return true;
			}
			// ��1
			else if (objData.ForwardMatchName(L"backGroundModel_box1") == true) {
				m_backGround = NewGO<BackGround>(0, "box1");
				m_backGround->SetTypeNum(nsBackGround::enModelType_Box1);
				m_backGround->SetPosition(objData.position);
				m_backGround->SetRotation(objData.rotation);
				m_backGround->SetScale(objData.scale);
				return true;
			}
			// ��2
			else if (objData.ForwardMatchName(L"backGroundModel_box2") == true) {
				m_backGround = NewGO<BackGround>(0, "box2");
				m_backGround->SetTypeNum(nsBackGround::enModelType_Box2);
				m_backGround->SetPosition(objData.position);
				m_backGround->SetRotation(objData.rotation);
				m_backGround->SetScale(objData.scale);
				return true;
			}
			// ��3
			else if (objData.ForwardMatchName(L"backGroundModel_box3") == true) {
				m_backGround = NewGO<BackGround>(0, "box3");
				m_backGround->SetTypeNum(nsBackGround::enModelType_Box3);
				m_backGround->SetPosition(objData.position);
				m_backGround->SetRotation(objData.rotation);
				m_backGround->SetScale(objData.scale);
				return true;
			}
			// �v���C���[�L�����N�^�[
			else if (objData.EqualObjectName(L"WD") == true) {
				m_player = NewGO<Player>(0, "player");
				m_player->SetPosition(objData.position);
				m_player->SetRotation(objData.rotation);
				m_player->SetScale(objData.scale);
				return true;
			}
			// �����O
			else if (objData.ForwardMatchName(L"ring") == true) {
				m_ring = NewGO<Ring>(0, "ring");
				m_ring->SetPosition(objData.position);
				m_ring->SetRotation(objData.rotation);
				m_ring->SetScale(objData.scale);
				return true;
			}
			return false;
		});
	// �Q�[���J����
	m_gameCamera = NewGO<GameCamera>(0, "gameCamera");
	// �X�J�C�L���[�u
	m_skyCube = NewGO<SkyCube>(0, "skyCube");
	// �d�̓Q�[�W
	m_gravityGauge = NewGO<GravityGauge>(0, "gravityGauge");
	// �Ə�
	m_sight = NewGO<Sight>(0, "sight");
	// ���b�N�I��
	m_lockOn = NewGO<LockOn>(0, "lockOn");
	// �^�C�}�[
	m_displayGameTimer = NewGO<DisplayGameTimer>(0, "gameTimer");
	// �t�F�[�h
	m_fade = NewGO<Fade>(4,"fade");
	m_fade->StartFadeIn();

	m_mainBGM = NewGO<MainBGM>(0, "mainBGM");
	return true;
}

void Game::Update()
{

}

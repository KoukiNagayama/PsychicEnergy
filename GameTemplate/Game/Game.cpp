#include "stdafx.h"
#include "Game.h"
#include "Player.h"
#include "GameCamera.h"
#include "TestModel.h"
#include "GravityGauge.h"
#include "Sight.h"
#include "BackGround.h"

Game::Game()
{

}

Game::~Game()
{

}

bool Game::Start()
{
	m_levelRender.Init("Assets/level3D/stageLevel_2.tkl", [&](LevelObjectData& objData)
		{
			if (objData.EqualObjectName(L"ground") == true) {
				m_backGround = NewGO<BackGround>(0, "ground");
				m_backGround->SetTypeNum(BackGround::enModelType_Ground);
				m_backGround->SetPosition(objData.position);
				m_backGround->SetRotation(objData.rotation);
				m_backGround->SetScale(objData.scale);
				return true;
			}
			else if (objData.ForwardMatchName(L"backGroundModel_box1") == true) {
				m_backGround = NewGO<BackGround>(0, "box1");
				m_backGround->SetTypeNum(BackGround::enModelType_Box1);
				m_backGround->SetPosition(objData.position);
				m_backGround->SetRotation(objData.rotation);
				m_backGround->SetScale(objData.scale);
				return true;
			}
			else if (objData.ForwardMatchName(L"backGroundModel_box2") == true) {
				m_backGround = NewGO<BackGround>(0, "box2");
				m_backGround->SetTypeNum(BackGround::enModelType_Box2);
				m_backGround->SetPosition(objData.position);
				m_backGround->SetRotation(objData.rotation);
				m_backGround->SetScale(objData.scale);
				return true;
			}
			else if (objData.ForwardMatchName(L"backGroundModel_box3") == true) {
				m_backGround = NewGO<BackGround>(0, "box3");
				m_backGround->SetTypeNum(BackGround::enModelType_Box3);
				m_backGround->SetPosition(objData.position);
				m_backGround->SetRotation(objData.rotation);
				m_backGround->SetScale(objData.scale);
				return true;
			}
			else if (objData.EqualObjectName(L"WD") == true) {
				m_player = NewGO<Player>(0, "player");
				m_player->SetPosition(objData.position);
				m_player->SetRotation(objData.rotation);
				m_player->SetScale(objData.scale);
				return true;
			}
			return false;
		});
	m_gameCamera = NewGO<GameCamera>(0, "gameCamera");

	m_skyCube = NewGO<SkyCube>(0, "skyCube");

	m_gravityGauge = NewGO<GravityGauge>(0, "gravityGauge");

	m_sight = NewGO<Sight>(0, "sight");
	return true;
}

void Game::Update()
{

}

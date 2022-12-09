#include "stdafx.h"
#include "Game.h"
#include "Player.h"
#include "GameCamera.h"
#include "TestModel.h"


Game::Game()
{

}

Game::~Game()
{

}

bool Game::Start()
{
	m_levelRender.Init("Assets/level3D/testLevel2.tkl", [&](LevelObjectData& objData)
		{
			if (objData.EqualObjectName(L"ground") == true) {
				m_testModel0 = NewGO<TestModel>(0, "testModel0");
				m_testModel0->SetTypeNum(0);
				m_testModel0->SetPosition(objData.position);
				m_testModel0->SetRotation(objData.rotation);
				m_testModel0->SetScale(objData.scale);
				return true;
			}
			else if (objData.ForwardMatchName(L"testModel1") == true) {
				m_testModel1 = NewGO<TestModel>(0, "testModel1");
				m_testModel1->SetTypeNum(1);
				m_testModel1->SetPosition(objData.position);
				m_testModel1->SetRotation(objData.rotation);
				m_testModel1->SetScale(objData.scale);
				return true;
			}
			else if (objData.ForwardMatchName(L"testModel2") == true) {
				m_testModel2 = NewGO<TestModel>(0, "testModel2");
				m_testModel2->SetTypeNum(2);
				m_testModel2->SetPosition(objData.position);
				m_testModel2->SetRotation(objData.rotation);
				m_testModel2->SetScale(objData.scale);
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

	m_sprite.Init(
		"Assets/sprite/screenEffect_test.DDS",
		1920.0f,
		1080.0f
	);
	return true;
}

void Game::Update()
{

}

void Game::Render(RenderContext& rc)
{
	m_sprite.Draw(rc);
}
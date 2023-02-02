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


Game::Game()
{

}

Game::~Game()
{

}

bool Game::Start()
{
	g_worldRotation = new WorldRotation();

	m_levelRender.Init("Assets/level3D/stageLevel_3.tkl", [&](LevelObjectData& objData)
		{
			// 地面
			if (objData.EqualObjectName(L"ground") == true) {
				m_backGround = NewGO<BackGround>(0, "ground");
				m_backGround->SetTypeNum(BackGround::enModelType_Ground);
				m_backGround->SetPosition(objData.position);
				m_backGround->SetRotation(objData.rotation);
				m_backGround->SetScale(objData.scale);
				return true;
			}
			// 箱1
			else if (objData.ForwardMatchName(L"backGroundModel_box1") == true) {
				m_backGround = NewGO<BackGround>(0, "box1");
				m_backGround->SetTypeNum(BackGround::enModelType_Box1);
				m_backGround->SetPosition(objData.position);
				m_backGround->SetRotation(objData.rotation);
				m_backGround->SetScale(objData.scale);
				return true;
			}
			// 箱2
			else if (objData.ForwardMatchName(L"backGroundModel_box2") == true) {
				m_backGround = NewGO<BackGround>(0, "box2");
				m_backGround->SetTypeNum(BackGround::enModelType_Box2);
				m_backGround->SetPosition(objData.position);
				m_backGround->SetRotation(objData.rotation);
				m_backGround->SetScale(objData.scale);
				return true;
			}
			// 箱3
			else if (objData.ForwardMatchName(L"backGroundModel_box3") == true) {
				m_backGround = NewGO<BackGround>(0, "box3");
				m_backGround->SetTypeNum(BackGround::enModelType_Box3);
				m_backGround->SetPosition(objData.position);
				m_backGround->SetRotation(objData.rotation);
				m_backGround->SetScale(objData.scale);
				return true;
			}
			// プレイヤーキャラクター
			else if (objData.EqualObjectName(L"WD") == true) {
				m_player = NewGO<Player>(0, "player");
				m_player->SetPosition(objData.position);
				m_player->SetRotation(objData.rotation);
				m_player->SetScale(objData.scale);
				return true;
			}
			else if (objData.ForwardMatchName(L"ring") == true) {
				m_ring = NewGO<Ring>(0, "ring");
				m_ring->SetPosition(objData.position);
				m_ring->SetRotation(objData.rotation);
				m_ring->SetScale(objData.scale);
				return true;
			}
			return false;
		});
	// ゲームカメラ
	m_gameCamera = NewGO<GameCamera>(0, "gameCamera");
	// スカイキューブ
	m_skyCube = NewGO<SkyCube>(0, "skyCube");
	// 重力ゲージ
	m_gravityGauge = NewGO<GravityGauge>(0, "gravityGauge");
	// 照準
	m_sight = NewGO<Sight>(0, "sight");
	m_lockOn = NewGO<LockOn>(3, "lockOn");

	m_displayGameTimer = NewGO<DisplayGameTimer>(0, "gameTimer");

	NewGO<Result>(0, "result");

	return true;
}

void Game::Update()
{

}

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
#include "CommonDataForTimer.h"
#include "Fade.h"
#include "MainBGM.h"

namespace
{
	const float SKY_CUBE_SCALE = 1500.0f;		// スカイキューブに設定するスケール
}


Game::Game()
{

}

Game::~Game()
{
	// BGMをフェードアウトさせる
	m_mainBGM->StartFadeOut();
	// 各オブジェクトを削除する。
	for (auto& backGround : m_backGroundArray) {
		DeleteGO(backGround);
	}
	DeleteGO(m_player);
	DeleteGO(m_gameCamera);
	
	const auto& ringArray = FindGOs<Ring>("ring");
	const int arraySize = ringArray.size();
	for (int i = 0; i < arraySize; i++) {
		DeleteGO(ringArray[i]);
	}
	DeleteGO(m_fade);
	DeleteGO(m_skyCube);
	DeleteGO(m_gravityGauge);
	DeleteGO(m_lockOn);
	DeleteGO(m_sight);

	// 登録データの破棄。
	g_worldRotation->DiscardRegisteredData();
}

bool Game::Start()
{


	m_levelRender.Init("Assets/level3D/stageLevel_3.tkl", [&](LevelObjectData& objData)
		{
			// 地面
			if (objData.EqualObjectName(L"ground") == true) {
				auto backGround = NewGO<BackGround>(0, "ground");
				backGround->SetTypeNum(nsBackGround::enModelType_Ground);
				backGround->SetPosition(objData.position);
				backGround->SetRotation(objData.rotation);
				backGround->SetScale(objData.scale);
				m_backGroundArray.push_back(backGround);
				return true;
			}
			// 箱1
			else if (objData.ForwardMatchName(L"backGroundModel_box1") == true) {
				auto backGround = NewGO<BackGround>(0, "box1");
				backGround->SetTypeNum(nsBackGround::enModelType_Box1);
				backGround->SetPosition(objData.position);
				backGround->SetRotation(objData.rotation);
				backGround->SetScale(objData.scale);
				m_backGroundArray.push_back(backGround);
				return true;
			}
			// 箱2
			else if (objData.ForwardMatchName(L"backGroundModel_box2") == true) {
				auto backGround = NewGO<BackGround>(0, "box2");
				backGround->SetTypeNum(nsBackGround::enModelType_Box2);
				backGround->SetPosition(objData.position);
				backGround->SetRotation(objData.rotation);
				backGround->SetScale(objData.scale);
				m_backGroundArray.push_back(backGround);
				return true;
			}
			// 箱3
			else if (objData.ForwardMatchName(L"backGroundModel_box3") == true) {
				auto backGround = NewGO<BackGround>(0, "box3");
				backGround->SetTypeNum(nsBackGround::enModelType_Box3);
				backGround->SetPosition(objData.position);
				backGround->SetRotation(objData.rotation);
				backGround->SetScale(objData.scale);
				m_backGroundArray.push_back(backGround);
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
			// リング
			else if (objData.ForwardMatchName(L"ring") == true) {
				m_ring = NewGO<Ring>(0, "ring");
				m_ring->SetPosition(objData.position);
				m_ring->SetRotation(objData.rotation);
				m_ring->SetScale(objData.scale);
				m_maxRing++;
				return true;
			}
			return false;
		});
	// ゲームカメラ
	m_gameCamera = NewGO<GameCamera>(0, "gameCamera");
	// スカイキューブ
	m_skyCube = NewGO<SkyCube>(0, "skyCube");
	m_skyCube->SetScale(SKY_CUBE_SCALE);
	// 重力ゲージ
	m_gravityGauge = NewGO<GravityGauge>(0, "gravityGauge");
	// 照準
	m_sight = NewGO<Sight>(0, "sight");
	// ロックオン
	m_lockOn = NewGO<LockOn>(0, "lockOn");
	// タイマー
	m_displayGameTimer = NewGO<DisplayGameTimer>(0, "gameTimer");
	// フェード
	m_fade = NewGO<Fade>(4,"fade");
	m_fade->StartFadeIn();

	m_mainBGM = NewGO<MainBGM>(0, "mainBGM");

	g_soundEngine->ResistWaveFileBank(nsSound::enSoundNumber_Acquisition, "Assets/sound/other/acquisition.wav");
	return true;
}

void Game::Update()
{
	FadeOut();

	// 取得したコインが
	if (m_numOfGetRing == m_maxRing
		|| m_displayGameTimer->GetTime() >= nsTimer::MAX_VALUE_OF_TIMER) {
		m_isFinishedInGame = true;
		m_fade->StartFadeOut();
		m_displayGameTimer->Disable();
		g_renderingEngine->SetIsFloating(false);
	}
}

void Game::FadeOut()
{
	if (m_isFinishedInGame) {
		if (!m_fade->IsFade()) {
			NewGO<Result>(0, "result");
			DeleteGO(this);
		}
	}
}

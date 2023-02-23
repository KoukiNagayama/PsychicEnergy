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
#include "CommonDataForPriority.h"


namespace
{
	// 定数
	const float SKY_CUBE_SCALE = 1500.0f;		// スカイキューブに設定するスケール

}


namespace nsPsychicEnergy
{

	namespace nsGame
	{

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

			const auto& ringArray = FindGOs<nsRing::Ring>("ring");
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
			nsK2EngineLow::nsWorldRotation::g_worldRotation->DiscardRegisteredData();
		}

		bool Game::Start()
		{

			m_levelRender.Init("Assets/level3D/stageLevel_3.tkl", [&](LevelObjectData& objData)
				{
					// 地面
					if (objData.EqualObjectName(L"ground") == true) {
						auto backGround = NewGO<nsBackGround::BackGround>(nsPriority::enPriorityFirst, "ground");
						backGround->SetTypeNum(nsBackGround::enModelType_Ground);
						backGround->SetPosition(objData.position);
						backGround->SetRotation(objData.rotation);
						backGround->SetScale(objData.scale);
						m_backGroundArray.push_back(backGround);
						return true;
					}
					// 箱1
					else if (objData.ForwardMatchName(L"backGroundModel_box1") == true) {
						auto backGround = NewGO<nsBackGround::BackGround>(nsPriority::enPriorityFirst, "box1");
						backGround->SetTypeNum(nsBackGround::enModelType_Box1);
						backGround->SetPosition(objData.position);
						backGround->SetRotation(objData.rotation);
						backGround->SetScale(objData.scale);
						m_backGroundArray.push_back(backGround);
						return true;
					}
					// 箱2
					else if (objData.ForwardMatchName(L"backGroundModel_box2") == true) {
						auto backGround = NewGO<nsBackGround::BackGround>(nsPriority::enPriorityFirst, "box2");
						backGround->SetTypeNum(nsBackGround::enModelType_Box2);
						backGround->SetPosition(objData.position);
						backGround->SetRotation(objData.rotation);
						backGround->SetScale(objData.scale);
						m_backGroundArray.push_back(backGround);
						return true;
					}
					// 箱3
					else if (objData.ForwardMatchName(L"backGroundModel_box3") == true) {
						auto backGround = NewGO<nsBackGround::BackGround>(nsPriority::enPriorityFirst, "box3");
						backGround->SetTypeNum(nsBackGround::enModelType_Box3);
						backGround->SetPosition(objData.position);
						backGround->SetRotation(objData.rotation);
						backGround->SetScale(objData.scale);
						m_backGroundArray.push_back(backGround);
						return true;
					}
					// プレイヤーキャラクター
					else if (objData.EqualObjectName(L"WD") == true) {
						m_player = NewGO<nsPlayer::Player>(nsPriority::enPriorityFirst, "player");
						m_player->SetPosition(objData.position);
						m_player->SetRotation(objData.rotation);
						m_player->SetScale(objData.scale);
						return true;
					}
					// リング
					else if (objData.ForwardMatchName(L"ring") == true) {
						m_ring = NewGO<nsRing::Ring>(nsPriority::enPriorityFirst, "ring");
						m_ring->SetPosition(objData.position);
						m_ring->SetRotation(objData.rotation);
						m_ring->SetScale(objData.scale);
						m_maxRing++;
						return true;
					}
					return false;
				});
			// ゲームカメラ
			m_gameCamera = NewGO<nsGameCamera::GameCamera>(nsPriority::enPriorityFirst, "gameCamera");
			
			// スカイキューブ
			m_skyCube = NewGO<SkyCube>(nsPriority::enPriorityFirst, "skyCube");
			m_skyCube->SetScale(SKY_CUBE_SCALE);
			
			// 重力ゲージ
			m_gravityGauge = NewGO<nsGravityGauge::GravityGauge>(nsPriority::enPriorityFirst, "gravityGauge");
			
			// 照準
			m_sight = NewGO<nsSight::Sight>(nsPriority::enPriorityFirst, "sight");
			
			// ロックオン
			m_lockOn = NewGO<nsLockOn::LockOn>(nsPriority::enPriorityFirst, "lockOn");
			
			// タイマー
			m_displayGameTimer = NewGO<nsTimer::DisplayGameTimer>(nsPriority::enPriorityFirst, "gameTimer");
			
			// フェード
			m_fade = NewGO<nsFade::Fade>(nsPriority::enPrioritySecond, "fade");
			m_fade->StartFadeIn();
			
			// メインBGM
			m_mainBGM = NewGO<nsSound::MainBGM>(nsPriority::enPriorityFirst, "mainBGM");
			
			// リングの取得音のwaveファイルを登録する。
			g_soundEngine->ResistWaveFileBank(nsSound::enSoundNumber_Acquisition, "Assets/sound/other/acquisition.wav");
			
			return true;
		}

		void Game::Update()
		{
			// フェードアウトさせる。
			FadeOut();

			// 取得したコインが最大数あるかタイマーが終了しているならば
			if (m_numOfGetRing == m_maxRing
				|| m_displayGameTimer->GetTime() >= nsTimer::MAX_VALUE_OF_TIMER) {
				
				// インゲームを終了するための準備をする。
				m_isFinishedInGame = true;
				m_fade->StartFadeOut();
				m_displayGameTimer->Disable();
				g_renderingEngine->SetIsFloating(false);
			}
		}

		void Game::FadeOut()
		{
			// インゲームが終了しているならばフェードアウトさせる
			if (m_isFinishedInGame) {
				if (!m_fade->IsFade()) {
					NewGO<nsResult::Result>(0, "result");
					DeleteGO(this);
				}
			}
		}

	}
}

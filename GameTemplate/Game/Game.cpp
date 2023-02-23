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
	// �萔
	const float SKY_CUBE_SCALE = 1500.0f;		// �X�J�C�L���[�u�ɐݒ肷��X�P�[��

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
			// BGM���t�F�[�h�A�E�g������
			m_mainBGM->StartFadeOut();
			// �e�I�u�W�F�N�g���폜����B
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

			// �o�^�f�[�^�̔j���B
			nsK2EngineLow::nsWorldRotation::g_worldRotation->DiscardRegisteredData();
		}

		bool Game::Start()
		{

			m_levelRender.Init("Assets/level3D/stageLevel_3.tkl", [&](LevelObjectData& objData)
				{
					// �n��
					if (objData.EqualObjectName(L"ground") == true) {
						auto backGround = NewGO<nsBackGround::BackGround>(nsPriority::enPriorityFirst, "ground");
						backGround->SetTypeNum(nsBackGround::enModelType_Ground);
						backGround->SetPosition(objData.position);
						backGround->SetRotation(objData.rotation);
						backGround->SetScale(objData.scale);
						m_backGroundArray.push_back(backGround);
						return true;
					}
					// ��1
					else if (objData.ForwardMatchName(L"backGroundModel_box1") == true) {
						auto backGround = NewGO<nsBackGround::BackGround>(nsPriority::enPriorityFirst, "box1");
						backGround->SetTypeNum(nsBackGround::enModelType_Box1);
						backGround->SetPosition(objData.position);
						backGround->SetRotation(objData.rotation);
						backGround->SetScale(objData.scale);
						m_backGroundArray.push_back(backGround);
						return true;
					}
					// ��2
					else if (objData.ForwardMatchName(L"backGroundModel_box2") == true) {
						auto backGround = NewGO<nsBackGround::BackGround>(nsPriority::enPriorityFirst, "box2");
						backGround->SetTypeNum(nsBackGround::enModelType_Box2);
						backGround->SetPosition(objData.position);
						backGround->SetRotation(objData.rotation);
						backGround->SetScale(objData.scale);
						m_backGroundArray.push_back(backGround);
						return true;
					}
					// ��3
					else if (objData.ForwardMatchName(L"backGroundModel_box3") == true) {
						auto backGround = NewGO<nsBackGround::BackGround>(nsPriority::enPriorityFirst, "box3");
						backGround->SetTypeNum(nsBackGround::enModelType_Box3);
						backGround->SetPosition(objData.position);
						backGround->SetRotation(objData.rotation);
						backGround->SetScale(objData.scale);
						m_backGroundArray.push_back(backGround);
						return true;
					}
					// �v���C���[�L�����N�^�[
					else if (objData.EqualObjectName(L"WD") == true) {
						m_player = NewGO<nsPlayer::Player>(nsPriority::enPriorityFirst, "player");
						m_player->SetPosition(objData.position);
						m_player->SetRotation(objData.rotation);
						m_player->SetScale(objData.scale);
						return true;
					}
					// �����O
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
			// �Q�[���J����
			m_gameCamera = NewGO<nsGameCamera::GameCamera>(nsPriority::enPriorityFirst, "gameCamera");
			
			// �X�J�C�L���[�u
			m_skyCube = NewGO<SkyCube>(nsPriority::enPriorityFirst, "skyCube");
			m_skyCube->SetScale(SKY_CUBE_SCALE);
			
			// �d�̓Q�[�W
			m_gravityGauge = NewGO<nsGravityGauge::GravityGauge>(nsPriority::enPriorityFirst, "gravityGauge");
			
			// �Ə�
			m_sight = NewGO<nsSight::Sight>(nsPriority::enPriorityFirst, "sight");
			
			// ���b�N�I��
			m_lockOn = NewGO<nsLockOn::LockOn>(nsPriority::enPriorityFirst, "lockOn");
			
			// �^�C�}�[
			m_displayGameTimer = NewGO<nsTimer::DisplayGameTimer>(nsPriority::enPriorityFirst, "gameTimer");
			
			// �t�F�[�h
			m_fade = NewGO<nsFade::Fade>(nsPriority::enPrioritySecond, "fade");
			m_fade->StartFadeIn();
			
			// ���C��BGM
			m_mainBGM = NewGO<nsSound::MainBGM>(nsPriority::enPriorityFirst, "mainBGM");
			
			// �����O�̎擾����wave�t�@�C����o�^����B
			g_soundEngine->ResistWaveFileBank(nsSound::enSoundNumber_Acquisition, "Assets/sound/other/acquisition.wav");
			
			return true;
		}

		void Game::Update()
		{
			// �t�F�[�h�A�E�g������B
			FadeOut();

			// �擾�����R�C�����ő吔���邩�^�C�}�[���I�����Ă���Ȃ��
			if (m_numOfGetRing == m_maxRing
				|| m_displayGameTimer->GetTime() >= nsTimer::MAX_VALUE_OF_TIMER) {
				
				// �C���Q�[�����I�����邽�߂̏���������B
				m_isFinishedInGame = true;
				m_fade->StartFadeOut();
				m_displayGameTimer->Disable();
				g_renderingEngine->SetIsFloating(false);
			}
		}

		void Game::FadeOut()
		{
			// �C���Q�[�����I�����Ă���Ȃ�΃t�F�[�h�A�E�g������
			if (m_isFinishedInGame) {
				if (!m_fade->IsFade()) {
					NewGO<nsResult::Result>(0, "result");
					DeleteGO(this);
				}
			}
		}

	}
}

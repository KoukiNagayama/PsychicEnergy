#include "stdafx.h"
#include "Title.h"
#include "Game.h"
#include "TitleBackGround.h"
#include "TitleCharacter.h"
#include "TitleCamera.h"
#include "TitleSprite.h"
#include "CommonDataForBackGround.h"
#include "Fade.h"
#include "CommonDataForSound.h"
#include "sound/SoundEngine.h"
#include "TitleBGM.h"

namespace
{
	const float		ADD_ALPHA_VALUE_AT_FADE_IN = 0.002f;				// �t�F�[�h�C�����̃��l�̑�����
	const float		ADD_ALPHA_VALUE_AT_FADE_OUT = -0.003f;				// �t�F�[�h�A�E�g���̃��l�̑�����
	const Vector3	TITLE_SPRITE_MUL_COLOR_RGB = { 1.0f, 1.0f, 1.0f };	// �^�C�g���X�v���C�g�̏�Z����J���[��RGB�v�f
	const float		MAX_ALPHA = 1.0f;									// ���l�̍ő�l
	const float		MIN_ALPHA = 0.0f;									// ���l�̍ŏ��l
	const float		TITLE_SPRITE_WIDTH = 1920.0f;						// �^�C�g���X�v���C�g�̉���
	const float		TITLE_SPRITE_HEIGHT = 1080.0f;						// �^�C�g���X�v���C�g�̏c��
}

Title::~Title()
{
	// BGM�̓t�F�[�h�A�E�g������B
	m_titleBGM->StartFadeOut();

	// �I�u�W�F�N�g���폜�B
	DeleteGO(m_titleSprite);
	DeleteGO(m_titleCharacter);
	DeleteGO(m_titleCamera);
	for (auto titleBackGround : m_titleBackGroundArray) {
		DeleteGO(titleBackGround);
	}
	DeleteGO(m_skyCube);

	DeleteGO(m_fade);

}

bool Title::Start()
{


	m_titleLevel.Init("Assets/level3D/titleLevel.tkl", [&](LevelObjectData& objData)
		{
			// �n��
			if (objData.EqualObjectName(L"ground") == true) {
				auto titleBackGround = NewGO<TitleBackGround>(0, "ground");
				titleBackGround->SetTypeNum(nsBackGround::enModelType_Ground);
				titleBackGround->SetPosition(objData.position);
				titleBackGround->SetRotation(objData.rotation);
				titleBackGround->SetScale(objData.scale);
				m_titleBackGroundArray.push_back(titleBackGround);
				return true;
			}
			// ��1
			else if (objData.ForwardMatchName(L"backGroundModel_box1") == true) {
				auto titleBackGround = NewGO<TitleBackGround>(0, "box1");
				titleBackGround->SetTypeNum(nsBackGround::enModelType_Box1);
				titleBackGround->SetPosition(objData.position);
				titleBackGround->SetRotation(objData.rotation);
				titleBackGround->SetScale(objData.scale);
				m_titleBackGroundArray.push_back(titleBackGround);
				return true;
			}
			// ��2
			else if (objData.ForwardMatchName(L"backGroundModel_box2") == true) {
				auto titleBackGround = NewGO<TitleBackGround>(0, "box2");
				titleBackGround->SetTypeNum(nsBackGround::enModelType_Box2);
				titleBackGround->SetPosition(objData.position);
				titleBackGround->SetRotation(objData.rotation);
				titleBackGround->SetScale(objData.scale);
				m_titleBackGroundArray.push_back(titleBackGround);
				return true;
			}
			// ��3
			else if (objData.ForwardMatchName(L"backGroundModel_box3") == true) {
				auto titleBackGround = NewGO<TitleBackGround>(0, "box3");
				titleBackGround->SetTypeNum(nsBackGround::enModelType_Box3);
				titleBackGround->SetPosition(objData.position);
				titleBackGround->SetRotation(objData.rotation);
				titleBackGround->SetScale(objData.scale);
				m_titleBackGroundArray.push_back(titleBackGround);
				return true;
			}
			// �L�����N�^�[
			else if (objData.EqualObjectName(L"WD") == true) {
				m_titleCharacter = NewGO<TitleCharacter>(0, "titleCharacter");
				m_titleCharacter->SetPosition(objData.position);
				m_titleCharacter->SetRotation(objData.rotation);
				m_titleCharacter->SetScale(objData.scale);
				return true;
			}
			return false;
		});
	// �^�C�g���p�J����
	m_titleCamera = NewGO<TitleCamera>(0, "titleCamera");
	// �^�C�g���p�X�v���C�g
	m_titleSprite = NewGO<TitleSprite>(0, "titleSprite");

	// �X�J�C�L���[�u
	m_skyCube = NewGO<SkyCube>(0, "skyCube");

	// �t�F�[�h
	m_fade = NewGO<Fade>(0, "fade");
	// �t�F�[�h�C�����J�n����
	m_fade->StartFadeIn();
	// �^�C�g��BGM
	m_titleBGM = NewGO<TitleBGM>(0, "titleBGM");

	g_soundEngine->ResistWaveFileBank(nsSound::enSoundNumber_Decision, "Assets/sound/other/decision.wav");
	return true;
}

void Title::Update()
{
	if (!m_isPushedB && g_pad[0]->IsTrigger(enButtonB)) {
		m_isPushedB = true;
		m_decisionSound = NewGO<SoundSource>(0, "decisionSound");
		m_decisionSound->Init(nsSound::enSoundNumber_Decision);
		m_decisionSound->SetVolume(nsSound::DECISION_VOLUME);
		m_decisionSound->Play(false);
	}
	FadeOut();

}

void Title::FadeOut()
{
	if (m_isWaitFadeout) {
		if (!m_fade->IsFade()) {
			NewGO<Game>(0, "game");
			DeleteGO(this);
		}
	}
	else {
		m_isWaitFadeout = m_titleCamera->IsStartFadeOut();
		if (m_isWaitFadeout) {
			m_fade->StartFadeOut();
		}
	}
}

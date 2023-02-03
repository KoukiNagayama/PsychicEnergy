#pragma once
#include "LevelRender.h"
#include "SkyCube.h"
#include "Fade.h"
#include "sound/SoundSource.h"

class TitleBackGround;
class TitleCharacter;
class TitleCamera;
class TitleSprite;
class Fade;
class TitleBGM;
/// <summary>
/// �^�C�g���N���X
/// </summary>
class Title : public IGameObject
{
public:
	Title() {}
	///<summary>
	/// �f�X�g���N�^
	///</summary>
	~Title();

	bool Start();
	void Update();
	/// <summary>
	/// B�{�^���͉����ꂽ��
	/// </summary>
	bool& IsPushedB()
	{
		return m_isPushedB;
	}
	/// <summary>
	/// �t�F�[�h�A�E�g���J�n
	/// </summary>
	void StartFadeOut()
	{
		m_isFadeOut = true;
	}
private:
	/// <summary>
	/// �t�F�[�h�A�E�g
	/// </summary>
	void FadeOut();

private:
	LevelRender			m_titleLevel;							// �^�C�g���̃��x��
	std::vector<TitleBackGround*> m_titleBackGroundArray;		// �^�C�g���Ŏg�p����w�i���f��
	TitleCharacter*		m_titleCharacter = nullptr;				// �^�C�g���Ŏg�p����L�����N�^�[���f��
	TitleCamera*		m_titleCamera = nullptr;				// �^�C�g���Ŏg�p����J����
	TitleSprite*		m_titleSprite = nullptr;				// �^�C�g���Ŏg�p����X�v���C�g
	SpriteRender		m_titleTextSprite;						// �^�C�g���Ɏg�p����e�L�X�g�̃X�v���C�g
	float				m_currentAlpha = 0.0f;					// �X�v���C�g�̏�Z���錻�݂̃��l
	SkyCube*			m_skyCube = nullptr;					// �X�J�C�L���[�u
	Fade*				m_fade = nullptr;						// �t�F�[�h
	bool				m_isWaitFadeout = false;				// �t�F�[�h�A�E�g��ҋ@���Ă��邩
	bool				m_isFadeOut = false;
	TitleBGM*			m_titleBGM = nullptr;
	SoundSource*		m_decisionSound = nullptr;				// ���艹
	bool				m_isPushedB = false;					// B�{�^���͉����ꂽ��

};


#pragma once
class Fade;
class DisplayGameTimer;
class ResultBGM;
/// <summary>
/// ���U���g�N���X
/// </summary>
class Result : public IGameObject
{
public:
	Result() {}
	~Result();

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	bool Start();
	/// <summary>
	/// �X�V����
	/// </summary>
	void Update();
	/// <summary>
	/// �`�揈��
	/// </summary>
	/// <param name="rc"></param>
	void Render(RenderContext& rc);
private:
	void FadeOut();
private:
	enum EnResultState
	{
		enResultState_GameClear,
		enResultState_TimeUp
	};
	EnResultState		m_resultState;
	SpriteRender		m_resultBGSprite;
	int					m_recordedTime = -1;		// �L�^���ꂽ����
	float				m_pushDeltaTime = 10000.0f;	
	bool				m_disable = true;
	Fade*				m_fade = nullptr;			// �t�F�[�h
	bool				m_isWaitFadeout = false;
	SoundSource*		m_decisionSound = nullptr;
	DisplayGameTimer*	m_displayGameTimer = nullptr;
	ResultBGM*			m_resultBGM = nullptr;
};


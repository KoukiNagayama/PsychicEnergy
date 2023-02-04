#pragma once
class Fade;
class DisplayGameTimer;
class ResultBGM;
class ResultSprite;
/// <summary>
/// ���U���g�N���X
/// </summary>
class Result : public IGameObject
{
public:
	enum EnResultState
	{
		enResultState_GameClear,
		enResultState_TimeUp
	};
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
	/// <summary>
	/// ���U���g�̃X�e�[�g���擾�B
	/// </summary>
	EnResultState& GetResultState()
	{
		return m_resultState;
	}
	int& GetRecordedTime()
	{
		return m_recordedTime;
	}
private:
	void FadeOut();
private:

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
	ResultSprite*		m_resultSprite = nullptr;
};


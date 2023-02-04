#pragma once
class Fade;
class DisplayGameTimer;
class ResultBGM;
class ResultSprite;
/// <summary>
/// リザルトクラス
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
	/// 更新処理
	/// </summary>
	void Update();
	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="rc"></param>
	void Render(RenderContext& rc);
	/// <summary>
	/// リザルトのステートを取得。
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
	int					m_recordedTime = -1;		// 記録された時間
	float				m_pushDeltaTime = 10000.0f;	
	bool				m_disable = true;
	Fade*				m_fade = nullptr;			// フェード
	bool				m_isWaitFadeout = false;
	SoundSource*		m_decisionSound = nullptr;
	DisplayGameTimer*	m_displayGameTimer = nullptr;
	ResultBGM*			m_resultBGM = nullptr;
	ResultSprite*		m_resultSprite = nullptr;
};


#pragma once
class Fade;
class DisplayGameTimer;
class ResultBGM;
/// <summary>
/// リザルトクラス
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
	/// 更新処理
	/// </summary>
	void Update();
	/// <summary>
	/// 描画処理
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
	int					m_recordedTime = -1;		// 記録された時間
	float				m_pushDeltaTime = 10000.0f;	
	bool				m_disable = true;
	Fade*				m_fade = nullptr;			// フェード
	bool				m_isWaitFadeout = false;
	SoundSource*		m_decisionSound = nullptr;
	DisplayGameTimer*	m_displayGameTimer = nullptr;
	ResultBGM*			m_resultBGM = nullptr;
};


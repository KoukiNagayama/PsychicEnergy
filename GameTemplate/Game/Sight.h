#pragma once
class Player;
class Sight : public IGameObject
{
public:
	Sight() {}
	~Sight() {}

	/// <summary>
	/// 開始処理。
	/// インスタンス生成時に初期化を行う。trueを返したときに呼ばれなくなる。
	/// </summary>
	bool Start();
	/// <summary>
	/// 更新処理。
	/// </summary>
	void Update();
	/// <summary>
	/// 描画処理。
	/// </summary>
	/// <param name="rc"></param>
	void Render(RenderContext& rc);
private:

private:
	enum enAlphaState
	{
		enAlphaState_Increase,
		enAlphaState_Decrease,
		enAlphaState_NoChange
	};
	SpriteRender		m_spriteRender;			// スプライトレンダー
	float				m_currentAlpha = 0.0f;	// 現在のα値
	Player*				m_player = nullptr;		// プレイヤー
};


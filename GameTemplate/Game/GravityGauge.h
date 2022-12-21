#pragma once
class Player;
class GravityGauge : public IGameObject
{
public:
	GravityGauge() {}
	~GravityGauge() {}
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
	/// <param name="rc">レンダリングコンテキスト</param>
	void Render(RenderContext& rc);
	
	// ↓どちらの単位が使い勝手が良いか分からないため両方用意
	/// <summary>
	/// Radian単位のゲージの表示範囲の角度を取得
	/// </summary>
	/// <returns></returns>
	float GetDisplayAreaAngleRad() const
	{
		return m_displayAreaRad;
	}
	/// <summary>
	/// Degree単位のゲージの表示範囲の角度を取得
	/// </summary>
	/// <returns></returns>
	float GetDisplayAreaAngleDeg() const
	{
		float deg = Math::RadToDeg(m_displayAreaRad);
		return deg;
	}
private:
	/// <summary>
	/// 表示する範囲を計算する。
	/// </summary>
	void CalcDisplayArea(bool isFloating);
private:
	SpriteRender	m_gaugeSprite;						// ゲージのスプライト
	SpriteRender	m_gaugeFrameSprite;					// ゲージの枠のスプライト
	float			m_displayAreaRad = Math::PI2;		// 表示範囲
	Player*			m_player = nullptr;					// プレイヤークラス
};


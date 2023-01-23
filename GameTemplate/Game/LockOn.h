#pragma once
class Ring;
class Player;
/// <summary>
/// ロックオン
/// </summary>
class LockOn : public IGameObject
{
public:
	LockOn() {}
	~LockOn() {}
	/// <summary>
	/// 開始処理。
	/// インスタンス生成時に初期化を行う。trueを返したときに呼ばれなくなる。
	/// </summary>
	bool Start();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Render(RenderContext& rc);
private:
	/// <summary>
	/// 目標を決める
	/// </summary>
	void DecideTarget();
	/// <summary>
	/// スプライトの座標を計算する
	/// </summary>
	void CalculatePositionOfSprite();
private:
	SpriteRender				m_lockOnSprite;					// ロックオン時に使用する画像
	std::vector<Ring*>			m_ringArray;					// リングの配列
	Player*						m_player = nullptr;				// プレイヤー
	bool						m_isLockOn = false;				// ロックオンしている？
	Vector3						m_targetPos = Vector3::Zero;	// 目標となる座標
	Ring*						m_targetRing = nullptr;			// 目標となるリング
	float						m_minRatingPoint = 10000.0f;	// 最小の評価点
	Vector2						m_screenPos = Vector2::Zero;	// スプライトを表示するスクリーン座標

};	


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
	/// 目標が視界内にあるか識別する
	/// </summary>
	void IdentifyIfTargetIsInView();
	/// <summary>
	/// 矢印用スプライトのスクリーン座標を計算する
	/// </summary>
	void CalculateScreenPositionOfSpriteForArrow();
	/// <summary>
	/// どの端に近いか求める
	/// </summary>
	void SeekWhichEdgeIsClose();
private:
	SpriteRender				m_lockOnSprite;						// ロックオン時に使用する画像
	SpriteRender				m_arrowSprite;						// 矢印の画像
	std::vector<Ring*>			m_ringArray;						// リングの配列
	Player*						m_player = nullptr;					// プレイヤー
	bool						m_isLockOn = false;					// ロックオンしている？
	Vector3						m_targetPos = Vector3::Zero;		// 目標となる座標
	Ring*						m_targetRing = nullptr;				// 目標となるリング
	float						m_minRatingPoint = 10000.0f;		// 最小の評価点
	Vector2						m_screenPos = Vector2::Zero;		// スプライトを表示するスクリーン座標
	bool						m_isTargetInView = false;			// 視界内にあるか？
	Vector3						m_camForward = Vector3::AxisZ;		// カメラの正面方向
	bool						m_isBehind = false;
	bool						m_isNearRightEdge = false;
	bool						m_isDisable = false;
	Vector3						m_playerToTargetPos = Vector3::Zero;
};	


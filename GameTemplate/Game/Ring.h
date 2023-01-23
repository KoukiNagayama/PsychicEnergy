#pragma once
class Player;
class Ring : public IGameObject
{
public:
	Ring() {}
	~Ring() {}

	bool Start();
	void Update();
	void Render(RenderContext& rc);
	/// <summary>
	/// 座標を設定。
	/// </summary>
	/// <param name="position">設定する座標。</param>
	void SetPosition(Vector3& position)
	{
		m_position = position;
	}
	/// <summary>
	/// 回転を設定。
	/// </summary>
	/// <param name="rotation">設定する回転値。</param>
	void SetRotation(Quaternion& rotation)
	{
		m_rotation = rotation;
	}
	/// <summary>
	/// 拡大率を設定。
	/// </summary>
	/// <param name="scale">設定する拡大率。</param>
	void SetScale(Vector3& scale)
	{
		m_scale = scale;
	}
	/// <summary>
	/// 座標を取得。
	/// </summary>
	/// <returns>座標。</returns>
	const Vector3& GetPosition() const
	{
		return m_position;
	}
	ModelRender& GetModelRender()
	{
		return m_modelRender;
	}
	inline void SetWorldMatrixWithLerp(const Matrix& worldMat)
	{
		m_prevMatrix = m_modelRender.GetWorldMatrix();
		m_nextMatrix = worldMat;
		m_rotateTimer = 0.0f;
	}
private:
	/// <summary>
	/// 回転
	/// </summary>
	void Rotation();
	/// <summary>
	/// 当たり判定
	/// </summary>
	void Collision();
	/// <summary>
	/// プレイヤーとの距離を計算
	/// </summary>
	float CalcDistanceToPlayer();
private:
	ModelRender				m_modelRender;				// モデルレンダー
	Vector3					m_position;					// 座標
	Vector3					m_scale;					// 拡大率
	Quaternion				m_rotation;					// 回転
	Player*					m_player = nullptr;			// プレイヤー
	Matrix					m_prevMatrix = Matrix::Identity;
	Matrix					m_nextMatrix = Matrix::Identity;
	float					m_rotateTimer = 1.0f;
};


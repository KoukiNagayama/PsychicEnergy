#pragma once
class Title;
class TitleCharacter : public IGameObject
{
public:
	TitleCharacter() {}
	~TitleCharacter();

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
private:
	/// <summary>
	/// アニメーションを再生。
	/// </summary>
	void PlayAnimation();
private:
	enum EnAnimationClip {
		enAnimationClip_Idle,		// 待機
		enAnimationClip_IdleAir,	// 空中での待機
		enAnimationClip_FallAir,	// 空中での落下
		enAnimationClip_Num			// アニメーションクリップの数
	};
	ModelRender			m_modelRender;							// モデルレンダー
	Vector3				m_position = Vector3::Zero;				// 座標
	Quaternion			m_rotation = Quaternion::Identity;		// 回転
	Vector3				m_scale = Vector3::One;					// 拡大率
	AnimationClip		m_animationClips[enAnimationClip_Num];	// アニメーションクリップ
	EnAnimationClip		m_currentAnimationClip = enAnimationClip_Idle;
	bool				m_isFloating = false;
	float				m_floatingCount = 0.0f;
	Title*				m_title = nullptr;						// タイトルクラス
	bool				m_isCompletedStateChange = false;
};


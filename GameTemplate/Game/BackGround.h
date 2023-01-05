#pragma once
#include "WorldRotation.h"

class Player;
/// <summary>
/// 背景モデルクラス
/// </summary>
class BackGround : public IGameObject
{
public:
	BackGround() {}
	~BackGround() {}
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
	/// モデルのタイプを設定。
	/// </summary>
	/// <param name="num">設定するタイプの番号。</param>
	void SetTypeNum(int num)
	{
		m_typeNum = num;
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
public:
	enum enModelType
	{
		enModelType_Ground,		// 地面
		enModelType_Box1,		// 箱1
		enModelType_Box2,		// 箱2
		enModelType_Box3		// 箱3
	};
private:
	ModelRender				m_modelRender;				// モデルレンダー
	Vector3					m_position;					// 座標
	Vector3					m_scale;					// 拡大率
	Quaternion				m_rotation;					// 回転
	int						m_typeNum;					// モデルのタイプ
	PhysicsStaticObject		m_physicsStaticObject;		// 静的物理オブジェクト
	Matrix					m_worldMatrix = Matrix::Identity;
	Player*					m_player = nullptr;
	Matrix					m_prevMatrix = Matrix::Identity;
	Matrix					m_nextMatrix = Matrix::Identity;
	float					m_rotateTimer = 1.0f;

};


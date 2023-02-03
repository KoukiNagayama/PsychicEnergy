#pragma once
#include "CommonDataForBackGround.h"
class TitleBackGround : public IGameObject
{
public:
	TitleBackGround() {}
	~TitleBackGround() {}

	bool Start();
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
private:
	ModelRender			m_modelRender;						// モデルレンダー
	Vector3				m_position = Vector3::Zero;			// 座標
	Vector3				m_scale = Vector3::One;				// 拡大率
	Quaternion			m_rotation = Quaternion::Identity;	// 回転
	int					m_typeNum = -1;						// モデルのタイプ番号
};


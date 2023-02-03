#pragma once
class Title;
class TitleCharacter;
/// <summary>
/// タイトルカメラクラス
/// </summary>
class TitleCamera : public IGameObject
{
public:
	TitleCamera() {}
	~TitleCamera() {}
	/// <summary>
	/// 開始処理。
	/// インスタンス生成時に初期化を行う。trueを返したときに呼ばれなくなる。
	/// </summary>
	bool Start();
	/// <summary>
	/// 更新処理。
	/// </summary>
	void Update();
	bool IsStartFadeOut()
	{
		return m_isStartFadeOut;
	}
private:
	Title*			m_title = nullptr;								// タイトルクラス
	TitleCharacter* m_titleCharacter = nullptr;						// タイトルキャラクタークラス
	Vector3			m_position = Vector3::Zero;						// カメラの座標
	Quaternion		m_rotation = Quaternion::Identity;				// カメラの回転
	Vector3			m_toCameraPos = Vector3::Zero;					// 注視点から視点へのベクトル
	Vector3			m_charaPos = Vector3::Zero;
	bool			m_isStartFadeOut = false;
};


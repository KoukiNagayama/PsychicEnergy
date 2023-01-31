#pragma once
/// <summary>
/// リザルトクラス
/// </summary>
class Result : public IGameObject
{
public:
	Result() {}
	~Result() {}

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

private:
	int		m_recordedTime = -1;		// 記録された時間

};


#pragma once
/// <summary>
/// ���U���g�N���X
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
	/// �X�V����
	/// </summary>
	void Update();
	/// <summary>
	/// �`�揈��
	/// </summary>
	/// <param name="rc"></param>
	void Render(RenderContext& rc);
private:

private:
	int		m_recordedTime = -1;		// �L�^���ꂽ����

};


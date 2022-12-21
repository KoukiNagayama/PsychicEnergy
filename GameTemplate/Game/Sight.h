#pragma once
class Player;
class Sight : public IGameObject
{
public:
	Sight() {}
	~Sight() {}

	/// <summary>
	/// �J�n�����B
	/// �C���X�^���X�������ɏ��������s���Btrue��Ԃ����Ƃ��ɌĂ΂�Ȃ��Ȃ�B
	/// </summary>
	bool Start();
	/// <summary>
	/// �X�V�����B
	/// </summary>
	void Update();
	/// <summary>
	/// �`�揈���B
	/// </summary>
	/// <param name="rc"></param>
	void Render(RenderContext& rc);
private:

private:
	enum enAlphaState
	{
		enAlphaState_Increase,
		enAlphaState_Decrease,
		enAlphaState_NoChange
	};
	SpriteRender		m_spriteRender;			// �X�v���C�g�����_�[
	float				m_currentAlpha = 0.0f;	// ���݂̃��l
	Player*				m_player = nullptr;		// �v���C���[
};


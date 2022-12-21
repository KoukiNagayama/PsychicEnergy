#pragma once
class Player;
class GravityGauge : public IGameObject
{
public:
	GravityGauge() {}
	~GravityGauge() {}
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
	/// <param name="rc">�����_�����O�R���e�L�X�g</param>
	void Render(RenderContext& rc);
	
	// ���ǂ���̒P�ʂ��g�����肪�ǂ���������Ȃ����ߗ����p��
	/// <summary>
	/// Radian�P�ʂ̃Q�[�W�̕\���͈͂̊p�x���擾
	/// </summary>
	/// <returns></returns>
	float GetDisplayAreaAngleRad() const
	{
		return m_displayAreaRad;
	}
	/// <summary>
	/// Degree�P�ʂ̃Q�[�W�̕\���͈͂̊p�x���擾
	/// </summary>
	/// <returns></returns>
	float GetDisplayAreaAngleDeg() const
	{
		float deg = Math::RadToDeg(m_displayAreaRad);
		return deg;
	}
private:
	/// <summary>
	/// �\������͈͂��v�Z����B
	/// </summary>
	void CalcDisplayArea(bool isFloating);
private:
	SpriteRender	m_gaugeSprite;						// �Q�[�W�̃X�v���C�g
	SpriteRender	m_gaugeFrameSprite;					// �Q�[�W�̘g�̃X�v���C�g
	float			m_displayAreaRad = Math::PI2;		// �\���͈�
	Player*			m_player = nullptr;					// �v���C���[�N���X
};


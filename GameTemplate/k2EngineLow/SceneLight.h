#pragma once
#include "MyRenderer.h"

namespace nsK2EngineLow
{
	const int NUM_DIRECTIONAL_LIGHT = 4;
	/// <summary>
	/// �f�B���N�V�������C�g
	/// </summary>
	struct DirectionalLight
	{
		Vector3 direction;		// ���C�g�̕���
		int		pad;			// �p�f�B���O
		Vector4 color;			// ���C�g�̃J���[
	};

	/// <summary>
	/// ���C�g�\����
	/// </summary>
	struct Light
	{
		DirectionalLight directionalLight[MAX_DIRECTIONAL_LIGHT];		// �f�B���N�V�������C�g
		Vector3 eyePos;							// �J�����̈ʒu
		float specPow;							// �X�y�L�����̍i��
		Vector3 ambientLight;					// ����
		float pad;
	};

	/// <summary>
	/// �V�[�����C�g�N���X
	/// </summary>
	class SceneLight : public Noncopyable
	{
	public:
		/// <summary>
		/// �������B
		/// </summary>
		void Init();
		/// <summary>
		/// �X�V�B
		/// </summary>
		void Update();
		/// <summary>
		/// ���C�g���擾�B
		/// </summary>
		/// <returns></returns>
		Light& GetLightData()
		{
			return m_light;
		}
	private:
		Light m_light;					// ���C�g
	};
}



#include "k2EngineLowPreCompile.h"
#include "SceneLight.h"

namespace nsK2EngineLow
{
	void SceneLight::Init()
	{
		// �f�B���N�V�������C�g�̏�����
		m_light.directionalLight[0].direction.x = -0.577f;
		m_light.directionalLight[0].direction.y = -0.577f;
		m_light.directionalLight[0].direction.z = -0.577f;
		m_light.directionalLight[0].direction.Normalize();

		m_light.directionalLight[0].color.x = 3.0f;
		m_light.directionalLight[0].color.y = 3.0f;
		m_light.directionalLight[0].color.z = 3.0f;

		m_light.directionalLight[1].direction.x = 0.0f;
		m_light.directionalLight[1].direction.y = 0.0f;
		m_light.directionalLight[1].direction.z = -1.0f;
		m_light.directionalLight[1].direction.Normalize();

		m_light.directionalLight[1].color.x = 1.5f;
		m_light.directionalLight[1].color.y = 1.5f;
		m_light.directionalLight[1].color.z = 1.5f;


		// �J�����̈ʒu�̏�����
		m_light.eyePos = g_camera3D->GetPosition();
		
		// �X�y�L�����̍i��̏�����
		m_light.specPow = 5.0f;

		// �����̏�����
		m_light.ambientLight.x = 0.3f;
		m_light.ambientLight.y = 0.3f;
		m_light.ambientLight.z = 0.3f;
	}

	void SceneLight::Update()
	{
		m_light.eyePos = g_camera3D->GetPosition();
	}
}

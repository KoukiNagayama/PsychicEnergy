#include "k2EngineLowPreCompile.h"
#include "SkyCube.h"

namespace nsK2EngineLow {

	SkyCube::SkyCube()
	{
		/*m_textureFilePaths[enSkyCubeType_Day] = L"Assets/modelData/sky/skyCubeMap.dds";
		m_textureFilePaths[enSkyCubeType_Night] = L"Assets/modelData/sky/skyCubeMapNight.dds";
		m_textureFilePaths[enSkyCubeType_Snow] = L"Assets/modelData/sky/skyCubeMapSnow.dds";
		m_textureFilePaths[enSkyCubeType_Snow_2] = L"Assets/modelData/sky/skyCubeMapSnow_2.dds";
		m_textureFilePaths[enSkyCubeType_Wild] = L"Assets/modelData/sky/skyCubeMapWild.dds";
		m_textureFilePaths[enSkyCubeType_Wild_Night] = L"Assets/modelData/sky/skyCubeMapWild_Night.dds";
		m_textureFilePaths[enSkyCubeType_Wild_2] = L"Assets/modelData/sky/skyCubeMapWild_2.dds";
		m_textureFilePaths[enSkyCubeType_Grass] = L"Assets/modelData/sky/skyCubeMapGrass.dds";
		m_textureFilePaths[enSkyCubeType_Euro] = L"Assets/modelData/sky/skyCubeMapEuro.dds";*/
		m_textureFilePaths[enSkyCubeType_DayToon] = L"Assets/modelData/sky/skyCubeMapDay_Toon.dds";
		m_textureFilePaths[enSkyCubeType_DayToon_2] = L"Assets/modelData/sky/skyCubeMapDay_Toon_02.dds";
		m_textureFilePaths[enSkyCubeType_DayToon_3] = L"Assets/modelData/sky/skyCubeMapDay_Toon_03.dds";
		m_textureFilePaths[enSkyCubeType_DayToon_4] = L"Assets/modelData/sky/skyCubeMapDay_Toon_04.dds";
		//m_textureFilePaths[enSkyCubeType_NightToon] = L"Assets/modelData/sky/skyCubeMapNight_Toon.dds";
		//m_textureFilePaths[enSkyCubeType_NightToon_2] = L"Assets/modelData/sky/skyCubeMapNight_Toon_02.dds";
		//m_textureFilePaths[enSkyCubeType_SunriseToon] = L"Assets/modelData/sky/skyCubeMapSunrise_Toon.dds";
		////m_textureFilePaths[enSkyCubeType_SpaceToon] = L"Assets/modelData/preset/skyCubeMapSpace_Toon.dds";
		//m_textureFilePaths[enSkyCubeType_SpaceToon_2] = L"Assets/modelData/sky/skyCubeMapSpace_Toon_2.dds";
	}

	SkyCube::~SkyCube()
	{

	}

	bool SkyCube::Start()
	{
		ModelInitData initData;
		//tkmファイルのファイルパスを指定する。
		initData.m_tkmFilePath = "Assets/modelData/sky/sky.tkm";
		//シェーダーファイルのファイルパスを指定する。
		initData.m_fxFilePath = "Assets/shader/SkyCubeMap.fx";
		initData.m_vsEntryPointFunc = "VSMain";
		initData.m_psEntryPointFunc = "PSMain";

		for (int i = 0; i < enSkyCubeType_Num; i++) {
			m_texture[i].InitFromDDSFile(m_textureFilePaths[i]);
		}

		initData.m_expandShaderResoruceView[0] = &m_texture[m_type];
		initData.m_expandConstantBuffer = &m_luminance;
		initData.m_expandConstantBufferSize = sizeof(m_luminance);

		m_modelRender.InitDirectlyNotifyForwardRendering(initData);
		m_modelRender.SetTRS(m_position, g_quatIdentity, m_scale);
		m_modelRender.Update();

		return true;
	}

	void SkyCube::Update()
	{
		//if (m_isDirty)
		//{
		//	m_modelRender.SetTRS(m_position, g_quatIdentity, m_scale);
		//	m_modelRender.Update();
		//	m_isDirty = false;
		//}
		m_position = g_camera3D->GetPosition();
		m_modelRender.SetPosition(m_position);
		m_modelRender.Update();
	}

	void SkyCube::Render(RenderContext& rc)
	{
		m_modelRender.Draw(rc);
	}
}
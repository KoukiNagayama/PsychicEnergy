#include "k2EngineLowPreCompile.h"
#include "ToonMap.h"

namespace nsK2EngineLow
{
	void ToonMap::Init()
	{
		// DDS�t�@�C�����烉���v�e�N�X�`�����������B
		m_toonMap.InitFromDDSFile(L"Assets/rampTexture/toonMap_6.DDS");
	}
}
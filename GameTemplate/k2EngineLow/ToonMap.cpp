#include "k2EngineLowPreCompile.h"
#include "ToonMap.h"

namespace nsK2EngineLow
{
	void ToonMap::Init()
	{
		// DDS�t�@�C������e�N�X�`�����������B
		m_toonMap.InitFromDDSFile(L"Assets/shader/toonMap.DDS");

	}
}
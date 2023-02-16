#include "k2EngineLowPreCompile.h"
#include "ToonMap.h"

namespace nsK2EngineLow
{
	void ToonMap::Init()
	{
		// DDSファイルからランプテクスチャを初期化。
		m_toonMap.InitFromDDSFile(L"Assets/rampTexture/toonMap_6.DDS");
	}
}
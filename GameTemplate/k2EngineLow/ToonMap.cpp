#include "k2EngineLowPreCompile.h"
#include "ToonMap.h"

namespace nsK2EngineLow
{
	void ToonMap::Init()
	{
		// DDSファイルからテクスチャを初期化。
		m_toonMap.InitFromDDSFile(L"Assets/shader/toonMap.DDS");

	}
}
#pragma once
namespace nsK2EngineLow
{
	// シャドウマップの種類
	enum {
		SHADOW_MAP_AREA_NEAR,		// 近影範囲のシャドウマップ。
		SHADOW_MAP_AREA_MIDDLE,		// 中影範囲のシャドウマップ。
		SHADOW_MAP_AREA_FAR,		// 遠影範囲のシャドウマップ。
		NUM_SHADOW_MAP				// シャドウマップの数。
	};

	const int MAX_DIRECTIONAL_LIGHT = 4;	// ディレクションライトの最大数。
}


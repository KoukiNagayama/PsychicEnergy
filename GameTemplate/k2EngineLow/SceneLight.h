#pragma once
#include "MyRenderer.h"

namespace nsK2EngineLow
{
	const int NUM_DIRECTIONAL_LIGHT = 4;
	/// <summary>
	/// ディレクションライト
	/// </summary>
	struct DirectionalLight
	{
		Vector3 direction;		// ライトの方向
		int		pad;			// パディング
		Vector4 color;			// ライトのカラー
	};

	/// <summary>
	/// ライト構造体
	/// </summary>
	struct Light
	{
		DirectionalLight directionalLight[MAX_DIRECTIONAL_LIGHT];		// ディレクションライト
		Vector3 eyePos;							// カメラの位置
		float specPow;							// スペキュラの絞り
		Vector3 ambientLight;					// 環境光
		float pad;
	};

	/// <summary>
	/// シーンライトクラス
	/// </summary>
	class SceneLight : public Noncopyable
	{
	public:
		/// <summary>
		/// 初期化。
		/// </summary>
		void Init();
		/// <summary>
		/// 更新。
		/// </summary>
		void Update();
		/// <summary>
		/// ライトを取得。
		/// </summary>
		/// <returns></returns>
		Light& GetLightData()
		{
			return m_light;
		}
	private:
		Light m_light;					// ライト
	};
}



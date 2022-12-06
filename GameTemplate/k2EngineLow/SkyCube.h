#pragma once

namespace nsK2EngineLow {
	enum EnSkyCubeType {
		enSkyCubeType_DayToon,	// 昼間(トゥーン調)
		enSkyCubeType_DayToon_2,// 昼間(トゥーン調)２
		enSkyCubeType_DayToon_3,// 昼間(トゥーン調)３
		enSkyCubeType_DayToon_4,// 昼間(トゥーン調)４
		enSkyCubeType_Num,
	};
	/// <summary>
	/// スカイキューブ
	/// </summary>
	class SkyCube : public IGameObject
	{
	public:
		SkyCube();
		~SkyCube();
		bool Start()override final;
		void Update()override final;
		void Render(RenderContext& rc)override final;
	public:
		void SetPosition(const Vector3& pos)
		{
			m_position = pos;
			m_isDirty = true;
		}

		void SetScale(const Vector3& scale)
		{
			m_scale = scale;
			m_isDirty = true;
		}
		void SetScale(const float scale)
		{
			m_scale = g_vec3One;
			m_scale.Scale(scale);
			m_isDirty = true;
		}
		/// <summary>
		/// スカイキューブのタイプを設定。
		/// </summary>
		/// <param name="type"></param>
		void SetType(EnSkyCubeType type)
		{
			m_type = type;
		}
		/// <summary>
		/// 明るさを設定。
		/// </summary>
		/// <param name="lum"></param>
		void SetLuminance(float lum)
		{
			m_luminance = lum;
		}
		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		const wchar_t* GetTextureFilePath()
		{
			return m_textureFilePaths[m_type];
		}
	private:
		ModelRender m_modelRender;

		Texture m_texture[enSkyCubeType_Num];					// テクスチャ
		const wchar_t* m_textureFilePaths[enSkyCubeType_Num];	
		Vector3 m_position = g_vec3Zero;
		Vector3 m_scale = g_vec3One * 1000.0f;					// 拡大率
		float m_luminance = 1.0f;								// 明るさ
		bool m_isDirty = false;
		EnSkyCubeType m_type = enSkyCubeType_DayToon;
	};
}


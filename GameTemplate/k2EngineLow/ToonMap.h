#pragma once
namespace nsK2EngineLow
{
	/// <summary>
	/// トゥーンマップ
	/// </summary>
	class ToonMap : public Noncopyable
	{
	public:
		/// <summary>
		/// 初期化。
		/// </summary>
		void Init();
		/// <summary>
		/// トゥーンマップを取得。
		/// </summary>
		/// <returns></returns>
		Texture& GetToonMap()
		{
			return m_toonMap;
		}
	private:
		Texture				m_toonMap;		// トゥーンマップ
	};
}



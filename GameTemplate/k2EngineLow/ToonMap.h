#pragma once
namespace nsK2EngineLow
{
	/// <summary>
	/// �g�D�[���}�b�v
	/// </summary>
	class ToonMap : public Noncopyable
	{
	public:
		/// <summary>
		/// �������B
		/// </summary>
		void Init();
		/// <summary>
		/// �g�D�[���}�b�v���擾�B
		/// </summary>
		/// <returns></returns>
		Texture& GetToonMap()
		{
			return m_toonMap;
		}
	private:
		Texture				m_toonMap;		// �g�D�[���}�b�v
	};
}



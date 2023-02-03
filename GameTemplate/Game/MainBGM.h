#pragma once
#include "sound/SoundSource.h"
class MainBGM : public IGameObject
{
public:
	MainBGM() {}
	~MainBGM() {}

	bool Start();
	void Update();
	void StartFadeOut()
	{
		m_isStartFadeOut = true;
	}
private:
	SoundSource*	m_mainBGM = nullptr;
	float			m_volume = 0.0f;
	bool			m_isStartFadeOut = false;
};


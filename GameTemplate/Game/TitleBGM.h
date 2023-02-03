#pragma once
#include "sound/SoundSource.h"
class TitleBGM : public IGameObject
{
public:
	TitleBGM() {}
	~TitleBGM() {}

	bool Start();
	void Update();
	void StartFadeOut()
	{
		m_isStartFadeOut = true;
	}
private:
	SoundSource*	m_titleBGM = nullptr;
	float			m_volume = 0.0f;
	bool			m_isStartFadeOut = false;
};


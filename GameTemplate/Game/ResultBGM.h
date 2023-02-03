#pragma once
#include "sound/SoundSource.h"
class ResultBGM : public IGameObject
{
public:
	ResultBGM() {}
	~ResultBGM() {}

	bool Start();
	void Update();
	void StartFadeOut()
	{
		m_isStartFadeOut = true;
	}
private:
	SoundSource*	m_resultBGM = nullptr;
	float			m_volume = 0.0f;
	bool			m_isStartFadeOut = false;
};


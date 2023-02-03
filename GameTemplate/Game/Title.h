#pragma once
#include "LevelRender.h"
#include "SkyCube.h"
#include "Fade.h"
#include "sound/SoundSource.h"

class TitleBackGround;
class TitleCharacter;
class TitleCamera;
class TitleSprite;
class Fade;
class TitleBGM;
/// <summary>
/// タイトルクラス
/// </summary>
class Title : public IGameObject
{
public:
	Title() {}
	///<summary>
	/// デストラクタ
	///</summary>
	~Title();

	bool Start();
	void Update();
	/// <summary>
	/// Bボタンは押されたか
	/// </summary>
	bool& IsPushedB()
	{
		return m_isPushedB;
	}
	/// <summary>
	/// フェードアウトを開始
	/// </summary>
	void StartFadeOut()
	{
		m_isFadeOut = true;
	}
private:
	/// <summary>
	/// フェードアウト
	/// </summary>
	void FadeOut();

private:
	LevelRender			m_titleLevel;							// タイトルのレベル
	std::vector<TitleBackGround*> m_titleBackGroundArray;		// タイトルで使用する背景モデル
	TitleCharacter*		m_titleCharacter = nullptr;				// タイトルで使用するキャラクターモデル
	TitleCamera*		m_titleCamera = nullptr;				// タイトルで使用するカメラ
	TitleSprite*		m_titleSprite = nullptr;				// タイトルで使用するスプライト
	SpriteRender		m_titleTextSprite;						// タイトルに使用するテキストのスプライト
	float				m_currentAlpha = 0.0f;					// スプライトの乗算する現在のα値
	SkyCube*			m_skyCube = nullptr;					// スカイキューブ
	Fade*				m_fade = nullptr;						// フェード
	bool				m_isWaitFadeout = false;				// フェードアウトを待機しているか
	bool				m_isFadeOut = false;
	TitleBGM*			m_titleBGM = nullptr;
	SoundSource*		m_decisionSound = nullptr;				// 決定音
	bool				m_isPushedB = false;					// Bボタンは押されたか

};


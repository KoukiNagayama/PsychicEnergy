#pragma once
#include "LevelRender.h"
#include "SkyCube.h"
#include "WorldRotation.h"
class TestModel;
class Player;
class GameCamera;
class GravityGauge;
class Sight;
class BackGround;

class Game : public IGameObject
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Game();
	/// <summary>
	/// デスストラクタ
	/// </summary>
	~Game();	
	/// <summary>
	/// 開始処理。
	/// インスタンス生成時に一度だけ実行。
	/// </summary>
	bool Start();
	/// <summary>
	/// 更新処理。
	/// </summary>
	void Update();

private:

private:
	LevelRender		m_levelRender;						// レベルレンダー
	Player*			m_player = nullptr;					// プレイヤー
	GameCamera*		m_gameCamera = nullptr;				// ゲームカメラ
	BackGround*		m_backGround = nullptr;
	SkyCube*		m_skyCube = nullptr;				// スカイキューブ
	SpriteRender	m_sprite;
	GravityGauge*	m_gravityGauge = nullptr;
	Sight*			m_sight = nullptr;
};


#pragma once
#include "LevelRender.h"
#include "SkyCube.h"
#include "WorldRotation.h"
class Player;
class GameCamera;
class GravityGauge;
class Sight;
class BackGround;
class Ring;
class LockOn;
class DisplayGameTimer;

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
	BackGround*		m_backGround = nullptr;				// 背景
	SkyCube*		m_skyCube = nullptr;				// スカイキューブ						
	GravityGauge*	m_gravityGauge = nullptr;			// 重力ゲージ
	Sight*			m_sight = nullptr;					// 照準
	Ring*			m_ring = nullptr;					// リング
	LockOn*			m_lockOn = nullptr;
	DisplayGameTimer* m_displayGameTimer = nullptr;
};


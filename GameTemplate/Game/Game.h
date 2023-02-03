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
class Fade;
class MainBGM;
/// <summary>
/// インゲームクラス
/// </summary>
class Game : public IGameObject
{
public:
	Game();
	/// <summary>
	/// デストラクタ
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
	/// <summary>
	/// リングを取得
	/// </summary>
	void GetRing()
	{
		m_numOfGetRing++;
	}
	/// <summary>
	/// インゲームは終了しているか
	/// </summary>
	bool& IsFinishedInGame()
	{
		return m_isFinishedInGame;
	}
private:
	/// <summary>
	/// フェードアウト。
	/// </summary>
	void FadeOut();
private:
	LevelRender		m_levelRender;						// レベルレンダー
	Player*			m_player = nullptr;					// プレイヤー
	GameCamera*		m_gameCamera = nullptr;				// ゲームカメラ
	std::vector<BackGround*> m_backGroundArray;			// 背景
	SkyCube*		m_skyCube = nullptr;				// スカイキューブ						
	GravityGauge*	m_gravityGauge = nullptr;			// 重力ゲージ
	Sight*			m_sight = nullptr;					// 照準
	Ring*			m_ring = nullptr;					// リング
	LockOn*			m_lockOn = nullptr;
	DisplayGameTimer* m_displayGameTimer = nullptr;
	Fade*			m_fade = nullptr;					// フェード
	MainBGM*		m_mainBGM = nullptr;				// メインBGM
	int				m_maxRing = 0;
	int				m_numOfGetRing = 0;					// 取得したリングの数
	bool			m_isFinishedInGame = false;			// インゲームは終了したか？
};

